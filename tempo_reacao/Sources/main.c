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
#include "string.h"


int main(void)
{	
	//Inicializa conexao com LCD e roda as rotinas de inicializacao recomendadas
	GPIO_ativaConLCD ();
	GPIO_initLCD ();
	
	//Configura sinais de relogio para contadores TPM
	SIM_setaFLLPLL(0); //Seleciona FLL
	SIM_setaTPMSRC(1); //Fornece MCGFLLCLK aos modulos TPM
	
	//Habilita e aloca pinos para TPM0 e TPM1
	TPM0TPM1_PTA4PTA12PTE21_config_basica ();
	
	//Inicializa modulos TPM0 e TPM1
	TPM_config_especifica (0, 40960, 0b1111, 0, 0, 0, 0, 0, 0b111); // TPM0 com preescaler de 128 (2**7) e periodo 128*40960/20971520 = 0.25
	TPM_config_especifica (1, 1489, 0b1111, 0, 0, 0, 0, 0, 0b101); // TPM1 com preescaler de 32 (2**5) e frequencia 20971520/1489*32 = 440
	
	//Inicializa canais usados (desabilitados):
	TPM_CH_config_especifica (0, 1, 0b0000, 0); // TPM0_CH1 -> IC para a botoeria NMI
	TPM_CH_config_especifica (1, 0, 0b0000, 0); // TPM1_CH0 -> IC para a botoeria IRAQ12
	TPM_CH_config_especifica (1, 1, 0b0000, 0); // TPM1_CH1 -> EPWM para o buzzer
	
	//Inicializa canal extra para auxilio na contagem de tempo
	TPM_CH_config_especifica (0, 4, 0b0000, 0); // TPM0_CH4 -> OC para auxiliar na contagem
	
	//Habilita as interrupcoes relevantes
	TPM_habilitaNVICIRQ(17,1); //Habilita interrupcoes de TPM0
	TPM_habilitaNVICIRQ(18,1); //Habilita interrupcoes de TPM1
	TPM_habilitaInterrupCH(0,1); //Habilita interrupcoes de TPM0_CH1 (IC de NMI)
	TPM_habilitaInterrupCH(1,0); //Habilita interrupcoes de TPM1_CH0 (IC de IRQA12)
	TPM_habilitaInterrupCH(0,4); //Habilita interrupcoes de TPM0_CH4  (OC para contagem)
	
	// Cria bitmaps 
	char cComCedilha[8]={0x0e,0x11,0x10,0x10,0x11,0x0e,0x04,0x18};
	char aComTil[8]={0x05,0x0a,0x04,0x0A,0x11,0x1F,0x11,0x00};
		
	GPIO_escreveBitmapLCD (0x01, (uint8_t *)cComCedilha);
	GPIO_escreveBitmapLCD (0x02, (uint8_t *)aComTil);
	
	char tempoStr[6];
	char reacao[] = {'R','e','a',0x01,0x02,'o','\0'};
	char outStr[25];
	outStr[0]='\0';
	tempoStr[0]='\0';
	
	ISR_escreveEstado(PREPARA_INICIO);
	for(;;) {	   
		switch(ISR_LeEstado()){
			case PREPARA_INICIO:
				//Renderiza mensagem de entrada no LCD
				GPIO_escreveStringLCD(0x40,(uint8_t *)"        ");
				GPIO_escreveStringLCD(0x00,(uint8_t *)"                         " );
				GPIO_escreveStringLCD(0x00,(uint8_t *)"Aperte IRQA12" );
					
				//Gera novo numero aleatorio
				ISR_geraNumeroAleatorio();
					
				//Habilita TPM1_CH0 para detectar pressionamento de IRAQ12
				TPM_CH_config_especifica (1, 0, 0b0010, 0); //MS_ELS = 0010 -> IC para borda de descida
				
				//Chaveia para o estado INICIO
				ISR_escreveEstado(INICIO);
				break;
				
			case PREPARA_AUDITIVO:
				//Renderiza no LCD "Teste Auditivo"
				GPIO_escreveStringLCD(0x00,(uint8_t *)"                         " );
				GPIO_escreveStringLCD(0x00,(uint8_t *)"Teste Auditivo" );
					
				//Dispara a contagem do tempo aleatorio
				TPM_habilitaInterrupTOF(1);
				
				//Chaveia para ESPERA_ESTIMULO_AUDITIVO
				ISR_escreveEstado(ESPERA_ESTIMULO_AUDITIVO);
				
				break;
					
			case RESULTADO:
				//Converte o tempo de reacao para uma string
				ftoa(ISR_getTempoReacao(),tempoStr,1);
					
				//Renderiza a string no LCD
				GPIO_escreveStringLCD(0x00,(uint8_t *)"                         " );
				strcat(outStr,reacao);
				strcat(outStr," em ");
				strcat(outStr,tempoStr);
				GPIO_escreveStringLCD(0x00,(uint8_t *)outStr);
				GPIO_escreveStringLCD(0x40,(uint8_t *)"segundos");
								
				outStr[0]='\0';
					
				//Configura o canal TPM0_CH4 para gera um intervalo de 3 segundos
				TPM_CH_config_especifica (0, 4, 0b0111, TPM0_CNT); // //MS_ELS = 0111 -> OC set output on match
					
				//Chaveia para estado LEITURA
				ISR_escreveEstado(LEITURA);
				break;
					
			default:
				//Estados INICIO, ESPERA_ESTIMULO_AUDITIVO e LEITURA sao tratados inteiramente em ISR.c
				break;
		}
	}
	
	return 0;
}

