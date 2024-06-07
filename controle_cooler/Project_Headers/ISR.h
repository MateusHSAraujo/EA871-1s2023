/**
 *@file ISR.h
 *@brief Header para rotinas e variaveis associadas a ISR
 *@author Mateus Henrique Silva Araujo
 *@date 02/06/2023
 */

#ifndef IISR_H_
#define IISR_H_



#endif /* IISR_H_ */

/** 
 * @brief Enumeracao para os estados do sistema
 */
typedef enum estado_tag{
	AMOSTRA_VOLT, //!< Estado para amostragem do sinal do potenciometro
	AMOSTRA_TEMP, //!< Estado para amostragem do sinal de temperatura
	ATUALIZACAO, //!< Estado para atualizacao de interface com o usuario 
} tipo_estado;

/**
 * @brief Le o estado em que se encontra o sistema
 */
tipo_estado ISR_LeEstado ();

/**
 * @brief Altera o estado no qual o progarama se encontra
 * @param[in] novo estado para o qual o sistema sera alterado
 */
void ISR_escreveEstado (tipo_estado novo);

/**
 * @brief Le os valores mais recentemente amostrados
 * @param[out] v vetor para insercao dos valores amostrados. O primeiro elemento recebera a conversao do poteciometro e o segundo, do sensor de temperatura. 
 */
void ISR_LeValoresAmostrados (uint16_t *v);

/**
 * @brief Rotina de interrucao responsavel pelo tratamento das interrupcoes de ADC0. Nela, o seguinte comportamento e implementado:
 * 	- Durante o estado AMOSTRA_VOLT, o valor amostrado por ADC0 da voltagem do potenciometro e inserido no primeiro elemento da variavel global "valor". Alem disso, o sistema configura para que seja
 * 	executada em seguida uma amostragem do sensor de temperatura AN3031, disparada por software. Ao final, o sistema chaveia para o estado AMOSTRA_TEMP
 * 	- Durante o estado AMOSTRA_TEMP, o valor amostrado por ADC0 do sensor de temperatura e inserido no segundo elemento da variavel global "valor". Alem disso, o sistema faz as configuracoes necessaria 
 * 	 para que a proxima conversao de ADC0 seja disparada por software e amostre o sinal do potenciometro. Ao final, o sistema chaveia para o estado ATUALIZACAO
 */
void ADC0_IRQHandler(void);
