#include "daisy_seed.h"
#include "daisysp.h"
#include <stdio.h>
#include <string.h>
#include "uLCD.h"
#include "Inputs.h"

using namespace daisy;
using uLCD::ConvertColor;

// Dumb macros
#define f_unmount(path) f_mount(0, path, 0)


// Globals

SdmmcHandler sd;
FatFSInterface fsi;
FIL file;

DaisySeed hw;
uLCD::Display lcd;
//USBHostHandle usb_host;


constexpr size_t SAMPLE_RATE = 96000;

constexpr size_t DELAY_STEPS_PER_SEC = 20;
constexpr size_t DELAY_STEP_SIZE = SAMPLE_RATE / DELAY_STEPS_PER_SEC;
constexpr size_t DELAY_STEPS_DISABLED = 0;
constexpr size_t DELAY_STEPS_MIN = 1;
constexpr size_t DELAY_STEPS_MAX = 10;

constexpr float DECAY_MIN = 1.0/32;
constexpr float DECAY_MAX = 31.0/32;

constexpr size_t HISTORY_LENGTH = 48000;


// easy to adjust by just multiplying it
constexpr size_t DROP_WINDOW = 7680;
// derived as HISTORY_LENGTH * DROP_WINDOW / gcf(HISTORY_LENGTH, DROP_WINDOW)
// ensures smooth operation of drop with a non-multiple sized history
constexpr size_t DROP_WRAPAROUND = HISTORY_LENGTH * DROP_WINDOW;

constexpr int32_t DROP_OFFSET_A = DROP_WINDOW;
constexpr int32_t DROP_OFFSET_B = (DROP_WINDOW + DROP_WINDOW/2);

constexpr char TERMINATOR_CHAR = '\0';

enum Pages {
    RECORD_PAGE = 0,
    DELAY_PAGE,
    DROP_PAGE,
    PAGE_COUNT,
    TUNER_PAGE
};

enum RecordingCode {
    REC_OK,
    REC_TOO_MANY, // Too many recording files already exist
    REC_OUT_OF_SPACE, // Storage has run out of space
    REC_NO_STORAGE, // No storage attached / mounted
    REC_INVALID_ARGS, // Args to this function were invalid
    REC_STORAGE_ERROR, // Some unknown storage error
    REC_FILE_NOT_FOUND, // File doesn't exist
    REC_IN_PROGRESS, // Recording already in progress
};

typedef WavWriter<32768/2> WavWriterT;

struct RecordingState {
    bool mounted = false;
    bool is_recording = false;
    bool lockout = false;
    uint32_t current_index = 0;
    WavWriterT writer;
};

#define GET_STORAGE_FILE_SYSTEM fsi.GetSDFileSystem()
#define GET_STORAGE_PATH fsi.GetSDPath()
#define FILE_FORMATTING "AmpRec_%04lu.wav" 
#define MAX_FILE_INDEX 9999
#define WAV_CHANNELS 1
#define WAV_BITS_PER_SAMPLE 16 // can only be 16 or 32 apparently
#define MEDIA_TYPE FatFSInterface::Config::MEDIA_SD
constexpr size_t FILE_NAME_SIZE = 64;

float history[HISTORY_LENGTH] = {0};
int32_t curr_history_index = 0;
int32_t curr_drop_index = 0;

bool delay_enabled = false;
int delay_steps = 5;
float delay_decay = 16.0/32;

bool drop_enabled = false;
constexpr float drop_rho = 1.0;
constexpr float drop_gamma = 0.5;
constexpr float drop_S = DROP_WINDOW / 2.0;
// constants for first part of cubic
constexpr float drop_a = -3.5349e-11;
constexpr float drop_b = 2.0366e-07;
constexpr float drop_c = -4.0722e-07;
constexpr float drop_d = 2.0359e-07;

// constants for second part of cubic
constexpr float drop_aa = 3.5321e-11;
constexpr float drop_bb = -6.1035e-07;
constexpr float drop_cc = 3.1250e-03;
constexpr float drop_dd = -4;


RecordingState rec_state = RecordingState();

GPIO sel_but;
GPIO card_detect;
InputHandler input_handler;
/// Checks if a file exists
RecordingCode file_exists(const char* path) {
    FIL f;

    auto res = f_open(&f, path, FA_OPEN_EXISTING | FA_READ);
    if (res == FR_NO_FILE)
    {
        // File isn't found (doesn't cover path failures)
        f_close(&file);
        return REC_FILE_NOT_FOUND;
    }
    if (res == FR_OK) {
        // File is found
        f_close(&file);
        return REC_OK;
    }
    // Error occured
    f_close(&file);
    return REC_STORAGE_ERROR;
}

// each blink is ~0.2 seconds
void Blink(size_t num) {
    for (size_t i = 0; i < num; i++) {
        hw.SetLed(true);
        hw.DelayMs(100);
        hw.SetLed(false);
        hw.DelayMs(100);
    }
}

/// Gets the next "index" (number appended to file name) that is not already taken. Expects out to not be a nullptr.
/// Any return other than REC_OK means a file wasn't found, and the specific error code denotes why
// Directory should end in a '/' already
RecordingCode GetNextRecordingIndex(uint32_t & out, const std::string& directory, RecordingState &state) {
    if (!state.mounted) {
        return REC_NO_STORAGE;
    }
    char file_name[FILE_NAME_SIZE] = { TERMINATOR_CHAR };
    // Get file base of string
    std::string path;
    path.assign(directory);
    // We have a max recording index of 9999
    for (uint32_t i = state.current_index; i <= MAX_FILE_INDEX; i++) {
        // clear anything excess to "{path}/"
        if (path.size() > directory.size()) {
            path.erase(directory.size());
        }
        // Generate file name
        snprintf(file_name, FILE_NAME_SIZE, FILE_FORMATTING, i);
        path.append(file_name);
        // Check if file exists
        RecordingCode res = file_exists(path.c_str());
        // If the file exists, just keep going
        if (res == REC_OK) {
            continue;
        }
        // If the file doesn't exist, we found our next recording slot
        if (res == REC_FILE_NOT_FOUND) {
            out = i;
            return REC_OK;
        }
        // If there is an error, just forward it
        return res;
    }
    return REC_TOO_MANY;
}

RecordingCode AttemptStartRecording(const std::string& directory, RecordingState &state) {
    if (state.is_recording || state.lockout) {
        return REC_IN_PROGRESS;
    }
    state.lockout = true;

    uint32_t next_index = 0;
    RecordingCode res = GetNextRecordingIndex(next_index, directory, state);
    if (res != REC_OK) {
        state.lockout = false;
        return res;
    }

    // Generate final path
    std::string path;
    path.assign(directory);
    char file_name[FILE_NAME_SIZE] = { TERMINATOR_CHAR };
    snprintf(file_name, FILE_NAME_SIZE, FILE_FORMATTING, next_index);
    path.append(file_name);

    // Open up the writer
    state.writer.OpenFile(path.c_str());
    
    // Set the current index etc
    state.current_index = next_index;
    
    // Finally declare that we are recording
    state.is_recording = true;

    return REC_OK;
}

RecordingCode StopRecording(RecordingState &state) {
    // if it's already not recording, yippee!
    if (!state.is_recording) {
        return REC_OK;
    }
    // immediately disable recording to prevent any problems potentially caused by interrupts
    state.is_recording = false;

    // Save the file
    state.writer.SaveFile();

    // increment index since we know the current was already used (faster search next time :3)
    state.current_index += 1;

    // disable recording lockout
    state.lockout = false;

    return REC_OK;
}

// These two functions I got from messing around in desmos, for now, very uncommented, sorry!
inline int32_t DropSampleFunction(int32_t i, const int32_t d) {
    int32_t a = ((i - d) % DROP_WINDOW) / 2;
    int32_t b = (i - d) / DROP_WINDOW;
    return a + b * DROP_WINDOW;
}

inline float DropRatioFunction(float k) {
    float res = 0;
    float kk = k * k;
    float kkk = kk * k;
    if (k < drop_rho*drop_S) {
        res = drop_a * kkk + drop_b * kk + drop_c * k + drop_d;
    } else if (k <= drop_S) {
        res = 1;
    } else {
        res = drop_aa * kkk + drop_bb * kk + drop_cc * k + drop_dd;
    }
    return res;
}
inline float DropRatioFunctionOld(int32_t i) {
    constexpr float cutoff_multiplier = 20;
    i += DROP_WINDOW/2;
    float res = (2 * (i % DROP_WINDOW) / DROP_WINDOW);
    res = abs(res - 1.0) - 0.5;
    res *= cutoff_multiplier;
    if (res < -0.5) {
        res = -0.5;
    }
    if (res > 0.5) {
        res = 0.5;
    }
    res += 0.5;
    return res;
}

// actual drop function
inline float Drop(int32_t i) {
    // stupid stretch hack (low pass filter will handle it)
    if (i % 2 == 1)
        return 0;

    // get indices of drop sampling
    int32_t i_a = DropSampleFunction(i, DROP_OFFSET_A);
    int32_t i_b = DropSampleFunction(i, DROP_OFFSET_B);

    float sample_a = history[(i_a + HISTORY_LENGTH) % HISTORY_LENGTH];
    float sample_b = history[(i_b + HISTORY_LENGTH) % HISTORY_LENGTH];

    float r_a = DropRatioFunction(i % DROP_WINDOW);
    float r_b = DropRatioFunction((i + DROP_WINDOW/2) % DROP_WINDOW);

    return sample_a * r_a + sample_b * r_b;
}

void AudioProcessingCallback(AudioHandle::InputBuffer in,
                AudioHandle::OutputBuffer out,
                size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        curr_history_index++;
        curr_history_index %= HISTORY_LENGTH;
        curr_drop_index++;
        curr_drop_index %= DROP_WRAPAROUND;

        // Sample input
        float sample = in[0][i];

        // Apply delay
        if (delay_enabled) {
            sample += history[(curr_history_index + HISTORY_LENGTH - (delay_steps * DELAY_STEP_SIZE)) % HISTORY_LENGTH] * delay_decay;
        }
        
        // update history with processed signal
        history[curr_history_index] = sample;
        
        // Apply drop
        if (drop_enabled) {
            sample = Drop(curr_drop_index);
        }

        // send to WavWriter if recording
        if (rec_state.is_recording && (curr_history_index % 4 == 0)) {
            rec_state.writer.Sample(&sample);
        }

        // Set set output to processed signal
        out[0][i] = sample;
        out[1][i] = 0;
    }
}

struct WriteReturnCode {
    int stage;
    int result;
    WriteReturnCode(int s, int r) {
        stage = s;
        result = r;
    }
};

WriteReturnCode write_test(const char* filename, uLCD::Display &lcd) {
    // Initialize the SDMMC interface and FatFS drivers
    FatFSInterface::Config fsi_cfg;
    fsi_cfg.media = MEDIA_TYPE;
    fsi.Init(fsi_cfg);
    auto res = f_mount(& GET_STORAGE_FILE_SYSTEM,
        GET_STORAGE_PATH,
        0);
    if (res != FR_OK)
    {
        char err_str[32] = "failed to mount";
        lcd.String(err_str, 2, 3, ConvertColor(0xFFFFFF));
        // If some error is encountered in mounting the card, then simply return.
        return WriteReturnCode(0, res);
    }
    for (int i = 0; i < 100; i++) {
        //usb_host.Process();
    }

    char file_path[512] = {0};
    strcpy(file_path, GET_STORAGE_PATH);
    strcat(file_path, filename);
 
    // And Print Hello World!
    //hw.PrintLine("%s", file_path);
    //hw.PrintLine("here's the file path :3");
    //hw.PrintLine("BAZINGA");
    //hw.PrintLine("That's the file path :3");
    res = f_open(&file, file_path, FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK)
    {
        char err_str[32] = "failed to open";
        lcd.String(err_str, 2, 4, ConvertColor(0xFFFFFF));
        // If some error is encountered in opening the file, then simply return.
        f_close(&file);
        return WriteReturnCode(1, res);
    }


    UINT bytes_written = 0;
    constexpr size_t BUFFER_SIZE = 7;
    char buffer[BUFFER_SIZE] = "hello!";
    res = f_write(&file, buffer, BUFFER_SIZE, &bytes_written);
    if (res != FR_OK)
    {
        char err_str[32] = "failed to write";
        lcd.String(err_str, 2, 5, ConvertColor(0xFFFFFF));
        // If some error is encountered in opening the file, then simply return.
        f_close(&file);
        return WriteReturnCode(2, res);
    }
    f_close(&file);

    char suc_str[32] = "success!";
    lcd.String(suc_str, 2, 6, ConvertColor(0xFFFFFF));
    return WriteReturnCode(2, res);
}
//void write_test(const char* filename) {}

void ActiveCallback(void *userdata) {}
void ConnectCallback(void *data) {hw.SetLed(true);}
void DisconnectCallback(void *data) {hw.SetLed(false);}
void ErrorCallback(void *data) {}

constexpr uint32_t UNMOUNTED_BACKGROUND = 0xffa28e;
constexpr uint32_t MOUNTED_BACKGROUND = 0xFFFFFF;
constexpr uint32_t SCROLL_BACKGROUND = 0x444444;
constexpr uint32_t SCROLL_SELECTED = 0xFFFFFF;
constexpr uint32_t SCROLL_UNSELECTED = 0xBBBBBB;

constexpr uint8_t REC_SUBSECTION_START = 116;

const char* REC_SCROLL_TEXT = "RECORD";
const char* DELAY_SCROLL_TEXT = "DELAY";
const char* DROP_SCROLL_TEXT = "DROP";
const char* UNKNOWN_TEXT = "UNKWN";

const char* GetScrollText(uint16_t page) {
    switch (page) {
        case (RECORD_PAGE):
        return REC_SCROLL_TEXT;
        case (DELAY_PAGE):
        return DELAY_SCROLL_TEXT;
        case (DROP_PAGE):
        return DROP_SCROLL_TEXT;
        default :
        return UNKNOWN_TEXT;
    }
}

void render_scroll_wheel(uint16_t page, uLCD::Display &lcd) {
    // Clear out
    lcd.Rect(0, 0, 7 * 8 - 1, REC_SUBSECTION_START - 1, ConvertColor(SCROLL_BACKGROUND), true);
    // Set text background
    lcd.SetTextBackground(ConvertColor(SCROLL_BACKGROUND));
    lcd.Stall();

    // Selected text
    lcd.String(GetScrollText(page), 1, 6, ConvertColor(SCROLL_SELECTED));
    // Unselected stuff
    lcd.String(GetScrollText((page + (PAGE_COUNT - 1)) % PAGE_COUNT), 1, 9, ConvertColor(SCROLL_UNSELECTED));
    lcd.String(GetScrollText((page + 1) % PAGE_COUNT), 1, 3, ConvertColor(SCROLL_UNSELECTED));

    lcd.SetTextBackground(0); // Set back to black
}

void render_recording_subsection(RecordingState &state, uLCD::Display &lcd) {
    // Red background if unmounted, normal background if mounted
    uint16_t bkg = state.mounted ? ConvertColor(MOUNTED_BACKGROUND) : ConvertColor(UNMOUNTED_BACKGROUND);
    lcd.SetTextBackground(bkg); // Set background

    // Clear it
    lcd.Rect(0, REC_SUBSECTION_START, 127, 127, bkg, true);
    lcd.Stall();
    
    // Status
    if (state.mounted) {
        lcd.String(state.is_recording ? "RECORDING" :"WAITING", 0, 15, ConvertColor(0x000000));
    } else {
        lcd.String("NO_STRG", 0, 15, ConvertColor(0x000000));
    }

    lcd.SetTextBackground(0); // Reset bkg to black
}

void render_recording_timer(RecordingState &state, uLCD::Display &lcd) {
    // only bother if it is mounted
    if (!state.mounted) {
        return;
    }


    float time = state.writer.GetLengthSeconds();

    uint32_t time_centisec = static_cast<int>(time * 10);  // centiseconds
    // implicit maximum
    if (time_centisec > 600 * 99 - 1) {
        time_centisec = 600 * 99 - 1;
    }
    uint32_t time_min = time_centisec / 600;
    time_centisec = time_centisec % 600;

    char time_str[8] = "00:00.0";
    time_str[6] = '0' + (time_centisec % 10);
    time_centisec /= 10;
    time_str[4] = '0' + (time_centisec % 10);
    time_centisec /= 10;
    time_str[3] = '0' + (time_centisec % 10);

    time_str[1] = '0' + (time_min % 10);
    time_str[0] = '0' + ((time_min / 10) % 10);

    // Red background if unmounted, normal background if mounted
    uint16_t bkg = ConvertColor(MOUNTED_BACKGROUND);
    lcd.SetTextBackground(bkg); // Set background

    lcd.String(time_str, 11, 15, 0x000000);
    
    lcd.SetTextBackground(0); // Reset bkg to black
}

void MakeDecimal(char* out, float num) {
    int i = static_cast<int>(num * 100);
    out[0] = '0' + ((i / 100) % 10);
    out[1] = '.';
    out[2] = '0' + ((i / 10) % 10);
    out[3] = '0' + (i % 10);
}

const char* OnString = "On";
const char* OffString = "Off";

int main(void)
{
    char delay_str[6] = "X.XXs";
    char decay_str[5] = "X.XX";

    hw.Init();
    sel_but.Init(seed::D28, GPIO::Mode::INPUT, GPIO::Pull::NOPULL, GPIO::Speed::LOW);
    card_detect.Init(seed::D27, GPIO::Mode::INPUT, GPIO::Pull::PULLUP, GPIO::Speed::LOW);

    
    // Create an ADC Channel Config object
    AdcChannelConfig adc_config[2];

    // Set up the ADC config with a connection to pin A0
    adc_config[0].InitSingle(seed::A0);
    adc_config[1].InitSingle(seed::A1);

    // Initialize the ADC peripheral with that configuration
    hw.adc.Init(adc_config, 2);
    hw.adc.Start();

    hw.SetAudioBlockSize(16);

    Blink(1);
    
    lcd.Init(&hw, seed::D12, seed::D11, UartHandler::Config::Peripheral::UART_4, seed::D10, BAUDS::OK);

    lcd.Line(50, 50, 40, 80, ConvertColor(0xFFFFFF));
    
    lcd.Rect(10, 10, 40, 40, ConvertColor(0x444444), true);

    Blink(1);

    lcd.IndString("indep text\nis cool!\0", 2, 3, ConvertColor(0xFFFFFF));

    Blink(1);

    lcd.String( "hello!", 2, 2, ConvertColor(0xFFFFFF));
    
    Blink(1);
    
    WavWriterT::Config wav_cfg = {
        hw.AudioSampleRate() / 4, // we record at half speed due to drop existing
        WAV_CHANNELS,
        WAV_BITS_PER_SAMPLE,
    };
    rec_state.writer.Init(wav_cfg);
    
    // Initializing usb stuff
    //hw.usb_handle.Init(UsbHandle::FS_EXTERNAL);
    /*USBHostHandle::Config usb_host_cfg = USBHostHandle::Config();
    usb_host_cfg.class_active_callback = ActiveCallback;
    usb_host_cfg.connect_callback = ConnectCallback;
    usb_host_cfg.disconnect_callback = DisconnectCallback;
    usb_host_cfg.error_callback = ErrorCallback;
    usb_host.Init(usb_host_cfg);
    for (size_t i = 0; i < HISTORY_LENGTH; i++) {
        history[i] = 0.0;
    }
    for (int i = 0; i < 1; i++) {
        usb_host.Process();
    } */
    hw.StartAudio(AudioProcessingCallback);
     
    // Enable Logging, and set up the USB connection.
    hw.StartLog();

    // Initialize the SDMMC interface and FatFS drivers
    SdmmcHandler::Config sd_cfg;
    sd_cfg.Defaults();
    sd_cfg.clock_powersave = false;
	sd_cfg.width = SdmmcHandler::BusWidth::BITS_1;
    sd_cfg.speed = daisy::SdmmcHandler::Speed::VERY_FAST;
    auto sd_res = sd.Init(sd_cfg);
    if (sd_res == SdmmcHandler::Result::ERROR)
        return -1;
    FatFSInterface::Config fsi_cfg;
    fsi_cfg.media = MEDIA_TYPE;
    //auto res = f_mount(& GET_STORAGE_FILE_SYSTEM,
    //    GET_STORAGE_PATH,
    //    1);
    // Communicate results
    //rec_state.mounted = (res == FR_OK);

    //char * name = "test.txt";
    //WriteReturnCode out = write_test(name, lcd);
    //return 0;
    //hw.PrintLine("%d : CODE(%d)", out.stage, out.result);
    uint32_t ms = 0;

    std::string storage_path = GET_STORAGE_PATH;
    hw.PrintLine("%s", storage_path.c_str());

    lcd.Clear();

    constexpr uint32_t interval = 1;

    constexpr uint32_t display_interval = 50;
    
    constexpr uint32_t save_interval = 50;
    Debouncer sel_deb = Debouncer();
    Debouncer card_deb = Debouncer();

    bool prev_card = false;

    uint16_t page = RECORD_PAGE;
    
    bool first_run = true;

    uint16_t subpage = 0;

    while(1) {
        // Inputs
        sel_deb.next(!sel_but.Read());
        card_deb.next(card_detect.Read());
        bool sel = sel_deb.state;
        bool card_in = card_deb.state; 
        bool rerender_scroll = false;
        bool rerender_page = false;
        float x = hw.adc.GetFloat(0);
        float y = hw.adc.GetFloat(1);
        Directions::Directions dir = Directions::Center;
        if (x > 0.9) {
            dir = Directions::Right;
        } else if (x < 0.1) {
            dir = Directions::Left;
        } else if (y > 0.9) {
            dir = Directions::Up;
        } else if (y < 0.1) {
            dir = Directions::Down;
        }

        if (first_run) {
            rerender_scroll = true;
            first_run = false;
            render_recording_subsection(rec_state, lcd);
        }

        InputInstance pressed = input_handler.next_tick({dir, sel});

        // handle page switching
        switch (pressed.dir) {
        case (Directions::Up) :
            page = (page + 1) % PAGE_COUNT;
            rerender_scroll = true;
            subpage = 0;
            break;
        case (Directions::Down) :
            page = (page + (PAGE_COUNT - 1)) % PAGE_COUNT;
            rerender_scroll = true;
            subpage = 0;
            break;
        default :
            break;
        }

        // we need to render the main UI
        if (rerender_scroll) {
            render_scroll_wheel(page, lcd);
            rerender_page = true;
        }
        

        // card insertion and detection
        if (card_in != prev_card) {
            if (card_in) { // Card inserted
                hw.DelayMs(500);
                if (!rec_state.mounted) {
                    fsi.Init(fsi_cfg);
                    // mount it
                    rec_state.mounted = f_mount(& GET_STORAGE_FILE_SYSTEM,
                        GET_STORAGE_PATH,
                        1) == FR_OK;
                }

            } else { // Card ejected

                // if recording, stop (not like we can save anyways but still)
                if (rec_state.is_recording) {
                    StopRecording(rec_state);
                }

                // unmount
                f_unmount(GET_STORAGE_PATH);
                rec_state.mounted = false;
                fsi.DeInit();
            }
            render_recording_subsection(rec_state, lcd);
        }
        prev_card = card_in;

        // Actually handle stuff
        // Recording
        if (rec_state.mounted) {
            // if they press in, either stop or start recording (Input Handling)
            if (pressed.sel && page == RECORD_PAGE) {
                rerender_page = true;
                if (rec_state.is_recording) {
                    StopRecording(rec_state);
                    render_recording_subsection(rec_state, lcd);
                    hw.SetLed(false);
                } else {
                    auto res = AttemptStartRecording(storage_path, rec_state);
                    render_recording_subsection(rec_state, lcd);
                    if (res == REC_OK) {
                        hw.SetLed(true);
                        render_recording_timer(rec_state, lcd);
                        hw.DelayMs(5);
                    } else {
                        hw.SetLed(false);
                        hw.DelayMs(5);
                    };
                }
            }
        }
        // The actual recording timer
        if ((ms % (interval*display_interval)) == 0) {
            if (rec_state.is_recording) {
                render_recording_timer(rec_state, lcd);
            } else {
            }
        }

        if (page == DELAY_PAGE) {
            if (pressed.sel) {
                subpage = (subpage + 1) % 3;
                rerender_page = true;
            } else {
                switch (subpage) {
                    case(0) :
                    if (pressed.dir == Directions::Right || pressed.dir == Directions::Left) {
                        delay_enabled = !delay_enabled;
                        rerender_page = true;
                    }
                    break;
                    case(1) :
                    if (pressed.dir == Directions::Right) {
                        delay_steps += 1;
                        if (delay_steps > DELAY_STEPS_MAX) {
                            delay_steps = DELAY_STEPS_MAX;
                        }
                        rerender_page = true;
                    } else if (pressed.dir == Directions::Left) {
                        delay_steps -= 1;
                        if (delay_steps < DELAY_STEPS_MIN) {
                            delay_steps = DELAY_STEPS_MIN;
                        }
                        rerender_page = true;
                    }
                    break;
                    case(2) :
                    if (pressed.dir == Directions::Right) {
                        delay_decay += (1.0/32);
                        if (delay_decay > DECAY_MAX) {
                            delay_decay = DECAY_MAX;
                        }
                        rerender_page = true;
                    } else if (pressed.dir == Directions::Left) {
                        delay_decay -= (1.0/32);
                        if (delay_decay < DECAY_MIN) {
                            delay_decay = DECAY_MIN;
                        }
                        rerender_page = true;
                    }
                    break;
                    default:
                    break;
                }
                if (pressed.dir == Directions::Right) {
                    
                }
            }
        } else if (page == DROP_PAGE) {
            if (pressed.sel) {
                drop_enabled = !drop_enabled;
                rerender_page = true;
            }
        }

        if (rerender_page) {
            switch (page) {
            case(DELAY_PAGE):
                MakeDecimal(delay_str, float(delay_steps) / DELAY_STEPS_PER_SEC);
                MakeDecimal(decay_str, delay_decay);
                lcd.Rect(7 * 8, 0, 127, REC_SUBSECTION_START - 1, ConvertColor(0), true);
                lcd.Stall();
                lcd.String("Enabled", 8, 2, ConvertColor(0xFFFFFF));
                lcd.String(delay_enabled ? OnString : OffString, 9, 3, ConvertColor(0xBBBBBB));
                {
                    lcd.String("Delay", 8, 5, ConvertColor(0xFFFFFF));
                    lcd.String(delay_str, 9, 6, ConvertColor(0xBBBBBB));\
                }
                {
                    lcd.String("Decay", 8, 8, ConvertColor(0xFFFFFF));
                    lcd.String(decay_str, 9, 9, ConvertColor(0xBBBBBB));
                }
                // Selection
                lcd.Char('<', 8, 3 + (subpage*3), ConvertColor(0xFFFFFF));
                {
                    const char * bruh[3] = { delay_enabled ? OnString : OffString, delay_str, decay_str };
                    int32_t offset = strlen(bruh[subpage]);
                    
                    lcd.Char('>', 9+offset, 3 + (subpage*3), ConvertColor(0xFFFFFF));

                }
                break;
            case(DROP_PAGE):
                lcd.Rect(7 * 8, 0, 127, REC_SUBSECTION_START - 1, ConvertColor(0), true);
                lcd.Stall();
                lcd.String(drop_enabled ? OnString : OffString, 9, 6, drop_enabled ? ConvertColor(0xFFFFFF) : ConvertColor(0xBBBBBB));
                break;
            case(RECORD_PAGE):
                lcd.Rect(7 * 8, 0, 127, REC_SUBSECTION_START - 1, ConvertColor(0), true);
                lcd.Stall();
                {
                    char rec_str[10] = "Recording";
                    char not_str[14] = "Not\nRecording";
                    lcd.String(rec_state.is_recording ? rec_str : not_str, 9, 6, rec_state.is_recording ? ConvertColor(0xFFFFFF) : ConvertColor(0xBBBBBB));
                }
                break;
            default :
                lcd.Rect(7 * 8, 0, 127, REC_SUBSECTION_START - 1, ConvertColor(0), true);
                break;
            }
        }
        

        if (ms % save_interval == 0) {
            rec_state.writer.Write();
        }

        hw.DelayMs(interval);
        ms = ms + interval;
        if (ms > (1 << 30)) {
            ms = 1;
        }
    }

}
