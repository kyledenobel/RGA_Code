#include "gpio.h"
#include "stddef.h"

// Setup for keeping track of interrupts
#define GPIO_NUM                                8
gpio_interrupt_t gpio_interrupt_handler_arr[GPIO_NUM];


/* ====================================================== */
/* ===================== HELPER MACROS ================== */
/* ====================================================== */
/**
 * @brief checks if the port is a valid GPIO port
 * @param port a GPIO port
 */
#define VALID_PORT(port) \
            ((port == GPIO_PORT_A) ||\
            (port == GPIO_PORT_B) ||\
            (port == GPIO_PORT_C) ||\
            (port == GPIO_PORT_D) ||\
            (port == GPIO_PORT_E) ||\
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


/**
 * @brief checks if the edge selection if valid for GPIO
 * @param edge GPIO edge selection for an interrupt
 */
#define VALID_EDGE(edge) \
            ((edge == GPIO_LOW_LEVEL) ||\
             (edge == GPIO_RISING_EDGE) ||\
             (edge == GPIO_FALLING_EDGE) ||\
             (edge == GPIO_BOTH_EDGES))

/* ====================================================== */
/* ================== FUNCTION DEFINTIONS =============== */
/* ====================================================== */

error_t gpio_pin_init(gpio_t* gpio, gpio_init_t* init) {
    /* check params */
    // if((gpio == NULL) || (init == NULL)) {
    //     return NULL_POINTER;
    // }
    // if((!VALID_PORT(init->port)) || (!VALID_PIN(init->pin))) {
    //     return INVALID_ARG;
    // }
    // if(!VALID_DIR(init->dir)) {
    //     return INVALID_ARG;
    // }
    // if((!VALID_CR1(init->cr1)) || (!VALID_CR2(init->cr2))) {
    //     return INVALID_ARG;
    // }

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

bool gpio_read_pin(gpio_t* gpio) {
    return (bool) READ_BIT(gpio->port->IDR, gpio->pin);
}

void gpio_write_pin(gpio_t* gpio, bool value) {
    if(value) {
        SET_BIT(gpio->port->ODR, gpio->pin);
    }
    else {
        CLEAR_BIT(gpio->port->ODR, gpio->pin);
    }
}

error_t gpio_init_callback(gpio_callback_init_t* init) {
    /* check valid initialization struct */
    if((init == NULL) || (init->interrupt)) {
        return NULL_POINTER;
    }
    if(!VALID_PIN(init->pin)) {
        return INVALID_ARG;
    }
    if(!VALID_EDGE(init->edge)) {
        return INVALID_ARG;
    }
    /* config registers */
    uint8_t pos = init->pin;
    for(uint8_t i = 0; i < 4; i++) {
        pos = pos >> 1;
        if(pos == 0) {
            SET_BIT(EXTI->CR1, (init->edge) << (i << 1));
            gpio_interrupt_handler_arr[i] = init->interrupt;
            break;
        }
    }
    if(pos != 0) {
        for(uint8_t i = 0; i < 4; i++) {
            pos = pos >> 1;
            if(pos == 0) {
                SET_BIT(EXTI->CR2, (init->edge) << (i << 1));
                gpio_interrupt_handler_arr[i + 4] = init->interrupt;
                break;
            }
        }
    }

    return OK;
}


@inline void exti_interrupt_helper(pin) {
    /* clear interrupt */
    SET_BIT(EXTI->SR1, (1 << pin));
    /* get handler */
    gpio_interrupt_handler_arr[pin]();
}

/**
 * @brief interrupt for ALL PORT E and F pins
 */
@interrupt void EXTI_EF_PVD_interrupt(void) //__interrupt (5)
{
    // Not Implemented
}

/**
 * @brief interrupt for ALL PORT B pins
 */
@interrupt void EXTI_BG_interrupt(void) //__interrupt (6)
{
    // Not Implemented
}

/**
 * @brief interrupt for ALL PORT D pins
 */
@interrupt void EXTI_DH_interrupt(void) //__interrupt (7)
{
    // Not Implemented
}

/**
 * @brief interrupt for pin 0 on all ports
 */
@interrupt void EXTI0_interrupt(void) //__interrupt (8)
{
    exti_interrupt_helper(0);
}

/**
 * @brief interrupt for pin 1 on all ports
 */
@interrupt void EXTI1_interrupt(void) //__interrupt (9)
{
    exti_interrupt_helper(1);
}

/**
 * @brief interrupt for pin 2 on all ports
 */
@interrupt void EXTI2_interrupt(void) //__interrupt (10)
{
    exti_interrupt_helper(2);
}

/**
 * @brief interrupt for pin 3 on all ports
 */
@interrupt void EXTI3_interrupt(void) //__interrupt (11)
{
    exti_interrupt_helper(3);
}

/**
 * @brief interrupt for pin 4 on all ports
 */
@interrupt void EXTI4_interrupt(void) //__interrupt (12)
{
    exti_interrupt_helper(4);
}

/**
 * @brief interrupt for pin 5 on all ports
 */
@interrupt void EXTI5_interrupt(void) //__interrupt (13)
{
    exti_interrupt_helper(5);
}

/**
 * @brief interrupt for pin 6 on all ports
 */
@interrupt void EXTI6_interrupt(void) //__interrupt (14)
{
    exti_interrupt_helper(6);
}

/**
 * @brief interrupt for pin 7 on all ports
 */
@interrupt void EXTI7_interrupt(void) //__interrupt (15)
{
    exti_interrupt_helper(7);
}