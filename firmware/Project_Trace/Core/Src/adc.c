#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stm32h7xx_hal.h>
#include <hal_gpio.h>
#include <adc.h>

#include <meas.h>

#include <serialization.h>

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
extern DMA_HandleTypeDef hdma_spi1_rx;

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

	const GPIO_Pin_t SYNC = { GPIOA, GPIO_PIN_4 };
	const GPIO_Pin_t CLK_EN = { GPIOC, GPIO_PIN_8 };

    HAL_GPIO_WritePin(SYNC.bank, SYNC.pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(SYNC.bank, SYNC.pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(CLK_EN.bank, CLK_EN.pin, GPIO_PIN_RESET);

    /* Start DMA into buffer A */
    buffer_a.status = BUFFER_FILLING;

    HAL_DMA_Init(&hdma_spi1_rx);
    HAL_SPI_Receive_DMA(&hspi1,(uint8_t *)buffer_a.data,ADC_BUFFER_SIZE * sizeof(int16_t));
}

/* Call when DMA finishes writing to a buffer */
void adc_dma_done(void)
{
	// TODO: it is assumed this function will only be called when an ADC-related DMA transfer is complete.
	ADCBuffer* active_buffer;
	_Bool a_active = 0;
    if (buffer_a.status == BUFFER_FILLING)
    {
        buffer_a.status = BUFFER_FULL;
        buffer_b.status = BUFFER_FILLING;

        active_buffer = &buffer_a;
        a_active = 1;
        //HAL_SPI_Receive_DMA(&hspi1,(uint8_t *)buffer_b.data,ADC_BUFFER_SIZE * sizeof(int16_t));
    }
    else if (buffer_b.status == BUFFER_FILLING)
    {
        buffer_b.status = BUFFER_FULL;
        buffer_a.status = BUFFER_FILLING;

        active_buffer = &buffer_b;
        a_active = 0;
        //HAL_SPI_Receive_DMA(&hspi1,(uint8_t *)buffer_a.data,ADC_BUFFER_SIZE * sizeof(int16_t));
    }


    if(a_active) {
    	HAL_SPI_Receive_DMA(&hspi1,(uint8_t *)buffer_b.data,ADC_BUFFER_SIZE * sizeof(int16_t));
    } else {
    	HAL_SPI_Receive_DMA(&hspi1,(uint8_t *)buffer_a.data,ADC_BUFFER_SIZE * sizeof(int16_t));
    }


    // TODO: Testing solution
    int16_t printbuf[ADC_BUFFER_SIZE];
    memcpy(printbuf, (void*) active_buffer->data, ADC_BUFFER_SIZE * sizeof(int16_t));

    enum SerDataType column_types[1] = {Ser_Int32};
    // void* columns[1] = { (void*) active_buffer->data};

    int32_t sum = 0;
    for (size_t i = 0; i < ADC_BUFFER_SIZE; i++) {
    	sum += active_buffer->data[i];
    }
    int32_t mean = sum / ADC_BUFFER_SIZE;
    //void* columns[1] = { (void*) printbuf};
    void* columns[1] = { (void*) &mean};

    char* header_labels[1] = { "x" };
    (void)ser_file_write_csv_header(stdout, header_labels, 1);
    // enum SerResult res = ser_file_write_csv_data(stdout, columns, ADC_BUFFER_SIZE, column_types, 1);
    enum SerResult res = ser_file_write_csv_data(stdout, columns, 1, column_types, 1);
    (void) res;
}
