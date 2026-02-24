#include "gpio.h"
#include "stddef.h"

/* ====================================================== */
/* ===================== HELPER MACROS ================== */
/* ====================================================== */
/**
 * @brief checks if the port is a valid GPIO port
 * @param port a GPIO port
 */
#define VALID_PORT(port) \
            ((port == GPIO_PORT_A) || \
            (port == GPIO_PORT_B) || \
            (port == GPIO_PORT_C) || \
            (port == GPIO_PORT_D) || \
            (port == GPIO_PORT_E) || \
            (port == GPIO_PORT_F))

/**
 * @brief checks if the pin is a valid GPIO pin
 * @param pin a GPIO pin
 */
#define VALID_PIN(pin) \
            ((pin == GPIO_PIN_0) ||\
            (pin == GPIO_PIN_1) ||\
            (pin == GPIO_PIN_2) ||\
            (pin == GPIO_PIN_3) ||\
            (pin == GPIO_PIN_4) ||\
            (pin == GPIO_PIN_5) ||\
            (pin == GPIO_PIN_6) ||\
            (pin == GPIO_PIN_7))

/**
 * @brief checks if the direction is a valid GPIO dir
 * @param dir GPIO direction
 */
#define VALID_DIR(dir) \
            ((dir == GPIO_INPUT) ||\
             (dir == GPIO_OUTPUT))

/**
 * @brief checks if the cr1 selection is a valid GPIO cr1 value
 * @param cr1 GPIO cr1 selection
 */
#define VALID_CR1(cr1) \
            ((cr1 == INPUT_FLOATING___OUTPUT_OPEN_DRAIN) ||\
             (cr1 == INPUT_PULLUP___OUTPUT_PUSH_PULL))

/**
 * @brief checks if the cr2 selection is a valid GPIO cr2 value
 * @param cr2 GPIO cr2 selection
 */
#define VALID_CR2(cr2) \
            ((cr2 == INPUT_INT_DIS___OUTPUT_SLOW_SPEED) ||\
             (cr2 == INPUT_INT_EN___OUTPUT_HIGH_SPEED))


/* ====================================================== */
/* ================== FUNCTION DEFINTIONS =============== */
/* ====================================================== */

error_t init_pin(gpio_t* gpio, gpio_init_t* init) {
    /* check params */
    if((gpio == NULL) || (init == NULL)) {
        return NULL_POINTER;
    }
    if((!VALID_PORT(init->port)) || (!VALID_PIN(init->pin))) {
        return INVALID_ARG;
    }
    if(!VALID_DIR(init->dir)) {
        return INVALID_ARG;
    }
    if((!VALID_CR1(init->cr1)) || (!VALID_CR2(init->cr2))) {
        return INVALID_ARG;
    }

    /* initialize gpio struct */
    gpio->port = (gpio_typedef_t*) init->port;
    gpio->pin = init->pin;

    /* setup registers */
    if(init->dir == GPIO_OUTPUT) {
        SET_BIT(gpio->port->DDR, gpio->pin);
    }
    if(init->cr1 == INPUT_PULLUP___OUTPUT_PUSH_PULL) {
        SET_BIT(gpio->port->CR1, gpio->pin);
    }
    if(init->cr2 == INPUT_INT_EN___OUTPUT_HIGH_SPEED) {
        SET_BIT(gpio->port->CR2, gpio->pin);
    }

    return OK;
}

bool read_pin(gpio_t* gpio) {
    return (bool) READ_BIT(gpio->port->IDR, gpio->pin);
}

void write_pin(gpio_t* gpio, bool value) {
    if(value) {
        SET_BIT(gpio->port->ODR, gpio->pin);
    }
    else {
        CLEAR_BIT(gpio->port->ODR, gpio->pin);
    }
}

