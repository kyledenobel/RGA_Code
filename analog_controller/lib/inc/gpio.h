/**
 * @brief GPIO library for STM8L152K4T6
 */

#ifndef GPIO_H
#define GPIO_H

#include "stdint.h"
#include "stdbool.h"
#include "error.h"
#include "STM8L152K4T6.h"

typedef void (*gpio_interrupt_t)(void);

typedef enum {
    GPIO_PORT_A = (uint16_t) GPIOA,
    GPIO_PORT_B = (uint16_t) GPIOB,
    GPIO_PORT_C = (uint16_t) GPIOC,
    GPIO_PORT_D = (uint16_t) GPIOD,
    GPIO_PORT_E = (uint16_t) GPIOE,
    GPIO_PORT_F = (uint16_t) GPIOF
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
    GPIO_INPUT = 0x0,
    GPIO_OUTPUT = 0x1
} gpio_dir_t;


/**
 * @brief values for CR1 register
 * @param INPUT_FLOATING___OUTPUT_OPEN_DRAIN if input pin, floating. if output pin, open drain
 * @param INPUT_PULLUP___OUTPUT_PUSH_PULL if input, pullup. if output, push pull
 */
typedef enum {
    INPUT_FLOATING___OUTPUT_OPEN_DRAIN = 0x0,
    INPUT_PULLUP___OUTPUT_PUSH_PULL = 0x1
} gpio_cr1_t;


/**
 * @brief values for CR2
 * @param INPUT_INT_DIS___OUTPUT_SLOW_SPEED if input pin, interrupts disabled. If output pin, max speed 2 MHz
 * @param INPUT_INT_EN___OUTPUT_HIGH_SPEED if input pin, interrupts enabled. If output pin, max speed 10 MHz
 */
typedef enum {
    INPUT_INT_DIS___OUTPUT_SLOW_SPEED = 0x0,
    INPUT_INT_EN___OUTPUT_HIGH_SPEED = 0x1
} gpio_cr2_t;


typedef enum {
    GPIO_LOW_LEVEL = 0x0,
    GPIO_RISING_EDGE = 0x1,
    GPIO_FALLING_EDGE = 0x2,
    GPIO_BOTH_EDGES = 0x3
} gpio_edge_t;


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


typedef struct {
    gpio_interrupt_t interrupt;
    gpio_edge_t edge;
    gpio_pin_t pin;
} gpio_callback_init_t;



/* ============== Function Defines ============== */

/**
 * @brief initializes a GPIO pin
 * @param[out] gpio un-initialized gpio struct
 * @param[in] init pointer to a filled initialization struct
 * @retval NULL_POINTER -> if either input is NULL
 * @retval INVALID_ARG -> if invalid arguments are passed through init
 * @retval OK -> if function succeeds
 */
error_t gpio_pin_init(gpio_t* gpio, gpio_init_t* init);


/**
 * @brief returns the value of the gpio pin
 * @param[in] gpio initialized gpio struct
 * @retval true -> pin is high 
 * @retval false -> pin is low
 */
bool gpio_read_pin(gpio_t* gpio);

/**
 * @brief writes a value to a pin
 * @param[in] gpio initialized gpio struct
 * @param value value to write to pin
 */
void gpio_write_pin(gpio_t* gpio, bool value);

/**
 * @brief initializes a GPIO callback on a pin
 * @note all pins are tied to the same callback regardless of port
 */
error_t gpio_init_callback(gpio_callback_init_t* init);



/* ============== Interrupt Defines ============== */
void EXTI_EF_PVD_interrupt(void) __interrupt (5);
void EXTI_BG_interrupt(void) __interrupt (6);
void EXTI_DH_interrupt(void) __interrupt (7);
void EXTI0_interrupt(void) __interrupt (8);
void EXTI1_interrupt(void) __interrupt (9);
void EXTI2_interrupt(void) __interrupt (10);
void EXTI3_interrupt(void) __interrupt (11);
void EXTI4_interrupt(void) __interrupt (12);
void EXTI5_interrupt(void) __interrupt (13);
void EXTI6_interrupt(void) __interrupt (14);
void EXTI7_interrupt(void) __interrupt (15);

#endif