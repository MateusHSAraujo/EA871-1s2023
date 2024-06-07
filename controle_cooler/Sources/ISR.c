/*!
 * @file ISR.c
 * @brief Funcoes para tratamento de rotinas de interrupcao
 * @date 19/05/2023
 * @author Mateus Henrique Silva Araujo   
 */

#include "derivative.h"
#include "ISR.h"
#include "ADC.h"

/**
 * Variaveis GLOBAIS de controle
 */
static uint16_t valor[2];		///< Valores convertidos: 0 -> Potenciometro , 1 -> Temperatura
static tipo_estado estado;	///< estado do sistema


tipo_estado ISR_LeEstado () {
	return estado;
}

void ISR_escreveEstado (tipo_estado novo) {
	estado = novo;
	return;
}

void ISR_LeValoresAmostrados (uint16_t *v) {
	v[0] = valor[0];
	v[1] = valor[1];
}

/**
 *  Rotina de servico para tratar interrupcoes de ADC0
 */
void ADC0_IRQHandler(void) {
	if( ADC0_SC1A & ADC_SC1_COCO_MASK )
	{	
		uint16_t conversao = ADC0_RA;
		if (estado == AMOSTRA_VOLT) { // Conversao do potenciometro finalizada
			
			// Guarda o valor obtido na variável global:
			valor[0]=conversao;
			
			//Altera o sistema para conversao do sinal de temperatura (por software):
			ADC0_SC2 &= ~ADC_SC2_ADTRG_MASK; 	//	Muda para trigger por software
			ADC_selecionaCanal (0b11010);	 	// Seleciona o canal do sensor de temperatura
			
			//Chavei para estado AMOSTRA_TEMP
			estado = AMOSTRA_TEMP;
			return;
		} 
		else if (estado == AMOSTRA_TEMP) {
			// Guarda o valor obtido na variável global:
			valor[1]=conversao;
						
			//Altera o sistema para conversao do sinal de temperatura (por software):
			ADC0_SC2 |= ADC_SC2_ADTRG_MASK; 	// Muda para trigger por hardtware
			ADC_selecionaCanal (0b01001);	 	// Seleciona o canal de PTB1
			
			//Chaveia para estado ATUALIZACAO
			estado = ATUALIZACAO;
			return;
		}
	}
}

	
