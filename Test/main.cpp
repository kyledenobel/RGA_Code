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

constexpr size_t DELAY_STEP_SIZE = SAMPLE_RATE / 20;
constexpr size_t DELAY_STEPS_DISABLED = 0;
constexpr size_t DELAY_STEPS_MIN = 0;
constexpr size_t DELAY_STEPS_MAX = 10;

constexpr size_t HISTORY_LENGTH = 50000;


// easy to adjust by just multiplying it
#define DROP_WINDOW 2340
// derived as HISTORY_LENGTH * DROP_WINDOW / gcf(HISTORY_LENGTH, DROP_WINDOW)
// ensures smooth operation of drop with a non-multiple sized history
#define DROP_WRAPAROUND 5850000

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

int delay_steps = 2;
float delay_decay = 0.5;

bool drop_enabled = false;

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

inline int32_t DropRatioFunction(int32_t i) {
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

    float r = DropRatioFunction(i);

    return sample_a * r + sample_b * (1 - r);
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
        if (delay_steps != DELAY_STEPS_DISABLED) {
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

void render_scroll_wheel(uint16_t page, uLCD::Display lcd) {
    // Clear out
    lcd.Rect(0, 7 * 11 - 1, 0, 8 * 15 - 1, ConvertColor(SCROLL_BACKGROUND), true);
    // Set text background
    lcd.SetTextBackground(ConvertColor(SCROLL_BACKGROUND));

    // Selected text
    lcd.String(GetScrollText(page), 1, 5, ConvertColor(SCROLL_SELECTED));
    // Unselected stuff
    lcd.String(GetScrollText((page + (PAGE_COUNT - 1)) % PAGE_COUNT), 1, 3, ConvertColor(SCROLL_SELECTED));
    lcd.String(GetScrollText((page + 1) % PAGE_COUNT), 1, 7, ConvertColor(SCROLL_SELECTED));

    lcd.SetTextBackground(0); // Set back to black
}

void render_recording_subsection(RecordingState &state, uLCD::Display lcd) {
    // Red background if unmounted, normal background if mounted
    uint16_t bkg = state.mounted ? ConvertColor(MOUNTED_BACKGROUND) : ConvertColor(UNMOUNTED_BACKGROUND);
    lcd.SetTextBackground(bkg); // Set background

    // Clear it
    lcd.Rect(0, 119, 127, 127, bkg, true);

    if (state.mounted) {
        lcd.String("NO SRTG", 0, 15, ConvertColor(0x000000));
    } else {
        lcd.String("WAITING", 0, 15, ConvertColor(0x000000));
    }

    lcd.SetTextBackground(0); // Reset bkg to black
}

void render_recording_timer(RecordingState &state, uLCD::Display lcd) {
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
    time_str[6] = time_centisec % 10;
    time_centisec /= 10;
    time_str[4] = time_centisec % 10;
    time_centisec /= 10;
    time_str[3] = time_centisec % 10;

    time_str[1] = time_min % 10;
    time_str[0] = (time_min / 10) % 10;

    // Red background if unmounted, normal background if mounted
    uint16_t bkg = ConvertColor(MOUNTED_BACKGROUND);
    lcd.SetTextBackground(bkg); // Set background

    lcd.String(time_str, 11, 15, 0x000000);
}

int main(void)
{
    hw.Init();
    sel_but.Init(seed::D27, GPIO::Mode::INPUT, GPIO::Pull::NOPULL, GPIO::Speed::LOW);
    card_detect.Init(seed::D28, GPIO::Mode::INPUT, GPIO::Pull::PULLUP, GPIO::Speed::LOW);
    hw.SetAudioBlockSize(16);

    Blink(1);
    
    lcd.Init(&hw, seed::D12, seed::D11, UartHandler::Config::Peripheral::UART_4, seed::D10, BAUDS::OK);

    lcd.Line(50, 50, 40, 80, ConvertColor(0xFFFFFF));
    
    lcd.Rect(10, 10, 40, 40, ConvertColor(0x444444), true);

    Blink(1);

    char* start_str2 = "indep text\nis cool!\0";
    lcd.IndString(start_str2, 2, 3, ConvertColor(0xFFFFFF));
    Blink(1);

    char* start_str = "hello!\0";
    lcd.String(start_str, 2, 2, ConvertColor(0xFFFFFF));
    
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
    fsi.Init(fsi_cfg);
    auto res = f_mount(& GET_STORAGE_FILE_SYSTEM,
        GET_STORAGE_PATH,
        1);
    // Communicate results
    rec_state.mounted = (res == FR_OK);

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

    bool prev_card = 0;

    uint16_t page = RECORD_PAGE;

    while(1) {
        // Inputs
        sel_deb.next(sel_but.Read());
        card_deb.next(card_detect.Read());
        bool sel = sel_deb.state;
        bool card_in = !card_deb.state; 
        bool just_started = false;
        bool rerender_scroll = false;
        bool rerender_page = false;
        InputInstance pressed = input_handler.next_tick({Directions::Center, sel});

        // handle page switching
        switch (pressed.dir) {
        case (Directions::Up) :
            page = (page + 1) % PAGE_COUNT;
            rerender_scroll = true;
            break;
        case (Directions::Down) :
            page = (page + (PAGE_COUNT - 1)) % PAGE_COUNT;
            rerender_scroll = true;
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

                // mount it
                rec_state.mounted = f_mount(& GET_STORAGE_FILE_SYSTEM,
                    GET_STORAGE_PATH,
                    1) == FR_OK;

            } else { // Card ejected

                // if recording, stop (not like we can save anyways but still)
                if (rec_state.is_recording) {
                    StopRecording(rec_state);
                }

                // unmount
                f_unmount(GET_STORAGE_PATH);
            }
            render_recording_subsection(rec_state, lcd);
        }
        prev_card = card_in;

        // Actually handle stuff
        // Recording
        if (rec_state.mounted && page == RECORD_PAGE) {
            // if they press in, either stop or start recording (Input Handling)
            if (pressed.sel) {
                if (rec_state.is_recording) {
                    StopRecording(rec_state);
                    hw.SetLed(false);
                    char* start_str = "Recording Done...";
                    lcd.String(start_str, 0, 2, ConvertColor(0xFFFFFF));
                    hw.DelayMs(5);
                } else {
                    auto res = AttemptStartRecording(storage_path, rec_state);
                    if (res == REC_OK) {
                        hw.SetLed(true);
                        char* start_str = "Recording Start!";
                        lcd.String(start_str, 0, 2, ConvertColor(0xFFFFFF));
                        hw.DelayMs(5);
                        just_started = true;
                    } else {
                        hw.SetLed(false);
                        char* start_str = "Cannot Record.";
                        lcd.String(start_str, 0, 2, ConvertColor(0xFFFFFF));
                        hw.DelayMs(5);
                    };
                }
            }
            // The actual recording timer
            if (rec_state.is_recording && ((ms % (interval*display_interval)) == 0)) {
                render_recording_timer(rec_state, lcd);
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
