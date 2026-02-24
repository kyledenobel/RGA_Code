/**
 * @brief GPIO library for STM8L152K4T6
 */

#ifndef GPIO_H
#define GPIO_H

#include "stdint.h"
#include "stdbool.h"
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
    GPIO_PIN_0 = (uint8_t) 0x1,
    GPIO_PIN_1 = (uint8_t) 0x2,
    GPIO_PIN_2 = (uint8_t) 0x4,
    GPIO_PIN_3 = (uint8_t) 0x8,
    GPIO_PIN_4 = (uint8_t) 0x10,
    GPIO_PIN_5 = (uint8_t) 0x20,
    GPIO_PIN_6 = (uint8_t) 0x40,
    GPIO_PIN_7 = (uint8_t) 0x80
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
    gpio_typedef_t* port;
    gpio_pin_t pin;
} gpio_t;


typedef struct {
    gpio_port_t port;
    gpio_pin_t pin;
    gpio_dir_t dir;
    gpio_cr1_t cr1;
    gpio_cr2_t cr2;
} gpio_init_t;



/* ============== Function Defines ============== */

/**
 * @brief initializes a GPIO pin
 * @param[in] gpio un-initialized gpio struct
 * @param[in] init pointer to a filled initialization struct
 * @retval NULL_POINTER -> if either input is NULL
 * @retval INVALID_ARG -> if invalid arguments are passed through init
 * @retval OK -> if function succeeds
 */
error_t init_pin(gpio_t* gpio, gpio_init_t* init);


/**
 * @brief returns the value of the gpio pin
 * @param gpio initialized gpio struct
 * @retval true -> pin is high 
 * @retval false -> pin is low
 */
bool read_pin(gpio_t* gpio);

/**
 * @brief writes a value to a pin
 * @param gpio initialized gpio struct
 * @param value value to write to pin
 */
void write_pin(gpio_t* gpio, bool value);


#endif