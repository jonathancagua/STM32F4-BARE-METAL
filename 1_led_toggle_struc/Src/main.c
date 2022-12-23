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
#define LED_NBR					(14) /* <-- seleccione el pin de su placa.*/
#define PIN12					(1U<<LED_NBR)
#define LED_PIN					PIN12
#define MODER_LEN				(2)
#define LED_MODE_R				(LED_NBR * MODER_LEN)

/*
RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)
Address offset: 0x30
Reset value: 0x0000 0000
Access: no wait state, word, half-word and byte access.
*/
typedef struct
{
  volatile uint32_t DUMMY[12];
  volatile uint32_t AHB1ENR;
} rcc_t;

/*
GPIO port mode register (GPIOx_MODER) (x = A..E and H)
	• Address offset: 0x00
	• Reset values:
	• 0xA800 0000 for port A
	• 0x0000 0280 for port B
	• 0x0000 0000 for other ports
GPIO port output data register (GPIOx_ODR) (x = A..E and H)
	• Address offset: 0x14
	• Reset value: 0x0000 0000
 */
typedef struct
{
	volatile uint32_t MODER;
	volatile uint32_t DUMMY[4];
	volatile uint32_t ODR;
} gpio_t;

#define RCC		    ((rcc_t*) RCC_BASE)
#define GPIOD		((gpio_t*)GPIOD_BASE)

int main(void)
{
	uint32_t reg;
	/*Se habilita el clock del GPIOD*/
	RCC->AHB1ENR  |=  GPIODEN;

	//limpiamos los flags y ponemos en cero
	reg = GPIOD->MODER & ~ (0x03 << LED_MODE_R);
	//seteamos como general purpose output mode
	GPIOD->MODER = reg | (1 << LED_MODE_R);
	while(1)
	{

		GPIOD->ODR ^= LED_PIN;
		for(int i=0;i<100000;i++);

	}
}
