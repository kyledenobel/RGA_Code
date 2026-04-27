#include "uLCD.h"

using namespace daisy;
using namespace uLCD;

void Display::Init(DaisySeed *hw, Pin tx_pin, Pin rx_pin, UartHandler::Config::Peripheral peripheral, Pin rst_pin, BAUDS baud) {
    hw = hw;

    rst.Init(rst_pin, GPIO::Mode::OUTPUT_OD, GPIO::Pull::NOPULL, GPIO::Speed::LOW);
    rst.Write(true); // Pull Reset High (Off)

    // Configure the Uart Peripheral
    uart_conf = UartHandler::Config();
    uart_conf.periph        = peripheral;
    uart_conf.mode          = UartHandler::Config::Mode::TX_RX;
    uart_conf.baudrate = BaudCodetoRate(BAUDS::SLOW);
    uart_conf.parity = UartHandler::Config::Parity::NONE;
    uart_conf.stopbits = UartHandler::Config::StopBits::BITS_1;
    uart_conf.pin_config.tx = tx_pin;
    uart_conf.pin_config.rx = rx_pin;

    // Initialize the uart peripheral and start the DMA transmit
    uart.Init(uart_conf);

    Reset();
    Clear();

    hw->DelayMs(10);

    if (baud != BAUDS::SLOW) {
        SetBaud(baud);
    }

    hw->SetLed(true);
    hw->DelayMs(500);

    SetFont(DEFAULT_FONT);
    hw->SetLed(false);
    hw->DelayMs(500);
}

Result Display::WriteSpaced(uint8_t * bytes, uint32_t length) {
    for (uint32_t i = 0; i < length - 1; i++) {
        // Write a byte
        if (Write(bytes[i]) == Result::ERR) return Result::ERR;
        // Wait a moment
        hw->DelayMs(SPACED_DELAY_MS);
    }
    return Write(bytes[length - 1]);
}

Result Display::WriteCMD(uint8_t byte) {
    // Command time
    if (Write(CMD_CODE) == Result::ERR) return Result::ERR;
    hw->DelayMs(SPACED_DELAY_MS);
    if (Write(byte) == Result::ERR) return Result::ERR;
    // Do we get a response?
    return CheckAck();
}

Result Display::WriteCMD(uint8_t * bytes, uint32_t length) {
    // Command time
    if (Write(CMD_CODE) == Result::ERR) return Result::ERR;
    hw->DelayMs(SPACED_DELAY_MS);
    // We change behavior to avoid overloading the mbed for large commands
    if (length < 16) {
        if (Write(bytes, length) == Result::ERR) return Result::ERR;
    } else {
        if (Write(bytes, 15) == Result::ERR) return Result::ERR;
        if (WriteSpaced(bytes + 15, length - 15) == Result::ERR) return Result::ERR;
    }
    // Do we get a response?
    return CheckAck();
}

Result Display::WriteCMDNull(uint8_t byte) {
    // Command time
    if (Write(CMD_NULL_CODE) == Result::ERR) return Result::ERR;
    hw->DelayMs(SPACED_DELAY_MS);
    if (Write(byte) == Result::ERR) return Result::ERR;
    // Do we get a response?
    return CheckAck();
}

Result Display::WriteCMDNull(uint8_t * bytes, uint32_t length) {
    // Command time
    if (Write(CMD_NULL_CODE) == Result::ERR) return Result::ERR;
    // We change behavior to avoid overloading the mbed for large commands
    if (length < 16) {
        if (Write(bytes, length) == Result::ERR) return Result::ERR;
    } else {
        if (Write(bytes, 15) == Result::ERR) return Result::ERR;
        if (WriteSpaced(bytes + 15, length - 15) == Result::ERR) return Result::ERR;
    }
    // Do we get a response?
    return CheckAck();
}

Result Display::Clear() {
    Result res = WriteCMD(CLEAR_CMD);
    //CheckAck();
    hw->DelayMs(CLEAR_DELAY_MS);
    return res;
}
Result Display::Rect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color, bool filled) {
    uint8_t cmd[11] = { 
        filled ? FRECT_CMD : RECT_CMD, 
        (x1 >> 8) & 0xFF, x1 & 0xFF, 
        (y1 >> 8) & 0xFF, y1 & 0xFF,
        (x2 >> 8) & 0xFF, x2 & 0xFF,
        (y2 >> 8) & 0xFF, y2 & 0xFF,
        (color >> 8) & 0xFF, color & 0xFF
    };

    if (WriteCMD(cmd, 11) == Result::ERR) return Result::ERR;
    return CheckAck();
}
Result Display::Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color) {
    uint8_t cmd[11] = { 
        LINE_CMD, 
        (x1 >> 8) & 0xFF, x1 & 0xFF, 
        (y1 >> 8) & 0xFF, y1 & 0xFF,
        (x2 >> 8) & 0xFF, x2 & 0xFF,
        (y2 >> 8) & 0xFF, y2 & 0xFF,
        (color >> 8) & 0xFF, color & 0xFF
    };
    if (WriteCMD(cmd, 11) == Result::ERR) return Result::ERR;
    return CheckAck();
}

Result Display::SetFont(uint8_t font) {
    uint8_t cmd[3]= {
        SET_FONT_CMD,
        0,
        font
    };
    if (WriteCMD(cmd, 3) == Result::ERR) return Result::ERR;
    return CheckAck();

}

Result Display::SetTextColor(uint16_t color) {
    uint8_t cmd[3] = {
        SET_TXT_COLOR_CMD,
        (color >> 8) & 0xFF,
        color & 0xFF
    };
    if (WriteCMD(cmd, 3) == Result::ERR) return Result::ERR;
    return CheckAck();
}
Result Display::SetTextBackground(uint16_t color) {
    uint8_t cmd[3] = {
        SET_TXT_BKG_CMD,
        (color >> 8) & 0xFF,
        color & 0xFF
    };
    if (WriteCMD(cmd, 3) == Result::ERR) return Result::ERR;
    return CheckAck();
}

Result Display::MoveCursor(int8_t row, int8_t col) {
    uint8_t cmd[5] = {
        MOVE_CURSOR_CMD,
        0,
        row,
        0,
        col
    };
    return WriteCMD(cmd, 5);
}


Result Display::Char(char c, int8_t col, int8_t row) {
    // Go to where text should go
    if (MoveCursor(row, col) == Result::ERR) {
        //return Result::ERR
    };
    // Wait a moment (may be not needed, test)
    hw->DelayMs(SPACED_DELAY_MS);

    uint8_t cmd[3] = {
        CHAR_CMD,
        0,
        c
    };
    return WriteCMD(cmd, 3);
}


Result Display::Char(char c, int8_t col, int8_t row, uint16_t color) {
    // text color
    if (SetTextColor(color) == Result::ERR) {
        //return Result::ERR
    };
    // Go to where text should go
    if (MoveCursor(row, col) == Result::ERR) {
        //return Result::ERR
    };
    // Wait a moment (may be not needed, test)
    hw->DelayMs(SPACED_DELAY_MS);

    uint8_t cmd[3] = {
        CHAR_CMD,
        0,
        c
    };
    return WriteCMD(cmd, 3);
}

Result Display::IndString(const char *s, int8_t col, int8_t row, uint16_t color) {
    if (SetTextColor(color) == Result::ERR) return Result::ERR;
    int size = strlen(s);
    if (size > MAX_IND_STRING_LEN) {
        size = MAX_IND_STRING_LEN;
    }
    int h_counter = 0;
    int indent_counter = 0;
    for (int32_t i = 0; i < size; i++) {
        if (s[i] == '\n') {
            indent_counter++;
            h_counter = 0;
            continue;
        }
        if (Char(s[i], col + h_counter, row + indent_counter) == Result::ERR) return Result::ERR;
        h_counter++;
    }
    return Result::OK;
}

Result Display::String(const char *s, int8_t col, int8_t row, uint16_t color) {
    // Go to where text should go
    if (MoveCursor(row, col) == Result::ERR) {
        //return Result::ERR;
    }
    // Wait a moment (may be not needed, test)
    hw->DelayMs(SPACED_DELAY_MS);

    // Set color (delay to give time to update before writing string)
    if (SetTextColor(color) == Result::ERR) {
        //return Result::ERR;
    }
    hw->DelayMs(SPACED_DELAY_MS);

    uint8_t cmd[MAX_STRING_CMD_LEN] = "";
    int size = strlen(s);
    if (size > MAX_STRING_CMD_LEN - 2) {
        size = MAX_STRING_CMD_LEN - 2;
    }

    cmd[0] = STRING_CMD;
    for (int32_t i = 0; i < size; i++) {
        cmd[1+i] = s[i];
    }
    cmd[size + 1] = 0;

    auto res = WriteCMDNull(cmd, 2 + size);
    hw->DelayMs(5);
    return res;
}



Result Display::SetBaud(BAUDS baudcode) {
    uint8_t cmd[3]= {
        BAUDRATE_CMD,
        (((uint16_t)baudcode) >> 8) & 0xFF,
        ((uint16_t)baudcode) & 0xFF
    };
    Write(CMD_NULL_CODE);
    hw->DelayMs(SPACED_DELAY_MS);
    Write(cmd[0]);
    Write(cmd[1]);
    Write(cmd[2]);
    

    hw->DelayMs(10);

    uart_conf.baudrate = BaudCodetoRate(baudcode);
    uart.~UartHandler(); // Manually deconstructs
    uart = UartHandler(); // make a new one
    uart.Init(uart_conf); // oh we back wowza
    hw->DelayMs(10);

    uint8_t ack = 0;
    uart.BlockingReceive(&ack, 1, 250);
    switch (ack) {
    case (ACK) :
        return Result::OK;
    case (NAK) :
        return Result::ERR;
    default:
        return Result::ERR;
    }
}