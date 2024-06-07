/*!
 * @file buffer_circular.h
 * @brief Arquivo-cabecalho de buffer_circular.c
 * @author Wu Shin-Ting
 * @author Mateus Henrique Silva Araujo
 * @date 05/05/2023
 */
#include "stdint.h"

#ifndef BUFFER_CIRCULAR_H_
#define BUFFER_CIRCULAR_H_

//=====================================
// Estrutura de dados: buffer circular
//=====================================
/*!
 * @struct BufferCirc_tag
 */
typedef struct BufferCirc_tag
{
	char * dados;  	 // buffer de dados
	unsigned int tamanho;	// quantidade total de elementos
	unsigned int leitura;       // indice de leitura (tail)  
	unsigned int escrita;       // indice de escrita (head)
} BufferCirc_type;

/*!
 * @brief Inicializa o buffer circular
 * @param[in] buffer ponteiro para o buffer inicializado 
 * @param[in] tamanho maximo do buffer circular
 */
void BC_init(BufferCirc_type *buffer, unsigned int tamanho);

/*!
 * @brief Reinicializa o buffer circular
 * @param[in] buffer ponteiro para o buffer a ser reinicializado
 */
void BC_reset(BufferCirc_type *buffer);

/*!
 * @brief Desaloca da memoria o buffer circular
 * @param[in] buffer ponteiro para o buffer a ser desalocado 
 */
void BC_free(BufferCirc_type *buffer);

/*!
 * @brief Insere um item no buffer circular
 * @param[in] buffer ponteiro para o buffer 
 * @param[in] item a ser inserido
 */
int BC_push (BufferCirc_type *buffer, char item);

/*!
 * @brief Retira um item do buffer circular
 * @param[in] buffer ponteiro para o buffer 
 * @param[out] item retirado
 */
int BC_pop (BufferCirc_type *buffer, char *item);

/*!
 * @brief Conta a quantidade de elementos no buffer
 * @param[in] buffer ponteiro para o buffer 
 * @return quantidade de elementos no buffer
 */
unsigned int BC_elements (BufferCirc_type *buffer);

/*!
 * @brief Verifica se o buffer esta cheio
 * @param[in] buffer ponteiro para o buffer 
 * @return 1 se o buffer esta cheio, 0 caso contrario
 */
uint8_t BC_isFull (BufferCirc_type *buffer);

/*!
 * @brief Verifica se o buffer esta vazio
 * @param[in] buffer ponteiro para o buffer 
 * @return 1 se o buffer esta vazio, 0 caso contrario
 */
uint8_t BC_isEmpty (BufferCirc_type *buffer);


#endif /* BUFFER_CIRCULAR_H_ */
