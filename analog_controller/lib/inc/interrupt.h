/**
 * @brief contains interrupt handlers for all STM8L152K4T6 interrupts
 */


void TRAP_interrupt(void) __trap
{
    while(1);
}

void FLASH_interrupt(void) __interrupt (1) 
{
    // Not Implemented
}

void DMA_01_interrupt(void) __interrupt (2)
{
    // Not Implemented
}

void DMA_23_interrupt(void) __interrupt (3)
{
    // Not Implemented
}

void RTC_interrupt(void) __interrupt (4)
{
    // Not Implemented
}

void EXTI_EF_PVD_interrupt(void) __interrupt (5)
{
    // Not Implemented
}

void EXTI_BG_interrupt(void) __interrupt (6)
{
    // Not Implemented
}

void EXTI_DH_interrupt(void) __interrupt (7)
{
    // Not Implemented
}

void EXTI0_interrupt(void) __interrupt (8)
{
    // Not Implemented
}

void EXTI1_interrupt(void) __interrupt (9)
{
    // Not Implemented
}

void EXTI2_interrupt(void) __interrupt (10)
{
    // Not Implemented
}

void EXTI3_interrupt(void) __interrupt (11)
{
    // Not Implemented
}

void EXTI4_interrupt(void) __interrupt (12)
{
    // Not Implemented
}

void EXTI5_interrupt(void) __interrupt (13)
{
    // Not Implemented
}

void EXTI6_interrupt(void) __interrupt (14)
{
    // Not Implemented
}

void EXTI7_interrupt(void) __interrupt (15)
{
    // Not Implemented
}

void LCD_interrupt(void) __interrupt (16)
{
    // Not Implemented
}

void CLK_TIM1_DAC_interrupt(void) __interrupt (17)
{
    // Not Implemented
}

void COMP1_COMP2_ADC_interrupt(void) __interrupt (18)
{
    // Not Implemented
}

void TIM2_UPDATE_EVENT_interrupt(void) __interrupt (19)
{
    // Not Implemented
}

void TIM2_CAPTURE_COMPARE_interrupt(void) __interrupt (20)
{
    // Not Implemented
}

void TIM3_UPDATE_EVENT_interrupt(void) __interrupt (21)
{
    // Not Implemented
}

void TIM3_CAPTURE_COMPARE_interrupt(void) __interrupt (22)
{
    // Not Implemented
}

void TIM1_UPDATE_EVENT_interrupt(void) __interrupt (23)
{
    // Not Implemented
}

void TIM1_CAPTURE_COMPARE_interrupt(void) __interrupt (24)
{
    // Not Implemented
}

void TIM4_UPDATE_EVENT_interrupt(void) __interrupt (25)
{
    // Not Implemented
}

void SPI_interrupt(void) __interrupt (26)
{
    // Not Implemented
}

void USART1_TX_interrupt(void) __interrupt (27)
{
    // Not Implemented
}

void USART_RX_interrupt(void) __interrupt (28)
{
    // Not Implemented
}

void I2C_interrupt(void) __interrupt (29)
{
    // Not Implemented
}
