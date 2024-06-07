/**
 * @file util.c
 * @brief Definicao das funcoes uteis do projeto
 * @date 14/04/2023
 * @author Wu Shin-Ting 
 * @author Mateus Henrique Silva Araujo
 */
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"

void espera_5us (uint32_t multiplos)
{

		__asm__ (
				"iteracao: \n\t"
					"mov r2, #4 \n\t"
				"laco: \n\t" 
					"mov r3, #5 \n\t"
					"orr r3, r0 \n\t"
					"and r3, r0 \n\t"     
					"lsr r3, #1 \n\t"
					"asr r3, #1 \n\t"
					"and r3, r0 \n\t"
					"lsr r3, #1 \n\t"
					"asr r3, #1 \n\t"
					"asr r3, #1 \n\t"   
					"and r3, r0 \n\t"     
					"lsr r3, #1 \n\t"     
					"asr r3, #1 \n\t" 
					"asr r3, #1 \n\t"    
					"and r3, r0 \n\t"     
					"lsr r3, #1 \n\t"     
					"asr r3, #1 \n\t" 
					"asr r3, #1 \n\t"    
					"and r3, r0 \n\t"     
					"lsr r3, #1 \n\t"     
					"asr r3, #1 \n\t" 
					"asr r3, #1 \n\t"
					"sub r2, #1 \n\t"					 
					"cmp r2, #0 \n\t"
					"bne laco \n\t"
					"rev r3,r3 \n\t"
					"lsl r3,#0 \n\t"
					"sub %0,#1 \n\t"
					"cmp %0, #0 \n\t"
					"bne iteracao \n\t"
				 :
				 : "r" (multiplos)
				 : "r2","r3","cc"
		);
}

void espera (uint32_t valor)
{
	while (valor) valor--;
}

void UlToStr(char *s, uint32_t bin, uint8_t n)
{
    s += n;
    *s = '\0';

    while (n--)
    {
        *--s = (bin % 10) + '0';
        bin /= 10;
    }
}

void ConvertSectoDay(uint32_t n, uint32_t *days, uint32_t *hours, uint32_t *minutes, uint32_t *seconds)
{
    *days = n / (24 * 3600);
 
    n = n % (24 * 3600);
    *hours = n / 3600;
 
    n %= 3600;
    *minutes = n / 60 ;
 
    n %= 60;
    *seconds = n;     
}

char *ConvertSectoDayString (uint32_t seconds, char *string)
{
	uint32_t dd, hh, mm, ss;

	ConvertSectoDay (seconds, &dd, &hh, &mm, &ss);

	string[2] = string[5] = ':';    // inserir os espacadores
	string[8] = '\0';				// terminador

	if (hh > 23) {
		//!< horario invalido: FF:FF:FF
		string[0] = string[1] = string[3] = string[4] = string[6] = string[7] = 'F';
	}

	string[0] = (hh < 10)? '0': (hh/10)+'0';
	string[1] = hh%10+'0';

	string[3] = (mm < 10)? '0': (mm/10)+'0';
	string[4] = mm%10+'0';

	string[6] = (ss < 10)? '0': (ss/10)+'0';
	string[7] = ss%10+'0';

	return string;
}

void ConvertDaytoSec(uint32_t days, uint32_t hours, uint32_t minutes, uint32_t seconds, uint32_t *n)
{
	*n = days*86400+hours*3600+minutes*60+seconds;
}
