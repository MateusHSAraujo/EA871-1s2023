/*!
 * @file util.h
 * @brief Prototipos, macros e tipos de dados de util.c
 * @date 05/05/2023
 * @author Mateus Henrique Silva Araujo
 */

#ifndef UTIL_H_
#define UTIL_H_

/*!
 * @brief Converte uma string com um valor binario em um inteiro de 32 bits
 * @param[in] str String com o valor binario. O primeiro elemento da string deve ser 'B' ou 'b'
 * @param[out] valor Apontador para a insercao do valor numerico de 32 bits obtido
 * @return Retorna 0 caso bem-sucedido. Retorna 2 se a conversao falhar
 */
uint8_t ConvertBitStringtoUl32 (char* str, uint32_t *valor);

/*!
 * @brief Converte uma string com um valor decimal em um inteiro de 32 bits
 * @param[in] str String com o valor decimal
 * @param[out] valor Apontador para a insercao do valor numerico de 32 bits obtido
 * @return Retorna 0 caso bem-sucedido. Retorna 2 se a conversao falhar
 */
uint8_t ConvertDecStringtoUl32 (char* str, uint32_t *valor);

/*!
 * @brief Converte uma string com um valor hexadecimal em um inteiro de 32 bits
 * @param[in] str String com o valor hexadecimal. O primeiro elemento da string deve ser 'H' ou 'h'
 * @param[out] valor Apontador para a insercao do valor numerico de 32 bits obtido
 * @return Retorna 0 caso bem-sucedido. Retorna 2 se a conversao falhar
 */
uint8_t ConvertHexStringtoUl32 (char* str, uint32_t *valor);

/*!
 * @brief Encontra o bit de paridade para o valor de entrada conforme o tipo de paridade informada no argumento de entrada
 * @param[in] x Valor para ser encontrado o bit de paridade
 * @param[out] str String que indica o  tipo de paridade desejada. Deve ser igual a 'I','i','P','p'!
 * @return Retorna o bit de paridade desejado
 */
uint8_t findParityBit(uint32_t x, char* str);

/*!
 * @brief Converte um valor inteiro de 32 bits em uma string com sua representacao binaria
 * @param[in] value String com o valor binario. O primeiro elemento da string deve ser 'B' ou 'b'
 * @param[out] bin String para a insercao da conversao em binario
 * @return Retorna o apontador da string enviada com o argumento
 */
char* ConvertUl32toBitString (uint32_t value, char *bin);

/*!
 * @brief Cria a mensagem de saida do programa
 * @param[in] bin String com o valor binario de 32 bits
 * @param[in] parity String indicando o tipo de paridade desejada. Deve ser igual a 'I','i','P' ou 'p' 
 * @param[in] parity_bit Bit de paridade encontrado para ser inserido no final da mensagem
 * @param[out] out_str String de saida a ser construida
 */
void createOutputString(char* bin, char* parity, uint8_t parity_bit ,char* out_str);

#endif /* UTIL_H_ */
