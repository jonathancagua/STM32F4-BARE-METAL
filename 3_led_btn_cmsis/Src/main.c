/**
 * @file main.c
 * @author Jonathan
 * @brief ejemplo de bare metal sin uso del cubo.
 * 		  Solo se usa registros para fines didacticos
 * @version 0.1
 * @date 2022-12-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "stm32f4xx.h"
#define GPIODEN			(1U<<3)
#define GPIOAEN			(1U<<0)

#define LED_NBR			(14) /* <-- seleccione el pin de su placa.*/
#define PIN12			(1U<<LED_NBR)
#define LED_PIN			PIN12
#define MODER_LEN		(2)
#define LED_MODE_R		(LED_NBR * MODER_LEN)

#define BTN_NBR			(0) /* <-- seleccione el pin de su placa.*/
#define BTN_PIN			(1U<<BTN_NBR)
#define BTN_MODE_R		(BTN_NBR * MODER_LEN)

int main(void)
{
	uint32_t reg;
	RCC->AHB1ENR |=GPIODEN;
	RCC->AHB1ENR |=GPIOAEN;

	//limpiamos los flags y ponemos en cero
	reg = GPIOD->MODER & ~ (0x03 << LED_MODE_R);
	//seteamos como general purpose output mode
	GPIOD->MODER = reg | (1 << LED_MODE_R);

	//limpiamos los flags y ponemos en cero
	reg = GPIOA->MODER & ~ (0x03 << BTN_MODE_R);
	//seteamos como general purpose input mode
	GPIOA->MODER = reg;

	while(1)
	{
		/*Check if BTN is pressed*/
		if(GPIOA->IDR & BTN_PIN)
			GPIOD->BSRR =(1U<<((LED_NBR+1)*2));
		else
			GPIOD->BSRR =LED_PIN;
	}
}
