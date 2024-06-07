/**
 * @file main.h
 * @brief Fluxo principal de controle
 * @date 02/06/2023
 * @author Mateus Henrique Silva Araujo
 */

#ifndef MAIN_H_
#define MAIN_H_

/**
 * @brief Fluxo de controle principal do programa. Nessa funcao, sao tratados os seguintes estados:
 * 	- ATUALIZACAO: Neste estado, as amostras obtidas sao convertidas saoutilizadas para controle dos pulsos de EPWM e alteracao das strings de saida. Quando o sistema termina a 
 * 	atualizacao, ele chaveia para o estado AMOSTRA_VOLT
 */
int main(void);


#endif /* MAIN_H_ */
