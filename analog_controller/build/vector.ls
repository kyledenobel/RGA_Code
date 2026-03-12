   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.13.3 - 22 May 2025
   3                     ; Generator (Limited) V4.6.6 - 07 Jan 2026
   4                     ; Optimizer V4.6.6 - 07 Jan 2026
  15                     .vector:	section	.text
  16  0000               __vectab:
  17  0000 8200          	dc.b	130,0
  18  0002 0000          	dc.w	__stext
  19  0004 8200          	dc.b	130,0
  20  0006 0000          	dc.w	_TRAP_interrupt
  21  0008 82000000      	dc.b	130,0,0,0
  22  000c 8200          	dc.b	130,0
  23  000e 0000          	dc.w	_FLASH_interrupt
  24  0010 8200          	dc.b	130,0
  25  0012 0000          	dc.w	_DMA_01_interrupt
  26  0014 8200          	dc.b	130,0
  27  0016 0000          	dc.w	_DMA_23_interrupt
  28  0018 8200          	dc.b	130,0
  29  001a 0000          	dc.w	_RTC_interrupt
  30  001c 8200          	dc.b	130,0
  31  001e 0000          	dc.w	_EXTI_EF_PVD_interrupt
  32  0020 8200          	dc.b	130,0
  33  0022 0000          	dc.w	_EXTI_BG_interrupt
  34  0024 8200          	dc.b	130,0
  35  0026 0000          	dc.w	_EXTI_DH_interrupt
  36  0028 8200          	dc.b	130,0
  37  002a 0000          	dc.w	_EXTI0_interrupt
  38  002c 8200          	dc.b	130,0
  39  002e 0000          	dc.w	_EXTI1_interrupt
  40  0030 8200          	dc.b	130,0
  41  0032 0000          	dc.w	_EXTI2_interrupt
  42  0034 8200          	dc.b	130,0
  43  0036 0000          	dc.w	_EXTI3_interrupt
  44  0038 8200          	dc.b	130,0
  45  003a 0000          	dc.w	_EXTI4_interrupt
  46  003c 8200          	dc.b	130,0
  47  003e 0000          	dc.w	_EXTI5_interrupt
  48  0040 8200          	dc.b	130,0
  49  0042 0000          	dc.w	_EXTI6_interrupt
  50  0044 8200          	dc.b	130,0
  51  0046 0000          	dc.w	_EXTI7_interrupt
  52  0048 8200          	dc.b	130,0
  53  004a 0000          	dc.w	_LCD_interrupt
  54  004c 8200          	dc.b	130,0
  55  004e 0000          	dc.w	_CLK_TIM1_DAC_interrupt
  56  0050 8200          	dc.b	130,0
  57  0052 0000          	dc.w	_COMP1_COMP2_ADC_interrupt
  58  0054 8200          	dc.b	130,0
  59  0056 0000          	dc.w	_TIM2_UPDATE_EVENT_interrupt
  60  0058 8200          	dc.b	130,0
  61  005a 0000          	dc.w	_TIM2_CAPTURE_COMPARE_interrupt
  62  005c 8200          	dc.b	130,0
  63  005e 0000          	dc.w	_TIM3_UPDATE_EVENT_interrupt
  64  0060 8200          	dc.b	130,0
  65  0062 0000          	dc.w	_TIM3_CAPTURE_COMPARE_interrupt
  66  0064 8200          	dc.b	130,0
  67  0066 0000          	dc.w	_TIM1_UPDATE_EVENT_interrupt
  68  0068 8200          	dc.b	130,0
  69  006a 0000          	dc.w	_TIM1_CAPTURE_COMPARE_interrupt
  70  006c 8200          	dc.b	130,0
  71  006e 0000          	dc.w	_TIM4_UPDATE_EVENT_interrupt
  72  0070 8200          	dc.b	130,0
  73  0072 0000          	dc.w	_SPI_interrupt
  74  0074 8200          	dc.b	130,0
  75  0076 0000          	dc.w	_USART1_TX_interrupt
  76  0078 8200          	dc.b	130,0
  77  007a 0000          	dc.w	_USART_RX_interrupt
  78  007c 8200          	dc.b	130,0
  79  007e 0000          	dc.w	_I2C_interrupt
  80                     	xdef	__vectab
  81                     	xref	_I2C_interrupt
  82                     	xref	_USART_RX_interrupt
  83                     	xref	_USART1_TX_interrupt
  84                     	xref	_SPI_interrupt
  85                     	xref	_TIM4_UPDATE_EVENT_interrupt
  86                     	xref	_TIM1_CAPTURE_COMPARE_interrupt
  87                     	xref	_TIM1_UPDATE_EVENT_interrupt
  88                     	xref	_TIM3_CAPTURE_COMPARE_interrupt
  89                     	xref	_TIM3_UPDATE_EVENT_interrupt
  90                     	xref	_TIM2_CAPTURE_COMPARE_interrupt
  91                     	xref	_TIM2_UPDATE_EVENT_interrupt
  92                     	xref	_COMP1_COMP2_ADC_interrupt
  93                     	xref	_CLK_TIM1_DAC_interrupt
  94                     	xref	_LCD_interrupt
  95                     	xref	_EXTI7_interrupt
  96                     	xref	_EXTI6_interrupt
  97                     	xref	_EXTI5_interrupt
  98                     	xref	_EXTI4_interrupt
  99                     	xref	_EXTI3_interrupt
 100                     	xref	_EXTI2_interrupt
 101                     	xref	_EXTI1_interrupt
 102                     	xref	_EXTI0_interrupt
 103                     	xref	_EXTI_DH_interrupt
 104                     	xref	_EXTI_BG_interrupt
 105                     	xref	_EXTI_EF_PVD_interrupt
 106                     	xref	_RTC_interrupt
 107                     	xref	_DMA_23_interrupt
 108                     	xref	_DMA_01_interrupt
 109                     	xref	_FLASH_interrupt
 110                     	xref	_TRAP_interrupt
 111                     	xref	__stext
 112                     	end
