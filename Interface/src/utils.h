/*
 * utils.h
 *
 *  Created on: 26 Oct 2017
 *      Author: Mustafa
 */

#ifndef UTILS_H_
#define UTILS_H_

/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "tm_stm32f4_hd44780.h"
#include "tm_stm32f4_button.h"
#include "tm_stm32f4_delay.h"
#include <stdio.h>
#include <string.h>


/* Modes */
#define MODE_SHIFT 0
#define MODE_AUTOTUNE 1
#define MODE_REVERB 2
#define MODE_ECHO 3
#define VOLUME 4


/* LCD dimensions */
#define LCD_COLUMNS 16
#define LCD_ROWS 2

/* GPIO struct */
typedef struct {
	char * potVal;
	char * lcdStr;
}MODE ;

void config(void);
void delay_ms(uint32_t milli);
int8_t adc_convert(void);
char * int_to_char(int8_t potVal);

void config(void)
{

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //Buttons
	// Clock for SYSCFG
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Put your GPIO initialisation here (Hint use 'GPIO_Init function) */
	/* Configure the GPIO_LED pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_3 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Selects the GPIOA pin 0 used as external interrupt source
	//SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);

	ADC_InitTypeDef ADC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);// ADC1
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);//Clock for the ADC port

	//Analog pin configuration
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;//The channel 10 is connected to PC0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
	GPIO_Init(GPIOC,&GPIO_InitStruct);

	//ADC structure configuration
	ADC_DeInit();
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; //the conversion is continuous, the input data is converted more than once
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;// conversion is synchronous with TIM1 and CC1 (actually I'm not sure about this one :/)
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1,&ADC_InitStruct);
	//Enable ADC conversion
	ADC_Cmd(ADC1,ENABLE);
	//Select the channel to be read from
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_144Cycles);
	/*
	// External interrupt settings
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	// Nested vectored interrupt settings
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	// EXTI0_IRQn has Most important interrupt
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&NVIC_InitStruct);
	*/

}

void delay_ms(uint32_t milli)
{
	uint32_t delay = milli * 17612;              // approximate loops per ms at 168 MHz, Debug config
	for(; delay != 0; delay--);
}

int8_t adc_convert(){
	 ADC_SoftwareStartConv(ADC1);//Start the conversion
	 while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//Processing the conversion
	 return ADC_GetConversionValue(ADC1)/40.95; //Return the converted data

}



#endif /* UTILS_H_ */
