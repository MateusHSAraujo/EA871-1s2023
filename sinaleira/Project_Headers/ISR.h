/*
 * ISR.h
 * @brief Prototipos, macros e tipos de dados de ISR
 * @date Jan 11, 2023
 * @author Wu Shin-Ting
 */

#ifndef ISR_H_
#define ISR_H_

//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"

//Definicao como tipo_estado uma lista de identificadores de estados do projeto
typedef enum estado_tag {
	APAGADO,
	VERDE,
	AMARELO,
	VERMELHO,
	VERMELHO_AMARELO,
	VERDE_APAGADO,
	AMARELO_VERDE,
	APAGADO_VERMELHO
} tipo_estado;

/**
 * @brief reseta o contador postscaler de SysTick.
 */
void ISR_resetaCounter();

/**
 * @brief seta o valor máximo do contador postscaler de SysTick.
 * @param[in] valor maximo de postscaler para a contagem.
 */
void ISR_setaMaxPostScaler(uint32_t valor);

/**
 * @brief atualiza o estado
 * @param[in] valor
 */
void ISR_escreveEstado(tipo_estado valor);

/**
 * @brief le o estado
 * @return estado
 */
tipo_estado ISR_leEstado();

#endif /* ISR_H_ */
