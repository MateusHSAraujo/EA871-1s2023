/*!
 * @file RTC.h
 * @brief Declaracao de prototipos, macros, tipos de dados relativos a RTC
 * @date 14/04/2023
 * @author Wu Shin-Ting 
 * @author Mateus Henrique Silva Araujo
 */

#ifndef RTC_H_
#define RTC_H_

/**
 * @brief Inicializa RTC com LPO como a fonte de sinal de relogio.
 */
void RTClpo_init();

/**
 * @brief Le o tempo corrente em segundos
 * @param[out] seconds lidos
 * @return segundos lidos
 */
uint32_t RTClpo_getTime (uint32_t *seconds);

/**
 * @brief Configure o tempo corrente com segundos dados
 * @param[in] seconds a serem configurados em RTC
 */
void RTClpo_setTime (uint32_t seconds);

#endif /* RTC_H_ */
