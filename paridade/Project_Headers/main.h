/*!
 * @file main.h
 * @brief Implementacao do projeto Paridade 
 * @date 05/05/2023
 * @author Mateus Henrique Silva Araujo
 */

#ifndef MAIN_H_
#define MAIN_H_

/*!
 * @brief Extrai 2 tokens a partir de uma string
 * @param[in] str String de entrada
 * @param[out] tokens Apontador de um array para ser inserido os apontadores dos tokens gerados
 * @return Retorna 1 se a quantidade de tokens for diferente de 2. Retorna 3 se o token de paridade for invalido. Retorna 0 se a extracao for bem-sucedida
 */
uint8_t ExtraiString2Tokens(char *str, char **tokens);

/*!
 * @brief Obtem o valor numerico de 32 bits a partir do token respectivo
 * @param[in] val_str String de entrada para extracao do valor numerico
 * @param[out] val Apontador para a insercao do valor numerico obtido
 * @return Retorna 2 se o token for invalido. Retorna 0 caso bem-sucedido
 */
uint8_t getValue(char *val_str,uint32_t *val);

/*!
 * @brief Fluxo de controle central do sistema . O funcionamento pode ser resumido da seguinte forma:
 * - Inicia no estado MENSAGEM, solicitando a entrada do usuario. Chaveia para estado EXPRESSAO quando o buffer de saida for esvaziado
 * - No estado EXPRESSAO, espera ate que o usuario envie uma linha completa, terminada com '\r'. Chaveia para TOKENS quando esse caractere e detecado
 * - No estado TOKENS, extrai os tokens da string recebida e armazena em um array. Alem disso, tambem extrai o valor numerico enviado para comparacao. Chaveia para ERRO caso detectado uma entrada invalida. Chaveia para COMPUTO caso contrario
 * - No estado COMPUTO, calcula o bit de paridade e gera a string com a representacao binaria de saida. Chavei para RESULTADO quando terminada as operacoes
 * - No estado RESLTADO, constroi e envia a string de saida. Chavei para o estado MENSAGEM quando o buffer de saida for esvaziado
 * - No estado ERRO, imprime a mensagem indicando o erro cometido. Chavei para o estado MENSAGEM quando o buffer de saida for esvaziado 
 */
int main(void);

#endif /* MAIN_H_ */
