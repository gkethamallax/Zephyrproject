/* SoC level DTS fixup file */

#define DT_NUM_IRQ_PRIO_BITS	DT_ARM_V7M_NVIC_E000E100_ARM_NUM_IRQ_PRIORITY_BITS

#define DT_ADC_0_NAME			DT_NORDIC_NRF_SAADC_ADC_0_LABEL
#define DT_ADC_0_PARENT_NAME			DT_NORDIC_NRF_SAADC_ADC_0_PARENT_LABEL

#ifdef DT_NORDIC_NRF_UART_UART_0_LABEL
#define DT_UART_0_NAME			DT_NORDIC_NRF_UART_UART_0_LABEL
#define DT_UART_0_PARENT_NAME			DT_NORDIC_NRF_UART_UART_0_PARENT_LABEL
#else
#define DT_UART_0_NAME			DT_NORDIC_NRF_UARTE_UART_0_LABEL
#define DT_UART_0_PARENT_NAME			DT_NORDIC_NRF_UARTE_UART_0_PARENT_LABEL
#endif

#define DT_UART_1_NAME			DT_NORDIC_NRF_UARTE_UART_1_LABEL
#define DT_UART_1_PARENT_NAME			DT_NORDIC_NRF_UARTE_UART_1_PARENT_LABEL

#define DT_FLASH_DEV_NAME		DT_NORDIC_NRF52_FLASH_CONTROLLER_0_LABEL
#define DT_FLASH_PARENT_NAME		DT_NORDIC_NRF52_FLASH_CONTROLLER_0_PARENT_LABEL

#define DT_GPIO_P0_DEV_NAME		DT_NORDIC_NRF_GPIO_GPIO_0_LABEL
#define DT_GPIO_P0_PARENT_NAME		DT_NORDIC_NRF_GPIO_GPIO_0_PARENT_LABEL
#define DT_GPIO_P1_DEV_NAME		DT_NORDIC_NRF_GPIO_GPIO_1_LABEL
#define DT_GPIO_P1_PARENT_NAME		DT_NORDIC_NRF_GPIO_GPIO_1_PARENT_LABEL

#define DT_I2C_0_NAME			DT_NORDIC_NRF_I2C_I2C_0_LABEL
#define DT_I2C_0_PARENT_NAME			DT_NORDIC_NRF_I2C_I2C_0_PARENT_LABEL
#define DT_I2C_1_NAME			DT_NORDIC_NRF_I2C_I2C_1_LABEL
#define DT_I2C_1_PARENT_NAME			DT_NORDIC_NRF_I2C_I2C_1_PARENT_LABEL

#define DT_SPI_0_NAME			DT_NORDIC_NRF_SPI_SPI_0_LABEL
#define DT_SPI_0_PARENT_NAME			DT_NORDIC_NRF_SPI_SPI_0_PARENT_LABEL
#define DT_SPI_1_NAME			DT_NORDIC_NRF_SPI_SPI_1_LABEL
#define DT_SPI_1_PARENT_NAME			DT_NORDIC_NRF_SPI_SPI_1_PARENT_LABEL
#define DT_SPI_2_NAME			DT_NORDIC_NRF_SPI_SPI_2_LABEL
#define DT_SPI_2_PARENT_NAME			DT_NORDIC_NRF_SPI_SPI_2_PARENT_LABEL
#define DT_SPI_3_NAME			DT_NORDIC_NRF_SPI_SPI_3_LABEL
#define DT_SPI_3_PARENT_NAME			DT_NORDIC_NRF_SPI_SPI_3_PARENT_LABEL

#define DT_WDT_0_NAME			DT_NORDIC_NRF_WATCHDOG_WDT_0_LABEL
#define DT_WDT_0_PARENT_NAME			DT_NORDIC_NRF_WATCHDOG_WDT_0_PARENT_LABEL

/* End of SoC Level DTS fixup file */
