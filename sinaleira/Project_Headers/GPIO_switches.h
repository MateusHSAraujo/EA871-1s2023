/*
 * @file GPIO_switchhes.h
 * @brief Prototipos, macros e tipos de dados de GPIO_switches
 * @date Jan 13, 2023
 * @author Wu Shin-Ting
 */

#ifndef GPIO_SWITCHES_H_
#define GPIO_SWITCHES_H_
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"

void GPIO_initSwitchNMI (uint8_t IRQC, uint8_t prioridade);

#endif /* GPIO_SWITCHES_H_ */
