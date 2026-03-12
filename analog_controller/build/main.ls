   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.13.3 - 22 May 2025
   3                     ; Generator (Limited) V4.6.6 - 07 Jan 2026
   4                     ; Optimizer V4.6.6 - 07 Jan 2026
  15                     ; 4 int main(void)
  15                     ; 5 {
  16                     	scross	off
  17  0000               _main:
  19  0000               L3:
  20                     ; 20         GPIOA->CR1 |= 0x80;
  21  0000 721e5003      	bset	20483,#7
  22                     ; 21         GPIOA->CR2 |= 0x80;
  23  0004 721e5004      	bset	20484,#7
  24                     ; 22         GPIOA->DDR |= 0x80;
  25  0008 721e5002      	bset	20482,#7
  26                     ; 23         GPIOA->ODR |= 0x80;
  27  000c 721e5000      	bset	20480,#7
  29  0010 20ee          	jra	L3
  31                     	xdef	_main
  32                     	end
