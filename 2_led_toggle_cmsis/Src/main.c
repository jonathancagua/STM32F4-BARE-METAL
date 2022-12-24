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
#define LED_NBR			(14) /* <-- seleccione el pin de su placa.*/
#define PIN12			(1U<<LED_NBR)
#define LED_PIN			PIN12
#define MODER_LEN		(2)
#define LED_MODE_R		(LED_NBR * MODER_LEN)


int main(void)
{
	uint32_t reg;
	RCC->AHB1ENR |=GPIODEN;

	//limpiamos los flags y ponemos en cero
	reg = GPIOD->MODER & ~ (0x03 << LED_MODE_R);
	//seteamos como general purpose output mode
	GPIOD->MODER = reg | (1 << LED_MODE_R);

	while(1)
	{
		GPIOD->ODR ^=LED_PIN;
		for(int i=0;i<100000;i++){}
	}
}
