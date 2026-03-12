//#include "gpio.h"
#include "STM8L152K4T6.H"

int main(void)
{
    // gpio_t pb5;
    // gpio_init_t init = {.pin = GPIO_PIN_7,
    //                     .port = GPIO_PORT_A,
    //                     .dir = GPIO_OUTPUT,
    //                     .cr1 = INPUT_PULLUP___OUTPUT_PUSH_PULL,
    //                     .cr2 = INPUT_INT_EN___OUTPUT_HIGH_SPEED};

    


    // gpio_pin_init(&pb5, &init);
    while (1)
    {
        // gpio_write_pin(&pb5, 1);
        GPIOA->CR1 |= 0x80;
        GPIOA->CR2 |= 0x80;
        GPIOA->DDR |= 0x80;
        GPIOA->ODR |= 0x80;
    }
    
}