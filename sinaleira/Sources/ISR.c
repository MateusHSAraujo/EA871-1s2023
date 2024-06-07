/**
 * @file ISR.c
 * @brief Definicao das rotinas de servico
 * @date 31/03/2023
 * @author Mateus Henrique Silva Araujo
 */

//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"

//Inclusao de macros do IDE
#include "derivative.h"

//Inclusao de prototipos
#include "ISR.h"
#include "util.h"
#include "GPIO_ledRGB.h"
#include "SysTick.h"

static uint32_t counter;

static uint32_t maxCounter;

static tipo_estado estado;

/**
 * @brief Funcao de tratamento da interrupcao de NMI
 */
void PORTA_IRQHandler (){
	
	if (PORTA_PCR4 & PORT_PCR_ISF_MASK) {
		PORTA_PCR4 |= PORT_PCR_ISF_MASK; //limpa flag
		if(estado == APAGADO){ //NMI deve fazer efeito somente se estiver apagado
			SysTick_ativaInterrupt ();
			estado = APAGADO_VERMELHO;
		}
	}
	
}

/**
 * @brief Funcao de tratamento das interrupcoes temporarizadas
 */
void SysTick_Handler() {
	counter++;
		
	if (counter == maxCounter) {

		switch (estado) {
		case APAGADO:
			estado=APAGADO_VERMELHO;
			break;
		case VERMELHO:
			estado = VERMELHO_AMARELO;
			break;
		case AMARELO:
			estado = AMARELO_VERDE;
			break;
		case VERDE:
			estado = VERDE_APAGADO;
			break;
		default:
			
			break;
		}
		counter = 0;
	}
}

void ISR_resetaCounter() {
	counter = 0;
}

void ISR_setaMaxPostScaler(uint32_t valor) {
	maxCounter = valor;
}

void ISR_escreveEstado(tipo_estado valor) {
	estado = valor;
}

tipo_estado ISR_leEstado () {
	return estado;
}
