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
#include <stdint.h>

#define PERIPH_BASE				(0x40000000UL)
#define AHB1PERIPH_OFFSET		(0x00020000UL)
#define AHB1PERIPH_BASE	        (PERIPH_BASE + AHB1PERIPH_OFFSET)
#define GPIOD_OFFSET			(0X0C00UL)

#define GPIOD_BASE				(AHB1PERIPH_BASE + GPIOD_OFFSET)

#define RCC_OFFSET				(0x3800UL)
#define RCC_BASE				(AHB1PERIPH_BASE + RCC_OFFSET)


#define AHB1EN_R_OFFSET			(0x30UL)
#define RCC_AHB1EN_R            (*(volatile unsigned int *)(RCC_BASE +  AHB1EN_R_OFFSET))

#define MODE_R_OFFSET			(0x00UL)
#define GPIOD_MODE_R			(*(volatile unsigned int *)(GPIOD_BASE + MODE_R_OFFSET))


#define OD_R_OFFSET				(0x14UL)
#define GPIOD_OD_R				(*(volatile unsigned int *)(GPIOD_BASE +  OD_R_OFFSET))


#define  GPIODEN				(1U<<3)
#define LED_NBR					(12) /* <-- seleccione el pin de su placa.*/
#define PIN12					(1U<<LED_NBR)
#define LED_PIN					PIN12
#define MODER_LEN				(2)
#define LED_MODE_R				(LED_NBR * MODER_LEN)
int main(void)
{
	uint32_t reg;
	/*Se habilita el clock del GPIOD*/
	RCC_AHB1EN_R  |=  GPIODEN;

	//limpiamos los flags y ponemos en cero
	reg = GPIOD_MODE_R & ~ (0x03 << LED_MODE_R);
	//seteamos como general purpose output mode
	GPIOD_MODE_R = reg | (1 << LED_MODE_R);
	while(1)
	{

		GPIOD_OD_R ^= LED_PIN;
		for(int i=0;i<100000;i++);

	}
}
