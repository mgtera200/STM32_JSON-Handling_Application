/*
 * ADC.c
 *
 *  Created on: Dec 7, 2024
 *      Author: Dell
 */

#include "ADC.h"
/*
PA0 -> ADC12_IN0
PA1 -> ADC12_IN1
PA2 -> ADC12_IN2
PA3 -> ADC12_IN3
PA4 -> ADC12_IN4
PA5 -> ADC12_IN5
PA6 -> ADC12_IN6
PA7 -> ADC12_IN7
PB0 -> ADC12_IN8
PB1 -> ADC12_IN9

PC0 -> ADC12_IN10
PC1 -> ADC12_IN11
PC2 -> ADC12_IN12
PC3 -> ADC12_IN13
PC4 -> ADC12_IN14
PC5 -> ADC12_IN15

ADC12_IN16 input channel which is used to convert the sensor output voltage into a digital value.


 */




char adc_init(ADC_REGISTERS_t *ADCx, short port, short pin){
	char channel;
	char result = 0;
	if(port == PA)
	{
		if(pin < 8)
		{
			result = 1;
			channel = pin;
		}
	}
	else if (port == PB)
	{
		if(pin<2)
		{
			result = 1;
			channel = 8 + pin;
		}
	}
	else if (port == PC)
	{
		if(pin<6)
		{
			result = 1;
			channel = 10 + pin;
		}
	}
	if(result)
	{
		Pin_Config_t GPIO_Pin_CNFG_s;
		GPIO_Pin_CNFG_s.Pin_Num = pin;
		GPIO_Pin_CNFG_s.mode = Input_Analog;

		if (port == PB) {
			MCAL_GPIO_Init(GPIOB, &GPIO_Pin_CNFG_s);
		}
		else if (port == PA) {
			MCAL_GPIO_Init(GPIOA, &GPIO_Pin_CNFG_s);
		}
		else if (port == PC) {
			MCAL_GPIO_Init(GPIOC, &GPIO_Pin_CNFG_s);
		}

		if(ADCx == ADC1)
		{
			ADC1_CLOCK_EN();
		}
		else if(ADCx == ADC2)
		{
			ADC2_CLOCK_EN();
		}

		// ADC Configuration
		ADCx->ADC_CR2 = 0;
		ADCx->ADC_SQR3 = channel;               // Select channel (e.g., channel 0 for PA0)
		ADCx->ADC_CR2 |= (1 << 0);              // Enable ADC (ADON)

		// Allow stabilization time
		for (volatile int i = 0; i < 1000; i++);
		ADCx->ADC_CR2 |= (1 << 0);              // Enable ADC (ADON)

		ADCx->ADC_CR2 |= (1 << 1);              // Enable continuous conversion mode (CONT)

	}
	return result ;
}

char adc_Deinit(ADC_REGISTERS_t *ADCx, short port, short pin){


	// ADC Configuration
	ADCx->ADC_CR2 = 0;
	ADCx->ADC_SQR3 = 0;               // Select channel (e.g., channel 0 for PA0)


	if (port == PB) {
		MCAL_GPIO_DeInit(GPIOB, pin);
	}
	else if (port == PA) {
		MCAL_GPIO_DeInit(GPIOA, pin);
	}
	else if (port == PC) {
		MCAL_GPIO_DeInit(GPIOC, pin);
	}
}




char adc_check(ADC_REGISTERS_t *ADCx){

	char check = 0;

	if(ADCx->ADC_SR & 2)
	{
		check  = 1;
	}


	return check;
}
int adc_rx(ADC_REGISTERS_t *ADCx, short port, short pin){

	int data;
	// Read ADC value
	data = ADCx->ADC_DR;
	return data; // Return temperature in Celsius

}
//void adc_irq(ADC_REGISTERS_t *ADCx, char port, char pin){
//
//	adc_init(ADCx, port, pin);// Initialize the ADC: adc_init(ADC1 or ADC2, Port, Pin)
//	ADCx->ADC_CR1 |= 0x20; // Setting the Interrupt
//	ADCx->ADC_CR1 |= 0x40;
//
//
//		__disable_irq();
//		NVIC_EnableIRQ(ADC1_2_IRQn);
//		__enable_irq();
//
//}
//void adc_wd(ADC_REGISTERS_t *ADCx, char port, char pin, short htr, short ltr){
//
//
//}


//void adc_multi_ch_init(ADC_REGISTERS_t *ADCx, char channels, char * adc_channels){
//	char i = 0;
//	//Initiate the pins
//	for(i=0;i< channels;i++)
//	{
//		if(adc_channels[i]<8)
//		{
//			init_GP(PA,adc_channels[i],IN,I_AN);
//		}
//		else if(adc_channels[i]<10)
//		{
//			init_GP(PB,adc_channels[i]-8,IN,I_AN);
//		}
//		else if(adc_channels[i]<16)
//		{
//			init_GP(PC,adc_channels[i]-10,IN,I_AN);
//		}
//	}
//	if(adc == 1)
//	{
//		//Setup the control registers
//		RCC->APB2ENR |= 0x201;
//		ADC1->CR2 = 0;
//		ADC1->CR2 |= 1;
//		DelayMs(100);
//		ADC1->SQR3 = adc_channels[1];
//		ADC1->CR2 |= 2; //Continous readings
//		ADC1->CR2 |= 1; //starting the conversion
//	}
//	else if(adc == 2) {
//		//Setup the control registers
//		RCC->APB2ENR |= 0x401;
//		ADC2->CR2 = 0;
//		ADC2->CR2 |= 1;
//		DelayMs(100);
//		ADC2->SQR3 = adc_channels[1];
//		ADC2->CR2 |= 2; //Continous readings
//		ADC2->CR2 |= 1; //starting the conversion
//
//	}
//
//}
//void adc_multi_ch_rx(ADC_REGISTERS_t *ADCx, char channels, char * adc_channels, int * analog_rx){
//
//	char i = 0;
//	int temp_rx = 0;
//	if(adc==1)
//	{
//		while(1)
//		{
//			if(adc_check(adc1))
//			{
//				temp_rx = ADC1->DR;
//				//analog_rx[i] = (temp_rx  *1000) / 0xFFF;
//				analog_rx[i] = temp_rx ;
//				i++;
//				if(i == channels)
//				{
//					i = 0;
//					ADC1->SQR3 = adc_channels[i];
//					break;
//				}
//				else
//				{
//					ADC1->SQR3 = adc_channels[i];
//				}
//			}
//
//		}
//	}
//	else if(adc==2)
//	{
//		while(1)
//		{
//			if(adc_check(adc1))
//			{
//				temp_rx = ADC2->DR;
//				//analog_rx[i] = (temp_rx  *1000) / 0xFFF;
//				analog_rx[i] = temp_rx ;
//				i++;
//				if(i == channels)
//				{
//					i = 0;
//					ADC2->SQR3 = adc_channels[i];
//					break;
//				}
//				else
//				{
//					ADC2->SQR3 = adc_channels[i];
//				}
//			}
//
//		}
//	}
//
//}
