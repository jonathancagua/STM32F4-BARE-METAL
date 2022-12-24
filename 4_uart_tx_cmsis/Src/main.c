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
#include "stm32f4xx.h"

#define GPIOAEN			(1U<<0)
#define UART2EN			(1U<<17)

#define CR1_TE			(1U<<3)
#define CR1_UE			(1U<<13)
#define SR_TXE			(1U<<7)



#define SYS_FREQ		16000000
#define APB1_CLK		SYS_FREQ

#define UART_BAUDRATE		115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk,  uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

void uar2_tx_init(void);
void uart2_write(int ch);

int main(void)
{


	uar2_tx_init();

	while(1)
	{
		uart2_write('H');
		uart2_write('I');
		uart2_write('\n');
		uart2_write('\r');
		for(int i=0;i<900000;i++){}
	}
}


void uar2_tx_init(void)
{
	/****************Configure uart gpio pin***************/
	/*Habilitar el acceso del reloj al gpioa */
	RCC->AHB1ENR |= GPIOAEN;

	/*Poner el modo PA2 en modo de función alterna*/
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	/*Ajuste el tipo de función alternativa PA2 a UART_TX (AF07)*/
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	/****************Configurar módulo uart ***************/
	/*Habilitar el acceso del reloj a uart2 */
	RCC->APB1ENR |= UART2EN;

	/*configura baudrate*/
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);

	/*Configurar la dirección de transferencia*/
	 USART2->CR1 =  CR1_TE;

	/*habilitar uart module*/
	 USART2->CR1 |= CR1_UE;


}


void uart2_write(int ch)
{
  /*Asegúrese de que el registro de datos de transmisión está vacío*/
	while(!(USART2->SR & SR_TXE)){}

  /*Escritura en el registro de datos de transmisión*/
	USART2->DR	=  (ch & 0xFF);
}




static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk,  uint32_t BaudRate)
{
	USARTx->BRR =  compute_uart_bd(PeriphClk,BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}
