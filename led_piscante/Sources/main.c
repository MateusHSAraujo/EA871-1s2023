/**
 * @file main.c
 * @brief Led verde piscante em 2Hz
 * @author Wu Shin-Ting & Mateus Henrique Silva Araujo
 * @date 17/03/2023
 * @note Consulte no manual de referencia
 * (1) setup do microcontrolador: https://www.dca.fee.unicamp.br/cursos/EA871/references/ARM/KLQRUG.pdf
 * (2) detalhes dos registradores: https://www.dca.fee.unicamp.br/cursos/EA871/references/ARM/KLQRUG.pdf
 */

//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"

/**
 * @brief computo de numero de iteracoes a partir do multiplo <valor>
 * @param[in] valor multiplo
 * @param[out] i numero de iteracoes
 */
void multiplo_iteracoes (uint32_t valor, uint32_t *i)
{
	*i = valor * 32;
	
	return;
}

/**
 * @brief atraso em multiplo <valor> de 32 interacoes
 * @param[in] valor multiplo de 32
 */
void espera (uint32_t valor)
{
	 static uint16_t contador=0;
	 static uint32_t contador_offset=5;
	
	uint32_t i;
	

	contador++;
	contador_offset++;
	
	multiplo_iteracoes (valor, &i);
	
	while (i) i--;
}


/**
 * @brief funcao para criar atraso de 5 us por iteracao
 * @param[in] i valor que rege as iteracoes
 */
void espera_5us(unsigned int i)
{
	asm(
			"iteracao: \n\t"
				"mov r2, #4 \n\t"
			"laco: \n\t" 
				"mov r3, #5 \n\t"
				"orr r3, r0 \n\t"
				"and r3, r0 \n\t"     
				"lsr r3, #1 \n\t"
				"asr r3, #1 \n\t"
				"and r3, r0 \n\t"
				"lsr r3, #1 \n\t"
				"asr r3, #1 \n\t"
				"sub r2, #1 \n\t"					 
				"cmp r2, #0 \n\t"
				"bne laco \n\t"
				"rev r3,r3 \n\t"
				"lsl r3,#0 \n\t"
				"sub %0,#1 \n\t"
				"cmp %0, #0 \n\t"
				"bne iteracao \n\t"
			 :
			 : "r" (i)
			 : "r2","r3","cc"
	);
	return;
}


uint8_t teste;

/**
 * @brief Fluxo de controle principal
 */
int main(void)
{
	//Somente para verificar local de armazenamento das variaveis globais
	teste = 100;
	
	/**
	 * Configura o modulo SIM: ativacao dos sinais de relogio de PORTB
	 * registrador SIM_SCGC5 mapeado em 0x40048038u (pg. 206)
	 *
	 */
	//(*(unsigned int volatile *) 0x40048038u) =  *(unsigned int *) 0x40048038u | (1<<10);
	//equivalente a:
	*(uint32_t volatile *) 0x40048038u |= (1<<10);

	/**
	 * Configura o modulo PORTB_PCR19 e GPIOB: canal G do led RGB
	 * (pg. 10 em https://www.dca.fee.unicamp.br/cursos/EA871/references/ARM/FRDMKL25Z.pdf)
	 * registrador PORTB_PCR19 mapeado em 0x4004a04c (pg. 179)
	 */
	*(uint32_t volatile *) 0x4004a04cu &= ~0x700; // Zera bits 10, 9 e 8 (MUX) de PTB19
	(*(uint32_t volatile *) 0x4004a04cu) |= 0x00000100; // Seta bit 8 do MUX de PTB19

	/**
	 * Configura o modulo GPIOB: sentido de saida do sinal no pino PTB19
	 * registrador GPIOB_PDDR mapeado em 0x400ff054u (pg. 774)
	 */
	(*(uint32_t volatile *) 0x400ff054u) |= (1<<19);

	/**
	 * Inicializa o estado led G em 1 (ativo baixo)
	 * registrador GPIOB_PCOR mapeado em 0x400ff048u (pg. 776)
	 */
	(*(uint32_t volatile *) 0x400ff048u) = (1<<19);

    /* This for loop should be replaced. By default this loop allows a single stepping. */
    for (;;) {
    	/**
    	 * Reseta o estado led G em 0 (ativo baixo)
    	 * registrador GPIOB_PSOR mapeado em 0x400ff044u (pg. 774)
    	 */
    	(*(uint32_t volatile *) 0x400FF044u) = (1<<19);     	
    	espera_5us (100000);    	
    	/**
    	 * Seta o estado led G em 1 (ativo baixo)
    	 */
		(*(uint32_t volatile *) 0x400FF048u) = (1<<19); 
		espera_5us (100000);
    }

    /* Never leave main */
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
