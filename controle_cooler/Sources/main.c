/**
 * @file main.c
 * @brief Fluxo de controle principal da aplicacao
 * @date 19/05/2023
 * @author Mateus Henrique Silva Araujo   
 */


#include "derivative.h" /* include peripheral declarations */
#include "stdint.h"
#include "GPIO_latch_lcd.h"
#include "SIM.h"
#include "TPM.h"
#include "ISR.h"
#include "util.h"
#include "ADC.h"
#include "string.h"

//Configuracoes especificadas para o controle_cooler:
struct ADC_MemMap Master_Adc_Config = {
		.SC1[0]=AIEN_OFF 
		 | DIFF_SINGLE 
		 | ADC_SC1_ADCH(31),
		.SC1[1]=AIEN_OFF 
		 | DIFF_SINGLE 
		 | ADC_SC1_ADCH(31),
		.CFG1=ADLPC_NORMAL
		 | ADC_CFG1_ADIV(ADIV_8)
		 | ADLSMP_SHORT
		 | ADC_CFG1_MODE(MODE_16)
		 | ADC_CFG1_ADICLK(ADICLK_BUS_2),   
		.CFG2=MUXSEL_ADCA					
		 | ADACKEN_DISABLED
		 | ADHSC_NORMAL
		 | ADC_CFG2_ADLSTS(ADLSTS_20),
		.CV1=0x1234u,                                   
		.CV2=0x5678u,
		.SC2=ADTRG_HW //Hardware trigger
		 | ACFE_DISABLED
		 | ACFGT_GREATER
		 | ACREN_ENABLED
		 | DMAEN_DISABLED
		 | ADC_SC2_REFSEL(REFSEL_EXT),                                    
		.SC3=CAL_OFF
		 | ADCO_SINGLE
		 | AVGE_ENABLED
		 | ADC_SC3_AVGS(AVGS_16),
};


int main(void)
{
	// Inicializacao do LCD:
	GPIO_ativaConLCD(); // Configura a comunicacao
	GPIO_initLCD();		// Inicializa o dispositivo com as intruções do fabricante
	
	//Configura a frequencia do sinal de barramento:
	SIM_setaOUTDIV4 (0b000); // bus clock = system clock = 20971520 Hz
	
	// Configura os modulos TPM1 e TPM2:
	SIM_setaFLLPLL(0); // Seleciona FLL
	SIM_setaTPMSRC(0b01); // Fornece MCGFLLCLK aos modulos TPM (Nao foi especificado a fonte desejada, portando MCGFLLCLK foi usada)
	TPM1TPM2_PTB0PTB18PTB19_config_basica(); // Habilita sinais de relogio e configura pinos de TPM1 e TPM2
	
	// Configura periodo de TPM2 e TPM1 para ser igual a 645 vezes o tempo de conversao (contagem inicialmente pausada):
	TPM_config_especifica (2,0xFFFF, 0b1111, 0, 0, 0, 0, 0, 0b110); // 0.2 s = ( 65535 * 64 / 20971520 Hz )
	TPM_config_especifica (1,0xFFFF, 0b1111, 0, 0, 0, 0, 0, 0b110);	// 0.2 s = ( 65535 * 64 / 20971520 Hz )
	
	
	//Inicializa canais usados (inicialmente desabilitados):
	TPM_CH_config_especifica (1, 0, 0b0000, 0); // TPM1_CH0 -> EPWM para o Cooler
	TPM_CH_config_especifica (2, 0, 0b0000, 0); // TPM2_CH0 -> EPWM para o LED R
	TPM_CH_config_especifica (2, 1, 0b0000, 0); // TPM2_CH1 -> EPWM para o LED G
	
	// Inicializa, configura, calibra e seleciona canal de ADC0:
	ADC_PTB1_config_basica(TPM2_TRG);
	ADC_Config_Alt (ADC0_BASE_PTR, &Master_Adc_Config);	// Configura
	ADC_Cal (ADC0_BASE_PTR);							// Calibra	
	ADC_Config_Alt (ADC0_BASE_PTR, &Master_Adc_Config); // Reconfigura
	ADC_selecionaCanal (0b01001); 						// selecionar o canal de PTB1 (ADC0_SE9)

	// Habilita interrupcao do ADC:
	ADC_habilitaNVICIRQ(1);	
	ADC_habilitaInterrupCOCO();
	
	// Habilita contagem de TPMx:
	TPM_habilitaContagem(2);
	TPM_habilitaContagem(1);
	
	uint16_t amostras[2]; // Variavel para armazenar resultado das conversoes ( amostras[0]->Potenciometro , amostras[1]->Sensor de Temperatura )
	float temperatura; 	 // Variavel para armazenar a temperatura (em Celsius) medida no sensor
	float tpm2CxV; 	 // Variavel auxiliar para configuracao do valor de comparacao em TPM2 Cx
	char outStr[14], amsStr[7]; // String de saida
	outStr[0]='\0';
	amsStr[0]='\0';
	
	ISR_escreveEstado(AMOSTRA_VOLT);
	
	// Laco principal:
	for(;;){
		if (ISR_LeEstado() == ATUALIZACAO) {
		// Obtem as amostras mais recentes;
		ISR_LeValoresAmostrados(amostras);
			
		// Gera sinal de EPWM para o cooler (TPM1_CH0)
		TPM_CH_config_especifica (1, 0, 0b1010, amostras[0]);
		
		// Converte amostra do sensor de temperatura para valor em gruas celsius
		temperatura = AN3031_Celsius(amostras[1]);
		
		// Gera sinais de EPWM para LEDS
		if (temperatura - 25>=0){ // Temperatura acima de 25 graus celsius
			tpm2CxV = ((uint16_t)(temperatura - 25)/25)*TPM2_MOD;
			TPM_CH_config_especifica (2, 0, 0b1010, tpm2CxV); // Ativa EPWM para o LED R
			TPM_CH_config_especifica (2, 1, 0b0000, 0); 	  // Desativa EPWM para o LED G
		} 
		else{ // Temperatura abaixo de 25 graus celsius
			tpm2CxV = (((uint16_t)((-1)*(temperatura - 25)))/25)*TPM2_MOD;			
			TPM_CH_config_especifica (2, 0, 0b0000, 0); 	  // Desativa EPWM para o LED R
			TPM_CH_config_especifica (2, 1, 0b1010, tpm2CxV); // Ativa EPWM para o LED G
		}
		
		// Produz e envia strings para o meio do visor
		strcpy(outStr,"DUTY: ");
		ftoa((amostras[0]*100.0)/TPM2_MOD, amsStr, 2);
		strcat(outStr,amsStr);
		strcat(outStr,"  ");
		GPIO_escreveStringLCD(0x03,(uint8_t *)outStr);
		
		strcpy(outStr,"TEMP: ");
		ftoa(temperatura, amsStr, 2);
		strcat(outStr,amsStr);
		strcat(outStr," C");
		GPIO_escreveStringLCD(0x42,(uint8_t *)outStr);
				
		
		// Chaveia para estado AMOSTRA_VOLT
		ISR_escreveEstado(AMOSTRA_VOLT);
		}
	}
	
	return 0;
}
