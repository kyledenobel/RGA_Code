#include "daisy_seed.h"
#include "daisysp.h"
#include <stdio.h>
#include <string.h>

using namespace daisy;

FatFSInterface fsi;
FIL file;

DaisySeed hw;
daisysp::Oscillator osc;
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

constexpr char TERMINATOR_CHAR = 0;

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

struct RecordingState {
    bool mounted = false;
    bool is_recording = false;
    bool lockout = false;
    uint32_t current_index = 0;
    WavWriter<16384> writer;
};

#define GET_STORAGE_FILE_SYSTEM fsi.GetUSBFileSystem()
#define GET_STORAGE_PATH fsi.GetUSBPath()
#define FILE_FORMATTING "AmpRec_%04lu.wav" 
#define MAX_FILE_INDEX 9999
#define WAV_CHANNELS 1
#define WAV_BITS_PER_SAMPLE 12
#define MEDIA_TYPE FatFSInterface::Config::MEDIA_USB
constexpr size_t FILE_NAME_SIZE = 64;

float history[HISTORY_LENGTH] = {0};
int32_t curr_history_index = 0;
int32_t curr_drop_index = 0;

int delay_steps = 5;
float delay_decay = 0.5;

bool drop_enabled = true;

RecordingState rec_state = RecordingState();

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

        // The oscillator's Process function synthesizes, and
        // returns the next sample.
        float sine_signal = osc.Process();

        // Sample input
        float sample = in[0][i];
        
        // Apply drop
        if (drop_enabled) {
            sample = Drop(curr_drop_index);
        }

        // Apply delay
        if (delay_steps != DELAY_STEPS_DISABLED) {
            sample += history[(curr_history_index + HISTORY_LENGTH - (delay_steps * DELAY_STEP_SIZE)) % HISTORY_LENGTH] * delay_decay;
        }
        
        // update history with processed signal
        history[curr_history_index] = sample;

        // send to WavWriter if recording
        if (rec_state.is_recording) {
            rec_state.writer.Sample(&sample);
        }

        // Set set output to processed signal
        out[0][i] = sample;
        out[1][i] = sine_signal;
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

WriteReturnCode write_test(const char* filename) {
    // Initialize the SDMMC interface and FatFS drivers
    FatFSInterface::Config fsi_cfg;
    fsi_cfg.media = MEDIA_TYPE;
    fsi.Init(fsi_cfg);
    auto res = f_mount(& GET_STORAGE_FILE_SYSTEM,
        GET_STORAGE_PATH,
        0);
    if (res != FR_OK)
    {
        // If some error is encountered in mounting the card, then simply return.
        return WriteReturnCode(0, res);
    }
    for (int i = 0; i < 100; i++) {
        //usb_host.Process();
    }

    //hw.SetLed(false);
    hw.DelayMs(500);
    //hw.SetLed(true);
    hw.DelayMs(500);

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
        // If some error is encountered in opening the file, then simply return.
        f_close(&file);
        return WriteReturnCode(1, res);
    }

    //hw.SetLed(false);
    hw.DelayMs(500);
    //hw.SetLed(true);
    hw.DelayMs(500);


    UINT bytes_written = 0;
    constexpr size_t BUFFER_SIZE = 7;
    char buffer[BUFFER_SIZE] = "hello!";
    res = f_write(&file, buffer, BUFFER_SIZE, &bytes_written);
    if (res != FR_OK)
    {
        // If some error is encountered in opening the file, then simply return.
        f_close(&file);
        return WriteReturnCode(2, res);
    }
    f_close(&file);

    //hw.SetLed(false);
    hw.DelayMs(500);
    //hw.SetLed(true);
    hw.DelayMs(500);
    return WriteReturnCode(2, res);
}
//void write_test(const char* filename) {}

void ActiveCallback(void *userdata) {}
void ConnectCallback(void *data) {hw.SetLed(true);}
void DisconnectCallback(void *data) {hw.SetLed(false);}
void ErrorCallback(void *data) {}

int main(void)
{
    hw.Init();
    // We initialize the oscillator with the sample rate of the hardware
    // this ensures that the frequency of the Oscillator will be accurate.
    osc.Init(hw.AudioSampleRate());
    hw.SetAudioBlockSize(16);

    WavWriter<16384>::Config wav_cfg = {
        hw.AudioSampleRate(),
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
    
    Blink(5);

    // Initialize the SDMMC interface and FatFS drivers
    FatFSInterface::Config fsi_cfg;
    fsi_cfg.media = MEDIA_TYPE;
    fsi.Init(fsi_cfg);
    auto res = f_mount(& GET_STORAGE_FILE_SYSTEM,
        GET_STORAGE_PATH,
        0);
    // Communicate results
    if (res == FR_OK) {
        rec_state.mounted = true;
        Blink(5);
    } else {
        rec_state.mounted = false;
        hw.DelayMs(1000);
    }

    //WriteReturnCode out = write_test(name);
    //hw.PrintLine("%d : CODE(%d)", out.stage, out.result);
    uint32_t ms = 0;

    std::string storage_path = GET_STORAGE_PATH;

    while(1) {
        //usb_host.Process();
        if (ms == 1000) {
            if (AttemptStartRecording(storage_path, rec_state) == REC_OK) {
                hw.SetLed(true);
            };
        }

        rec_state.writer.Write();

        if (ms == 10000) {
            StopRecording(rec_state);
            hw.SetLed(false);
        }
        hw.DelayMs(1);
        ms++;
        if (ms > 1 << 30) {
            ms = 1 << 30;
        }
    }

}

/*#include "daisy_seed.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;

// Declare a DaisySeed object called hardware
DaisySeed hardware;


// These classes are necessary for interacting with FatFS
SdmmcHandler sd;
FatFSInterface fsi;
FIL file;

#define SAMPLE_RATE 48000
#define WAV_CHANNELS 1
#define WAV_BITS_PER_SAMPLE 12

// Powers of two are best for this data buffer to keep QSPI
// writes aligned to the sectors.
#define WRITE_BUFFER_LEN 4096
uint8_t write_buffer[WRITE_BUFFER_LEN];

// Change this to the file you'd like to copy
#define FILE_NAME "read_file.wav"
#define WFILE_NAME "written_file.wav"

// You can find the information on the QSPI's length
// in the linker scripts, among other places
#define QSPI_LEN     0x00800000U

#define SECTOR_SIZE  65536

void write_wav(const char* filename);

void AudioCallback(
    AudioHandle::InputBuffer in,
    AudioHandle::OutputBuffer out,
    size_t size
    ) {
    // output silence
    // std::memcpy(&out[0][0], &in[0][0], size * sizeof(float));
    // std::memcpy(&out[0][0], &in[0][0], size * sizeof(float));
    // std::memcpy(&out[1][0], &in[1][0], size * sizeof(float));
    
    std::fill(&out[0][0], &out[0][size], 0.5f);
    hardware.SetLed(false);
}

// TODO : Convert to USB
void write_wav(const char* filename, const char*)
{
    // Initialize the SDMMC interface and FatFS drivers
    fsi.Init(FatFSInterface::Config::MEDIA_USB);

    if (f_mount(&fsi.GetSDFileSystem(), fsi.GetSDPath(), 1) != FR_OK)
    {
        // If some error is encountered in mounting the card, then simply return.
        return;
    }

    if (f_open(&file, filename, FA_OPEN_EXISTING | FA_READ) != FR_OK)
    {
        // If some error is encountered in opening the file, then simply return.
        f_close(&file);
        return;
    }

    UINT data_read;
    WAV_FormatTypeDef wav_header;

    // This will populate the wav_header struct with all the data we 
    // need to parse the wav file. Currently, the code doesn't actually use this header,
    // but you could write code that converts from the
    // several possible WAV storage types to int16_t or float for consistency.
    // It seems easier to simply ensure your WAV files are all in the same format, though.

    // You could also write this header along with the audio data if you'd like to
    // parse the wav file in your actual project.

    f_read(&file, &wav_header, sizeof(wav_header), &data_read);

    // This helps us keep track of the QSPI address to write to
    // between file data chunks. You can set its initial value to
    // whatever you want -- you'll just need to remember where that
    // is so your target application can load it from QSPI.
    uint32_t current_qspi_offset = 0;

    do
    {
        // This avoids attempted writes beyond the QSPI's address space
        if (current_qspi_offset >= QSPI_LEN)
            break;
        
        // The QSPI chip must be erased before any writes can be made,
        // and it can only be erased in certain sizes (4K, 32K, 64K, and full chip erase).
        // 64K is the fastest per byte without being too large, so that's what we'll use.
        if (current_qspi_offset % SECTOR_SIZE == 0)
        {
            hardware.qspi.Erase(current_qspi_offset, current_qspi_offset + SECTOR_SIZE);
        }
        f_read(&file, write_buffer, WRITE_BUFFER_LEN, &data_read);

        hardware.qspi.Write(current_qspi_offset, data_read, write_buffer);
        current_qspi_offset += data_read;
    } while (data_read == WRITE_BUFFER_LEN);

    f_close(&file);
}

int main(void)
{
    // Declare a variable to store the state we want to set for the LED.
    bool led_state;
    led_state = true;

    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hardware.Configure();
    hardware.Init();
    // hardware.usb_handle.Init(UsbHandle::FS_EXTERNAL);
    hardware.SetLed(true);

    // start audio
    size_t blocksize = 4;
    System::Delay(1000);
    hardware.SetAudioBlockSize(blocksize);
    hardware.StartAudio(AudioCallback);

    while (true) {
    };

    // wav writing
    WavWriter<32768> writer;
    
    WavWriter<32768>::Config writer_cfg = {
        SAMPLE_RATE,
        WAV_CHANNELS,
        WAV_BITS_PER_SAMPLE
    };

    writer.Init(writer_cfg);
    
    // testing basic writing
    if (false) {
        char* filename = "stupid.txt";

        write_test(filename);
    }

    // testing wav file writing
    if (false) {
        char* wav_file = "test.wav";
        writer.OpenFile(wav_file);

        float sample;
        for (int d = -65792; d < 65792; d++) {
            sample = float(d) * (0.5 / 65792.0);
            writer.Sample(&sample); // adds to buffer
            writer.Write(); // writes if it needs to
        }

        writer.SaveFile();
    }

    // Loop forever
    for(;;)
    {
        continue;
        // Set the onboard LED
        hardware.SetLed(led_state);

        // Toggle the LED state for the next time around.
        led_state = !led_state;

        // Wait 500ms
        System::Delay(100);
    }
}
*/