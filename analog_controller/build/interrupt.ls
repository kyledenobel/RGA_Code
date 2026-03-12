   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.13.3 - 22 May 2025
   3                     ; Generator (Limited) V4.6.6 - 07 Jan 2026
   4                     ; Optimizer V4.6.6 - 07 Jan 2026
  15                     ; 8 @interrupt void TRAP_interrupt(void) //__trap
  15                     ; 9 {
  16                     	scross	off
  17  0000               _TRAP_interrupt:
  19  0000               L3:
  20                     ; 10     while(1);
  21  0000 20fe          	jra	L3
  23                     ; 13 @interrupt void FLASH_interrupt(void) //__interrupt (1) 
  23                     ; 14 {
  24  0002               _FLASH_interrupt:
  26                     ; 16 }
  27  0002 80            	iret	
  29                     ; 18 @interrupt void DMA_01_interrupt(void) //__interrupt (2)
  29                     ; 19 {
  30  0003               _DMA_01_interrupt:
  32                     ; 21 }
  33  0003 80            	iret	
  35                     ; 23 @interrupt void DMA_23_interrupt(void) //__interrupt (3)
  35                     ; 24 {
  36  0004               _DMA_23_interrupt:
  38                     ; 26 }
  39  0004 80            	iret	
  41                     ; 28 @interrupt void RTC_interrupt(void) //__interrupt (4)
  41                     ; 29 {
  42  0005               _RTC_interrupt:
  44                     ; 31 }
  45  0005 80            	iret	
  47                     ; 33 @interrupt void LCD_interrupt(void) //__interrupt (16)
  47                     ; 34 {
  48  0006               _LCD_interrupt:
  50                     ; 36 }
  51  0006 80            	iret	
  53                     ; 38 @interrupt void CLK_TIM1_DAC_interrupt(void) //__interrupt (17)
  53                     ; 39 {
  54  0007               _CLK_TIM1_DAC_interrupt:
  56                     ; 41 }
  57  0007 80            	iret	
  59                     ; 43 @interrupt void COMP1_COMP2_ADC_interrupt(void) //__interrupt (18)
  59                     ; 44 {
  60  0008               _COMP1_COMP2_ADC_interrupt:
  62                     ; 46 }
  63  0008 80            	iret	
  65                     ; 48 @interrupt void TIM2_UPDATE_EVENT_interrupt(void) //__interrupt (19)
  65                     ; 49 {
  66  0009               _TIM2_UPDATE_EVENT_interrupt:
  68                     ; 51 }
  69  0009 80            	iret	
  71                     ; 53 @interrupt void TIM2_CAPTURE_COMPARE_interrupt(void) //__interrupt (20)
  71                     ; 54 {
  72  000a               _TIM2_CAPTURE_COMPARE_interrupt:
  74                     ; 56 }
  75  000a 80            	iret	
  77                     ; 58 @interrupt void TIM3_UPDATE_EVENT_interrupt(void) //__interrupt (21)
  77                     ; 59 {
  78  000b               _TIM3_UPDATE_EVENT_interrupt:
  80                     ; 61 }
  81  000b 80            	iret	
  83                     ; 63 @interrupt void TIM3_CAPTURE_COMPARE_interrupt(void) //__interrupt (22)
  83                     ; 64 {
  84  000c               _TIM3_CAPTURE_COMPARE_interrupt:
  86                     ; 66 }
  87  000c 80            	iret	
  89                     ; 68 @interrupt void TIM1_UPDATE_EVENT_interrupt(void) //__interrupt (23)
  89                     ; 69 {
  90  000d               _TIM1_UPDATE_EVENT_interrupt:
  92                     ; 71 }
  93  000d 80            	iret	
  95                     ; 73 @interrupt void TIM1_CAPTURE_COMPARE_interrupt(void) //__interrupt (24)
  95                     ; 74 {
  96  000e               _TIM1_CAPTURE_COMPARE_interrupt:
  98                     ; 76 }
  99  000e 80            	iret	
 101                     ; 78 @interrupt void TIM4_UPDATE_EVENT_interrupt(void) //__interrupt (25)
 101                     ; 79 {
 102  000f               _TIM4_UPDATE_EVENT_interrupt:
 104                     ; 81 }
 105  000f 80            	iret	
 107                     ; 83 @interrupt void SPI_interrupt(void) //__interrupt (26)
 107                     ; 84 {
 108  0010               _SPI_interrupt:
 110                     ; 86 }
 111  0010 80            	iret	
 113                     ; 88 @interrupt void USART1_TX_interrupt(void) //__interrupt (27)
 113                     ; 89 {
 114  0011               _USART1_TX_interrupt:
 116                     ; 91 }
 117  0011 80            	iret	
 119                     ; 93 @interrupt void USART_RX_interrupt(void) //__interrupt (28)
 119                     ; 94 {
 120  0012               _USART_RX_interrupt:
 122                     ; 96 }
 123  0012 80            	iret	
 125                     ; 98 @interrupt void I2C_interrupt(void) //__interrupt (29)
 125                     ; 99 {
 126  0013               _I2C_interrupt:
 128                     ; 101 }
 129  0013 80            	iret	
 131                     ; 118 @interrupt void EXTI_EF_PVD_interrupt(void) //__interrupt (5)
 131                     ; 119 {
 132  0014               _EXTI_EF_PVD_interrupt:
 134                     ; 121 }
 135  0014 80            	iret	
 137                     ; 126 @interrupt void EXTI_BG_interrupt(void) //__interrupt (6)
 137                     ; 127 {
 138  0015               _EXTI_BG_interrupt:
 140                     ; 129 }
 141  0015 80            	iret	
 143                     ; 134 @interrupt void EXTI_DH_interrupt(void) //__interrupt (7)
 143                     ; 135 {
 144  0016               _EXTI_DH_interrupt:
 146                     ; 137 }
 147  0016 80            	iret	
 149                     ; 142 @interrupt void EXTI0_interrupt(void) //__interrupt (8)
 149                     ; 143 {
 150  0017               _EXTI0_interrupt:
 152                     ; 145 }
 153  0017 80            	iret	
 155                     ; 150 @interrupt void EXTI1_interrupt(void) //__interrupt (9)
 155                     ; 151 {
 156  0018               _EXTI1_interrupt:
 158                     ; 153 }
 159  0018 80            	iret	
 161                     ; 158 @interrupt void EXTI2_interrupt(void) //__interrupt (10)
 161                     ; 159 {
 162  0019               _EXTI2_interrupt:
 164                     ; 161 }
 165  0019 80            	iret	
 167                     ; 166 @interrupt void EXTI3_interrupt(void) //__interrupt (11)
 167                     ; 167 {
 168  001a               _EXTI3_interrupt:
 170                     ; 169 }
 171  001a 80            	iret	
 173                     ; 174 @interrupt void EXTI4_interrupt(void) //__interrupt (12)
 173                     ; 175 {
 174  001b               _EXTI4_interrupt:
 176                     ; 177 }
 177  001b 80            	iret	
 179                     ; 182 @interrupt void EXTI5_interrupt(void) //__interrupt (13)
 179                     ; 183 {
 180  001c               _EXTI5_interrupt:
 182                     ; 185 }
 183  001c 80            	iret	
 185                     ; 190 @interrupt void EXTI6_interrupt(void) //__interrupt (14)
 185                     ; 191 {
 186  001d               _EXTI6_interrupt:
 188                     ; 193 }
 189  001d 80            	iret	
 191                     ; 198 @interrupt void EXTI7_interrupt(void) //__interrupt (15)
 191                     ; 199 {
 192  001e               _EXTI7_interrupt:
 194                     ; 201 }
 195  001e 80            	iret	
 197                     	xdef	_EXTI7_interrupt
 198                     	xdef	_EXTI6_interrupt
 199                     	xdef	_EXTI5_interrupt
 200                     	xdef	_EXTI4_interrupt
 201                     	xdef	_EXTI3_interrupt
 202                     	xdef	_EXTI2_interrupt
 203                     	xdef	_EXTI1_interrupt
 204                     	xdef	_EXTI0_interrupt
 205                     	xdef	_EXTI_DH_interrupt
 206                     	xdef	_EXTI_BG_interrupt
 207                     	xdef	_EXTI_EF_PVD_interrupt
 208                     	xdef	_I2C_interrupt
 209                     	xdef	_USART_RX_interrupt
 210                     	xdef	_USART1_TX_interrupt
 211                     	xdef	_SPI_interrupt
 212                     	xdef	_TIM4_UPDATE_EVENT_interrupt
 213                     	xdef	_TIM1_CAPTURE_COMPARE_interrupt
 214                     	xdef	_TIM1_UPDATE_EVENT_interrupt
 215                     	xdef	_TIM3_CAPTURE_COMPARE_interrupt
 216                     	xdef	_TIM3_UPDATE_EVENT_interrupt
 217                     	xdef	_TIM2_CAPTURE_COMPARE_interrupt
 218                     	xdef	_TIM2_UPDATE_EVENT_interrupt
 219                     	xdef	_COMP1_COMP2_ADC_interrupt
 220                     	xdef	_CLK_TIM1_DAC_interrupt
 221                     	xdef	_LCD_interrupt
 222                     	xdef	_RTC_interrupt
 223                     	xdef	_DMA_23_interrupt
 224                     	xdef	_DMA_01_interrupt
 225                     	xdef	_FLASH_interrupt
 226                     	xdef	_TRAP_interrupt
 227                     	end
