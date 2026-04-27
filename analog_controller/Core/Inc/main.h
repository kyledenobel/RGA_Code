/**
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 */

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gtsr_adc.h"
#include "gtsr_spi.h"
#include "stm32g4xx.h"
#include "gtsr_gpio.h"
#include "stdint.h"


/* =============================================== */
/* ==================== GPIO ===================== */
/* =============================================== */
#define SPDT_IO_0               GPIOC, GTSR_GPIO_PIN_0
#define SPDT_IO_1               GPIOA, GTSR_GPIO_PIN_1
#define SPDT_IO_2               GPIOB, GTSR_GPIO_PIN_1

#define EFFECT_SELECT_0         GPIOC, GTSR_GPIO_PIN_8
#define EFFECT_SELECT_1         GPIOC, GTSR_GPIO_PIN_7

#define LED_0                   GPIOB, GTSR_GPIO_PIN_14
#define LED_1                   GPIOB, GTSR_GPIO_PIN_15
#define LED_2                   GPIOC, GTSR_GPIO_PIN_6

#define EFFECT_TOGGLE           GPIOC, GTSR_GPIO_PIN_9

#define DEBOUNCE_LENGTH         32
#define DEBOUND_DIV_FACTOR      5
typedef struct {
    bool values[DEBOUNCE_LENGTH];
    uint8_t idx;
    uint8_t val;
    uint8_t last_val;
} debounce_t;

static debounce_t toggle;

/* =============================================== */
/* ===================== ADC ===================== */
/* =============================================== */

/* buffers for ADC and Buttons */
#define NUM_ADC_SAMPLES         32
static uint16_t adc3_raw_samples[1] = {0};
static uint16_t adc5_raw_samples[2] = {0};
#define POT_0_CHANNEL_NUM       5
#define POT_1_CHANNEL_NUM       1
#define POT_2_CHANNEL_NUM       2
#define ADC_TIMEOUT             1000
#define ADC_DIV_FACTOR          9           // divide by NUM_ADC_SAMPLES because we need normalized 8-bit value
#define UPDATE_ADC_VOLUME_POT   0
#define UPDATE_ADC_TONE_POT     0
#define UPDATE_ADC_GAIN_POT     1
#define MAX_POT_VALUE           0xFF
#define POT_DIFF_THRESHOLD      10

/* =============================================== */
/* ===================== SPI ===================== */
/* =============================================== */
#define SPI_CLOCK_FREQUENCY     125000
#define NUM_PERIPHERALS         3
#define NUM_POTS                4
#define DATA_LENGTH             2
static uint16_t spi_peripheral_data_matrix[NUM_PERIPHERALS][NUM_POTS];
static uint16_t spi_dummy_rec[1];

#define EFFECT_0                0
#define EFFECT_1                1
#define EFFECT_2                2
#define VOLUME                  0
#define TONE                    1
#define GAIN                    2
#define VOLUME2                 3


/* =============================================== */
/* =================== structs =================== */
/* =============================================== */

/**
 * @brief Struct for an individual effect
 */
typedef struct {
    gtsr_spi_peripheral_t volume;
    gtsr_spi_peripheral_t tone;
    gtsr_spi_peripheral_t gain;
    gtsr_spi_peripheral_t volume2;
    uint8_t active;
} effect_t;

/**
 * @brief Struct for a pot
 */
typedef struct {
    gtsr_adc_ctx_t* adc;
    uint16_t adc_arr[NUM_ADC_SAMPLES];
    uint32_t num_samples_to_average;
    volatile uint8_t value;
    uint8_t last_val;
    uint8_t idx;
} pot_t;

/* effects for Big Muff, Phase 90, and Overdrive */
static effect_t effect_0;
static effect_t effect_1;
static effect_t effect_2;

/* pots for each analog pot */
static pot_t volume;                // POT 0
static pot_t tone;                  // POT 1
static pot_t gain;                  // POT 2








#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
