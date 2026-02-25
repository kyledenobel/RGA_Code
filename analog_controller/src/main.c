#include "gpio.h"
#include "interrupt.h"

int main(void)
{
    gpio_t pA0;
    gpio_init_t init = {.pin = GPIO_PIN_0,
                        .port = GPIO_PORT_A,
                        .dir = GPIO_OUTPUT,
                        .cr1 = INPUT_PULLUP___OUTPUT_PUSH_PULL,
                        .cr2 = INPUT_INT_EN___OUTPUT_HIGH_SPEED};


    gpio_pin_init(&pA0, &init);

    while (true)
    {
        gpio_write_pin(&pA0, 1);
    }
    
}