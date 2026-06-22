#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stm32h7xx_hal.h>
#include <hal_gpio.h>
#include <adc.h>
#include <fixpoint.h>
#include <main.h>

#include <menu_system.h>

#include <sample_buffer.h>

#include <meas.h>
#include <calibration.h>

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
//ADCBuffer buffer_a = {0};
//ADCBuffer buffer_b = {0};


ADCBuffer adcbuf = {0};

extern SampleBuffer sample_buffer;

/*
 * Returns pointer to full buffer.
 * Returns NULL if no buffer is ready.
 */
volatile int16_t *adc_get_sample(void)
{
//    if (buffer_a.status == BUFFER_FULL)
//    {
//        buffer_a.status = BUFFER_EMPTY;
//        return (int16_t*)buffer_a.data;
//    }
//
//    if (buffer_b.status == BUFFER_FULL)
//    {
//        buffer_b.status = BUFFER_EMPTY;
//        return (int16_t*)buffer_b.data;
//    }
//
//    return NULL;

	return adcbuf.data;
}

/* Initializes ADC sampling and DMA */
void adc_init(void)
{
	printf("adc_init\n");
    //memset((int16_t*)buffer_a.data, 0, sizeof(buffer_a.data));
    //memset((int16_t*)buffer_b.data, 0, sizeof(buffer_b.data));

    //buffer_a.status = BUFFER_EMPTY;
    //buffer_b.status = BUFFER_EMPTY;


	const GPIO_Pin_t SYNC = { GPIOA, GPIO_PIN_4 };
	const GPIO_Pin_t CLK_EN = { GPIOC, GPIO_PIN_8 };

    HAL_GPIO_WritePin(SYNC.bank, SYNC.pin, GPIO_PIN_SET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(SYNC.bank, SYNC.pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(CLK_EN.bank, CLK_EN.pin, GPIO_PIN_RESET);

    adcbuf.status = BUFFER_FILLING;

    HAL_SPI_Receive_DMA(&hspi1,(uint8_t *)adcbuf.data,ADC_BUFFER_SIZE);
}

void adc_debug_print(void) {
	ADCBuffer* active_buffer = &adcbuf;
    int16_t printbuf[ADC_BUFFER_SIZE];
    memcpy(printbuf, (void*) active_buffer->data, ADC_BUFFER_SIZE * sizeof(int16_t));
    enum SerDataType column_types[1] = {Ser_Int16};
    int mean = active_buffer->data[0];
    const void* columns[1] = { (void*) &mean};
    enum SerResult res = ser_file_write_csv_data(stdout, columns, 1, column_types, 1);
    (void) res;
}

/* Call when DMA finishes writing to a buffer */
size_t adc_counter = 0;


void adc_dma_done(void)
{
	//printf("adc_dma_done\n");
	int16_t adc_samples[ADC_BUFFER_SIZE];
	memcpy(adc_samples, (int16_t *)adcbuf.data, ADC_BUFFER_SIZE * sizeof(int16_t));
	fix9_23 buf[ADC_BUFFER_SIZE];
	cal_calibrate_buffer(buf, adc_samples, ADC_BUFFER_SIZE);
	sample_buffer_write_samples(&sample_buffer, buf, ADC_BUFFER_SIZE);

	if (adc_counter == 500) {
		adc_counter = 0;
		printf("Fire Frame_Update\n");
		menu_system_frame_update();
	} else adc_counter++;
}
