#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stm32h7xx_hal.h>
#include <adc.h>

/*TODO: set SPI data size to 16 bits*/
/*TODO: peripheral-to-memory mode for DMA*/
/*TODO: Add variable for GPIO pin */

/*
HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
    case GPIO_PIN_14:
        fso_flag = 1;
        break;

    default:
        break;
    }
}

GPIO_Pin == GPIO_PIN_14;
*/
/*

*/
/*HAL_GPIO_TogglePin(PA4_GPIO_Port,PA4_Pin);
    HAL_Delay(500);*/

extern SPI_HandleTypeDef hspi1;

/* Double buffers */
ADCBuffer buffer_a = {0};
ADCBuffer buffer_b = {0};

/*
 * Returns pointer to full buffer.
 * Returns NULL if no buffer is ready.
 */
int16_t *adc_get_sample(void)
{
    if (buffer_a.status == BUFFER_FULL)
    {
        buffer_a.status = BUFFER_EMPTY;
        return (int16_t*)buffer_a.data;
    }

    if (buffer_b.status == BUFFER_FULL)
    {
        buffer_b.status = BUFFER_EMPTY;
        return (int16_t*)buffer_b.data;
    }

    return NULL;
}

/* Initializes ADC sampling and DMA */
void adc_init(void)
{
    memset((int16_t*)buffer_a.data, 0, sizeof(buffer_a.data));
    memset((int16_t*)buffer_b.data, 0, sizeof(buffer_b.data));

    buffer_a.status = BUFFER_EMPTY;
    buffer_b.status = BUFFER_EMPTY;

    GPIO_TypeDef* SYNC_BANK = GPIOA;
	const uint16_t SYNC_PIN = GPIO_PIN_4;

    HAL_GPIO_WritePin(SYNC_BANK, SYNC_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(SYNC_BANK, SYNC_PIN, GPIO_PIN_SET);

    /* Start DMA into buffer A */
    buffer_a.status = BUFFER_FILLING;
    HAL_SPI_Receive_DMA(&hspi1,(uint8_t *)buffer_a.data,ADC_BUFFER_SIZE * sizeof(int16_t));
}

/* Call when DMA finishes writing to a buffer */
void adc_dma_done(void)
{
    if (buffer_a.status == BUFFER_FILLING)
    {
        buffer_a.status = BUFFER_FULL;
        buffer_b.status = BUFFER_FILLING;
        HAL_SPI_Receive_DMA(&hspi1,(uint8_t *)buffer_b.data,ADC_BUFFER_SIZE * sizeof(int16_t));
    }
    else if (buffer_b.status == BUFFER_FILLING)
    {
        buffer_b.status = BUFFER_FULL;
        buffer_a.status = BUFFER_FILLING;
        HAL_SPI_Receive_DMA(&hspi1,(uint8_t *)buffer_a.data,ADC_BUFFER_SIZE * sizeof(int16_t));
    }
}
