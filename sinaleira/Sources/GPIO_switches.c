/*
 * @file GPIO_switches.c
 * @brief Definicao das funcoes declaradas em GPIO_switches.h
 * @date 31/03/2023
 * @author Mateus Henrique Silva Araujo
 */

#include "derivative.h"

/**
 * @brief Funcao para inicializar a botoeira NMI
 * @param[in] IRQC interrupcao sendo ativada ; prioridade valor de prioridade da interrupcao 
 */
void GPIO_initSwitchNMI (uint8_t IRQC, uint8_t prioridade) {
	/**
	 * Configura o modulo SIM: ativacao dos sinais de relogio de PORTB
	 * Ativacao dos sinais de relogio de PORTA
	 */
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; // Seta bit 9 de SIM_SCGC5

	/**
	 * Configura o modulo PORTA_PCR4
	 * registrador PORTA_PCR4 mapeado em 0x40049010 
	 */
	
	PORTA_PCR4 &= ~PORT_PCR_MUX_MASK; // Zera bits 10, 9 e 8 (MUX) de PTA4
	PORTA_PCR4 |= PORT_PCR_MUX(1);  // Seta bit 8 do MUX de PTA4


	/**
	 * Configura o modulo GPIOA: sentido de entrada do sinal no pino PTA4, sem
	 * interrupcao habilitada
	 * registrador GPIOA_PDDR mapeado em 0x400ff014u
	 */
	GPIOA_PDDR &= ~GPIO_PDDR_PDD(1<<4); // Reseta bit 4 de GPIOA_PDDR

	/**
	 * Limpa flag de interrupcao do pino 4
	 * registrador PORTA_PCR4 mapeado em 0x40049010u 
	 */
	PORTA_PCR4 |= PORT_PCR_ISF_MASK;
			
	/**
	 * Habilita a interrupcao do pino 4 para IRQC
	 */
	PORTA_PCR4 &= ~PORT_PCR_IRQC_MASK;
	PORTA_PCR4 |= PORT_PCR_IRQC(IRQC); 

	/**
	 * Configura o modulo NVIC: habilita IRQ 30
	 * registrador NVIC_ISER mapeado em 0xe000e100u
	 */
	NVIC_ISER  = (1<<30);	

	/**
	 * Configura o modulo NVIC: limpa pendencias IRQ 30
	 * registrador NVIC_ICPR mapeado em 0xe000e280u
	 */
	NVIC_ICPR = (1<<30);

	/**
	 * Configura o modulo NVIC: seta prioriodade para IRQ30
	 * registrador NVIC_IPR7 mapeado em 0xe000e41cu (Tabela 3-7/p. 54 do Manual)
	 */
	NVIC_IPR7 |= (prioridade<<22); //(Secao 3.3.2.1/p. 52 do Manual)

	
	return;
}

