/**
 * @file           : main.c
 * @brief          : Main program body
 */

#include "main.h"
#include "gtsr_can.h"
#include "gtsr_clock.h"
#include "gtsr_debug.h"
#include "gtsr_gpio.h"
#include "gtsr_timers.h"
#include "gtsr_version.h"
#include "stm32g4xx.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_def.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

/******************************************
 * Please update to usable pins on your device
 ********************************************/
// Ex: GPIOA
#define INPUT_TEST_GPIO_PORT  GPIOA
#define INPUT_TEST_GPIO_PIN   GTSR_GPIO_PIN_0

#define OUTPUT_TEST_GPIO_PORT GPIOA
#define OUTPUT_TEST_GPIO_PIN  GTSR_GPIO_PIN_1

gtsr_timer_t timer;
gtsr_can_t can;

static void gpio_callback(volatile void *args);
static void timer_callback(volatile void *args);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    /*TODO: Remove once HAL is no longer needed */
    HAL_Init();

    system_clock_init();

    GTSR_DEBUG_INIT();

    GTSR_LOGE("Error output - Hello world");
    GTSR_LOGW("Warning output - Hello world");
    GTSR_LOGI("Informational output - Hello world. Clock speed is %ld.", get_AHB_clock_speed());
    PRINT_VERSION();
    GTSR_LOGD("Debug output - Hello world. This file was last modified at %s.", __TIMESTAMP__);
    GTSR_LOGV("Verbose outptut - Hello world. This file was compiled at %s %s", __DATE__, __TIME__);
    GTSR_LOGV("ARM's CMSIS interface libary version: %d.%d", __CM_CMSIS_VERSION_MAIN,
              __CM_CMSIS_VERSION_SUB);
#ifdef __GNUC__
    GTSR_LOGV("GCC compiler version is: %d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#endif

    HAL_Delay(1);

    gtsr_gpio_init_t input_test_pin = DEFAULT_GPIO_INITALIZATION(
        INPUT_TEST_GPIO_PORT, INPUT_TEST_GPIO_PIN, GTSR_GPIO_INPUT, NOPULL);
    gtsr_gpio_init_t output_test_pin = DEFAULT_GPIO_INITALIZATION(
        OUTPUT_TEST_GPIO_PORT, OUTPUT_TEST_GPIO_PIN, GTSR_GPIO_OUTPUT, NOPULL);
    gtsr_gpio_callback_init_t input_callback = {.base_port = INPUT_TEST_GPIO_PORT,
                                                .pins = INPUT_TEST_GPIO_PIN,
                                                .edge_trigger = RISING_AND_FALLING_EDGE,
                                                .callback = gpio_callback,
                                                .args = __TIMESTAMP__};
    GTSR_LOGI("Initalizing a pin to read from");
    gpio_pin_init(&input_test_pin);
    GTSR_LOGI("Current reading of input test pin is: %d",
              gpio_read_pin(INPUT_TEST_GPIO_PORT, INPUT_TEST_GPIO_PIN));

    gpio_callback_init(&input_callback);
    GTSR_LOGI("Initalizing a pin to written to");
    gpio_pin_init(&output_test_pin);
    gpio_write(OUTPUT_TEST_GPIO_PORT, OUTPUT_TEST_GPIO_PIN, true);
    GTSR_LOGI("Setting value to %d", gpio_read_pin(OUTPUT_TEST_GPIO_PORT, OUTPUT_TEST_GPIO_PIN));

    gtsr_timer_init_t timer_init = {
        .timer_peripheral = GTSR_TIM1,
        .frequency = 5,
        .callbacks = {{.n_ticks = 1, .handler = timer_callback, .args = NULL}},
        .number_of_callbacks = 1,
    };
    gtsr_timer_init(&timer_init, &timer);
    while (true) {
        GTSR_LOGI("Waiting");
        HAL_Delay(2);
    }
}

static void gpio_callback(volatile void *args) {
    volatile char *timestamp = (volatile char *)args;
    GTSR_LOGI("An edge was detected");
    GTSR_LOGI("It was a %s edge",
              gpio_read_pin(INPUT_TEST_GPIO_PORT, INPUT_TEST_GPIO_PIN) ? "rising" : "falling");
    GTSR_LOGI("This file was modified at %s", timestamp);
}

static void timer_callback(volatile void *args) {
    (void)args;
    static uint32_t counter = 0;
    counter++;
    GTSR_LOGI("The timer has been triggered. This is the %ld time", counter);
    gpio_toggle(OUTPUT_TEST_GPIO_PORT, OUTPUT_TEST_GPIO_PIN);
    GTSR_LOGI("The output GPIO pin is toggling its state to: %d",
              gpio_read_pin(OUTPUT_TEST_GPIO_PORT, OUTPUT_TEST_GPIO_PIN));
}
