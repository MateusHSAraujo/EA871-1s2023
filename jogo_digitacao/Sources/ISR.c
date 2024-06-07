#include "ISR.h"
#include "stdint.h"
#include "TPM.h"
#include "pilha_LCD.h"
#include "derivative.h"

static tipo_estado estado;
static uint8_t tempoLvls[] = {20,16,12,8,6,5,4,3};
//static uint8_t tempoLvls[] = {20,20,20,20,20,20,20,20}; //DEBBUG
static uint8_t  lvl=0,contador,segundosLeitura;
static BufferPilha_type pilha;


void ISR_inicializaBP(){ //Incializa pilha do LCD
	BP_init(&pilha);
}

void ISR_geraPilhaInicial(){ // Gera a pilha com letras aleatorias do inicio do jogo
	BP_geraPilhaInicial(&pilha);
}

void ISR_printaPilhaInicial(){ // Printa a pilha do inicio do level e ativa o buzzer
	BP_printaPilhaInicial(&pilha);
}

uint8_t ISR_pilhaCheia(){
	return BP_isFull (&pilha);
}

uint8_t ISR_pilhaVazia(){
	return BP_isEmpty (&pilha);
}

void ISR_resetaPilha(){
	BP_reset(&pilha);
}

uint8_t ISR_apagaLetra(){
	return BP_del (&pilha, 1, 1);
}

uint8_t ISR_adicionaLetra(){
	char nv_letra = BP_geraLetra();
	return BP_add(&pilha,nv_letra,1,1);
}

tipo_estado ISR_LeEstado () {
	return estado;
}

void ISR_escreveEstado (tipo_estado novo) {
	estado = novo;
	return;
}

uint8_t ISR_getSegundosLeitura(){
	return segundosLeitura;
}

void ISR_setSegundosLeitura(uint8_t seg){
	segundosLeitura = seg;
	contador = 0;
}

uint8_t ISR_getLVL(){
	return lvl;
}
void ISR_resetLVL(){
	lvl = 0;
}

void ISR_aumentaLVL(){
	lvl++;
}

void ISR_ajustaContadorParaLVL(){
	contador = tempoLvls[lvl-1];
}

void FTM0_IRQHandler () {
	
		if (TPM0_STATUS & TPM_STATUS_CH0F_MASK){ // Canal de temporarizacao 
				TPM0_C0SC |= TPM_CnSC_CHF_MASK; //Limpa flag
				
				if (estado==CONTAGEM_REGRESSIVA || estado==LEITURA){
					contador++;
					if (contador%4==0) segundosLeitura--; //Se o contador formultiplo de 4, passaram-se 1 segundo
					if (segundosLeitura==0){//Se o tempo setado em segundos para contagem zerar, desabilita o canal de temporarizacao
						TPM_CH_config_especifica (0, 0, 0b0000, 0);
						
						if(estado == LEITURA){
							if (lvl == 0) estado = ESPERA_INICIO;
							else estado = PREPARA_LCD;
						}
					}
				}
				else if( estado==EXECUTA_LVL){
					contador--;
					if (contador == 0){
						ISR_ajustaContadorParaLVL();
						estado = ADICIONA_LETRA;
					}
				}
				
		}
		
	return;
}

void UART0_IRQHandler(){
	char item;
	if (UART0_S1 & UART0_S1_RDRF_MASK) {
		
		item = UART0_D;
		
		if (estado != ESPERA_INICIO && estado != EXECUTA_LVL) return;
		
		UART0_D = item;
		
		if (item == '\r' && estado==ESPERA_INICIO) {
			while (!(UART0_S1 & UART_S1_TDRE_MASK));
			UART0_D = '\n';
			estado = PREPARA_LCD;
		}
		
		if (estado == EXECUTA_LVL){
			char letra_atual;
			if (BP_get(&pilha,&letra_atual)==0 && letra_atual == item){
				estado = APAGA_LETRA;
			}
		}
	}
}

