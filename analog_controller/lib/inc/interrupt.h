/**
 * @brief contains interrupt handlers for all STM8L152K4T6 interrupts
 */


void trap_interrupt(void) __trap
{
    while(1);
}

void flash_interrupt(void) __interrupt (1) 
{
    // Not Implemented
}

void dma1_01_interrupt(void) __interrupt (2)
{
    // Not Implemented
}

