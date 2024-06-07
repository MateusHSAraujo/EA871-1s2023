/*!
 *@file ISR.h
 *@brief Header para rotinas e variaveis associadas a ISR
 *@author Mateus Henrique Silva Araujo
 *@date 19/05/20023
 */

#ifndef ISR_H_
#define ISR_H_

#endif /* ISR_H_ */

/** 
 * @brief Enumeracao para os estados do sistema
 */
typedef enum estado_tag{
	PREPARA_INICIO, //!< Estado para preaparacao da entrada
	INICIO, //!< Estado de espera para o pressionamento de IRQA12
	PREPARA_AUDITIVO, //!< Estado de preparacao para espera aleatoria 
	ESPERA_ESTIMULO_AUDITIVO, //!< Estado de espera de tempo aleatorio e meissao do sinal sonoro
	ESPERA_REACAO_AUDITIVA, //!< Estado de espera e captura da reacao auditiva
	RESULTADO, //!< Estado para a producao do resultado do teste
	LEITURA, //!< Estado de espera para a leitura pelo usuario do resultado
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
 * @brief Gera um novo numero aleatorio para controle do tempo de reacao aleatorio
 */
void ISR_geraNumeroAleatorio();

/**
 * @brief Retorna o tempo de reacao capturado no ultimo ciclo de processamento
 * @return tempo de reacao calculado 
 */
float ISR_getTempoReacao();

/**
 * @brief Rotina de interrucao responsavel pelo tratamento das interrupcoes de TPM1. Nela, o seguinte comportamento e implementado:
 * 	- Durante o estado INICIO, ao pressionar-se a botoeira IRQA12, a rotina desabilita o canal associado a ela e chaveia o sistema para o estado PREPARA_AUDITIVO
 * 	- Durante o estado ESPERA_ESTIMULO_AUDITIVO, a rotina espera a passagem de um periodo aleatorio de tempo, baseado no valor da variavel tempo_aleatorio. Quando esse periodo se esgotar,
 * 	o sinal para acionamento do buzzer e gerado, os canais para contagem de tempo e deteccao de pressionamento de NMI sao ativados. Alem disso, as interrupcoes por overlow de TPM1 sao desabilitadas.
 * 	Por fim, o sistema e chaveado para o estado ESPERA_REACAO_AUDITIVA 
 */
void FTM1_IRQHandler ();

/**
 * @brief Rotina de interrucao responsavel pelo tratamento das interrupcoes de TPM0. Nela, o seguinte comportamento e implementado:
 * 	- Durante o estado ESPERA_REACAO_AUDITIVA, os seguintes eventos podem ocorrer:
 * 	 	-# Se essa rotina for chamada em virtude de um macth no canal 4, aumenta-se o numero de periodos contados, avaliados pela variavel contador;
 * 		-# Se essa rotina for chamada em virtude o input capture do canal 1 (associado ao pressionamento de NMI), significa que o usuario detectou o estimulo sonoro. Portanto, a rotina desabilita os
 * 		canais utilizados por ela, calcula o tempo de reacao correspondente, reseta a variavel contador e chaveia para o estado RESULTADO.
 * 	- Durante o estado LEITURA, a rotina espera a passsagem de 3 segundos totais para a leitura do resultado no LCD pelo usuario. Quando isso ocorre, os canais usados para contagem de tempo sao
 * 	desabilitados e o sistema e resetado para o sistema PREPARA_INICIO
 * 	
 */
void FTM0_IRQHandler ();
