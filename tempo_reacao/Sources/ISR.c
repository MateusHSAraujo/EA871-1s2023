/**
 * @file ISR.c
 * @brief Funcoes para tratamento de rotinas de interrupcao
 * @date 19/05/2023
 * @author Mateus Henrique Silva Araujo   
 */

#include "derivative.h"
#include "ISR.h"
#include "TPM.h"

static tipo_estado estado;
static uint16_t  ct1, ct2, tempo_aleatorio, contador;
static float tempo_reacao;


tipo_estado ISR_LeEstado () {
	return estado;
}

void ISR_escreveEstado (tipo_estado novo) {
	estado = novo;
	return;
}

void ISR_geraNumeroAleatorio(){ // Gerar um numero aleatorio para gerar intervalos de tempo entre 1 e 3 segundos
	tempo_aleatorio = TPM1_CNT;
	while(tempo_aleatorio <= 500){
		tempo_aleatorio = TPM1_CNT;
	}
	return;
}

float ISR_getTempoReacao(){
	return tempo_reacao;
}

void FTM1_IRQHandler () {
	
	if ((TPM1_STATUS & TPM_STATUS_CH0F_MASK)){ //Pressionamento de IRQA12
		
		//Limpa flag e desabilita o canal
		TPM1_C0SC |= TPM_CnSC_CHF_MASK; 
		if (estado!=INICIO) return;
		TPM_CH_config_especifica (1, 0, 0b0000, 0);
		
		//Chaveia para o estado PREAPARA_AUDITIVO
		estado=PREPARA_AUDITIVO;
		
	}
	else if ((TPM1_STATUS & TPM_STATUS_TOF_MASK)){ //Espera aleatoria acabou, gerando estimulo sonoro
		
		
		TPM1_SC |= TPM_SC_TOF_MASK; //Limpa flag
		if (estado!=ESPERA_ESTIMULO_AUDITIVO) return;
		
		tempo_aleatorio--; //Decrementa o contador para o tempo aleatorio
		
		if (tempo_aleatorio==0){
			
			//Desabilitar evento de interrupção TOF de TPM1;
			TPM1_SC &= ~TPM_SC_TOIE_MASK;
			
			//Habilitar o buzzer no modo EPWM atribuindo um valor diferente de 0 e TPM1_MOD+1 no canal e mudando MS_ELS 
			TPM_CH_config_especifica (1, 1, 0b1010, 1400); //MS_ELS = 1010 -> EPWM high true
			
			//Reseta tempo_reacao e contador;
			tempo_reacao=0;
			contador =0;
			
			//Hablitica TPM0_CH4 para a contagem de periodos transcorridos
			ct1 = TPM0_CNT;
			TPM_CH_config_especifica (0, 4, 0b0111, ct1); //MS_ELS = 0111 -> OC set output on match
			
			//Habilitar a botoeira NMI no modo IC com interrupção
			TPM_CH_config_especifica (0, 1, 0b0010, 0); //MS_ELS = 0010 -> IC para borda de descida
			
			//Chaveia para estado ESPERA_REACAO_AUDITIVA
			estado = ESPERA_REACAO_AUDITIVA;
		}
	}
	
	return;
}

void FTM0_IRQHandler () {
	
		if (TPM0_STATUS & TPM_STATUS_CH4F_MASK){ // Ocorreu overflow no contador antes do pressionamento de NMI. Incrementa o contador 
				TPM0_C4SC |= TPM_CnSC_CHF_MASK; //Limpa flag
				if (estado==ESPERA_REACAO_AUDITIVA){
					contador++; //Incrementa o número de periodos
					
				}
				else if(estado==LEITURA){
					contador++; // Incrementa contador
								
					if (contador==12){ //Tempo de leitura completo
									
						TPM_CH_config_especifica (0, 4, 0b0000, 0); //MS_ELS = 0000 -> TPM0_CH4 (AUX) desabilitado
						estado = PREPARA_INICIO; //Chaveia para PREPARA_INICIO
					}
				}
		}
		else if (TPM0_STATUS & TPM_STATUS_CH1F_MASK){ //Pressionamento da botoeira NMI detectado
			
			TPM0_C1SC |= TPM_CnSC_CHF_MASK; //Limpa flag
			if (estado!=ESPERA_REACAO_AUDITIVA) return;
			
			ct2 = TPM0_C1V; // Armazena segundo valor de contagem
			
			//Desabilita 0 canal de NMI, o canal auxiliar e o canal do buzzer
			TPM_CH_config_especifica (0, 1, 0b0000, 0); //MS_ELS = 0000 -> TPM0_CH1 (NMI) desabilitado 
			TPM_CH_config_especifica (0, 4, 0b0000, 0); //MS_ELS = 0000 -> TPM0_CH4 (AUX) desabilitado
			TPM_CH_config_especifica (1, 1, 0b0000, 0); //MS_ELS = 0000 -> TPM1_CH1 (BUZZER) desabilitado 
			
			//Calcula o tempo de reacao
			uint16_t TPM0MOD =  TPM0_MOD;
			if (ct1>=ct2){
				tempo_reacao = ((contador*1.0) + (((TPM0MOD - ct1 + ct2 )*1.0)/(TPM0MOD*1.0)))*0.25;
			} else {
				tempo_reacao = ((contador*1.0) + (((ct2 - ct1 )*1.0)/(TPM0MOD*1.0)))*0.25;
			}
			
			//Reseta o contador para ser usado em LEITURA
			contador=0;
			
			//Chaveia para o estado RESULTADO
			estado = RESULTADO;
		}
		
	
	
	return;
}


