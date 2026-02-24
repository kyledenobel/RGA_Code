/**
 * @brief GPIO library for STM8L152K4T6
 */

#ifndef GPIO_H
#define GPIO_H

#include "stdint.h"
#include "error.h"
#include "STM8L152K4T6.h"

typedef enum {
    GPIO_PORT_A = (uint8_t) GPIOA,
    GPIO_PORT_B = (uint8_t) GPIOB,
    GPIO_PORT_C = (uint8_t) GPIOC,
    GPIO_PORT_D = (uint8_t) GPIOD,
    GPIO_PORT_E = (uint8_t) GPIOE,
    GPIO_PORT_F = (uint8_t) GPIOF
} gpio_port_t;


typedef enum {
    GPIO_PIN_0 = 0x1,
    GPIO_PIN_1 = 0x2,
    GPIO_PIN_2 = 0x4,
    GPIO_PIN_3 = 0x8,
    GPIO_PIN_4 = 0x10,
    GPIO_PIN_5 = 0x20,
    GPIO_PIN_6 = 0x40,
    GPIO_PIN_7 = 0x80
} gpio_pin_t;


typedef enum {
    GPIO_INPUT,
    GPIO_OUTPUT
} gpio_dir_t;


/**
 * @brief values for CR1 register
 * @param INPUT_FLOATING___OUTPUT_OPEN_DRAIN if input pin, floating. if output pin, open drain
 * @param INPUT_PULLUP___OUTPUT_PUSH_PULL if input, pullup. if output, push pull
 */
typedef enum {
    INPUT_FLOATING___OUTPUT_OPEN_DRAIN,
    INPUT_PULLUP___OUTPUT_PUSH_PULL
} gpio_cr1_t;


/**
 * @brief values for CR2
 * @param INPUT_INT_DIS___OUTPUT_SLOW_SPEED if input pin, interrupts disabled. If output pin, max speed 2 MHz
 * @param INPUT_INT_EN___OUTPUT_HIGH_SPEED if input pin, interrupts enabled. If output pin, max speed 10 MHz
 */
typedef enum {
    INPUT_INT_DIS___OUTPUT_SLOW_SPEED,
    INPUT_INT_EN___OUTPUT_HIGH_SPEED
} gpio_cr2_t;



typedef struct {
    gpio_port_t port;
    gpio_pin_t pin;
} gpio_t;



/* ============== Function Defines ============== */
error_t init_pin(void);

uint8_t read_pin(void);

void write_pin(void);


#endif