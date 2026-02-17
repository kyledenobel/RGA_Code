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
    volatile unsigned char PCKENR3;         /* offset 0x10 */
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
    volatile unsigned char RESERVED1[16]; 
    volatile unsigned char CR;              /* offset 0x20 */
    volatile unsigned char IOMR1;           /* offset 0x21 */
    volatile unsigned char IOMR2;           /* offset 0x22 */
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

#endif