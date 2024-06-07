/*
 * pilha_LCD.c
 *
 *  Created on: Jun 16, 2023
 *      Author: ea871
 */

#include "stdint.h"
#include "pilha_LCD.h"
#include "GPIO_latch_lcd.h"
#include "TPM.h"
#include "derivative.h"

void BP_init(BufferPilha_type *buffer) {
	buffer->tail = -1;
	return;
}

uint8_t LCD_printOffset(uint8_t idx){
	// O inicio da pilha e mapeado no ultimo endereco do LCD 
	// O topo da pilha e mapeado em direcao ao primeiro endereco do LCD 
	return (idx<16)? 0x4f-idx : 31-idx;
}

uint16_t calcMod(uint8_t idx){
	uint16_t cnt[] = { 2505, 2231, 1988, 1876, 1672, 1489, 1327, 1252};
	uint16_t mod = cnt[idx/4];
	return mod;
}

char BP_geraLetra(){
	char letra = (TPM1_CNT & 0xFF);
	while( !( ( letra >= 65 && letra <= 90 ) ||  (letra >= 97 && letra <= 122) ) ){
		if ( letra<65 ) letra+=65;
		if ( letra>122 ) letra = letra%122;
		if ( letra>=91 && letra <= 96) letra+=6;
	}
	return letra;
}

void BP_geraPilhaInicial(BufferPilha_type *buffer){
	uint8_t i=1;
	while (i<=16){
		BP_add (buffer, BP_geraLetra(),0,0);
		i++;
	}
	
	return;
}

void BP_printaPilhaInicial(BufferPilha_type *buffer){
	uint8_t i =0;
	while (i <= buffer->tail){
		GPIO_escreveCharLCD(LCD_printOffset(i),buffer->dados[i]);
		i++;
	}
	uint16_t mod = calcMod(15);
	TPM_config_especifica (1, mod, 0b1111, 0, 0, 0, 0, 0, 0b101);
	TPM_CH_config_especifica (1, 1, 0b1010, mod/2); //MS_ELS = 1010 -> EPWM high true
}

uint8_t BP_add (BufferPilha_type *buffer, char item, uint8_t print, uint8_t buzzerUp){
	uint8_t next = buffer->tail+1;
	
	if (next>=32) return -1; //Sem espaco no buffer
	
	if(print) GPIO_escreveCharLCD(LCD_printOffset(next),item);
	if(buzzerUp){
		uint16_t mod = calcMod(next);
		TPM_config_especifica (1, mod, 0b1111, 0, 0, 0, 0, 0, 0b101);
		TPM_CH_config_especifica (1, 1, 0b1010, mod/2); //MS_ELS = 1010 -> EPWM high true
	}
	
	(buffer->dados)[next] = item;
	buffer->tail=next;
	
	return 0;
}

uint8_t BP_del (BufferPilha_type *buffer, uint8_t print, uint8_t buzzerDown){
	int8_t prev = buffer->tail-1;
	
	if (prev<-1) return -1; //Sem item no buffer
	
	if (print) GPIO_escreveCharLCD(LCD_printOffset(buffer->tail),' ');
	if(buzzerDown){
		uint16_t mod = calcMod(prev);
		TPM_config_especifica (1, mod, 0b1111, 0, 0, 0, 0, 0, 0b101);
		TPM_CH_config_especifica (1, 1, 0b1010, mod/2); //MS_ELS = 1010 -> EPWM high true
	}
	
	buffer->tail=prev;
	
	return 0;
}

uint8_t BP_get (BufferPilha_type *buffer, char* item){
	if (buffer->tail > -1){
		*item = buffer->dados[buffer->tail];
		return 0;
	}
	return -1;
}

uint8_t BP_isFull (BufferPilha_type *buffer){
	return ( buffer->tail == 31 )?  1:0;
}

uint8_t BP_isEmpty (BufferPilha_type *buffer){
	return ( buffer->tail == -1 )?  1:0;
}


void BP_reset(BufferPilha_type *buffer){
	buffer->tail=-1;
}

