/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "stdint.h"
#include "GPIO_latch_lcd.h"
#include "ISR.h"
#include "SIM.h"
#include "TPM.h"
#include "derivative.h" /* include peripheral declarations */



int main(void){
	

	//Habilitar MCGFLLCLK 20971520 Hz
	SIM_setaOUTDIV4 (0b000);
	SIM_setaFLLPLL (0);              //seta FLL
		
	//Configura sinais de relogio para contadores TPM
	SIM_setaTPMSRC(1); //Fornece MCGFLLCLK aos modulos TPM
		
	//Habilita e aloca pinos para TPM0 e TPM1
	TPM0TPM1_PTE21_config_basica ();
		
	//Inicializa modulos TPM0 e TPM1
	TPM_config_especifica (0, 40960, 0b1111, 0, 0, 0, 0, 0, 0b111); // TPM0 com preescaler de 128 (2**7) e periodo 128*40960/20971520 = 0.25
	TPM_config_especifica (1, 1489, 0b1111, 0, 0, 0, 0, 0, 0b101); // TPM1 com preescaler de 32 (2**5) e frequencia 20971520/1489*32 = 440
		
	
	//Inicializa canais usados (desabilitados):
	TPM_CH_config_especifica (0, 0, 0b0000, 0); // TPM0_CH0 -> Realizar temporarizacoes necessarias
	TPM_CH_config_especifica (1, 1, 0b0000, 0); // TPM1_CH1 -> EPWM para o buzzer

	//Habilita as interrupcoes relevantes
	TPM_habilitaNVICIRQ(17,1); //Habilita interrupcoes de TPM0
	TPM_habilitaInterrupCH(0,0); //Habilita interrupcoes de TPM0_CH0  (OC para contagem)
	
	TPM_CH_config_especifica (0, 0, 0b0111, TPM0_CNT); //MS_ELS = 0111 -> OC set output on match
	
	initSongs();
	for(;;){}
	return 0;
	}
