#ifndef TUNER_H
#define TUNER_H


constexpr float TUNER_TARGET_REL_AMP_PEAK = 0.9;
constexpr float TUNER_TARGET_REL_AMP_LOW = 0.2;

constexpr float ROLLING_AVERAGE_RATIO = 0.4;

constexpr size_t DELTA_HISTORY_SIZE = 5;
constexpr size_t DELTA_HISTORY_TARGET = (DELTA_HISTORY_SIZE) / 2;

constexpr float cutoff = 1e-3;

template<size_t MAX_SAMPLES>
class Tuner {
    size_t fs;
    size_t min_sample_gap;

    // history stuff
    float history[MAX_SAMPLES] = { 0 };
    float curr_max = 0;
    float curr_min = 0;

    // targets for detecting peaks
    float high_target = 1;
    float mid_target = 0.2;
    float low_target = -1;

    // History Index
    size_t h_i = 0;
    size_t last_peak = 0;

    bool start_of_sector = true;

    // outlier removal
    size_t delta_history[DELTA_HISTORY_SIZE] = { 0 };

    // rolling average
    float rolling_average = 0.0;

    float curr_freq = 0.0;


    inline void history_iterate(float sample) {
        // Processing for next thing
        history[h_i] = sample;
        curr_max = sample > curr_max ? sample : curr_max;
        curr_min = sample > curr_min ? sample : curr_max;

        h_i = (h_i + 1) % MAX_SAMPLES;
    }

    inline size_t iterate_delta_history(size_t delta) {
        float sorted_arr[DELTA_HISTORY_SIZE];

        // insert new guy
        for (size_t i = DELTA_HISTORY_SIZE; i > 0; i--) {
            delta_history[i] = delta_history[i-1];
            sorted_arr[i] = delta_history[i-1];
        }
        delta_history[0] = delta;
        sorted_arr[0] = delta;

        size_t temp;

        // sort until we got what we want (bubble cause this is small) (~ 12 operations max with a total array size of 5)
        for (size_t sorted = 0; sorted < DELTA_HISTORY_TARGET; sorted++) {
            for(size_t i = DELTA_HISTORY_SIZE; i > sorted; i--) {
                if (sorted_arr[i] < sorted_arr[i-1]) {
                    // maybe swap for xor swap? compiler should do it itself tho.
                    temp = sorted_arr[i-1];
                    sorted_arr[i-1] = sorted_arr[i];
                    sorted_arr[i] = temp;
                }
            }
        }

        return sorted_arr[DELTA_HISTORY_TARGET];
    }

    inline void rolling_iterate(float delta) {
        rolling_average = (1.0 - ROLLING_AVERAGE_RATIO) * rolling_average + ROLLING_AVERAGE_RATIO * delta;
        curr_freq = fs/rolling_average;
    }


    bool reached_high = false;
    bool reached_low = false;

public:

    Tuner() {
        fs = 96000;
        size_t max_freq = 600;
        min_sample_gap = fs / max_freq;
    }

    Tuner(size_t fs) {
        this->fs = fs;
        size_t max_freq = 600;
        min_sample_gap = fs / max_freq;
    }

    void Init(size_t fs) {
        this->fs = fs;
        size_t max_freq = 600;
        min_sample_gap = fs / max_freq;
    }

    void Sample(float sample) {
        // Handle sectors
        if (h_i == 0) {
            start_of_sector = true;
            high_target = curr_max * TUNER_TARGET_REL_AMP_PEAK;
            low_target = curr_min * TUNER_TARGET_REL_AMP_PEAK;
            mid_target = abs(curr_max) + abs(curr_min) * TUNER_TARGET_REL_AMP_LOW / 2.0;
            curr_max = 0;
            curr_min = 0;
        }

        if (high_target < cutoff) {
            if (start_of_sector) {
                rolling_iterate(0);
                start_of_sector = false;
            }
            // deal with history (required at the end of all return paths)
            history_iterate(sample);
            return;
        }

        // Peak detection
        float s = history[h_i]; // get sample from history (we have to wait for min / max data)
        reached_high = s > high_target;

        if (reached_high && reached_low) {
            reached_low = false;
            if (start_of_sector) {
                start_of_sector = false;
                last_peak = h_i;
            } else {
                size_t delta = h_i - last_peak;
                if (delta > min_sample_gap) {
                    last_peak = h_i;

                    delta = iterate_delta_history(delta);

                    rolling_iterate(delta);
                }
            }
        }

        if (sample < low_target) {
            reached_low = true;
        }
        
        // deal with history (required at the end of all return paths)
        history_iterate(sample);
        return;
    }

    float get_curr_freq() const {
        return curr_freq;
    }
};

#endif