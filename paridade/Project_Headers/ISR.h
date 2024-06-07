/*!
 * @file ISR.h
 * @brief Prototipos, macros e tipos de dados referentes a ISR
 * @date 05/05/2023
 * @author Mateus Henrique Silva Araujo
 */

#ifndef ISR_H_
#define ISR_H_

typedef enum estado_tag {
	MENSAGEM,
	TOKENS,
	EXPRESSAO,
	COMPUTO,
	RESULTADO,
	ERRO,
} tipo_estado;

/*!
 * @brief Inicializa buffers circulares para o aplicativo
 */
void ISR_inicializaBC ();

/*!
 * @brief Extrai do buffer de entrada uma linha de caracteres.
 * @param[out] string contendo a linha terminada com '\0'.
 */
void ISR_ExtraiString (char *string);

/*!
 * @brief Envia uma string para Terminal. A forma de onda eh espelhada no pino 2 do header H5
 * @param[in] string a ser enviado para Terminal via buffer circular
 */
void ISR_EnviaString (char *string);

/*!
 * @brief Le o estado do aplicativo
 */
tipo_estado ISR_LeEstado ();

/*!
 * @brief Atualiza o estado do aplicativo
 * @param[in] novo estado
 */
void ISR_escreveEstado (tipo_estado novo);

/*!
 * @brief Verificar se o buffer de saida esta vazio
 * @return estado vazio do buffer (0 se sim, 1 se nao)
 */
uint8_t ISR_BufferSaidaVazio ();

/*!
 * @brief Rotina de tratamento do modulo UART0. Implementa o seguinte comportamento:
 * - Quando no estado EXPRESSAO, ecoa os caracteres do teclado e armazena-os no buffer de entrada, ate que '\r' seja lido. Nesse momento, chaveia para estado TOKENS
 * - Quando em qualquer outro estado, retorna sem fazer nada 
 */
void UART0_IRQHandler();

#endif /* ISR_H_ */
