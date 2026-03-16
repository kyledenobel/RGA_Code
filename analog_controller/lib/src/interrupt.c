/**
 * @brief contains interrupt handlers for all STM8L152K4T6 interrupts
 * @note all interrupts must be seen by main.c, so if moved out of this file
 *          they must be included in a header that is included by main.c
 */


@interrupt void TRAP_interrupt(void) //__trap
{
    while(1);
}

@interrupt void FLASH_interrupt(void) //__interrupt (1) 
{
    // Not Implemented
}

@interrupt void DMA_01_interrupt(void) //__interrupt (2)
{
    // Not Implemented
}

@interrupt void DMA_23_interrupt(void) //__interrupt (3)
{
    // Not Implemented
}

@interrupt void RTC_interrupt(void) //__interrupt (4)
{
    // Not Implemented
}

@interrupt void LCD_interrupt(void) //__interrupt (16)
{
    // Not Implemented
}

@interrupt void CLK_TIM1_DAC_interrupt(void) //__interrupt (17)
{
    // Not Implemented
}

@interrupt void COMP1_COMP2_ADC_interrupt(void) //__interrupt (18)
{
    // Not Implemented
}

@interrupt void TIM2_UPDATE_EVENT_interrupt(void) //__interrupt (19)
{
    // Not Implemented
}

@interrupt void TIM2_CAPTURE_COMPARE_interrupt(void) //__interrupt (20)
{
    // Not Implemented
}

@interrupt void TIM3_UPDATE_EVENT_interrupt(void) //__interrupt (21)
{
    // Not Implemented
}

@interrupt void TIM3_CAPTURE_COMPARE_interrupt(void) //__interrupt (22)
{
    // Not Implemented
}

@interrupt void TIM1_UPDATE_EVENT_interrupt(void) //__interrupt (23)
{
    // Not Implemented
}

@interrupt void TIM1_CAPTURE_COMPARE_interrupt(void) //__interrupt (24)
{
    // Not Implemented
}

@interrupt void TIM4_UPDATE_EVENT_interrupt(void) //__interrupt (25)
{
    // Not Implemented
}

@interrupt void SPI_interrupt(void) //__interrupt (26)
{
    // Not Implemented
}

@interrupt void USART1_TX_interrupt(void) //__interrupt (27)
{
    // Not Implemented
}

@interrupt void USART_RX_interrupt(void) //__interrupt (28)
{
    // Not Implemented
}

@interrupt void I2C_interrupt(void) //__interrupt (29)
{
    // Not Implemented
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
    // exti_interrupt_helper(0);
}

/**
 * @brief interrupt for pin 1 on all ports
 */
@interrupt void EXTI1_interrupt(void) //__interrupt (9)
{
    // exti_interrupt_helper(1);
}

/**
 * @brief interrupt for pin 2 on all ports
 */
@interrupt void EXTI2_interrupt(void) //__interrupt (10)
{
    // exti_interrupt_helper(2);
}

/**
 * @brief interrupt for pin 3 on all ports
 */
@interrupt void EXTI3_interrupt(void) //__interrupt (11)
{
    // exti_interrupt_helper(3);
}

/**
 * @brief interrupt for pin 4 on all ports
 */
@interrupt void EXTI4_interrupt(void) //__interrupt (12)
{
    // exti_interrupt_helper(4);
}

/**
 * @brief interrupt for pin 5 on all ports
 */
@interrupt void EXTI5_interrupt(void) //__interrupt (13)
{
    // exti_interrupt_helper(5);
}

/**
 * @brief interrupt for pin 6 on all ports
 */
@interrupt void EXTI6_interrupt(void) //__interrupt (14)
{
    //exti_interrupt_helper(6);
}

/**
 * @brief interrupt for pin 7 on all ports
 */
@interrupt void EXTI7_interrupt(void) //__interrupt (15)
{
    //exti_interrupt_helper(7);
}