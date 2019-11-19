#define ADC_0_PIN 23
#define ADC_0_PORT 0
#define ADC_0_FUNC 1

#define ADC_INT_ENABLE NVIC_EnableIRQ(ADC_IRQn);
#define ADC_INT_DISABLE NVIC_DisableIRQ(ADC_IRQn);

void adc_init();
uint32_t adc_read_blocking();

void adc_int_config (void (*adc_int_handler)(uint32_t value));

void adc_start();

void adc_stop();

void ADC_IRQHandler(void);