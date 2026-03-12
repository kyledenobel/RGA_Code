/**
 * @brief Vector interrupt table initialization
 */


// need to extern all handlers for every interrupt
extern void _stext();		/* startup routine */
extern void TRAP_interrupt();
extern void FLASH_interrupt();
extern void DMA_01_interrupt();
extern void DMA_23_interrupt();
extern void RTC_interrupt();
extern void EXTI_EF_PVD_interrupt();
extern void EXTI_BG_interrupt();
extern void EXTI_DH_interrupt();
extern void EXTI0_interrupt();
extern void EXTI1_interrupt();
extern void EXTI2_interrupt();
extern void EXTI3_interrupt();
extern void EXTI4_interrupt();
extern void EXTI5_interrupt();
extern void EXTI6_interrupt();
extern void EXTI7_interrupt();
extern void LCD_interrupt();
extern void CLK_TIM1_DAC_interrupt();
extern void COMP1_COMP2_ADC_interrupt();
extern void TIM2_UPDATE_EVENT_interrupt();
extern void TIM2_CAPTURE_COMPARE_interrupt();
extern void TIM3_UPDATE_EVENT_interrupt();
extern void TIM3_CAPTURE_COMPARE_interrupt();
extern void TIM1_UPDATE_EVENT_interrupt();
extern void TIM1_CAPTURE_COMPARE_interrupt();
extern void TIM4_UPDATE_EVENT_interrupt();
extern void SPI_interrupt();
extern void USART1_TX_interrupt();
extern void USART_RX_interrupt();
extern void I2C_interrupt();



#pragma section const {vector}

void (* const @vector _vectab[32])() = {
	_stext,			/* RESET        */
	TRAP_interrupt,			/* TRAP         */
	0,			/* RESERVED     */
	FLASH_interrupt,			/* FLASH        */
	DMA_01_interrupt,			/* DMA1 0/1     */
	DMA_23_interrupt,			/* DMA1 2/3     */
	RTC_interrupt,			/* RTC          */
	EXTI_EF_PVD_interrupt,			/* EXTI E/F     */
	EXTI_BG_interrupt,			/* EXTI B/G     */
	EXTI_DH_interrupt,			/* EXTI D/H     */
	EXTI0_interrupt,			/* EXTI0        */
	EXTI1_interrupt,			/* EXTI1        */
	EXTI2_interrupt,			/* EXTI2        */
	EXTI3_interrupt,			/* EXTI3        */
	EXTI4_interrupt,			/* EXTI4        */
	EXTI5_interrupt,			/* EXTI5        */
	EXTI6_interrupt,			/* EXTI6        */
	EXTI7_interrupt,			/* EXTI7        */
	LCD_interrupt,			/* LCD          */
	CLK_TIM1_DAC_interrupt,			/* CLK/TIM1/DAC */
	COMP1_COMP2_ADC_interrupt,			/* COMP1/COMP2/ADC1     */
	TIM2_UPDATE_EVENT_interrupt,			/* TIM2 UPDATE          */
	TIM2_CAPTURE_COMPARE_interrupt,			/* TIM2 CAPTURE         */
	TIM3_UPDATE_EVENT_interrupt,			/* TIM3 UPDATE          */
	TIM3_CAPTURE_COMPARE_interrupt,			/* TIM3 CAPTURE         */
	TIM1_UPDATE_EVENT_interrupt,			/* TIM1 UPDATE          */
	TIM1_CAPTURE_COMPARE_interrupt,			/* TIM1 CAPTURE         */
	TIM4_UPDATE_EVENT_interrupt,			/* TIM4         */
	SPI_interrupt,			/* SPI1         */
	USART1_TX_interrupt,			/* USART1 TRANSMIT      */
	USART_RX_interrupt,			/* USART1 REC   */
	I2C_interrupt,			/* I2C1         */
	};
