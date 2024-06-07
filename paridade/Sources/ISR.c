/*
 * @file ISR.c
 * @brief Interrupt handling related functions
 * @date Jan 22, 2023
 * @author Wu Shin-Ting
 */

#include "derivative.h"
#include "buffer_circular.h"
#include "ISR.h"

#define TAM_MAX 100	// Tamanho de buffer aumentado para comportar a nova versao da saida
static BufferCirc_type bufferE;	//buffer de entrada
static BufferCirc_type bufferS;	//buffer de saida 
static tipo_estado estado;

void ISR_inicializaBC () {
	/*!
	 * Inicializa um buffer circular de entrada e um de saida
	 */
	BC_init(&bufferE, TAM_MAX);
	BC_init(&bufferS, TAM_MAX);

}

void ISR_ExtraiString (char *string) {
	//Entrada de uma nova string
	uint8_t i=0;
	BC_pop (&bufferE, &string[i]);
	while (string[i] != '\0') {
		BC_pop (&bufferE, &string[++i]);				
	}
}

void ISR_EnviaString (char *string) {
	uint8_t i;
	
	while (BC_push( &bufferS, string[0])==-1);
	UART0_C2 |= UART0_C2_TIE_MASK;
	i=1;
	
	while (string[i] != '\0') {
		while (BC_push( &bufferS, string[i])==-1);
		i++;
	}
}

tipo_estado ISR_LeEstado () {
	return estado;
}

void ISR_escreveEstado (tipo_estado novo) {
	estado = novo;
	return;
}

uint8_t ISR_BufferSaidaVazio () {
	return BC_isEmpty (&bufferS);
}

void UART0_IRQHandler()
{
	char item;
	if (UART0_S1 & UART0_S1_RDRF_MASK) {
		item= UART0_D;
		if (estado != EXPRESSAO) return;
		UART0_D = item;
		if (item == '\r') {
			BC_push (&bufferE, '\0');
			while (!(UART0_S1 & UART_S1_TDRE_MASK));
			UART0_D = '\n';
			ISR_escreveEstado(TOKENS);
		} else {
			BC_push (&bufferE, item);
		}
	} else if (UART0_S1 & UART0_S1_TDRE_MASK) {
		if (BC_isEmpty(&bufferS)) {
			UART0_C2 &= ~UART0_C2_TIE_MASK;
		}
		else {
			BC_pop (&bufferS, &item);
			UART0_D = item;
		}
	}
}

