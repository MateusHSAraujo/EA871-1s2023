/*
 * @file PIT.c
 * @brief Definicao das funcoes relacionadas com o modulo PIT
 * @date 14/04/2023
 * @author Wu Shin-Ting 
 * @author Mateus Henrique Silva Araujo
 */

#include "derivative.h"
#include "util.h"

void PIT_initTimer0(uint32_t periodo, uint8_t prioridade) {
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;            // ativar o sinal de relogio do PIT

	PIT_TCTRL0 &= ~(PIT_TCTRL_TEN_MASK); 		// desativar o timer 
	PIT_TCTRL0 &= ~PIT_TCTRL_CHN_MASK;   		// timers nao encadeados
	PIT_LDVAL0 = PIT_LDVAL_TSV(periodo);       	// carregar o valor de recarga

	PIT_TCTRL0 |= (PIT_TCTRL_TEN_MASK);  		// ativa timer 0

    // Ativa IRQ 22 (PIT) no NVIC: ativar, limpar pendencias e setar prioridade 
    NVIC_ISER = (1 << 22);           	 // NVIC_ISER[22]=1 (habilita IRQ22)
    NVIC_ICPR = (1 << 22);         	 // NVIC_ICPR[22]=1 (limpa as pendências)
    NVIC_IPR5 |= NVIC_IP_PRI_22(prioridade << 6); // 22/4 -> 5

    PIT_TCTRL0  |= PIT_TCTRL_TIE_MASK;	  // ativar interrupcao em PIT

	PIT_MCR &= ~(PIT_MCR_FRZ_MASK |             // continua contagem no modo Debug
			PIT_MCR_MDIS_MASK );     			// habilita modulo PIT	
}

void PIT_ativaNVICIRQ (uint8_t prioridade) { 
    // Ativa IRQ 22 (PIT) no NVIC: ativar, limpar pendencias e setar prioridade 
    NVIC_ISER = (1 << 22);           	 // NVIC_ISER[22]=1 (habilita IRQ22)
    NVIC_ICPR = (1 << 22);         	 // NVIC_ICPR[22]=1 (limpa as pendências)
    NVIC_IPR5 |= NVIC_IP_PRI_22(prioridade << 6); // 22/4 -> 5

    PIT_TCTRL0  |= PIT_TCTRL_TIE_MASK;	  // ativar interrupcao em PIT
    
    return;
}

void PIT_desabilitaIRQ () {
    PIT_TCTRL0  &= ~PIT_TCTRL_TIE_MASK;	  // desativar interrupcao em PIT

    return;
}

void PIT_reabilitaIRQ () {
    PIT_TCTRL0  |= PIT_TCTRL_TIE_MASK;	  // desativar interrupcao em PIT

    return;
}

void PIT_setaLDVAL (uint32_t periodo) {
	PIT_LDVAL0 = PIT_LDVAL_TSV(periodo);       	// carregar o valor de recarga

    return;
}

void PIT_desativaTimer0 () {
	PIT_TCTRL0 &= ~(PIT_TCTRL_TEN_MASK);  		// desativa timer 0

    return;
}

void PIT_ativaTimer0() {
	PIT_TCTRL0 |= (PIT_TCTRL_TEN_MASK);  		// ativa timer 0

	return;
}




