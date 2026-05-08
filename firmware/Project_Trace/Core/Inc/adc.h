#include "main.h"
#include <stdint.h>

/* Buffer size in samples */
#define ADC_BUFFER_SIZE 256

typedef enum
{
    BUFFER_EMPTY = 0,
    BUFFER_FULL,
    BUFFER_FILLING
} BufferStatus;

typedef struct
{
    volatile BufferStatus status;
    int16_t data[ADC_BUFFER_SIZE];
} ADCBuffer;

/* Returns pointer to a full buffer.
 * Returns NULL if no buffer is ready. */
int16_t *adc_get_sample(void);

/* Initializes ADC sampling and DMA */
void adc_init(void);

/* Call when DMA finishes writing to a buffer */
void adc_dma_done(void);
