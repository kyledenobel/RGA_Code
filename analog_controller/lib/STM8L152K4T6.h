// ==================================
//  Register Define File STM8L152K4T6
//  Author: Kyle de Nobel
// ===================================


#ifndef STM8L152K4T6_H
#define STM8L152K4T6_H


#define DEF_8BIT_REG_AT(NAME,ADDRESS)   volatile unsigned char* NAME = (unsigned char*) ADDRESS

/**
 * @brief Sets the bit or bits in a register
 */
#define SET_BIT(REG, BIT)       (REG = REG | BIT)

/**
 * @brief Clears the bit or bits in a register
 */
#define CLEAR_BIT(REG, BIT)     (REG = REG & (~BIT))

/**
 * @brief Places data into register
 */
#define WRITE_REG(REG, DATA)    (REG = DATA)

#include "STM8L152.h"

/* ===================================== */
/* ======== peripheral typedefs ======== */
/* ===================================== */
// GPIO
typedef struct {
    volatile unsigned char ODR;
    volatile unsigned char IDR;
    volatile unsigned char DDR;
    volatile unsigned char CR1;
    volatile unsigned char CR2;
} gpio_typedef_t;


// Clock Control (CLK)
typedef struct {
    volatile unsigned char CKDIVR;          /* offset 0x00 */
    volatile unsigned char CRTCR;           /* offset 0x01 */
    volatile unsigned char ICKCR;           /* offset 0x02 */
    volatile unsigned char PCKENR1;         /* offset 0x03 */
    volatile unsigned char PCKENR2;         /* offset 0x04 */
    volatile unsigned char CCOR;            /* offset 0x05 */
    volatile unsigned char ECKCR;           /* offset 0x06 */
    volatile unsigned char SCSR;            /* offset 0x07 */
    volatile unsigned char SWR;             /* offset 0x08 */
    volatile unsigned char SWCR;            /* offset 0x09 */
    volatile unsigned char CSSR;            /* offset 0x0A */
    volatile unsigned char CBEEPR;          /* offset 0x0B */
    volatile unsigned char HSICALR;         /* offset 0x0C */
    volatile unsigned char HSITRIMR;        /* offset 0x0D */
    volatile unsigned char HSIUNLCKR;       /* offset 0x0E */
    volatile unsigned char REGCSR;          /* offset 0x0F */
} clock_typedef_t;


// Routing Interface (RI)
typedef struct {
    volatile unsigned char RESERVED;        
    volatile unsigned char ICR1;            /* offset 0x01 */
    volatile unsigned char ICR2;            /* offset 0x02 */
    volatile unsigned char IOIR1;           /* offset 0x03 */
    volatile unsigned char IOIR2;           /* offset 0x04 */
    volatile unsigned char IOIR3;           /* offset 0x05 */
    volatile unsigned char IOCMR1;          /* offset 0x06 */
    volatile unsigned char IOCMR2;          /* offset 0x07 */
    volatile unsigned char IOCMR3;          /* offset 0x08 */
    volatile unsigned char IOSR1;           /* offset 0x09 */
    volatile unsigned char IOSR2;           /* offset 0x0A */
    volatile unsigned char IOSR3;           /* offset 0x0B */
    volatile unsigned char IOGCR;           /* offset 0x0C */
    volatile unsigned char ASCR1;           /* offset 0x0D */
    volatile unsigned char ASCR2;           /* offset 0x0E */
    volatile unsigned char RCR;             /* offset 0x0F */
} ri_typedef_t;


// System Configuration Controller (SYSCFG)
typedef struct {
    volatile unsigned char RMPCR1;          /* offset 0x00 */
    volatile unsigned char RMPCR2;          /* offset 0x01 */
} syscfg_typedef_t;
// NOTE: this device does not have RMPCR3 register


// Software Priority (SPR)
typedef struct {
    volatile unsigned char SPR1;            /* offset 0x00 */
    volatile unsigned char SPR2;            /* offset 0x01 */
    volatile unsigned char SPR3;            /* offset 0x02 */
    volatile unsigned char SPR4;            /* offset 0x03 */
    volatile unsigned char SPR5;            /* offset 0x04 */
    volatile unsigned char SPR6;            /* offset 0x05 */
    volatile unsigned char SPR7;            /* offset 0x06 */
    volatile unsigned char SPR8;            /* offset 0x07 */
} spr_typedef_t;


// Extended Interrupt (EXTI)
typedef struct {
    volatile unsigned char CR1;             /* offset 0x00 */
    volatile unsigned char CR2;             /* offset 0x01 */
    volatile unsigned char CR3;             /* offset 0x02 */
    volatile unsigned char SR1;             /* offset 0x03 */
    volatile unsigned char SR2;             /* offset 0x04 */
    volatile unsigned char CONF1;           /* offset 0x05 */
} exti_typedef_t;


// Universal Synchronous/Asynchronous Receiver Transmitter (USART)
typedef struct {
    volatile unsigned char SR;
    volatile unsigned char DR;
    volatile unsigned char BRR1;
    volatile unsigned char BRR2;
    volatile unsigned char CR1;
    volatile unsigned char CR2;
    volatile unsigned char CR3;
    volatile unsigned char CR4;
    volatile unsigned char CR5;
    volatile unsigned char GTR;
    volatile unsigned char PSCR;
} usart_typedef_t;


// Serial Peripheral Interface (SPI)
typedef struct {
    volatile unsigned char CR1;
    volatile unsigned char CR2;
    volatile unsigned char ICR;
    volatile unsigned char SR;
    volatile unsigned char DR;
    volatile unsigned char CRCPR;
    volatile unsigned char RXCRCR;
    volatile unsigned char TXCRCR;
} spi_typedef_t;


// Timers - 8 bits
typedef struct {
    volatile unsigned char CR1;             /* offset 0x00 */ 
    volatile unsigned char CR2;             /* offset 0x01 */ 
    volatile unsigned char SMCR;            /* offset 0x02 */
    volatile unsigned char DER;             /* offset 0x03 */
    volatile unsigned char IER;             /* offset 0x04 */
    volatile unsigned char SR1;             /* offset 0x05 */
    volatile unsigned char EGR;             /* offset 0x06 */
    volatile unsigned char CNTR;            /* offset 0x07 */
    volatile unsigned char PSCR;            /* offset 0x08 */
    volatile unsigned char ARR;             /* offset 0x09 */
} timer_8_bit_typedef_t;


// Power
typedef struct {
    volatile unsigned char CSR1;            /* offset 0x00 */
    volatile unsigned char CSR2;            /* offset 0x01 */
} power_typedef_t;


// Reset
typedef struct {
    volatile unsigned char CR;              /* offset 0x00 */
    volatile unsigned char SR;              /* offset 0x01 */
} reset_typedef_t;


// ADC
typedef struct {
    volatile unsigned char CR1;             /* offset 0x00 */
    volatile unsigned char CR2;             /* offset 0x01 */
    volatile unsigned char CR3;             /* offset 0x02 */
    volatile unsigned char SR;              /* offset 0x03 */
    volatile unsigned char DRH;             /* offset 0x04 */
    volatile unsigned char DRL;             /* offset 0x05 */
    volatile unsigned char HTRH;            /* offset 0x06 */
    volatile unsigned char HTRL;            /* offset 0x07 */
    volatile unsigned char LTRH;            /* offset 0x08 */
    volatile unsigned char LTRL;            /* offset 0x09 */
    volatile unsigned char SQR1;            /* offset 0x0A */
    volatile unsigned char SQR2;            /* offset 0x0B */
    volatile unsigned char SQR3;            /* offset 0x0C */
    volatile unsigned char SQR4;            /* offset 0x0D */
    volatile unsigned char TRIGR1;          /* offset 0x0E */
    volatile unsigned char TRIGR2;          /* offset 0x0F */
    volatile unsigned char TRIGR3;          /* offset 0x10 */
    volatile unsigned char TRIGR4;          /* offset 0x11 */
} adc_typedef_t;


/* ===================================== */
/* ====== peripheral base defines ====== */
/* ===================================== */
// GPIO
#define GPIOA_BASE              0x5000
#define GPIOB_BASE              0x5005
#define GPIOC_BASE              0x500a
#define GPIOD_BASE              0x500f
#define GPIOE_BASE              0x5014
#define GPIOF_BASE              0x5019
// CLOCK CONTROL (CLK)
#define CLOCK_BASE              0x50c0
// ROUTING INTERFACE (RI)
#define RI_BAES                 0x5430
// SYSTEM CONFIGURATION CONTROLLER (SYSCFG)
#define SYSCFG_BASE             0x509e
// SOFTWARE PRIORITY (ITC)
#define SPR_BASE                0x7f70
// EXTENDED INTERRUPT (EXTI)
#define EXTI_BASE               0x50A0
// USART
#define USART_BASE              0x5230
// SPI
#define SPI_BASE                0x5200 
// Timer, 8-bit
#define TIM4_BASE               0x52E0
// Power
#define POWER_BASE              0x50B2
// Reset
#define RESET_BASE              0x50B0
// ADC
#define ADC_BASE                0x5340


/* ===================================== */
/* ========= peripheral defines ======== */
/* ===================================== */
// GPIO
#define GPIOA  ((gpio_typedef_t*) GPIOA_BASE)
#define GPIOB  ((gpio_typedef_t*) GPIOB_BASE)
#define GPIOC  ((gpio_typedef_t*) GPIOC_BASE)
#define GPIOD  ((gpio_typedef_t*) GPIOD_BASE)
#define GPIOE  ((gpio_typedef_t*) GPIOE_BASE)
#define GPIOF  ((gpio_typedef_t*) GPIOF_BASE)
// CLOCK CONTROL (CLK)
#define CLOCK  ((clock_typedef_t*) CLOCK_BASE)
// ROUTING INTERFACE (RI)
#define RI     ((ri_typedef_t*) RI_BAES)
// SYSTEM CONFIGURATION CONTROLLER (SYSCFG)
#define SYSCFG ((syscfg_typedef_t*) SYSCFG_BASE)
// SOFTWARE PRIORITY (SPR)
#define SPR    ((spr_typedef_t*) SPR_BASE)
// EXTENDED INTERRUPT (EXTI)
#define EXTI   ((exti_typedef_t*) EXTI_BASE)
// USART
#define USART  ((usart_typedef_t*) USART_BASE)
// SPI
#define SPI    ((spi_typedef_t*) SPI_BASE)
// Timer, 8-bit
#define TIM4   ((timer_8_bit_typedef_t*) TIM4_BASE)
// Power
#define POWER  ((power_typedef_t*) POWER_BASE)
// Reset
#define RESET  ((reset_typedef_t*) RESET_BASE)
// ADC
#define ADC    ((adc_typedef_t*) ADC_BASE)


/* ===================================== */
/* ====== Register Value Defines ======= */
/* ===================================== */

/* =============== GPIO ================ */
// ODR
#define GPIO_ODR_ODR0_OFFSET            0
#define GPIO_ODR_ODR0_MASK              0x01
#define GPIO_ODR_ODR0                   (GPIO_ODR_ODR0_MASK << GPIO_ODR_ODR0_OFFSET)
#define GPIO_ODR_ODR1_OFFSET            1
#define GPIO_ODR_ODR1_MASK              0x01
#define GPIO_ODR_ODR1                   (GPIO_ODR_ODR1_MASK << GPIO_ODR_ODR1_OFFSET)
#define GPIO_ODR_ODR2_OFFSET            2
#define GPIO_ODR_ODR2_MASK              0x01
#define GPIO_ODR_ODR2                   (GPIO_ODR_ODR2_MASK << GPIO_ODR_ODR2_OFFSET)
#define GPIO_ODR_ODR3_OFFSET            3
#define GPIO_ODR_ODR3_MASK              0x01
#define GPIO_ODR_ODR3                   (GPIO_ODR_ODR3_MASK << GPIO_ODR_ODR3_OFFSET)
#define GPIO_ODR_ODR4_OFFSET            4
#define GPIO_ODR_ODR4_MASK              0x01
#define GPIO_ODR_ODR4                   (GPIO_ODR_ODR4_MASK << GPIO_ODR_ODR4_OFFSET)
#define GPIO_ODR_ODR5_OFFSET            5
#define GPIO_ODR_ODR5_MASK              0x01
#define GPIO_ODR_ODR5                   (GPIO_ODR_ODR5_MASK << GPIO_ODR_ODR5_OFFSET)
#define GPIO_ODR_ODR6_OFFSET            6
#define GPIO_ODR_ODR6_MASK              0x01
#define GPIO_ODR_ODR6                   (GPIO_ODR_ODR6_MASK << GPIO_ODR_ODR6_OFFSET)
#define GPIO_ODR_ODR7_OFFSET            7
#define GPIO_ODR_ODR7_MASK              0x01
#define GPIO_ODR_ODR7                   (GPIO_ODR_ODR7_MASK << GPIO_ODR_ODR7_OFFSET)
// IDR
#define GPIO_IDR_IDR0_OFFSET            0
#define GPIO_IDR_IDR0_MASK              0x01
#define GPIO_IDR_IDR0                   (GPIO_IDR_IDR0_MASK << GPIO_IDR_IDR0_OFFSET)
#define GPIO_IDR_IDR1_OFFSET            1
#define GPIO_IDR_IDR1_MASK              0x01
#define GPIO_IDR_IDR1                   (GPIO_IDR_IDR1_MASK << GPIO_IDR_IDR1_OFFSET)
#define GPIO_IDR_IDR2_OFFSET            2
#define GPIO_IDR_IDR2_MASK              0x01
#define GPIO_IDR_IDR2                   (GPIO_IDR_IDR2_MASK << GPIO_IDR_IDR2_OFFSET)
#define GPIO_IDR_IDR3_OFFSET            3
#define GPIO_IDR_IDR3_MASK              0x01
#define GPIO_IDR_IDR3                   (GPIO_IDR_IDR3_MASK << GPIO_IDR_IDR3_OFFSET)
#define GPIO_IDR_IDR4_OFFSET            4
#define GPIO_IDR_IDR4_MASK              0x01
#define GPIO_IDR_IDR4                   (GPIO_IDR_IDR4_MASK << GPIO_IDR_IDR4_OFFSET)
#define GPIO_IDR_IDR5_OFFSET            5
#define GPIO_IDR_IDR5_MASK              0x01
#define GPIO_IDR_IDR5                   (GPIO_IDR_IDR5_MASK << GPIO_IDR_IDR5_OFFSET)
#define GPIO_IDR_IDR6_OFFSET            6
#define GPIO_IDR_IDR6_MASK              0x01
#define GPIO_IDR_IDR6                   (GPIO_IDR_IDR6_MASK << GPIO_IDR_IDR6_OFFSET)
#define GPIO_IDR_IDR7_OFFSET            7
#define GPIO_IDR_IDR7_MASK              0x01
#define GPIO_IDR_IDR7                   (GPIO_IDR_IDR7_MASK << GPIO_IDR_IDR7_OFFSET)
// DDR
#define GPIO_DDR_DDR0_OFFSET            0
#define GPIO_DDR_DDR0_MASK              0x01
#define GPIO_DDR_DDR0                   (GPIO_DDR_DDR0_MASK << GPIO_DDR_DDR0_OFFSET)
#define GPIO_DDR_DDR1_OFFSET            1
#define GPIO_DDR_DDR1_MASK              0x01
#define GPIO_DDR_DDR1                   (GPIO_DDR_DDR1_MASK << GPIO_DDR_DDR1_OFFSET)
#define GPIO_DDR_DDR2_OFFSET            2
#define GPIO_DDR_DDR2_MASK              0x01
#define GPIO_DDR_DDR2                   (GPIO_DDR_DDR2_MASK << GPIO_DDR_DDR2_OFFSET)
#define GPIO_DDR_DDR3_OFFSET            3
#define GPIO_DDR_DDR3_MASK              0x01
#define GPIO_DDR_DDR3                   (GPIO_DDR_DDR3_MASK << GPIO_DDR_DDR3_OFFSET)
#define GPIO_DDR_DDR4_OFFSET            4
#define GPIO_DDR_DDR4_MASK              0x01
#define GPIO_DDR_DDR4                   (GPIO_DDR_DDR4_MASK << GPIO_DDR_DDR4_OFFSET)
#define GPIO_DDR_DDR5_OFFSET            5
#define GPIO_DDR_DDR5_MASK              0x01
#define GPIO_DDR_DDR5                   (GPIO_DDR_DDR5_MASK << GPIO_DDR_DDR5_OFFSET)
#define GPIO_DDR_DDR6_OFFSET            6
#define GPIO_DDR_DDR6_MASK              0x01
#define GPIO_DDR_DDR6                   (GPIO_DDR_DDR6_MASK << GPIO_DDR_DDR6_OFFSET)
#define GPIO_DDR_DDR7_OFFSET            7
#define GPIO_DDR_DDR7_MASK              0x01
#define GPIO_DDR_DDR7                   (GPIO_DDR_DDR7_MASK << GPIO_DDR_DDR7_OFFSET)
// CR1
#define GPIO_CR1_CR10_OFFSET            0
#define GPIO_CR1_CR10_MASK              0x01
#define GPIO_CR1_CR10                   (GPIO_CR1_CR10_MASK << GPIO_CR1_CR10_OFFSET)
#define GPIO_CR1_CR11_OFFSET            1
#define GPIO_CR1_CR11_MASK              0x01
#define GPIO_CR1_CR11                   (GPIO_CR1_CR11_MASK << GPIO_CR1_CR11_OFFSET)
#define GPIO_CR1_CR12_OFFSET            2
#define GPIO_CR1_CR12_MASK              0x01
#define GPIO_CR1_CR12                   (GPIO_CR1_CR12_MASK << GPIO_CR1_CR12_OFFSET)
#define GPIO_CR1_CR13_OFFSET            3
#define GPIO_CR1_CR13_MASK              0x01
#define GPIO_CR1_CR13                   (GPIO_CR1_CR13_MASK << GPIO_CR1_CR13_OFFSET)
#define GPIO_CR1_CR14_OFFSET            4
#define GPIO_CR1_CR14_MASK              0x01
#define GPIO_CR1_CR14                   (GPIO_CR1_CR14_MASK << GPIO_CR1_CR14_OFFSET)
#define GPIO_CR1_CR15_OFFSET            5
#define GPIO_CR1_CR15_MASK              0x01
#define GPIO_CR1_CR15                   (GPIO_CR1_CR15_MASK << GPIO_CR1_CR15_OFFSET)
#define GPIO_CR1_CR16_OFFSET            6
#define GPIO_CR1_CR16_MASK              0x01
#define GPIO_CR1_CR16                   (GPIO_CR1_CR16_MASK << GPIO_CR1_CR16_OFFSET)
#define GPIO_CR1_CR17_OFFSET            7
#define GPIO_CR1_CR17_MASK              0x01
#define GPIO_CR1_CR17                   (GPIO_CR1_CR17_MASK << GPIO_CR1_CR17_OFFSET)
// CR2
#define GPIO_CR2_CR20_OFFSET            0
#define GPIO_CR2_CR20_MASK              0x01
#define GPIO_CR2_CR20                   (GPIO_CR2_CR20_MASK << GPIO_CR2_CR20_OFFSET)
#define GPIO_CR2_CR21_OFFSET            1
#define GPIO_CR2_CR21_MASK              0x01
#define GPIO_CR2_CR21                   (GPIO_CR2_CR21_MASK << GPIO_CR2_CR21_OFFSET)
#define GPIO_CR2_CR22_OFFSET            2
#define GPIO_CR2_CR22_MASK              0x01
#define GPIO_CR2_CR22                   (GPIO_CR2_CR22_MASK << GPIO_CR2_CR22_OFFSET)
#define GPIO_CR2_CR23_OFFSET            3
#define GPIO_CR2_CR23_MASK              0x01
#define GPIO_CR2_CR23                   (GPIO_CR2_CR23_MASK << GPIO_CR2_CR23_OFFSET)
#define GPIO_CR2_CR24_OFFSET            4
#define GPIO_CR2_CR24_MASK              0x01
#define GPIO_CR2_CR24                   (GPIO_CR2_CR24_MASK << GPIO_CR2_CR24_OFFSET)
#define GPIO_CR2_CR25_OFFSET            5
#define GPIO_CR2_CR25_MASK              0x01
#define GPIO_CR2_CR25                   (GPIO_CR2_CR25_MASK << GPIO_CR2_CR25_OFFSET)
#define GPIO_CR2_CR26_OFFSET            6
#define GPIO_CR2_CR26_MASK              0x01
#define GPIO_CR2_CR26                   (GPIO_CR2_CR26_MASK << GPIO_CR2_CR26_OFFSET)
#define GPIO_CR2_CR27_OFFSET            7
#define GPIO_CR2_CR27_MASK              0x01
#define GPIO_CR2_CR27                   (GPIO_CR2_CR27_MASK << GPIO_CR2_CR27_OFFSET)

/* =============== CLOCK ================ */
// CKDIVR
#define CLOCK_CKDIVR_CMK0_OFFSET        0
#define CLOCK_CKDIVR_CMK0_MASK          0x01
#define CLOCK_CKDIVR_CMK0               (CLOCK_CKDIVR_CMK0_MASK <<  CLOCK_CKDIVR_CMK0_OFFSET)
#define CLOCK_CKDIVR_CMK1_OFFSET        1
#define CLOCK_CKDIVR_CMK1_MASK          0x01
#define CLOCK_CKDIVR_CMK1               (CLOCK_CKDIVR_CMK1_MASK <<  CLOCK_CKDIVR_CMK1_OFFSET)
#define CLOCK_CKDIVR_CMK2_OFFSET        2
#define CLOCK_CKDIVR_CMK2_MASK          0x01
#define CLOCK_CKDIVR_CMK2               (CLOCK_CKDIVR_CMK2_MASK <<  CLOCK_CKDIVR_CMK2_OFFSET)
// CRTCR
#define CLOCK_CRTCR_RTCSWBSY_OFFSET     0
#define CLOCK_CRTCR_RTCSWBSY_MASK       0x01
#define CLOCK_CRTCR_RTCSWBSY            (CLOCK_CRTCR_RTCSWBSY_MASK << CLOCK_CRTCR_RTCSWBSY_OFFSET)
#define CLOCK_CRTCR_RTCSEL0_OFFSET      1
#define CLOCK_CRTCR_RTCSEL0_MASK        0x01
#define CLOCK_CRTCR_RTCSEL0             (CLOCK_CRTCR_RTCSEL0_MASK << CLOCK_CRTCR_RTCSEL0_OFFSET)
#define CLOCK_CRTCR_RTCSEL1_OFFSET      2
#define CLOCK_CRTCR_RTCSEL1_MASK        0x01
#define CLOCK_CRTCR_RTCSEL1             (CLOCK_CRTCR_RTCSEL1_MASK << CLOCK_CRTCR_RTCSEL1_OFFSET)
#define CLOCK_CRTCR_RTCSEL2_OFFSET      3
#define CLOCK_CRTCR_RTCSEL2_MASK        0x01
#define CLOCK_CRTCR_RTCSEL2             (CLOCK_CRTCR_RTCSEL2_MASK << CLOCK_CRTCR_RTCSEL2_OFFSET)
#define CLOCK_CRTCR_RTCSEL3_OFFSET      4
#define CLOCK_CRTCR_RTCSEL3_MASK        0x01
#define CLOCK_CRTCR_RTCSEL3             (CLOCK_CRTCR_RTCSEL3_MASK << CLOCK_CRTCR_RTCSEL3_OFFSET)
#define CLOCK_CRTCR_RTCDIV0_OFFSET      5
#define CLOCK_CRTCR_RTCDIV0_MASK        0x01
#define CLOCK_CRTCR_RTCDIV0             (CLOCK_CRTCR_RTCDIV0_MASK << CLOCK_CRTCR_RTCDIV0_OFFSET)
#define CLOCK_CRTCR_RTCDIV1_OFFSET      6
#define CLOCK_CRTCR_RTCDIV1_MASK        0x01
#define CLOCK_CRTCR_RTCDIV1             (CLOCK_CRTCR_RTCDIV1_MASK << CLOCK_CRTCR_RTCDIV1_OFFSET)
#define CLOCK_CRTCR_RTCDIV2_OFFSET      7
#define CLOCK_CRTCR_RTCDIV2_MASK        0x01
#define CLOCK_CRTCR_RTCDIV2             (CLOCK_CRTCR_RTCDIV2_MASK << CLOCK_CRTCR_RTCDIV2_OFFSET)
// ICKCR
#define CLOCK_ICKCR_HSION_OFFSET        0
#define CLOCK_ICKCR_HSION_MASK          0x01
#define CLOCK_ICKCR_HSION               (CLOCK_ICKCR_HSION_MASK << CLOCK_ICKCR_HSION_OFFSET)
#define CLOCK_ICKCR_HSIRDY_OFFSET       1
#define CLOCK_ICKCR_HSIRDY_MASK         0x01
#define CLOCK_ICKCR_HSIRDY              (CLOCK_ICKCR_HSIRDY_MASK <<CLOCK_ICKCR_HSIRDY_OFFSET)
#define CLOCK_ICKCR_LSION_OFFSET        2
#define CLOCK_ICKCR_LSION_MASK          0x01
#define CLOCK_ICKCR_LSION               (CLOCK_ICKCR_LSION_MASK <<CLOCK_ICKCR_LSION_OFFSET)
#define CLOCK_ICKCR_LSIRDY_OFFSET       3
#define CLOCK_ICKCR_LSIRDY_MASK         0x01
#define CLOCK_ICKCR_LSIRDY              (CLOCK_ICKCR_LSIRDY_MASK << CLOCK_ICKCR_LSIRDY_OFFSET)
#define CLOCK_ICKCR_SAHALT_OFFSET       4
#define CLOCK_ICKCR_SAHALT_MASK         0x01
#define CLOCK_ICKCR_SAHALT              (CLOCK_ICKCR_SAHALT_MASK << CLOCK_ICKCR_SAHALT_OFFSET)
#define CLOCK_ICKCR_FHW_OFFSET          5
#define CLOCK_ICKCR_FHW_MASK            0x01
#define CLOCK_ICKCR_FHW                 (CLOCK_ICKCR_FHW_MASK << CLOCK_ICKCR_FHW_OFFSET)
#define CLOCK_ICKCR_BEEPAHALT_OFFSET    6
#define CLOCK_ICKCR_BEEPAHALT_MASK      0x01
#define CLOCK_ICKCR_BEEPAHALT           (CLOCK_ICKCR_BEEPAHALT_MASK << CLOCK_ICKCR_BEEPAHALT_OFFSET)
// PCKENR1
#define CLOCK_PCKENR1_PCKEN10_OFFSET    0
#define CLOCK_PCKENR1_PCKEN10_MASK      0x01
#define CLOCK_PCKENR1_PCKEN10           (CLOCK_PCKENR1_PCKEN10_MASK << CLOCK_PCKENR1_PCKEN10_OFFSET)
#define CLOCK_PCKENR1_PCKEN11_OFFSET    1
#define CLOCK_PCKENR1_PCKEN11_MASK      0x01
#define CLOCK_PCKENR1_PCKEN11           (CLOCK_PCKENR1_PCKEN11_MASK << CLOCK_PCKENR1_PCKEN11_OFFSET)
#define CLOCK_PCKENR1_PCKEN12_OFFSET    2
#define CLOCK_PCKENR1_PCKEN12_MASK      0x01
#define CLOCK_PCKENR1_PCKEN12           (CLOCK_PCKENR1_PCKEN12_MASK << CLOCK_PCKENR1_PCKEN12_OFFSET)
#define CLOCK_PCKENR1_PCKEN13_OFFSET    3
#define CLOCK_PCKENR1_PCKEN13_MASK      0x01
#define CLOCK_PCKENR1_PCKEN13           (CLOCK_PCKENR1_PCKEN13_MASK << CLOCK_PCKENR1_PCKEN13_OFFSET)
#define CLOCK_PCKENR1_PCKEN14_OFFSET    4
#define CLOCK_PCKENR1_PCKEN14_MASK      0x01
#define CLOCK_PCKENR1_PCKEN14           (CLOCK_PCKENR1_PCKEN14_MASK << CLOCK_PCKENR1_PCKEN14_OFFSET)
#define CLOCK_PCKENR1_PCKEN15_OFFSET    5
#define CLOCK_PCKENR1_PCKEN15_MASK      0x01
#define CLOCK_PCKENR1_PCKEN15           (CLOCK_PCKENR1_PCKEN15_MASK << CLOCK_PCKENR1_PCKEN15_OFFSET)
#define CLOCK_PCKENR1_PCKEN16_OFFSET    6
#define CLOCK_PCKENR1_PCKEN16_MASK      0x01
#define CLOCK_PCKENR1_PCKEN16           (CLOCK_PCKENR1_PCKEN16_MASK << CLOCK_PCKENR1_PCKEN16_OFFSET)
#define CLOCK_PCKENR1_PCKEN17_OFFSET    0
#define CLOCK_PCKENR1_PCKEN17_MASK      0x01
#define CLOCK_PCKENR1_PCKEN17           (CLOCK_PCKENR1_PCKEN17_MASK << CLOCK_PCKENR1_PCKEN17_OFFSET)
// PCKENR2
#define CLOCK_PCKENR2_PCKEN20_OFFSET    0
#define CLOCK_PCKENR2_PCKEN20_MASK      0x01
#define CLOCK_PCKENR2_PCKEN20           (CLOCK_PCKENR2_PCKEN20_MASK << CLOCK_PCKENR2_PCKEN20_OFFSET)
#define CLOCK_PCKENR2_PCKEN21_OFFSET    1
#define CLOCK_PCKENR2_PCKEN21_MASK      0x01
#define CLOCK_PCKENR2_PCKEN21           (CLOCK_PCKENR2_PCKEN21_MASK << CLOCK_PCKENR2_PCKEN21_OFFSET)
#define CLOCK_PCKENR2_PCKEN22_OFFSET    2
#define CLOCK_PCKENR2_PCKEN22_MASK      0x01
#define CLOCK_PCKENR2_PCKEN22           (CLOCK_PCKENR2_PCKEN22_MASK << CLOCK_PCKENR2_PCKEN22_OFFSET)
#define CLOCK_PCKENR2_PCKEN23_OFFSET    3
#define CLOCK_PCKENR2_PCKEN23_MASK      0x01
#define CLOCK_PCKENR2_PCKEN23           (CLOCK_PCKENR2_PCKEN23_MASK << CLOCK_PCKENR2_PCKEN23_OFFSET)
#define CLOCK_PCKENR2_PCKEN24_OFFSET    4
#define CLOCK_PCKENR2_PCKEN24_MASK      0x01
#define CLOCK_PCKENR2_PCKEN24           (CLOCK_PCKENR2_PCKEN24_MASK << CLOCK_PCKENR2_PCKEN24_OFFSET)
#define CLOCK_PCKENR2_PCKEN25_OFFSET    5
#define CLOCK_PCKENR2_PCKEN25_MASK      0x01
#define CLOCK_PCKENR2_PCKEN25           (CLOCK_PCKENR2_PCKEN25_MASK << CLOCK_PCKENR2_PCKEN25_OFFSET)
#define CLOCK_PCKENR2_PCKEN27_OFFSET    7
#define CLOCK_PCKENR2_PCKEN27_MASK      0x01
#define CLOCK_PCKENR2_PCKEN27           (CLOCK_PCKENR2_PCKEN27_MASK << CLOCK_PCKENR2_PCKEN27_OFFSET)
// CCOR
#define CLOCK_CCOR_CCOSWBSY_OFFSET      0
#define CLOCK_CCOR_CCOSWBSY_MASK        0x01
#define CLOCK_CCOR_CCOSWBSY             (CLOCK_CCOR_CCOSWBSY_MASK << CLOCK_CCOR_CCOSWBSY_OFFSET)
#define CLOCK_CCOR_CCOSEL0_OFFSET       1
#define CLOCK_CCOR_CCOSEL0_MASK         0x01
#define CLOCK_CCOR_CCOSEL0              (CLOCK_CCOR_CCOSEL0_MASK << CLOCK_CCOR_CCOSEL0_OFFSET)
#define CLOCK_CCOR_CCOSEL1_OFFSET       2
#define CLOCK_CCOR_CCOSEL1_MASK         0x01
#define CLOCK_CCOR_CCOSEL1              (CLOCK_CCOR_CCOSEL1_MASK << CLOCK_CCOR_CCOSEL1_OFFSET)
#define CLOCK_CCOR_CCOSEL2_OFFSET       3
#define CLOCK_CCOR_CCOSEL2_MASK         0x01
#define CLOCK_CCOR_CCOSEL2              (CLOCK_CCOR_CCOSEL2_MASK << CLOCK_CCOR_CCOSEL2_OFFSET)
#define CLOCK_CCOR_CCOSEL3_OFFSET       4
#define CLOCK_CCOR_CCOSEL3_MASK         0x01
#define CLOCK_CCOR_CCOSEL3              (CLOCK_CCOR_CCOSEL3_MASK << CLOCK_CCOR_CCOSEL3_OFFSET)
#define CLOCK_CCOR_CCODIV0_OFFSET       5
#define CLOCK_CCOR_CCODIV0_MASK         0x01
#define CLOCK_CCOR_CCODIV0              (CLOCK_CCOR_CCODIV0_MASK << CLOCK_CCOR_CCODIV0_OFFSET)
#define CLOCK_CCOR_CCODIV1_OFFSET       6
#define CLOCK_CCOR_CCODIV1_MASK         0x01
#define CLOCK_CCOR_CCODIV1              (CLOCK_CCOR_CCODIV1_MASK << CLOCK_CCOR_CCODIV1_OFFSET)
#define CLOCK_CCOR_CCODIV2_OFFSET       7
#define CLOCK_CCOR_CCODIV2_MASK         0x01
#define CLOCK_CCOR_CCODIV2              (CLOCK_CCOR_CCODIV2_MASK << CLOCK_CCOR_CCODIV2_OFFSET)
// ECKCR
#define CLOCK_ECKCR_HSEON_OFFSET        0
#define CLOCK_ECKCR_HSEON_MASK          0x01
#define CLOCK_ECKCR_HSEON               (CLOCK_ECKCR_HSEON_MASK << CLOCK_ECKCR_HSEON_OFFSET)
#define CLOCK_ECKCR_HSERDY_OFFSET       1
#define CLOCK_ECKCR_HSERDY_MASK         0x01
#define CLOCK_ECKCR_HSERDY              (CLOCK_ECKCR_HSERDY_MASK << CLOCK_ECKCR_HSERDY_OFFSET)
#define CLOCK_ECKCR_LSEON_OFFSET        2
#define CLOCK_ECKCR_LSEON_MASK          0x01
#define CLOCK_ECKCR_LSEON               (CLOCK_ECKCR_LSEON_MASK << CLOCK_ECKCR_LSEON_OFFSET)
#define CLOCK_ECKCR_LSERDY_OFFSET       3
#define CLOCK_ECKCR_LSERDY_MASK         0x01
#define CLOCK_ECKCR_LSERDY              (CLOCK_ECKCR_LSERDY_MASK << CLOCK_ECKCR_LSERDY_OFFSET)
#define CLOCK_ECKCR_HSEBYP_OFFSET       4
#define CLOCK_ECKCR_HSEBYP_MASK         0x01
#define CLOCK_ECKCR_HSEBYP              (CLOCK_ECKCR_HSEBYP_MASK << CLOCK_ECKCR_HSEBYP_OFFSET)
#define CLOCK_ECKCR_LSEBYP_OFFSET       5
#define CLOCK_ECKCR_LSEBYP_MASK         0x01
#define CLOCK_ECKCR_LSEBYP              (CLOCK_ECKCR_LSEBYP_MASK << CLOCK_ECKCR_LSEBYP_OFFSET)
// SCSR
#define CLOCK_SCSR_CKM0_OFFSET          0
#define CLOCK_SCSR_CKM0_MASK            0x01
#define CLOCK_SCSR_CKM0                 (CLOCK_SCSR_CKM0_MASK << CLOCK_SCSR_CKM0_OFFSET)
#define CLOCK_SCSR_CKM1_OFFSET          1
#define CLOCK_SCSR_CKM1_MASK            0x01
#define CLOCK_SCSR_CKM1                 (CLOCK_SCSR_CKM1_MASK << CLOCK_SCSR_CKM1_OFFSET)
#define CLOCK_SCSR_CKM2_OFFSET          2
#define CLOCK_SCSR_CKM2_MASK            0x01
#define CLOCK_SCSR_CKM2                 (CLOCK_SCSR_CKM2_MASK << CLOCK_SCSR_CKM2_OFFSET)
#define CLOCK_SCSR_CKM3_OFFSET          3
#define CLOCK_SCSR_CKM3_MASK            0x01
#define CLOCK_SCSR_CKM3                 (CLOCK_SCSR_CKM3_MASK << CLOCK_SCSR_CKM3_OFFSET)
#define CLOCK_SCSR_CKM4_OFFSET          4
#define CLOCK_SCSR_CKM4_MASK            0x01
#define CLOCK_SCSR_CKM4                 (CLOCK_SCSR_CKM4_MASK << CLOCK_SCSR_CKM4_OFFSET)
#define CLOCK_SCSR_CKM5_OFFSET          5
#define CLOCK_SCSR_CKM5_MASK            0x01
#define CLOCK_SCSR_CKM5                 (CLOCK_SCSR_CKM5_MASK << CLOCK_SCSR_CKM5_OFFSET)
#define CLOCK_SCSR_CKM6_OFFSET          6
#define CLOCK_SCSR_CKM6_MASK            0x01
#define CLOCK_SCSR_CKM6                 (CLOCK_SCSR_CKM6_MASK << CLOCK_SCSR_CKM6_OFFSET)
#define CLOCK_SCSR_CKM7_OFFSET          7
#define CLOCK_SCSR_CKM7_MASK            0x01
#define CLOCK_SCSR_CKM7                 (CLOCK_SCSR_CKM7_MASK << CLOCK_SCSR_CKM7_OFFSET)
// SWR
#define CLOCK_SWR_SWI0_OFFSET           0
#define CLOCK_SWR_SWI0_MASK             0x01
#define CLOCK_SWR_SWI0                  (CLOCK_SWR_SWI0_MASK << CLOCK_SWR_SWI0_OFFSET)
#define CLOCK_SWR_SWI1_OFFSET           1
#define CLOCK_SWR_SWI1_MASK             0x01
#define CLOCK_SWR_SWI1                  (CLOCK_SWR_SWI1_MASK << CLOCK_SWR_SWI1_OFFSET)
#define CLOCK_SWR_SWI2_OFFSET           2
#define CLOCK_SWR_SWI2_MASK             0x01
#define CLOCK_SWR_SWI2                  (CLOCK_SWR_SWI2_MASK << CLOCK_SWR_SWI2_OFFSET)
#define CLOCK_SWR_SWI3_OFFSET           3
#define CLOCK_SWR_SWI3_MASK             0x01
#define CLOCK_SWR_SWI3                  (CLOCK_SWR_SWI3_MASK << CLOCK_SWR_SWI3_OFFSET)
#define CLOCK_SWR_SWI4_OFFSET           4
#define CLOCK_SWR_SWI4_MASK             0x01
#define CLOCK_SWR_SWI4                  (CLOCK_SWR_SWI4_MASK << CLOCK_SWR_SWI4_OFFSET)
#define CLOCK_SWR_SWI5_OFFSET           5
#define CLOCK_SWR_SWI5_MASK             0x01
#define CLOCK_SWR_SWI5                  (CLOCK_SWR_SWI5_MASK << CLOCK_SWR_SWI5_OFFSET)
#define CLOCK_SWR_SWI6_OFFSET           6
#define CLOCK_SWR_SWI6_MASK             0x01
#define CLOCK_SWR_SWI6                  (CLOCK_SWR_SWI6_MASK << CLOCK_SWR_SWI6_OFFSET)
#define CLOCK_SWR_SWI7_OFFSET           7
#define CLOCK_SWR_SWI7_MASK             0x01
#define CLOCK_SWR_SWI7                  (CLOCK_SWR_SWI7_MASK << CLOCK_SWR_SWI7_OFFSET)
// SWCR
#define CLOCK_SWCR_SWBSY_OFFSET         0
#define CLOCK_SWCR_SWBSY_MASK           0x01
#define CLOCK_SWCR_SWBSY                (CLOCK_SWCR_SWBSY_MASK << CLOCK_SWCR_SWBSY_OFFSET)
#define CLOCK_SWCR_SWEN_OFFSET          1
#define CLOCK_SWCR_SWEN_MASK            0x01
#define CLOCK_SWCR_SWEN                 (CLOCK_SWCR_SWEN_MASK << CLOCK_SWCR_SWEN_OFFSET)
#define CLOCK_SWCR_SWIEN_OFFSET         2
#define CLOCK_SWCR_SWIEN_MASK           0x01
#define CLOCK_SWCR_SWIEN                (CLOCK_SWCR_SWIEN_MASK << CLOCK_SWCR_SWIEN_OFFSET)
#define CLOCK_SWCR_SWIF_OFFSET          3
#define CLOCK_SWCR_SWIF_MASK            0x01
#define CLOCK_SWCR_SWIF                 (CLOCK_SWCR_SWIF_MASK << CLOCK_SWCR_SWIF_OFFSET)
// CSSR
#define CLOCK_CSSR_CSSEN_OFFSET         0
#define CLOCK_CSSR_CSSEN_MASK           0x01
#define CLOCK_CSSR_CSSEN                (CLOCK_CSSR_CSSEN_MASK << CLOCK_CSSR_CSSEN_OFFSET)
#define CLOCK_CSSR_AUX_OFFSET           1
#define CLOCK_CSSR_AUX_MASK             0x01
#define CLOCK_CSSR_AUX                  (CLOCK_CSSR_AUX_MASK << CLOCK_CSSR_AUX_OFFSET)
#define CLOCK_CSSR_CSSDIE_OFFSET        2
#define CLOCK_CSSR_CSSDIE_MASK          0x01
#define CLOCK_CSSR_CSSDIE               (CLOCK_CSSR_CSSDIE_MASK << CLOCK_CSSR_CSSDIE_OFFSET)
#define CLOCK_CSSR_CSSD_OFFSET          3
#define CLOCK_CSSR_CSSD_MASK            0x01
#define CLOCK_CSSR_CSSD                 (CLOCK_CSSR_CSSD_MASK << CLOCK_CSSR_CSSD_OFFSET)
#define CLOCK_CSSR_CSSDGON_OFFSET       4
#define CLOCK_CSSR_CSSDGON_MASK         0x01
#define CLOCK_CSSR_CSSDGON              (CLOCK_CSSR_CSSDGON_MASK << CLOCK_CSSR_CSSDGON_OFFSET)
// CBEEPR
#define CLOCK_CBEEPR_BEEPSWBSY_OFFSET   0
#define CLOCK_CBEEPR_BEEPSWBSY_MASK     0x01
#define CLOCK_CBEEPR_BEEPSWBSY          (CLOCK_CBEEPR_BEEPSWBSY_MASK << CLOCK_CBEEPR_BEEPSWBSY_OFFSET)
#define CLOCK_CBEEPR_CLKBEEPSEL0_OFFSET 1
#define CLOCK_CBEEPR_CLKBEEPSEL0_MASK   0x01
#define CLOCK_CBEEPR_CLKBEEPSEL0        (CLOCK_CBEEPR_CLKBEEPSEL0_MASK << CLOCK_CBEEPR_CLKBEEPSEL0_OFFSET)
#define CLOCK_CBEEPR_CLKBEEPSEL1_OFFSET 2
#define CLOCK_CBEEPR_CLKBEEPSEL1_MASK   0x01
#define CLOCK_CBEEPR_CLKBEEPSEL1        (CLOCK_CBEEPR_CLKBEEPSEL1_MASK << CLOCK_CBEEPR_CLKBEEPSEL1_OFFSET)
// HSICALR
#define CLOCK_SWR_HSICAL0_OFFSET        0
#define CLOCK_SWR_HSICAL0_MASK          0x01
#define CLOCK_SWR_HSICAL0               (CLOCK_SWR_HSICAL0_MASK << CLOCK_SWR_HSICAL0_OFFSET)
#define CLOCK_SWR_HSICAL1_OFFSET        1
#define CLOCK_SWR_HSICAL1_MASK          0x01
#define CLOCK_SWR_HSICAL1               (CLOCK_SWR_HSICAL1_MASK << CLOCK_SWR_HSICAL1_OFFSET)
#define CLOCK_SWR_HSICAL2_OFFSET        2
#define CLOCK_SWR_HSICAL2_MASK          0x01
#define CLOCK_SWR_HSICAL2               (CLOCK_SWR_HSICAL2_MASK << CLOCK_SWR_HSICAL2_OFFSET)
#define CLOCK_SWR_HSICAL3_OFFSET        3
#define CLOCK_SWR_HSICAL3_MASK          0x01
#define CLOCK_SWR_HSICAL3               (CLOCK_SWR_HSICAL3_MASK << CLOCK_SWR_HSICAL3_OFFSET)
#define CLOCK_SWR_HSICAL4_OFFSET        4
#define CLOCK_SWR_HSICAL4_MASK          0x01
#define CLOCK_SWR_HSICAL4               (CLOCK_SWR_HSICAL4_MASK << CLOCK_SWR_HSICAL4_OFFSET)
#define CLOCK_SWR_HSICAL5_OFFSET        5
#define CLOCK_SWR_HSICAL5_MASK          0x01
#define CLOCK_SWR_HSICAL5               (CLOCK_SWR_HSICAL5_MASK << CLOCK_SWR_HSICAL5_OFFSET)
#define CLOCK_SWR_HSICAL6_OFFSET        6
#define CLOCK_SWR_HSICAL6_MASK          0x01
#define CLOCK_SWR_HSICAL6               (CLOCK_SWR_HSICAL6_MASK << CLOCK_SWR_HSICAL6_OFFSET)
#define CLOCK_SWR_HSICAL7_OFFSET        7
#define CLOCK_SWR_HSICAL7_MASK          0x01
#define CLOCK_SWR_HSICAL7               (CLOCK_SWR_HSICAL7_MASK << CLOCK_SWR_HSICAL7_OFFSET)
// HSITRIMR
#define CLOCK_SWR_HSITRIMR0_OFFSET      0
#define CLOCK_SWR_HSITRIMR0_MASK        0x01
#define CLOCK_SWR_HSITRIMR0             (CLOCK_SWR_HSITRIMR0_MASK << CLOCK_SWR_HSITRIMR0_OFFSET)
#define CLOCK_SWR_HSITRIMR1_OFFSET      1
#define CLOCK_SWR_HSITRIMR1_MASK        0x01
#define CLOCK_SWR_HSITRIMR1             (CLOCK_SWR_HSITRIMR1_MASK << CLOCK_SWR_HSITRIMR1_OFFSET)
#define CLOCK_SWR_HSITRIMR2_OFFSET      2
#define CLOCK_SWR_HSITRIMR2_MASK        0x01
#define CLOCK_SWR_HSITRIMR2             (CLOCK_SWR_HSITRIMR2_MASK << CLOCK_SWR_HSITRIMR2_OFFSET)
#define CLOCK_SWR_HSITRIMR3_OFFSET      3
#define CLOCK_SWR_HSITRIMR3_MASK        0x01
#define CLOCK_SWR_HSITRIMR3             (CLOCK_SWR_HSITRIMR3_MASK << CLOCK_SWR_HSITRIMR3_OFFSET)
#define CLOCK_SWR_HSITRIMR4_OFFSET      4
#define CLOCK_SWR_HSITRIMR4_MASK        0x01
#define CLOCK_SWR_HSITRIMR4             (CLOCK_SWR_HSITRIMR4_MASK << CLOCK_SWR_HSITRIMR4_OFFSET)
#define CLOCK_SWR_HSITRIMR5_OFFSET      5
#define CLOCK_SWR_HSITRIMR5_MASK        0x01
#define CLOCK_SWR_HSITRIMR5             (CLOCK_SWR_HSITRIMR5_MASK << CLOCK_SWR_HSITRIMR5_OFFSET)
#define CLOCK_SWR_HSITRIMR6_OFFSET      6
#define CLOCK_SWR_HSITRIMR6_MASK        0x01
#define CLOCK_SWR_HSITRIMR6             (CLOCK_SWR_HSITRIMR6_MASK << CLOCK_SWR_HSITRIMR6_OFFSET)
#define CLOCK_SWR_HSITRIMR7_OFFSET      7
#define CLOCK_SWR_HSITRIMR7_MASK        0x01
#define CLOCK_SWR_HSITRIMR7             (CLOCK_SWR_HSITRIMR7_MASK << CLOCK_SWR_HSITRIMR7_OFFSET)
// HSIUNLCKR
#define CLOCK_SWR_HSIUNLCKR0_OFFSET     0
#define CLOCK_SWR_HSIUNLCKR0_MASK       0x01
#define CLOCK_SWR_HSIUNLCKR0            (CLOCK_SWR_HSIUNLCKR0_MASK << CLOCK_SWR_HSIUNLCKR0_OFFSET)
#define CLOCK_SWR_HSIUNLCKR1_OFFSET     1
#define CLOCK_SWR_HSIUNLCKR1_MASK       0x01
#define CLOCK_SWR_HSIUNLCKR1            (CLOCK_SWR_HSIUNLCKR1_MASK << CLOCK_SWR_HSIUNLCKR1_OFFSET)
#define CLOCK_SWR_HSIUNLCKR2_OFFSET     2
#define CLOCK_SWR_HSIUNLCKR2_MASK       0x01
#define CLOCK_SWR_HSIUNLCKR2            (CLOCK_SWR_HSIUNLCKR2_MASK << CLOCK_SWR_HSIUNLCKR2_OFFSET)
#define CLOCK_SWR_HSIUNLCKR3_OFFSET     3
#define CLOCK_SWR_HSIUNLCKR3_MASK       0x01
#define CLOCK_SWR_HSIUNLCKR3            (CLOCK_SWR_HSIUNLCKR3_MASK << CLOCK_SWR_HSIUNLCKR3_OFFSET)
#define CLOCK_SWR_HSIUNLCKR4_OFFSET     4
#define CLOCK_SWR_HSIUNLCKR4_MASK       0x01
#define CLOCK_SWR_HSIUNLCKR4            (CLOCK_SWR_HSIUNLCKR4_MASK << CLOCK_SWR_HSIUNLCKR4_OFFSET)
#define CLOCK_SWR_HSIUNLCKR5_OFFSET     5
#define CLOCK_SWR_HSIUNLCKR5_MASK       0x01
#define CLOCK_SWR_HSIUNLCKR5            (CLOCK_SWR_HSIUNLCKR5_MASK << CLOCK_SWR_HSIUNLCKR5_OFFSET)
#define CLOCK_SWR_HSIUNLCKR6_OFFSET     6
#define CLOCK_SWR_HSIUNLCKR6_MASK       0x01
#define CLOCK_SWR_HSIUNLCKR6            (CLOCK_SWR_HSIUNLCKR6_MASK << CLOCK_SWR_HSIUNLCKR6_OFFSET)
#define CLOCK_SWR_HSIUNLCKR7_OFFSET     7
#define CLOCK_SWR_HSIUNLCKR7_MASK       0x01
#define CLOCK_SWR_HSIUNLCKR7            (CLOCK_SWR_HSIUNLCKR7_MASK << CLOCK_SWR_HSIUNLCKR7_OFFSET)
// REGCSR
#define CLOCK_REGCSR_REGREADY_OFFSET    0
#define CLOCK_REGCSR_REGREADY_MASK      0x01
#define CLOCK_REGCSR_REGREADY           (CLOCK_REGCSR_REGREADY_MASK << CLOCK_REGCSR_REGREADY_OFFSET)
#define CLOCK_REGCSR_REGOFF_OFFSET      1
#define CLOCK_REGCSR_REGOFF_MASK        0x01
#define CLOCK_REGCSR_REGOFF             (CLOCK_REGCSR_REGOFF_MASK << CLOCK_REGCSR_REGOFF_OFFSET)
#define CLOCK_REGCSR_HSIPD_OFFSET       2
#define CLOCK_REGCSR_HSIPD_MASK         0x01
#define CLOCK_REGCSR_HSIPD              (CLOCK_REGCSR_HSIPD_MASK << CLOCK_REGCSR_HSIPD_OFFSET)
#define CLOCK_REGCSR_LSIPD_OFFSET       3
#define CLOCK_REGCSR_LSIPD_MASK         0x01
#define CLOCK_REGCSR_LSIPD              (CLOCK_REGCSR_LSIPD_MASK << CLOCK_REGCSR_LSIPD_OFFSET)
#define CLOCK_REGCSR_HSEPD_OFFSET       4
#define CLOCK_REGCSR_HSEPD_MASK         0x01
#define CLOCK_REGCSR_HSEPD              (CLOCK_REGCSR_HSEPD_MASK << CLOCK_REGCSR_HSEPD_OFFSET)
#define CLOCK_REGCSR_LSEPD_OFFSET       5
#define CLOCK_REGCSR_LSEPD_MASK         0x01
#define CLOCK_REGCSR_LSEPD              (CLOCK_REGCSR_LSEPD_MASK << CLOCK_REGCSR_LSEPD_OFFSET)
#define CLOCK_REGCSR_EEBUSY_OFFSET      6
#define CLOCK_REGCSR_EEBUSY_MASK        0x01
#define CLOCK_REGCSR_EEBUSY             (CLOCK_REGCSR_EEBUSY_MASK << CLOCK_REGCSR_EEBUSY_OFFSET)
#define CLOCK_REGCSR_EEREADY_OFFSET     7
#define CLOCK_REGCSR_EEREADY_MASK       0x01
#define CLOCK_REGCSR_EEREADY            (CLOCK_REGCSR_EEREADY_MASK << CLOCK_REGCSR_EEREADY_OFFSET)

/* ================ RI ================= */
// ICR1
#define RI_ICR1_IC2CS0_OFFSET           0
#define RI_ICR1_IC2CS0_MASK             0x01
#define RI_ICR1_IC2CS0                  (RI_ICR1_IC2CS0_MASK << RI_ICR1_IC2CS0_OFFSET)
#define RI_ICR1_IC2CS1_OFFSET           1
#define RI_ICR1_IC2CS1_MASK             0x01
#define RI_ICR1_IC2CS1                  (RI_ICR1_IC2CS1_MASK << RI_ICR1_IC2CS1_OFFSET)
#define RI_ICR1_IC2CS2_OFFSET           2
#define RI_ICR1_IC2CS2_MASK             0x01
#define RI_ICR1_IC2CS2                  (RI_ICR1_IC2CS2_MASK << RI_ICR1_IC2CS2_OFFSET)
#define RI_ICR1_IC2CS3_OFFSET           3
#define RI_ICR1_IC2CS3_MASK             0x01
#define RI_ICR1_IC2CS3                  (RI_ICR1_IC2CS3_MASK << RI_ICR1_IC2CS3_OFFSET)
#define RI_ICR1_IC2CS4_OFFSET           4
#define RI_ICR1_IC2CS4_MASK             0x01
#define RI_ICR1_IC2CS4                  (RI_ICR1_IC2CS4_MASK << RI_ICR1_IC2CS4_OFFSET)
// ICR2
#define RI_ICR2_IC2CS0_OFFSET           0
#define RI_ICR2_IC2CS0_MASK             0x01
#define RI_ICR2_IC2CS0                  (RI_ICR2_IC2CS0_MASK << RI_ICR2_IC2CS0_OFFSET)
#define RI_ICR2_IC2CS1_OFFSET           1
#define RI_ICR2_IC2CS1_MASK             0x01
#define RI_ICR2_IC2CS1                  (RI_ICR2_IC2CS1_MASK << RI_ICR2_IC2CS1_OFFSET)
#define RI_ICR2_IC2CS2_OFFSET           2
#define RI_ICR2_IC2CS2_MASK             0x01
#define RI_ICR2_IC2CS2                  (RI_ICR2_IC2CS2_MASK << RI_ICR2_IC2CS2_OFFSET)
#define RI_ICR2_IC2CS3_OFFSET           3
#define RI_ICR2_IC2CS3_MASK             0x01
#define RI_ICR2_IC2CS3                  (RI_ICR2_IC2CS3_MASK << RI_ICR2_IC2CS3_OFFSET)
#define RI_ICR2_IC2CS4_OFFSET           4
#define RI_ICR2_IC2CS4_MASK             0x01
#define RI_ICR2_IC2CS4                  (RI_ICR2_IC2CS4_MASK << RI_ICR2_IC2CS4_OFFSET)
// IOIR1
#define RI_IOIR1_CH1I_OFFSET            0
#define RI_IOIR1_CH1I_MASK              0x01
#define RI_IOIR1_CH1I                   (RI_IOIR1_CH1I_MASK << RI_IOIR1_CH1I_OFFSET)
#define RI_IOIR1_CH4I_OFFSET            1
#define RI_IOIR1_CH4I_MASK              0x01
#define RI_IOIR1_CH4I                   (RI_IOIR1_CH4I_MASK << RI_IOIR1_CH4I_OFFSET)
#define RI_IOIR1_CH7I_OFFSET            2
#define RI_IOIR1_CH7I_MASK              0x01
#define RI_IOIR1_CH7I                   (RI_IOIR1_CH7I_MASK << RI_IOIR1_CH7I_OFFSET)
#define RI_IOIR1_CH10I_OFFSET           3
#define RI_IOIR1_CH10I_MASK             0x01
#define RI_IOIR1_CH10I                  (RI_IOIR1_CH10I_MASK << RI_IOIR1_CH10I_OFFSET)
#define RI_IOIR1_CH13I_OFFSET           4
#define RI_IOIR1_CH13I_MASK             0x01
#define RI_IOIR1_CH13I                  (RI_IOIR1_CH13I_MASK << RI_IOIR1_CH13I_OFFSET)
#define RI_IOIR1_CH16I_OFFSET           5
#define RI_IOIR1_CH16I_MASK             0x01
#define RI_IOIR1_CH16I                  (RI_IOIR1_CH16I_MASK << RI_IOIR1_CH16I_OFFSET)
#define RI_IOIR1_CH19I_OFFSET           6
#define RI_IOIR1_CH19I_MASK             0x01
#define RI_IOIR1_CH19I                  (RI_IOIR1_CH19I_MASK << RI_IOIR1_CH19I_OFFSET)
#define RI_IOIR1_CH22I_OFFSET           7
#define RI_IOIR1_CH22I_MASK             0x01
#define RI_IOIR1_CH22I                  (RI_IOIR1_CH22I_MASK << RI_IOIR1_CH22I_OFFSET)
// IOIR2
#define RI_IOIR2_CH2I_OFFSET            0
#define RI_IOIR2_CH2I_MASK              0x01
#define RI_IOIR2_CH2I                   (RI_IOIR2_CH2I_MASK << RI_IOIR2_CH2I_OFFSET)
#define RI_IOIR2_CH5I_OFFSET            1
#define RI_IOIR2_CH5I_MASK              0x01
#define RI_IOIR2_CH5I                   (RI_IOIR2_CH5I_MASK << RI_IOIR2_CH5I_OFFSET)
#define RI_IOIR2_CH8I_OFFSET            2
#define RI_IOIR2_CH8I_MASK              0x01
#define RI_IOIR2_CH8I                   (RI_IOIR2_CH8I_MASK << RI_IOIR2_CH8I_OFFSET)
#define RI_IOIR2_CH11I_OFFSET           3
#define RI_IOIR2_CH11I_MASK             0x01
#define RI_IOIR2_CH11I                  (RI_IOIR2_CH11I_MASK << RI_IOIR2_CH11I_OFFSET)
#define RI_IOIR2_CH14I_OFFSET           4
#define RI_IOIR2_CH14I_MASK             0x01
#define RI_IOIR2_CH14I                  (RI_IOIR2_CH14I_MASK << RI_IOIR2_CH14I_OFFSET)
#define RI_IOIR2_CH17I_OFFSET           5
#define RI_IOIR2_CH17I_MASK             0x01
#define RI_IOIR2_CH17I                  (RI_IOIR2_CH17I_MASK << RI_IOIR2_CH17I_OFFSET)
#define RI_IOIR2_CH20I_OFFSET           6
#define RI_IOIR2_CH20I_MASK             0x01
#define RI_IOIR2_CH20I                  (RI_IOIR2_CH20I_MASK << RI_IOIR2_CH20I_OFFSET)
#define RI_IOIR2_CH23I_OFFSET           7
#define RI_IOIR2_CH23I_MASK             0x01
#define RI_IOIR2_CH23I                  (RI_IOIR2_CH23I_MASK << RI_IOIR2_CH23I_OFFSET)
// IOIR3
#define RI_IOIR3_CH3I_OFFSET            0
#define RI_IOIR3_CH3I_MASK              0x01
#define RI_IOIR3_CH3I                   (RI_IOIR3_CH3I_MASK << RI_IOIR3_CH3I_OFFSET)
#define RI_IOIR3_CH6I_OFFSET            1
#define RI_IOIR3_CH6I_MASK              0x01
#define RI_IOIR3_CH6I                   (RI_IOIR3_CH6I_MASK << RI_IOIR3_CH6I_OFFSET)
#define RI_IOIR3_CH9I_OFFSET            2
#define RI_IOIR3_CH9I_MASK              0x01
#define RI_IOIR3_CH9I                   (RI_IOIR3_CH9I_MASK << RI_IOIR3_CH9I_OFFSET)
#define RI_IOIR3_CH12I_OFFSET           3
#define RI_IOIR3_CH12I_MASK             0x01
#define RI_IOIR3_CH12I                  (RI_IOIR3_CH12I_MASK << RI_IOIR3_CH12I_OFFSET)
#define RI_IOIR3_CH15I_OFFSET           4
#define RI_IOIR3_CH15I_MASK             0x01
#define RI_IOIR3_CH15I                  (RI_IOIR3_CH15I_MASK << RI_IOIR3_CH15I_OFFSET)
#define RI_IOIR3_CH18I_OFFSET           5
#define RI_IOIR3_CH18I_MASK             0x01
#define RI_IOIR3_CH18I                  (RI_IOIR3_CH18I_MASK << RI_IOIR3_CH18I_OFFSET)
#define RI_IOIR3_CH21I_OFFSET           6
#define RI_IOIR3_CH21I_MASK             0x01
#define RI_IOIR3_CH21I                  (RI_IOIR3_CH21I_MASK << RI_IOIR3_CH21I_OFFSET)
#define RI_IOIR3_CH24I_OFFSET           7
#define RI_IOIR3_CH24I_MASK             0x01
#define RI_IOIR3_CH24I                  (RI_IOIR3_CH24I_MASK << RI_IOIR3_CH24I_OFFSET)
// IOCMR1
#define RI_IOCMR1_CH1M_OFFSET            0
#define RI_IOCMR1_CH1M_MASK              0x01
#define RI_IOCMR1_CH1M                   (RI_IOCMR1_CH1M_MASK << RI_IOCMR1_CH1M_OFFSET)
#define RI_IOCMR1_CH4M_OFFSET            1
#define RI_IOCMR1_CH4M_MASK              0x01
#define RI_IOCMR1_CH4M                   (RI_IOCMR1_CH4M_MASK << RI_IOCMR1_CH4M_OFFSET)
#define RI_IOCMR1_CH7M_OFFSET            2
#define RI_IOCMR1_CH7M_MASK              0x01
#define RI_IOCMR1_CH7M                   (RI_IOCMR1_CH7M_MASK << RI_IOCMR1_CH7M_OFFSET)
#define RI_IOCMR1_CH10M_OFFSET           3
#define RI_IOCMR1_CH10M_MASK             0x01
#define RI_IOCMR1_CH10M                  (RI_IOCMR1_CH10M_MASK << RI_IOCMR1_CH10M_OFFSET)
#define RI_IOCMR1_CH13M_OFFSET           4
#define RI_IOCMR1_CH13M_MASK             0x01
#define RI_IOCMR1_CH13M                  (RI_IOCMR1_CH13M_MASK << RI_IOCMR1_CH13M_OFFSET)
#define RI_IOCMR1_CH16M_OFFSET           5
#define RI_IOCMR1_CH16M_MASK             0x01
#define RI_IOCMR1_CH16M                  (RI_IOCMR1_CH16M_MASK << RI_IOCMR1_CH16M_OFFSET)
#define RI_IOCMR1_CH19M_OFFSET           6
#define RI_IOCMR1_CH19M_MASK             0x01
#define RI_IOCMR1_CH19M                  (RI_IOCMR1_CH19M_MASK << RI_IOCMR1_CH19M_OFFSET)
#define RI_IOCMR1_CH22M_OFFSET           7
#define RI_IOCMR1_CH22M_MASK             0x01
#define RI_IOCMR1_CH22M                  (RI_IOCMR1_CH22M_MASK << RI_IOCMR1_CH22M_OFFSET)
// IOCMR2
#define RI_IOCMR2_CH2M_OFFSET            0
#define RI_IOCMR2_CH2M_MASK              0x01
#define RI_IOCMR2_CH2M                   (RI_IOCMR2_CH2M_MASK << RI_IOCMR2_CH2M_OFFSET)
#define RI_IOCMR2_CH5M_OFFSET            1
#define RI_IOCMR2_CH5M_MASK              0x01
#define RI_IOCMR2_CH5M                   (RI_IOCMR2_CH5M_MASK << RI_IOCMR2_CH5M_OFFSET)
#define RI_IOCMR2_CH8M_OFFSET            2
#define RI_IOCMR2_CH8M_MASK              0x01
#define RI_IOCMR2_CH8M                   (RI_IOCMR2_CH8M_MASK << RI_IOCMR2_CH8M_OFFSET)
#define RI_IOCMR2_CH11M_OFFSET           3
#define RI_IOCMR2_CH11M_MASK             0x01
#define RI_IOCMR2_CH11M                  (RI_IOCMR2_CH11M_MASK << RI_IOCMR2_CH11M_OFFSET)
#define RI_IOCMR2_CH14M_OFFSET           4
#define RI_IOCMR2_CH14M_MASK             0x01
#define RI_IOCMR2_CH14M                  (RI_IOCMR2_CH14M_MASK << RI_IOCMR2_CH14M_OFFSET)
#define RI_IOCMR2_CH17M_OFFSET           5
#define RI_IOCMR2_CH17M_MASK             0x01
#define RI_IOCMR2_CH17M                  (RI_IOCMR2_CH17M_MASK << RI_IOCMR2_CH17M_OFFSET)
#define RI_IOCMR2_CH20M_OFFSET           6
#define RI_IOCMR2_CH20M_MASK             0x01
#define RI_IOCMR2_CH20M                  (RI_IOCMR2_CH20M_MASK << RI_IOCMR2_CH20M_OFFSET)
#define RI_IOCMR2_CH23M_OFFSET           7
#define RI_IOCMR2_CH23M_MASK             0x01
#define RI_IOCMR2_CH23M                  (RI_IOCMR2_CH23M_MASK << RI_IOCMR2_CH23M_OFFSET)
// IOCMR3
#define RI_IOCMR3_CH3M_OFFSET            0
#define RI_IOCMR3_CH3M_MASK              0x01
#define RI_IOCMR3_CH3M                   (RI_IOCMR3_CH3M_MASK << RI_IOCMR3_CH3M_OFFSET)
#define RI_IOCMR3_CH6M_OFFSET            1
#define RI_IOCMR3_CH6M_MASK              0x01
#define RI_IOCMR3_CH6M                   (RI_IOCMR3_CH6M_MASK << RI_IOCMR3_CH6M_OFFSET)
#define RI_IOCMR3_CH9M_OFFSET            2
#define RI_IOCMR3_CH9M_MASK              0x01
#define RI_IOCMR3_CH9M                   (RI_IOCMR3_CH9M_MASK << RI_IOCMR3_CH9M_OFFSET)
#define RI_IOCMR3_CH12M_OFFSET           3
#define RI_IOCMR3_CH12M_MASK             0x01
#define RI_IOCMR3_CH12M                  (RI_IOCMR3_CH12M_MASK << RI_IOCMR3_CH12M_OFFSET)
#define RI_IOCMR3_CH15M_OFFSET           4
#define RI_IOCMR3_CH15M_MASK             0x01
#define RI_IOCMR3_CH15M                  (RI_IOCMR3_CH15M_MASK << RI_IOCMR3_CH15M_OFFSET)
#define RI_IOCMR3_CH18M_OFFSET           5
#define RI_IOCMR3_CH18M_MASK             0x01
#define RI_IOCMR3_CH18M                  (RI_IOCMR3_CH18M_MASK << RI_IOCMR3_CH18M_OFFSET)
#define RI_IOCMR3_CH21M_OFFSET           6
#define RI_IOCMR3_CH21M_MASK             0x01
#define RI_IOCMR3_CH21M                  (RI_IOCMR3_CH21M_MASK << RI_IOCMR3_CH21M_OFFSET)
#define RI_IOCMR3_CH24M_OFFSET           7
#define RI_IOCMR3_CH24M_MASK             0x01
#define RI_IOCMR3_CH24M                  (RI_IOCMR3_CH24M_MASK << RI_IOCMR3_CH24M_OFFSET)
// IOSR1
#define RI_IOSR1_CH1E_OFFSET             0
#define RI_IOSR1_CH1E_MASK               0x01
#define RI_IOSR1_CH1E                    (RI_IOSR1_CH1E_MASK << RI_IOSR1_CH1E_OFFSET)
#define RI_IOSR1_CH4E_OFFSET             1
#define RI_IOSR1_CH4E_MASK               0x01
#define RI_IOSR1_CH4E                    (RI_IOSR1_CH4E_MASK << RI_IOSR1_CH4E_OFFSET)
#define RI_IOSR1_CH7E_OFFSET             2
#define RI_IOSR1_CH7E_MASK               0x01
#define RI_IOSR1_CH7E                    (RI_IOSR1_CH7E_MASK << RI_IOSR1_CH7E_OFFSET)
#define RI_IOSR1_CH10E_OFFSET            3
#define RI_IOSR1_CH10E_MASK              0x01
#define RI_IOSR1_CH10E                   (RI_IOSR1_CH10E_MASK << RI_IOSR1_CH10E_OFFSET)
#define RI_IOSR1_CH13E_OFFSET            4
#define RI_IOSR1_CH13E_MASK              0x01
#define RI_IOSR1_CH13E                   (RI_IOSR1_CH13E_MASK << RI_IOSR1_CH13E_OFFSET)
#define RI_IOSR1_CH16E_OFFSET            5
#define RI_IOSR1_CH16E_MASK              0x01
#define RI_IOSR1_CH16E                   (RI_IOSR1_CH16E_MASK << RI_IOSR1_CH16E_OFFSET)
#define RI_IOSR1_CH19E_OFFSET            6
#define RI_IOSR1_CH19E_MASK              0x01
#define RI_IOSR1_CH19E                   (RI_IOSR1_CH19E_MASK << RI_IOSR1_CH19E_OFFSET)
#define RI_IOSR1_CH22E_OFFSET            7
#define RI_IOSR1_CH22E_MASK              0x01
#define RI_IOSR1_CH22E                   (RI_IOSR1_CH22E_MASK << RI_IOSR1_CH22E_OFFSET)
// IOSR2
#define RI_IOSR2_CH2E_OFFSET             0
#define RI_IOSR2_CH2E_MASK               0x01
#define RI_IOSR2_CH2E                    (RI_IOSR2_CH2E_MASK << RI_IOSR2_CH2E_OFFSET)
#define RI_IOSR2_CH5E_OFFSET             1
#define RI_IOSR2_CH5E_MASK               0x01
#define RI_IOSR2_CH5E                    (RI_IOSR2_CH5E_MASK << RI_IOSR2_CH5E_OFFSET)
#define RI_IOSR2_CH8E_OFFSET             2
#define RI_IOSR2_CH8E_MASK               0x01
#define RI_IOSR2_CH8E                    (RI_IOSR2_CH8E_MASK << RI_IOSR2_CH8E_OFFSET)
#define RI_IOSR2_CH11E_OFFSET            3
#define RI_IOSR2_CH11E_MASK              0x01
#define RI_IOSR2_CH11E                   (RI_IOSR2_CH11E_MASK << RI_IOSR2_CH11E_OFFSET)
#define RI_IOSR2_CH14E_OFFSET            4
#define RI_IOSR2_CH14E_MASK              0x01
#define RI_IOSR2_CH14E                   (RI_IOSR2_CH14E_MASK << RI_IOSR2_CH14E_OFFSET)
#define RI_IOSR2_CH17E_OFFSET            5
#define RI_IOSR2_CH17E_MASK              0x01
#define RI_IOSR2_CH17E                   (RI_IOSR2_CH17E_MASK << RI_IOSR2_CH17E_OFFSET)
#define RI_IOSR2_CH20E_OFFSET            6
#define RI_IOSR2_CH20E_MASK              0x01
#define RI_IOSR2_CH20E                   (RI_IOSR2_CH20E_MASK << RI_IOSR2_CH20E_OFFSET)
#define RI_IOSR2_CH23E_OFFSET            7
#define RI_IOSR2_CH23E_MASK              0x01
#define RI_IOSR2_CH23E                   (RI_IOSR2_CH23E_MASK << RI_IOSR2_CH23E_OFFSET)
// IOSR3
#define RI_IOSR3_CH3E_OFFSET             0
#define RI_IOSR3_CH3E_MASK               0x01
#define RI_IOSR3_CH3E                    (RI_IOSR3_CH3E_MASK << RI_IOSR3_CH3E_OFFSET)
#define RI_IOSR3_CH6E_OFFSET             1
#define RI_IOSR3_CH6E_MASK               0x01
#define RI_IOSR3_CH6E                    (RI_IOSR3_CH6E_MASK << RI_IOSR3_CH6E_OFFSET)
#define RI_IOSR3_CH9E_OFFSET             2
#define RI_IOSR3_CH9E_MASK               0x01
#define RI_IOSR3_CH9E                    (RI_IOSR3_CH9E_MASK << RI_IOSR3_CH9E_OFFSET)
#define RI_IOSR3_CH12E_OFFSET            3
#define RI_IOSR3_CH12E_MASK              0x01
#define RI_IOSR3_CH12E                   (RI_IOSR3_CH12E_MASK << RI_IOSR3_CH12E_OFFSET)
#define RI_IOSR3_CH15E_OFFSET            4
#define RI_IOSR3_CH15E_MASK              0x01
#define RI_IOSR3_CH15E                   (RI_IOSR3_CH15E_MASK << RI_IOSR3_CH15E_OFFSET)
#define RI_IOSR3_CH18E_OFFSET            5
#define RI_IOSR3_CH18E_MASK              0x01
#define RI_IOSR3_CH18E                   (RI_IOSR3_CH18E_MASK << RI_IOSR3_CH18E_OFFSET)
#define RI_IOSR3_CH21E_OFFSET            6
#define RI_IOSR3_CH21E_MASK              0x01
#define RI_IOSR3_CH21E                   (RI_IOSR3_CH21E_MASK << RI_IOSR3_CH21E_OFFSET)
#define RI_IOSR3_CH24E_OFFSET            7
#define RI_IOSR3_CH24E_MASK              0x01
#define RI_IOSR3_CH24E                   (RI_IOSR3_CH24E_MASK << RI_IOSR3_CH24E_OFFSET)
// IOGCR
#define RI_IOGCR_IOM10_OFFSET           0
#define RI_IOGCR_IOM10_MASK             0x01
#define RI_IOGCR_IOM10                  (RI_IOGCR_IOM10_MASK << RI_IOGCR_IOM10_OFFSET)
#define RI_IOGCR_IOM11_OFFSET           1
#define RI_IOGCR_IOM11_MASK             0x01
#define RI_IOGCR_IOM11                  (RI_IOGCR_IOM11_MASK << RI_IOGCR_IOM11_OFFSET)
#define RI_IOGCR_IOM20_OFFSET           2
#define RI_IOGCR_IOM20_MASK             0x01
#define RI_IOGCR_IOM20                  (RI_IOGCR_IOM20_MASK << RI_IOGCR_IOM20_OFFSET)
#define RI_IOGCR_IOM21_OFFSET           3
#define RI_IOGCR_IOM21_MASK             0x01
#define RI_IOGCR_IOM21                  (RI_IOGCR_IOM21_MASK << RI_IOGCR_IOM21_OFFSET)
#define RI_IOGCR_IOM30_OFFSET           4
#define RI_IOGCR_IOM30_MASK             0x01
#define RI_IOGCR_IOM30                  (RI_IOGCR_IOM30_MASK << RI_IOGCR_IOM30_OFFSET)
#define RI_IOGCR_IOM31_OFFSET           5
#define RI_IOGCR_IOM31_MASK             0x01
#define RI_IOGCR_IOM31                  (RI_IOGCR_IOM31_MASK << RI_IOGCR_IOM31_OFFSET)
#define RI_IOGCR_IOM40_OFFSET           6
#define RI_IOGCR_IOM40_MASK             0x01
#define RI_IOGCR_IOM40                  (RI_IOGCR_IOM40_MASK << RI_IOGCR_IOM40_OFFSET)
#define RI_IOGCR_IOM41_OFFSET           7
#define RI_IOGCR_IOM41_MASK             0x01
#define RI_IOGCR_IOM41                  (RI_IOGCR_IOM41_MASK << RI_IOGCR_IOM41_OFFSET)
// ASCR1
#define RI_ASCR1_AS0_OFFSET             0
#define RI_ASCR1_AS0_MASK               0x01
#define RI_ASCR1_AS0                    (RI_ASCR1_AS0_MASK << RI_ASCR1_AS0_OFFSET)
#define RI_ASCR1_AS1_OFFSET             1
#define RI_ASCR1_AS1_MASK               0x01
#define RI_ASCR1_AS1                    (RI_ASCR1_AS1_MASK << RI_ASCR1_AS1_OFFSET)
#define RI_ASCR1_AS2_OFFSET             2
#define RI_ASCR1_AS2_MASK               0x01
#define RI_ASCR1_AS2                    (RI_ASCR1_AS2_MASK << RI_ASCR1_AS2_OFFSET)
#define RI_ASCR1_AS3_OFFSET             3
#define RI_ASCR1_AS3_MASK               0x01
#define RI_ASCR1_AS3                    (RI_ASCR1_AS3_MASK << RI_ASCR1_AS3_OFFSET)
#define RI_ASCR1_AS4_OFFSET             4
#define RI_ASCR1_AS4_MASK               0x01
#define RI_ASCR1_AS4                    (RI_ASCR1_AS4_MASK << RI_ASCR1_AS4_OFFSET)
#define RI_ASCR1_AS5_OFFSET             5
#define RI_ASCR1_AS5_MASK               0x01
#define RI_ASCR1_AS5                    (RI_ASCR1_AS5_MASK << RI_ASCR1_AS5_OFFSET)
#define RI_ASCR1_AS6_OFFSET             6
#define RI_ASCR1_AS6_MASK               0x01
#define RI_ASCR1_AS6                    (RI_ASCR1_AS6_MASK << RI_ASCR1_AS6_OFFSET)
#define RI_ASCR1_AS7_OFFSET             7
#define RI_ASCR1_AS7_MASK               0x01
#define RI_ASCR1_AS7                    (RI_ASCR1_AS7_MASK << RI_ASCR1_AS7_OFFSET)
// ASCR2
#define RI_ASCR2_AS8_OFFSET             0
#define RI_ASCR2_AS8_MASK               0x01
#define RI_ASCR2_AS8                    (RI_ASCR2_AS8_MASK << RI_ASCR2_AS8_OFFSET)
#define RI_ASCR2_AS9_OFFSET             1
#define RI_ASCR2_AS9_MASK               0x01
#define RI_ASCR2_AS9                    (RI_ASCR2_AS9_MASK << RI_ASCR2_AS9_OFFSET)
#define RI_ASCR2_AS10_OFFSET            2
#define RI_ASCR2_AS10_MASK              0x01
#define RI_ASCR2_AS10                   (RI_ASCR2_AS10_MASK << RI_ASCR2_AS10_OFFSET)
#define RI_ASCR2_AS11_OFFSET            3
#define RI_ASCR2_AS11_MASK              0x01
#define RI_ASCR2_AS11                   (RI_ASCR2_AS11_MASK << RI_ASCR2_AS11_OFFSET)
#define RI_ASCR2_AS14_OFFSET            6
#define RI_ASCR2_AS14_MASK              0x01
#define RI_ASCR2_AS14                   (RI_ASCR2_AS14_MASK << RI_ASCR2_AS14_OFFSET)
// RCR
#define RI_RCR_10KPU_OFFSET             0
#define RI_RCR_10KPU_MASK               0x01
#define RI_RCR_10KPU                    (RI_RCR_10KPU_MASK << RI_RCR_10KPU_OFFSET)
#define RI_RCR_400KPU_OFFSET            1
#define RI_RCR_400KPU_MASK              0x01
#define RI_RCR_400KPU                   (RI_RCR_400KPU_MASK << RI_RCR_400KPU_OFFSET)
#define RI_RCR_10KPD_OFFSET             2
#define RI_RCR_10KPD_MASK               0x01
#define RI_RCR_10KPD                    (RI_RCR_10KPD_MASK << RI_RCR_10KPD_OFFSET)
#define RI_RCR_400KPD_OFFSET            3
#define RI_RCR_400KPD_MASK              0x01
#define RI_RCR_400KPD                   (RI_RCR_400KPD_MASK << RI_RCR_400KPD_OFFSET)

/* ============== SYSCFG =============== */
// RMPCR1
#define SYSCFG_RMPCR1_ADC1DMA_REMAP_OFFSET      0
#define SYSCFG_RMPCR1_ADC1DMA_REMAP_MASK        0x02
#define SYSCFG_RMPCR1_ADC1DMA_REMAP             (SYSCFG_RMPCR1_ADC1DMA_REMAP_MASK << SYSCFG_RMPCR1_ADC1DMA_REMAP_OFFSET)
#define SYSCFG_RMPCR1_TIM4DMA_REMAP_OFFSET      2
#define SYSCFG_RMPCR1_TIM4DMA_REMAP_MASK        0x02
#define SYSCFG_RMPCR1_TIM4DMA_REMAP             (SYSCFG_RMPCR1_TIM4DMA_REMAP_MASK << SYSCFG_RMPCR1_TIM4DMA_REMAP_OFFSET)
#define SYSCFG_RMPCR1_USART1TR_REMAP_OFFSET     4
#define SYSCFG_RMPCR1_USART1TR_REMAP_MASK       0x02
#define SYSCFG_RMPCR1_USART1TR_REMAP            (SYSCFG_RMPCR1_USART1TR_REMAP_MASK << SYSCFG_RMPCR1_USART1TR_REMAP_OFFSET)
#define SYSCFG_RMPCR1_USART1CK_REMAP_OFFSET     6
#define SYSCFG_RMPCR1_USART1CK_REMAP_MASK       0x01
#define SYSCFG_RMPCR1_USART1CK_REMAP            (SYSCFG_RMPCR1_USART1CK_REMAP_MASK << SYSCFG_RMPCR1_USART1CK_REMAP_OFFSET)
#define SYSCFG_RMPCR1_SPI1_REMAP1_OFFSET        7
#define SYSCFG_RMPCR1_SPI1_REMAP1_MASK          0x01
#define SYSCFG_RMPCR1_SPI1_REMAP1               (SYSCFG_RMPCR1_SPI1_REMAP1_MASK << SYSCFG_RMPCR1_SPI1_REMAP1_OFFSET)
// RMPCR2
#define SYSCFG_RMPCR2_ADC1TRIG_REMAP_OFFSET     0
#define SYSCFG_RMPCR2_ADC1TRIG_REMAP_MASK       0x01
#define SYSCFG_RMPCR2_ADC1TRIG_REMAP            (SYSCFG_RMPCR2_ADC1TRIG_REMAP_MASK << SYSCFG_RMPCR2_ADC1TRIG_REMAP_OFFSET)
#define SYSCFG_RMPCR2_TIM2TRIG_REMAP_OFFSET     1
#define SYSCFG_RMPCR2_TIM2TRIG_REMAP_MASK       0x01
#define SYSCFG_RMPCR2_TIM2TRIG_REMAP            (SYSCFG_RMPCR2_TIM2TRIG_REMAP_MASK << SYSCFG_RMPCR2_TIM2TRIG_REMAP_OFFSET)
#define SYSCFG_RMPCR2_TIM3TRIG_REMAP_OFFSET     1
#define SYSCFG_RMPCR2_TIM3TRIG_REMAP_MASK       0x01
#define SYSCFG_RMPCR2_TIM3TRIG_REMAP            (SYSCFG_RMPCR2_TIM3TRIG_REMAP_MASK << SYSCFG_RMPCR2_TIM3TRIG_REMAP_OFFSET)
#define SYSCFG_RMPCR2_TIM2TRIGLSE_REMAP_OFFSET  3
#define SYSCFG_RMPCR2_TIM2TRIGLSE_REMAP_MASK    0x01
#define SYSCFG_RMPCR2_TIM2TRIGLSE_REMAP         (SYSCFG_RMPCR2_TIM2TRIGLSE_REMAP_MASK << SYSCFG_RMPCR2_TIM2TRIGLSE_REMAP_OFFSET)
#define SYSCFG_RMPCR2_TIM3TRIGLSE_REMAP_OFFSET  4
#define SYSCFG_RMPCR2_TIM3TRIGLSE_REMAP_MASK    0x01
#define SYSCFG_RMPCR2_TIM3TRIGLSE_REMAP         (SYSCFG_RMPCR2_TIM3TRIGLSE_REMAP_MASK << SYSCFG_RMPCR2_TIM3TRIGLSE_REMAP_OFFSET)

/* ============== ITC =============== */
//SPR

/* ============== EXTI =============== */
//CR1
#define EXTI_CR1_P0ISO_OFFSET       0 
#define EXTI_CR1_P0IS0_MASK         0x01
#define EXTI_CR1_P0IS0              (EXTI_CR1_P0IS0_MASK << EXTI_CR1_P0ISO_OFFSET)
#define EXTI_CR1_P0IS1_OFFSET       1 
#define EXTI_CR1_P0IS1_MASK         0x01
#define EXTI_CR1_P0IS1              (EXTI_CR1_P0IS1_MASK << EXTI_CR1_P0IS1_OFFSET)
#define EXTI_CR1_P1IS0_OFFSET       2
#define EXTI_CR1_P1IS0_MASK         0x01
#define EXTI_CR1_P1IS0              (EXTI_CR1_P1IS0_MASK << EXTI_CR1_P1IS0_OFFSET)
#define EXTI_CR1_P1IS1_OFFSET       3 
#define EXTI_CR1_P1IS1_MASK         0x01
#define EXTI_CR1_P1IS1              (EXTI_CR1_P1IS1_MASK << EXTI_CR1_P1IS1_OFFSET)
#define EXTI_CR1_P2IS0_OFFSET       4
#define EXTI_CR1_P2IS0_MASK         0x01
#define EXTI_CR1_P2IS0              (EXTI_CR1_P2IS0_MASK << EXTI_CR1_P2IS0_OFFSET)
#define EXTI_CR1_P2IS1_OFFSET       5
#define EXTI_CR1_P2IS1_MASK         0x01
#define EXTI_CR1_P2IS1              (EXTI_CR1_P2IS1_MASK << EXTI_CR1_P2IS1_OFFSET)
#define EXTI_CR1_P3IS0_OFFSET       6
#define EXTI_CR1_P3IS0_MASK         0x01
#define EXTI_CR1_P3IS0              (EXTI_CR1_P3IS0_MASK << EXTI_CR1_P3IS0_OFFSET)
#define EXTI_CR1_P3IS1_OFFSET       7
#define EXTI_CR1_P3IS1_MASK         0x01
#define EXTI_CR1_P3IS1              (EXTI_CR1_P3IS1_MASK << EXTI_CR1_P3IS1_OFFSET)

//CR2
#define EXTI_CR2_P4ISO_OFFSET       0 
#define EXTI_CR2_P4IS0_MASK         0x01
#define EXTI_CR2_P4IS0              (EXTI_CR2_P4IS0_MASK << EXTI_CR2_P4ISO_OFFSET)
#define EXTI_CR2_P4IS1_OFFSET       1 
#define EXTI_CR2_P4IS1_MASK         0x01
#define EXTI_CR2_P4IS1              (EXTI_CR2_P4IS1_MASK << EXTI_CR2_P4IS1_OFFSET)
#define EXTI_CR2_P5IS0_OFFSET       2
#define EXTI_CR2_P5IS0_MASK         0x01
#define EXTI_CR2_P5IS0              (EXTI_CR2_P5IS0_MASK << EXTI_CR2_P5IS0_OFFSET)
#define EXTI_CR2_P5IS1_OFFSET       3 
#define EXTI_CR2_P5IS1_MASK         0x01
#define EXTI_CR2_P5IS1              (EXTI_CR2_P5IS1_MASK << EXTI_CR2_P1IS1_OFFSET)
#define EXTI_CR2_P6IS0_OFFSET       4
#define EXTI_CR2_P6IS0_MASK         0x01
#define EXTI_CR2_P6IS0              (EXTI_CR2_P6IS0_MASK << EXTI_CR2_P2IS0_OFFSET)
#define EXTI_CR2_P6IS1_OFFSET       5
#define EXTI_CR2_P6IS1_MASK         0x01
#define EXTI_CR2_P6IS1              (EXTI_CR2_P6IS1_MASK << EXTI_CR2_P2IS1_OFFSET)
#define EXTI_CR2_P7IS0_OFFSET       6
#define EXTI_CR2_P7IS0_MASK         0x01
#define EXTI_CR2_P7IS0              (EXTI_CR2_P7IS0_MASK << EXTI_CR2_P3IS0_OFFSET)
#define EXTI_CR2_P7IS1_OFFSET       7
#define EXTI_CR2_P7IS1_MASK         0x01
#define EXTI_CR2_P7IS1              (EXTI_CR2_P7IS1_MASK << EXTI_CR2_P3IS1_OFFSET)

//CR3
#define EXTI_CR3_PBIS0_OFFSET       0
#define EXTI_CR3_PBIS0_MASK         0x01
#define EXTI_CR3_PBIS0              (EXTI_CR3_PBIS0_MASK << EXTI_CR3_PBIS0_OFFSET)
#define EXTI_CR3_PBIS1_OFFSET       1
#define EXTI_CR3_PBIS1_MASK         0x01
#define EXTI_CR3_PBIS1              (EXTI_CR3_PBIS1_MASK << EXTI_CR3_PBIS1_OFFSET)
#define EXTI_CR3_PDIS0_OFFSET       2
#define EXTI_CR3_PDIS0_MASK         0x01
#define EXTI_CR3_PDIS0              (EXTI_CR3_PDIS0_MASK << EXTI_CR3_PDIS0_OFFSET)
#define EXTI_CR3_PDIS1_OFFSET       3
#define EXTI_CR3_PDIS1_MASK         0x01
#define EXTI_CR3_PDIS1              (EXTI_CR3_PDIS1_MASK << EXTI_CR3_PDIS1_OFFSET)
#define EXTI_CR3_PEIS0_OFFSET       4
#define EXTI_CR3_PEIS0_MASK         0x01
#define EXTI_CR3_PEIS0              (EXTI_CR3_PEIS0_MASK << EXTI_CR3_PEIS0_OFFSET)
#define EXTI_CR3_PEIS1_OFFSET       5
#define EXTI_CR3_PEIS1_MASK         0x01
#define EXTI_CR3_PEIS1              (EXTI_CR3_PEIS1_MASK << EXTI_CR3_PEIS1_OFFSET)
#define EXTI_CR3_PFIS0_OFFSET       6
#define EXTI_CR3_PFIS0_MASK         0x01
#define EXTI_CR3_PFIS0              (EXTI_CR3_PFIS0_MASK << EXTI_CR3_PFIS0_OFFSET)
#define EXTI_CR3_PFIS1_OFFSET       7
#define EXTI_CR3_PFIS1_MASK         0x01
#define EXTI_CR3_PFIS1              (EXTI_CR3_PFIS1_MASK << EXTI_CR3_PFIS1_OFFSET)

//SR1
#define EXTI_SR1_P0F_OFFSET         0
#define EXTI_SR1_P0F_MASK           0x01
#define EXTI_SR1_P0F                (EXTI_SR1_P0F_MASK << EXTI_SR1_P0F_OFFSET)
#define EXTI_SR1_P1F_OFFSET         1
#define EXTI_SR1_P1F_MASK           0x01
#define EXTI_SR1_P1F                (EXTI_SR1_P1F_MASK << EXTI_SR1_P1F_OFFSET)
#define EXTI_SR1_P2F_OFFSET         2
#define EXTI_SR1_P2F_MASK           0x01
#define EXTI_SR1_P2F                (EXTI_SR1_P2F_MASK << EXTI_SR1_P2F_OFFSET)
#define EXTI_SR1_P3F_OFFSET         3
#define EXTI_SR1_P3F_MASK           0x01
#define EXTI_SR1_P3F                (EXTI_SR1_P3F_MASK << EXTI_SR1_P3F_OFFSET)
#define EXTI_SR1_P4F_OFFSET         4
#define EXTI_SR1_P4F_MASK           0x01
#define EXTI_SR1_P4F                (EXTI_SR1_P4F_MASK << EXTI_SR1_P4F_OFFSET)
#define EXTI_SR1_P5F_OFFSET         5
#define EXTI_SR1_P5F_MASK           0x01
#define EXTI_SR1_P5F                (EXTI_SR1_P5F_MASK << EXTI_SR1_P5F_OFFSET)
#define EXTI_SR1_P6F_OFFSET         6
#define EXTI_SR1_P6F_MASK           0x01
#define EXTI_SR1_P6F                (EXTI_SR1_P6F_MASK << EXTI_SR1_P6F_OFFSET)
#define EXTI_SR1_P7F_OFFSET         7
#define EXTI_SR1_P7F_MASK           0x01
#define EXTI_SR1_P7F                (EXTI_SR1_P7F_MASK << EXTI_SR1_P7F_OFFSET)

//SR2
#define EXTI_SR2_PBF_OFFSET         0
#define EXTI_SR2_PBF_MASK           0x01
#define EXTI_SR2_PBF                (EXTI_SR2_PBF_MASK << EXTI_SR2_PBF_OFFSET)
#define EXTI_SR2_PDF_OFFSET         1
#define EXTI_SR2_PDF_MASK           0x01
#define EXTI_SR2_PDF                (EXTI_SR2_PDF_MASK << EXTI_SR2_PDF_OFFSET)
#define EXTI_SR2_PEF_OFFSET         2
#define EXTI_SR2_PEF_MASK           0x01
#define EXTI_SR2_PEF                (EXTI_SR2_PEF_MASK << EXTI_SR2_PEF_OFFSET)
#define EXTI_SR2_PFF_OFFSET         3
#define EXTI_SR2_PFF_MASK           0x01
#define EXTI_SR2_PFF                (EXTI_SR2_PFF_MASK << EXTI_SR2_PFF_OFFSET)

//CONF1
#define EXTI_CONF1_PBLIS_OFFSET     0
#define EXTI_CONF1_PBLIS_MASK       0x01
#define EXTI_CONF1_PBLIS            (EXTI_CONF1_PBLIS_MASK << EXTI_CONF1_PBLIS_OFFSET)
#define EXTI_CONF1_PBHIS_OFFSET     1
#define EXTI_CONF1_PBHIS_MASK       0x01
#define EXTI_CONF1_PBHIS            (EXTI_CONF1_PBHIS_MASK << EXTI_CONF1_PBHIS_OFFSET)
#define EXTI_CONF1_PDLIS_OFFSET     2
#define EXTI_CONF1_PDLIS_MASK       0x01
#define EXTI_CONF1_PDLIS            (EXTI_CONF1_PDLIS_MASK << EXTI_CONF1_PDLIS_OFFSET)
#define EXTI_CONF1_PDHIS_OFFSET     3
#define EXTI_CONF1_PDHIS_MASK       0x01
#define EXTI_CONF1_PDHIS            (EXTI_CONF1_PDHIS_MASK << EXTI_CONF1_PDHIS_OFFSET)
#define EXTI_CONF1_PELIS_OFFSET     4
#define EXTI_CONF1_PELIS_MASK       0x01
#define EXTI_CONF1_PELIS            (EXTI_CONF1_PELIS_MASK << EXTI_CONF1_PELIS_OFFSET)
#define EXTI_CONF1_PEHIS_OFFSET     5
#define EXTI_CONF1_PEHIS_MASK       0x01
#define EXTI_CONF1_PEHIS            (EXTI_CONF1_PEHIS_MASK << EXTI_CONF1_PEHIS_OFFSET)
#define EXTI_CONF1_PFLIS_OFFSET     6
#define EXTI_CONF1_PFLIS_MASK       0x01
#define EXTI_CONF1_PFLIS            (EXTI_CONF1_PFLIS_MASK << EXTI_CONF1_PFLIS_OFFSET)
#define EXTI_CONF1_PFHIS_OFFSET     7
#define EXTI_CONF1_PFHIS_MASK       0x01
#define EXTI_CONF1_PFHIS            (EXTI_CONF1_PFHIS_MASK << EXTI_CONF1_PFHIS_OFFSET)

#endif