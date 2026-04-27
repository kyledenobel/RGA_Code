#ifndef INPUTS_H
#define INPUTS_H 1

namespace Directions {
    enum Directions {
        Center,
        Up,
        Down,
        Left,
        Right
    };
}

struct InputInstance {
    // uses Direction Enum
    int8_t dir = Directions::Center;
    // selection button
    bool sel = false;
};

struct Debouncer {
    uint16_t counter = 0;
    bool state = false;

    inline void next(bool pressed) {
        if (pressed) {
            if (counter >= 32) {
                state = true;
            } else {
                counter = counter + 1;
            }
        } else {
            if (counter > 0) {
                counter -= 1;
            } else {
                state = false;
            }
        }
    }
};

struct InputHandler {
    InputInstance curr;
    InputInstance prev;
    
    InputInstance next_tick(InputInstance curr_inputs) {
        prev = curr;
        curr = curr_inputs;
        InputInstance pressed = { curr.dir != prev.dir ? curr.dir : Directions::Center, curr.sel != prev.sel ? curr.sel : false };
        return pressed;
    }
};

#endif