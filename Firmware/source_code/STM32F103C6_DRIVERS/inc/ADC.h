/*
 * ADC.h
 *
 *  Created on: Dec 7, 2024
 *      Author: Dell
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_


//-----------------------------------------
//-------<< INCLUDES >>--------------------
//-----------------------------------------
#include "STM32F103x8.h"
#include "GPIO_DRIVER.h"


#define PA          1
#define PB          2
#define PC          2

enum channels
{
	PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PB0,PB1,PC0,PC1,PC2,PC3,PC4,PC5,temp_sensor
};


char adc_init(ADC_REGISTERS_t *ADCx, short port, short pin);
char adc_Deinit(ADC_REGISTERS_t *ADCx, short port, short pin);
char adc_check(ADC_REGISTERS_t *ADCx);
int adc_rx(ADC_REGISTERS_t *ADCx, short port, short pin);
void adc_irq(ADC_REGISTERS_t *ADCx, char port, char pin);
void adc_wd(ADC_REGISTERS_t *ADCx, char port, char pin, short htr, short ltr);
void adc_multi_ch_init(ADC_REGISTERS_t *ADCx, char channels, char * adc_channels);
void adc_multi_ch_rx(ADC_REGISTERS_t *ADCx, char channels, char * adc_channels, int * analog_rx);



#endif /* INC_ADC_H_ */
