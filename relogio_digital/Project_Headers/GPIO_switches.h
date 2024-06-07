/*!
 * @file GPIO_switches.h
 * @brief Prototipos, macros e tipos de dados de GPIO_switches
 * @date 14/04/2023
 * @author Wu Shin-Ting 
 * @author Mateus Henrique Silva Araujo
 */

#ifndef GPIO_SWITCHES_H_
#define GPIO_SWITCHES_H_
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"

/**
 * @brief Inicializa botoeira NMI
 * @param[in] IRQC modo de interrupcao
 * @param[in] prioridade de atendimento
 */
void GPIO_initSwitchNMI (uint8_t IRQC, uint8_t prioridade);

/***
 * @brief Inicializa botoeiras NMI, IRQA5 e IRQA12
 * @param[in] NMI_IRQC modo de interrupção de NMI
 * @param[in] IRQA5_IRQC modo de interrupção de IRQA5
 * @param[in] IRQA12_IRQC modo de interrupção de IRQA12
 */
void GPIO_initSwitches(uint8_t NMI_IRQC, uint8_t IRQA5_IRQC, uint8_t IRQA12_IRQC, uint8_t prioridade);

/**
 * @brief Desativa IRQ30, sem limpar as pendencias
 */
void GPIO_desativaSwitchesNVICInterrupt ();

/**
 * @brief Reativa IRQ30, sem limpar as pendencias
 */
void GPIO_reativaSwitchesNVICInterrupt ();

/**
 * @bief Desativa as botoeiras de PORTA
 */
void GPIO_desativaSwitches();

/**
 * @brief Reativa as botoeiras de PORTA
 * @param[in] NMI_IRQC
 * @param[in] IRQA5_IRQC
 * @param[in] IRQA12_IRQC
 */
void GPIO_reativaSwitches(uint8_t NMI_IRQC, uint8_t IRQA5_IRQC, uint8_t IRQA12_IRQC);



#endif /* GPIO_SWITCHES_H_ */
