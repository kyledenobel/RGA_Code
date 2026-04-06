/**
 * @brief Main Program for Analog Controller Board for RGA
 * @author Kyle de Nobel
 * 
 * @note In file "./lib/GTSR/stmlibs/g4/src/gtsr_adc.c" 
 *      line 68 of "gtsr_adc.c" needs to only include ADC_CFGR_OVRMOD. Please remove ADC_CFGR_CONT from this line.
 * 
 * @note adc polling must now have a call to gtsr_adc_start immediately before
 */

#include "stm32g4xx_hal.h"
#include "stm32g4xx.h"
#include "gtsr_clock.h"
#include "gtsr_spi.h"
#include "gtsr_gpio.h"
#include "gtsr_debug.h"
#include "gtsr_adc.h"
#include "stdint.h"
#include "stdbool.h"
#include "main.h"


static void update_pot(pot_t* pot, uint8_t buf_select);
static void update_toggle(debounce_t* toggle);



int main(void) {
    // Initialize System
    HAL_Init();
    system_clock_init();
    GTSR_DEBUG_INIT();      // TODO: this needs to be changed to match the UART we are using

    /* Initialize GPIO, ADC, SPI */
    // SPDT_IO
    gpio_pin_init(&DEFAULT_GPIO_INITALIZATION(GPIOC, GTSR_GPIO_PIN_0, GTSR_GPIO_OUTPUT, NOPULL));
    gpio_pin_init(&DEFAULT_GPIO_INITALIZATION(GPIOA, GTSR_GPIO_PIN_1, GTSR_GPIO_OUTPUT, NOPULL));
    gpio_pin_init(&DEFAULT_GPIO_INITALIZATION(GPIOB, GTSR_GPIO_PIN_1, GTSR_GPIO_OUTPUT, NOPULL));

    // Effect Select
    gpio_pin_init(&DEFAULT_GPIO_INITALIZATION(GPIOC, GTSR_GPIO_PIN_8, GTSR_GPIO_INPUT, NOPULL));
    gpio_pin_init(&DEFAULT_GPIO_INITALIZATION(GPIOC, GTSR_GPIO_PIN_9, GTSR_GPIO_INPUT, NOPULL));

    // LED
    gpio_pin_init(&DEFAULT_GPIO_INITALIZATION(GPIOB, GTSR_GPIO_PIN_14, GTSR_GPIO_OUTPUT, NOPULL));
    gpio_pin_init(&DEFAULT_GPIO_INITALIZATION(GPIOB, GTSR_GPIO_PIN_15, GTSR_GPIO_OUTPUT, NOPULL));
    gpio_pin_init(&DEFAULT_GPIO_INITALIZATION(GPIOC, GTSR_GPIO_PIN_6, GTSR_GPIO_OUTPUT, NOPULL));

    // Effect Toggle
    gpio_pin_init(&DEFAULT_GPIO_INITALIZATION(GPIOC, GTSR_GPIO_PIN_7, GTSR_GPIO_INPUT, NOPULL));

    // ADC
    gtsr_gpio_init_t gpio_analog_init;
    gpio_analog_init.type = GTSR_GPIO_ANALOG;
    gpio_analog_init.pull = NOPULL;
    gpio_analog_init.switching_speed = GPIO_HIGH_SPEED;
    gpio_analog_init.output_type = PUSH_PULL;
    gpio_analog_init.alternate_function = 0;
    gpio_analog_init.base_port = GPIOB;
    gpio_analog_init.pins = GTSR_GPIO_PIN_13;
    gpio_pin_init(&gpio_analog_init);

    gpio_analog_init.base_port = GPIOA;
    gpio_analog_init.pins = GTSR_GPIO_PIN_8;
    gpio_pin_init(&gpio_analog_init);

    gpio_analog_init.pins = GTSR_GPIO_PIN_9;
    gpio_pin_init(&gpio_analog_init);


    gtsr_adc_ctx_t adc3;
    gtsr_adc_ctx_t adc5;

    uint8_t channels[2] = {0};
    channels[0] = POT_0_CHANNEL_NUM;
    
    gtsr_adc_ctx_bind(&adc3, ADC3, &(adc3_raw_samples[0]));
    gtsr_adc_set_sampling_resolution(&adc3, GTSR_ADC_SMPLR_8BIT);
    gtsr_adc_set_common_clock(ADC345_COMMON);
    gtsr_adc_set_sequence(&adc3, channels, 1);

    channels[0] = POT_1_CHANNEL_NUM;
    channels[1] = POT_2_CHANNEL_NUM;
    gtsr_adc_ctx_bind(&adc5, ADC5, &(adc5_raw_samples[0]));
    gtsr_adc_set_sampling_resolution(&adc5, GTSR_ADC_SMPLR_8BIT);
    gtsr_adc_set_common_clock(ADC345_COMMON);
    SET_BIT(ADC345_COMMON->CCR, ADC_CCR_VREFEN);
    gtsr_adc_set_sequence(&adc5, channels, sizeof(channels));

    gtsr_adc_enable(&adc3);
    gtsr_adc_start(&adc3);
    gtsr_adc_enable(&adc5);
    gtsr_adc_start(&adc5);

    // SPI
    gtsr_spi_t spi1;
    gtsr_spi_initializer_t spi_init;
    spi_init.clock_frequency = SPI_CLOCK_FREQUENCY;
    spi_init.clock_phase = GTSR_SPI_SAMPLE_FIRST_EDGE;
    spi_init.clock_polarity = GTSR_SPI_IDLE_HIGH;
    spi_init.data_direction = GTSR_SPI_MSB_FIRST;
    spi_init.data_size = GTSR_SPI_8_BIT_DATA;
    spi_init.frame_format = GTSR_SPI_MOTOROLA;
    spi_init.miso = GTSR_SPI1_MISO_PA6;
    spi_init.mosi = GTSR_SPI1_MOSI_PA7;
    spi_init.sck = GTSR_SPI1_SCK_PA5;
    spi_init.spi_select = GTSR_SPI1;
    gtsr_spi_init(&spi1, &spi_init);

    /* Initialize Structs */
    // POTs
    volume.adc = &adc3;
    volume.value = 0;
    volume.num_samples_to_average = NUM_ADC_SAMPLES;
    tone.adc = &adc5;
    tone.value = 0;
    tone.num_samples_to_average = NUM_ADC_SAMPLES;
    gain.adc = &adc5;
    gain.value = 0;
    gain.num_samples_to_average = NUM_ADC_SAMPLES;

    /* Initialize SPI Peripherals */
    // effect 0
    effect_0.active = false;
    // effect 0 volume digital pot
    effect_0.volume.cs.base = GPIOC;                // CS 0
    effect_0.volume.cs.pin = GTSR_GPIO_PIN_1;
    effect_0.volume.data_len = DATA_LENGTH;
    effect_0.volume.rec_buff = spi_dummy_rec;
    effect_0.volume.send_buff = &spi_peripheral_data_matrix[EFFECT_0][VOLUME];
    // effect 0 tone digital pot
    effect_0.tone.cs.base = GPIOC;                  // CS 1
    effect_0.tone.cs.pin = GTSR_GPIO_PIN_2;
    effect_0.tone.data_len = DATA_LENGTH;
    effect_0.tone.rec_buff = spi_dummy_rec;
    effect_0.tone.send_buff = &spi_peripheral_data_matrix[EFFECT_0][TONE];
    // effect 0 gain digital pot
    effect_0.gain.cs.base = GPIOC;                  // CS 2
    effect_0.gain.cs.pin = GTSR_GPIO_PIN_3;
    effect_0.gain.data_len = DATA_LENGTH;
    effect_0.gain.rec_buff = spi_dummy_rec;
    effect_0.gain.send_buff = &spi_peripheral_data_matrix[EFFECT_0][GAIN];
    // effect 0 volume2 digital pot
    effect_0.volume2.cs.base = GPIOA;               // CS 3
    effect_0.volume2.cs.pin = GTSR_GPIO_PIN_0;
    effect_0.volume2.data_len = DATA_LENGTH;
    effect_0.volume2.rec_buff = spi_dummy_rec;
    effect_0.volume2.send_buff = &spi_peripheral_data_matrix[EFFECT_0][VOLUME2];
    gtsr_spi_peripheral_init(&(effect_0.volume));
    gtsr_spi_peripheral_init(&(effect_0.tone));
    gtsr_spi_peripheral_init(&(effect_0.gain));
    gtsr_spi_peripheral_init(&(effect_0.volume2));

    // effect 1
    effect_1.active = false;
    // effect 1 volume digital pot
    effect_1.volume.cs.base = GPIOA;                // CS 4
    effect_1.volume.cs.pin = GTSR_GPIO_PIN_2;
    effect_1.volume.data_len = DATA_LENGTH;
    effect_1.volume.rec_buff = spi_dummy_rec;
    effect_1.volume.send_buff = &spi_peripheral_data_matrix[EFFECT_1][VOLUME];
    // effect 1 tone digital pot
    effect_1.tone.cs.base = GPIOC;                  // CS 5
    effect_1.tone.cs.pin = GTSR_GPIO_PIN_4;
    effect_1.tone.data_len = DATA_LENGTH;
    effect_1.tone.rec_buff = spi_dummy_rec;
    effect_1.tone.send_buff = &spi_peripheral_data_matrix[EFFECT_1][TONE];
    // effect 1 gain digital pot
    effect_1.gain.cs.base = GPIOC;                  // CS 6
    effect_1.gain.cs.pin = GTSR_GPIO_PIN_5;
    effect_1.gain.data_len = DATA_LENGTH;
    effect_1.gain.rec_buff = spi_dummy_rec;
    effect_1.gain.send_buff = &spi_peripheral_data_matrix[EFFECT_1][GAIN];
    // effect 1 volume2 digital pot
    effect_1.volume2.cs.base = GPIOB;               // CS 7
    effect_1.volume2.cs.pin = GTSR_GPIO_PIN_0;
    effect_1.volume2.data_len = DATA_LENGTH;
    effect_1.volume2.rec_buff = spi_dummy_rec;
    effect_1.volume2.send_buff = &spi_peripheral_data_matrix[EFFECT_1][VOLUME2];
    gtsr_spi_peripheral_init(&(effect_1.volume));
    gtsr_spi_peripheral_init(&(effect_1.tone));
    gtsr_spi_peripheral_init(&(effect_1.gain));
    gtsr_spi_peripheral_init(&(effect_1.volume2));

    // overdrive
    effect_2.active = false;
    // overdrive volume digital pot
    effect_2.volume.cs.base = GPIOB;                // CS 8
    effect_2.volume.cs.pin = GTSR_GPIO_PIN_2;
    effect_2.volume.data_len = DATA_LENGTH;
    effect_2.volume.rec_buff = spi_dummy_rec;
    effect_2.volume.send_buff = &spi_peripheral_data_matrix[EFFECT_2][VOLUME];
    // overdrive tone digital pot
    effect_2.tone.cs.base = GPIOB;                  // CS 9
    effect_2.tone.cs.pin = GTSR_GPIO_PIN_11;
    effect_2.tone.data_len = DATA_LENGTH;
    effect_2.tone.rec_buff = spi_dummy_rec;
    effect_2.tone.send_buff = &spi_peripheral_data_matrix[EFFECT_2][TONE];
    // overdrive gain digital pot
    effect_2.gain.cs.base = GPIOB;                  // CS 10
    effect_2.gain.cs.pin = GTSR_GPIO_PIN_10;
    effect_2.gain.data_len = DATA_LENGTH;
    effect_2.gain.rec_buff = spi_dummy_rec;
    effect_2.gain.send_buff = &spi_peripheral_data_matrix[EFFECT_2][GAIN];
    // overdrive volume2 digital pot
    effect_2.volume2.cs.base = GPIOB;               // CS 11
    effect_2.volume2.cs.pin = GTSR_GPIO_PIN_12;
    effect_2.volume2.data_len = DATA_LENGTH;
    effect_2.volume2.rec_buff = spi_dummy_rec;
    effect_2.volume2.send_buff = &spi_peripheral_data_matrix[EFFECT_2][VOLUME2];
    gtsr_spi_peripheral_init(&(effect_2.volume));
    gtsr_spi_peripheral_init(&(effect_2.tone));
    gtsr_spi_peripheral_init(&(effect_2.gain));
    gtsr_spi_peripheral_init(&(effect_2.volume2));

    // toggle button
    toggle.idx = 0;
    toggle.val = 0;
    for(uint8_t i = 0; i < DEBOUNCE_LENGTH; i++) {
        toggle.values[i] = false;
    }

    // pots
    volume.adc = &adc3;
    tone.adc = &adc5;
    gain.adc = &adc5;

    volume.num_samples_to_average = NUM_ADC_SAMPLES;
    tone.num_samples_to_average = NUM_ADC_SAMPLES;
    gain.num_samples_to_average = NUM_ADC_SAMPLES;

    volume.value = 0;
    tone.value = 0;
    gain.value = 0;

    volume.idx = 0;
    tone.idx = 0;
    gain.idx = 0;

    volume.last_val = 0;
    tone.last_val = 0;
    gain.last_val = 0;

    GTSR_PRINTF("initializing program\n");


    effect_t* selected_effect;
    uint8_t selected_effect_num;
    bool newly_selected = false;
    for(;;)
    {
        // check effect select
        if(gpio_read_pin(EFFECT_SELECT_0)) {
            selected_effect = &effect_0;
            selected_effect_num = EFFECT_0;
        }
        else if(gpio_read_pin(EFFECT_SELECT_1)) {
            selected_effect = &effect_1;
            selected_effect_num = EFFECT_1;
        }
        else {
            selected_effect = &effect_2;
            selected_effect_num = EFFECT_2;
        }

        // check toggle
        update_toggle(&toggle);
        if((toggle.val != toggle.last_val) && toggle.val == false) {
            selected_effect->active = selected_effect->active ? false : true;
            // toggle LED and turn on/off SPDT
            if(selected_effect_num == EFFECT_0) {
                gpio_write(LED_0, (bool)selected_effect->active);
                gpio_write(SPDT_IO_0, (bool)selected_effect->active);
            }
            else if(selected_effect_num == EFFECT_1) {
                gpio_write(LED_1, (bool)selected_effect->active);
                gpio_write(SPDT_IO_1, (bool)selected_effect->active);
            }
            else {
                gpio_write(LED_2, (bool)selected_effect->active);
                gpio_write(SPDT_IO_2, (bool)selected_effect->active);
            }
            newly_selected = true;
        }


        // read in pot values
        gtsr_adc_start(&adc3);
        gtsr_adc_poll(&adc3, ADC_TIMEOUT);
        gtsr_adc_start(&adc5);
        gtsr_adc_poll(&adc5, ADC_TIMEOUT);

        update_pot(&volume, UPDATE_ADC_VOLUME_POT);
        update_pot(&tone, UPDATE_ADC_TONE_POT);
        update_pot(&gain, UPDATE_ADC_GAIN_POT);


        // if effect is active, send value over SPI to effect board
        if(selected_effect->active && \
            ((volume.value != volume.last_val) \
            || (tone.value != tone.last_val) \
            || (gain.value != gain.last_val) \
            || newly_selected)) {

            // update spi peripheral matrix with pot values
            spi_peripheral_data_matrix[selected_effect_num][VOLUME] = (uint16_t) volume.value;
            spi_peripheral_data_matrix[selected_effect_num][TONE] = (uint16_t) tone.value;
            spi_peripheral_data_matrix[selected_effect_num][GAIN] = (uint16_t) gain.value;
            spi_peripheral_data_matrix[selected_effect_num][VOLUME2] = (uint16_t) (MAX_POT_VALUE - (volume.value));

            // send pot values to effect board
            gtsr_spi_transaction(&spi1, &(selected_effect->volume));
            gtsr_spi_transaction(&spi1, &(selected_effect->tone));
            gtsr_spi_transaction(&spi1, &(selected_effect->gain));
            gtsr_spi_transaction(&spi1, &(selected_effect->volume2));

            newly_selected = false;
        }



        // GTSR_PRINTF("volume: %lu | tone: %lu | gain: %lu\n", volume.value, tone.value, gain.value);



        // HAL_Delay(50);  // TODO:Remove
    }


}


void update_pot(pot_t* pot, uint8_t buf_select) {
    uint16_t adc_val = pot->adc->out_buf[buf_select];
    pot->adc_arr[pot->idx] = adc_val;
    pot->idx++;
    if(pot->idx == pot->num_samples_to_average) {
        pot->idx = 0;
    }
    uint32_t sum = 0;
    for(uint32_t i = 0; i < pot->num_samples_to_average; i++) {
        sum += pot->adc_arr[i];
    }
    pot->last_val = pot->value;
    pot->value = (uint8_t)(sum >> ADC_DIV_FACTOR);
}


inline void update_toggle(debounce_t* toggle) {
    toggle->values[toggle->idx] = gpio_read_pin(EFFECT_TOGGLE);
    toggle->idx++;
    if(toggle->idx == DEBOUNCE_LENGTH) {
        toggle->idx = 0;
    }
    uint32_t sum = 0;
    for(uint32_t i = 0; i < DEBOUNCE_LENGTH; i++) {
        sum += (uint32_t) (toggle->values[i]);
    }
    toggle->last_val = toggle->val;
    toggle->val = (uint8_t) (sum >> DEBOUND_DIV_FACTOR);
}
