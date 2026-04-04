/**
 * @file           : main.c
 * @brief          : Main program body
 */

#include "main.h"
#include "stm32g4xx.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_def.h"
#include "gtsr_clock.h"
#include "gtsr_spi.h"
#include "gtsr_gpio.h"
#include "stdint.h"
#include "gtsr_debug.h"

#define ever  (;;)

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
    /*TODO: Remove once HAL is no longer needed */
    HAL_Init();
    system_clock_init();
    GTSR_DEBUG_INIT();

    GTSR_PRINTF("starting program..\n");

    // spi object
    gtsr_spi_t spi1;

    // spi initializer
    gtsr_spi_initializer_t init;
    init.spi_select = GTSR_SPI1;
    init.clock_polarity = GTSR_SPI_IDLE_LOW;
    init.clock_phase = GTSR_SPI_SAMPLE_FIRST_EDGE;
    init.data_direction = GTSR_SPI_MSB_FIRST;
    init.data_size = GTSR_SPI_16_BIT_DATA;
    init.frame_format = GTSR_SPI_MOTOROLA;
    init.clock_frequency = 125000;
    init.sck = GTSR_SPI1_SCK_PA5;
    init.mosi = GTSR_SPI1_MOSI_PA7;
    init.miso = GTSR_SPI1_MISO_PA6;

    // initialize spi object
    gtsr_spi_init(&spi1, &init);

    // create peripheral
    gtsr_spi_peripheral_t peripheral;
    // make data buffers
    uint16_t rec[4];
    uint16_t send[4] = {1, 3, 0x33, 7};

    peripheral.data_len = sizeof(send);
    peripheral.rec_buff = rec;
    peripheral.send_buff = send;
    peripheral.cs.base = GPIOA;
    peripheral.cs.pin = GTSR_GPIO_PIN_1;

    // initialize peripheral
    gtsr_spi_peripheral_init(&peripheral);


    for(;;)
    {
        gtsr_spi_transaction(&spi1, &peripheral);
        HAL_Delay(1000);
        GTSR_PRINTF("rec[0] = %d | rec[1] = %d | rec[2] = %d | rec[3] = %d\n", rec[0], rec[1], rec[2], rec[3]);
    }


}


#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
       line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
