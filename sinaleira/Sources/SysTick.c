/**
 * @file SysTick.c
 * @brief Definicao das funcoes relativas ao modulo SysTick
 * @date Jan 11, 2023
 * @author Wu Shin-Ting
 */
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"
//Inclusao das macros do IDE
#include "derivative.h"

void SysTick_init (uint32_t periodo) {
	SYST_RVR = SysTick_RVR_RELOAD(periodo);   
	SYST_CVR = SysTick_CVR_CURRENT(0);          //reseta flag e recarga     
	SYST_CSR |= (
			SysTick_CSR_CLKSOURCE_MASK 		//fonte de CLK: nucleo
			| SysTick_CSR_ENABLE_MASK     	//habilita SysTick 
	);
}

void SysTick_ativaInterrupt () {
	SYST_CSR |= SysTick_CSR_TICKINT_MASK;    //!< \li \l ativa interrupcao do SysTick 	
}

void SysTick_desativaInterrupt () {
	SYST_CSR &= ~SysTick_CSR_TICKINT_MASK;    //!< \li \l desativa interrupcao do SysTick 	
}

void SysTick_resetaCVR() {
	SYST_CVR = SysTick_CVR_CURRENT(0);
}



