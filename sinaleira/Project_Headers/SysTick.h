/**
 * @file SysTick.h
 * @brief Prototipos, macros e tipos de dados de SysTick
 * @date Jan 11, 2023
 * @author Wu Shin-Ting
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

/**
 * @brief Inicializa e habilita o temporizador com periodo em contagem maxima, 
 * sem habilitar a interrupcao
 * 
 * @param[in] periodo
 */
void SysTick_init (uint32_t periodo);

void SysTick_ativaInterrupt ();

void SysTick_desativaInterrupt ();

void SysTick_resetaCVR();

#endif /* SYSTICK_H_ */
