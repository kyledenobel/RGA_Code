/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32g4xx_hal_conf.h
 * @author  MCD Application Team
 * @brief   HAL configuration template file.
 *          This file should be copied to the application folder and renamed
 *          to stm32g4xx_hal_conf.h.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32G4xx_HAL_CONF_H
#define STM32G4xx_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ########################## Module Selection ############################## */
/**
 * @brief This is the list of modules to be used in the HAL driver
 */

/* ########################## Register Callbacks selection ############################## */
/**
 * @brief This is the list of modules where register callback can be used
 */

#define USE_HAL_ADC_REGISTER_CALLBACKS       0U
#define USE_HAL_COMP_REGISTER_CALLBACKS      0U
#define USE_HAL_CORDIC_REGISTER_CALLBACKS    0U
#define USE_HAL_CRYP_REGISTER_CALLBACKS      0U
#define USE_HAL_DAC_REGISTER_CALLBACKS       0U
#define USE_HAL_EXTI_REGISTER_CALLBACKS      0U
#define USE_HAL_FDCAN_REGISTER_CALLBACKS     0U
#define USE_HAL_FMAC_REGISTER_CALLBACKS      0U
#define USE_HAL_HRTIM_REGISTER_CALLBACKS     0U
#define USE_HAL_I2C_REGISTER_CALLBACKS       0U
#define USE_HAL_I2S_REGISTER_CALLBACKS       0U
#define USE_HAL_IRDA_REGISTER_CALLBACKS      0U
#define USE_HAL_LPTIM_REGISTER_CALLBACKS     0U
#define USE_HAL_NAND_REGISTER_CALLBACKS      0U
#define USE_HAL_NOR_REGISTER_CALLBACKS       0U
#define USE_HAL_OPAMP_REGISTER_CALLBACKS     0U
#define USE_HAL_PCD_REGISTER_CALLBACKS       0U
#define USE_HAL_QSPI_REGISTER_CALLBACKS      0U
#define USE_HAL_RNG_REGISTER_CALLBACKS       0U
#define USE_HAL_RTC_REGISTER_CALLBACKS       0U
#define USE_HAL_SAI_REGISTER_CALLBACKS       0U
#define USE_HAL_SMARTCARD_REGISTER_CALLBACKS 0U
#define USE_HAL_SMBUS_REGISTER_CALLBACKS     0U
#define USE_HAL_SAI_REGISTER_CALLBACKS       0U
#define USE_HAL_SPI_REGISTER_CALLBACKS       0U
#define USE_HAL_SRAM_REGISTER_CALLBACKS      0U
#define USE_HAL_TIM_REGISTER_CALLBACKS       0U
#define USE_HAL_UART_REGISTER_CALLBACKS      0U
#define USE_HAL_USART_REGISTER_CALLBACKS     0U
#define USE_HAL_WWDG_REGISTER_CALLBACKS      0U

/* ########################## Oscillator Values adaptation ####################*/
/**
 * @brief Adjust the value of External High Speed oscillator (HSE) used in your application.
 *        This value is used by the RCC HAL module to compute the system frequency
 *        (when HSE is used as system clock source, directly or through the PLL).
 */
#if !defined(HSE_VALUE)
#define HSE_VALUE (24000000UL) /*!< Value of the External oscillator in Hz */
#endif                         /* HSE_VALUE */

#if !defined(HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT (100UL) /*!< Time out for HSE start up, in ms */
#endif                              /* HSE_STARTUP_TIMEOUT */

/**
 * @brief Internal High Speed oscillator (HSI) value.
 *        This value is used by the RCC HAL module to compute the system frequency
 *        (when HSI is used as system clock source, directly or through the PLL).
 */
#if !defined(HSI_VALUE)
#define HSI_VALUE (16000000UL) /*!< Value of the Internal oscillator in Hz*/
#endif                         /* HSI_VALUE */

/**
 * @brief Internal High Speed oscillator (HSI48) value for USB FS and RNG.
 *        This internal oscillator is mainly dedicated to provide a high precision clock to
 *        the USB peripheral by means of a special Clock Recovery System (CRS) circuitry.
 *        When the CRS is not used, the HSI48 RC oscillator runs on it default frequency
 *        which is subject to manufacturing process variations.
 */
#if !defined(HSI48_VALUE)
#define HSI48_VALUE                                                                                \
    (48000000UL) /*!< Value of the Internal High Speed oscillator for USB FS/RNG in Hz.            \
                      The real value my vary depending on manufacturing process variations.*/
#endif           /* HSI48_VALUE */

/**
 * @brief Internal Low Speed oscillator (LSI) value.
 */
#if !defined(LSI_VALUE)
#define LSI_VALUE (32000UL) /*!< LSI Typical Value in Hz*/
#endif /* LSI_VALUE */      /*!< Value of the Internal Low Speed oscillator in Hz                  \
                                 The real value may vary depending on the variations               \
                                 in voltage and temperature.*/
/**
 * @brief External Low Speed oscillator (LSE) value.
 *        This value is used by the UART, RTC HAL module to compute the system frequency
 */
#if !defined(LSE_VALUE)
#define LSE_VALUE (32768UL) /*!< Value of the External oscillator in Hz*/
#endif                      /* LSE_VALUE */

#if !defined(LSE_STARTUP_TIMEOUT)
#define LSE_STARTUP_TIMEOUT (5000UL) /*!< Time out for LSE start up, in ms */
#endif                               /* HSE_STARTUP_TIMEOUT */

/**
 * @brief External clock source for I2S and SAI peripherals
 *        This value is used by the I2S and SAI HAL modules to compute the I2S and SAI clock source
 *        frequency, this source is inserted directly through I2S_CKIN pad.
 */
#if !defined(EXTERNAL_CLOCK_VALUE)
#define EXTERNAL_CLOCK_VALUE (48000UL) /*!< Value of the External clock source in Hz*/
#endif                                 /* EXTERNAL_CLOCK_VALUE */

/* Tip: To avoid modifying this file each time you need to use different HSE,
   ===  you can define the HSE value in your toolchain compiler preprocessor. */

/* ########################### System Configuration ######################### */
/**
 * @brief This is the HAL system configuration section
 */
#define VDD_VALUE                (3300UL) /*!< Value of VDD in mv */
#define TICK_INT_PRIORITY        (0x0FUL) /*!< tick interrupt priority */
#define USE_RTOS                 0U
#define PREFETCH_ENABLE          0U
#define INSTRUCTION_CACHE_ENABLE 1U
#define DATA_CACHE_ENABLE        1U

/* ########################## Assert Selection ############################## */
/**
 * @brief Uncomment the line below to expanse the "assert_param" macro in the
 *        HAL drivers code
 */
/* #define USE_FULL_ASSERT               1U */

/* ################## SPI peripheral configuration ########################## */

/* CRC FEATURE: Use to activate CRC feature inside HAL SPI Driver
 * Activated: CRC code is present inside driver
 * Deactivated: CRC code cleaned from driver
 */

#define USE_SPI_CRC              1U

/* Exported macro ------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* STM32G4xx_HAL_CONF_H */