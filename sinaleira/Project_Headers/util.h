/**
 * @file util.h
 * @brief Prototipos, macros e tipos de dados de util.c
 * @date Jan 11, 2023
 * @author Wu Shin-Ting
 */

#ifndef UTIL_H_
#define UTIL_H_

#define GPIO_PIN(x)  ((1)<<(x))

typedef enum boolean_tag {
	OFF,     /**< falso/apaga/desativa/liga */
	ON      /**< verdadeiro/acende/ativa/fecha */
} tipo_booleano;

/**
 * @brief espera em multiplos de 5us
 * @param[in] multiplo de 5us
 */
void espera_5us (uint32_t multiplos);

/**
 * @brief espera em decrementos de valores ate 0
 * @param[in] valor de decremento
 */
void espera (uint32_t valor);


#endif /* UTIL_H_ */
