/*
 * @file OSC.c
 * @brief Funcoes relacionadas com o modulo de Oscilador OSC
 * @date 14/04/2023
 * @author Wu Shin-Ting 
 * @author Mateus Henrique Silva Araujo
 */

#include "derivative.h"

void OSC_LPO1kHz () {
	SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0b11); // LPO1kHz is RTC and LPTMR OSC CLOCK
}


