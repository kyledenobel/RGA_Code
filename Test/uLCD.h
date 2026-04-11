#ifndef uLCD_H
#define uLCD_H 1

#include "daisy_seed.h"
#include "daisysp.h"

#define TIMEOUT_MS 2
#define SPACED_DELAY_MS 1
#define CLEAR_DELAY_MS 5

#define RESET_TIME_MS 8
#define POST_RESET_DELAY_MS 3200

enum BAUDS {
	SLOW = 312,
	OK = 155,
	MEDIUM = 53,
	FAST = 25
};

constexpr uint16_t BaudCodetoRate(BAUDS baud) {
	switch (baud) {
		case(FAST):
			return 115200;
		case(MEDIUM):
			return 56000;
		case(OK):
			return 19200;
		case(SLOW):
		default:
		return 9600;
	}
}


#define ACK 0x06
#define NAK 0x15

#define CMD_CODE 0xFF
#define CMD_NULL_CODE 0x00
#define CLEAR_CMD 0xD7
#define FRECT_CMD 0xCE
#define RECT_CMD 0xCF
#define SET_FONT_CMD 0x7D
#define MOVE_CURSOR_CMD 0xE4
#define SET_TXT_COLOR_CMD 0x7F
#define SET_TXT_BKG_CMD 0x7E
#define STRING_CMD 0x06
#define BAUDRATE_CMD 0x0B
#define CHAR_CMD 0xFE
#define LINE_CMD 0xD2

#define DEFAULT_FONT 0x00

#define MAX_STRING_CMD_LEN 128
#define MAX_IND_STRING_LEN 512

namespace uLCD {

	typedef daisy::UartHandler::Result Result;

	constexpr int16_t ConvertColor(uint32_t color) {
		int red5   = (color >> (16 + 3)) & 0x1F;               // get red on 5 bits
		int green6 = (color >> (8 + 2))  & 0x3F;               // get green on 6 bits
		int blue5  = (color >> (0 + 3))  & 0x1F;               // get blue on 5 bits

		int16_t out = (((red5 << 3)   + (green6 >> 3)) & 0xFF) << 8;   // first part of 16 bits color
		out = out | (((green6 << 5) + (blue5 >>  0)) & 0xFF);  // second part of 16 bits color
		return out;
	}

	class Display {
		Result MoveCursor(int8_t row, int8_t col);
	public:
    	daisy::UartHandler::Config uart_conf;
		daisy::UartHandler uart;
		daisy::DaisySeed *hw;
		daisy::GPIO rst;
		
		void Init(daisy::DaisySeed *hw, daisy::Pin tx_pin, daisy::Pin rx_pin, daisy::UartHandler::Config::Peripheral peripheral, daisy::Pin rst_pin, BAUDS baud);

		inline void Reset() {
			hw->DelayMs(RESET_TIME_MS); // ensure everything is cleared out
			rst.Write(false); // Pull reset low (On)
			hw->DelayMs(RESET_TIME_MS); // Actually reset
			rst.Write(true); // Pull reset high (Off)
			hw->DelayMs(POST_RESET_DELAY_MS); // Wait for screen to reset
		}

		// Basic wrappers
		inline Result Write(uint8_t byte) {
			return uart.BlockingTransmit(&byte, 1, TIMEOUT_MS);
		}
		inline Result Write(uint8_t * bytes, uint32_t length) {
			Result res;
			for (uint32_t i = 0; i < length; i++) {
				res = uart.BlockingTransmit(&bytes[i], 1, TIMEOUT_MS);
				if (res == Result::ERR)
					return Result::ERR;
			}
			return res;
		}
		inline Result CheckAck() {
			uint8_t ack = 0;
			uart.BlockingReceive(&ack, 1, TIMEOUT_MS);
			switch (ack) {
			case (ACK) :
				return Result::OK;
			case (NAK) :
				return Result::ERR;
			default:
				return Result::ERR;
			}
		}
		
		Result WriteSpaced(uint8_t * bytes, uint32_t length);
		Result WriteCMD(uint8_t byte);
		Result WriteCMD(uint8_t * bytes, uint32_t length);
		Result WriteCMDNull(uint8_t byte);
		Result WriteCMDNull(uint8_t * bytes, uint32_t length);

		// Actual Commands

		Result Clear();

		Result Rect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color, bool filled);
		Result Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color);
		
		Result SetFont(uint8_t font);
		Result SetTextColor(uint16_t color);
		Result SetTextBackground(uint16_t color);
		Result String(const char *s, int8_t col, int8_t row, uint16_t color);
		Result Char(char c, int8_t col, int8_t row);
		Result Char(char c, int8_t col, int8_t row, uint16_t color);
		Result IndString(const char *s, int8_t col, int8_t row, uint16_t color);
		
		Result SetBaud(BAUDS baudcode);
	};

}

#endif