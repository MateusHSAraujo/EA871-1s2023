/*!
 * @file main.h
 * @author Mateus Henrique Silva Araujo
 * @date 19/05/2023
 * @brief Fluxo de controle principal do projeto tempo_reacao
 */

#ifndef MAIN_H_
#define MAIN_H_

#endif /* MAIN_H_ */

/**
 * @brief Fluxo de controle principal do programa. Nessa funcao, sao tratados os seguintes estados:
 * 	- PREPARA_INICIO: Neste estado, e renderizado a mensagem de entrada para o usuario, um novo numero aleatorio e produzido e o canal para a deteccao do pressionamento de IRQA12 e habilitado.
 * 	Por fim, o sistema chaveia para o estado INICIO;
 * 	- PREPARA_AUDITIVO: Neste estado, a string "Teste Auditivo" e renderizada no LCD e as interrupcoes por overflow de TPM1 sao habilitadas para a passagem do tempo aleatorio. Por fim, o sistema
 * 	e chaveado para o estado ESPERA_ESTIMULO_AUDITIVO;
 * 	- RESULTADO:  Neste estado, a string de saida e produzida e renderizada no LCD. O canal TPM0_CH4 e configurado para gerar uma espera de 3 segundos no estado LEITURA (ao qual o sistema e
 * 	chaveado no fim do processamento deste estado).
 */
int main(void);
