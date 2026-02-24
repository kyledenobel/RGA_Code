#include "gpio.h"
#include "interrupt.h"

int main(void)
{
    init_pin();
    write_pin();
    while(1);
}