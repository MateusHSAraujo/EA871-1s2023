/**
 * @file main.c
 * @brief Sinaleira que alterna LED RGB com interrupcoes temporarizadas e assincronas.
 * @author Mateus Henrique Silva Araujo
 * @date 31/03/2023
 */

//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"
//Inclusao dos prototipos
#include "util.h"
#include "GPIO_ledRGB.h"
#include "GPIO_latch_lcd.h"
#include "SysTick.h"
#include "ISR.h"
#include "GPIO_switches.h"
/**
 * @brief Funcao de inicializacao e tratamento de estados de transicao
 */
int main(void)
{
	
	GPIO_initLedRGB();
	
	//Configura SysTick com contagem maxima 10485760/2 (= 0.25s)
	SysTick_init (10485760/2);   //periodo = 20971520*0.25 = 10485760/2
	
	
	//Inicializa botoeiras:
	GPIO_initSwitchNMI(0xa,0);
	
	//Habilita conexao mcu e LCD
	GPIO_ativaConLCD ();
	//Inicializa LCD
	GPIO_initLCD();
	
	// Cria bitmaps 
	char aComAcento[] = {0x01,0x02,0x04,0x0A,0x11,0x1F,0x11,0x00};
	char cComCedilha[8]={0x0e,0x11,0x10,0x10,0x11,0x0e,0x04,0x18};
	char aComTil[8]={0x05,0x0a,0x04,0x0A,0x11,0x1F,0x11,0x00};
	
	GPIO_escreveBitmapLCD (0x01, (uint8_t *)cComCedilha);
	GPIO_escreveBitmapLCD (0x02, (uint8_t *)aComTil);
	GPIO_escreveBitmapLCD (0x03, (uint8_t *)aComAcento);
	
	char bells[] = {'P',0x03,'R','E',' ',' ',' ','\0'};
	char hearts[] = {'A', 'T', 'E','N',0x01,0x02,'O','\0'};
	
	
	//Inicializa o estado do LED RGB
	GPIO_escreveStringLCD (0x05, (uint8_t *)"APERTE");
	ISR_escreveEstado(APAGADO);
	GPIO_ledRGB(OFF, OFF, OFF);
	
	for (;;) {
		switch (ISR_leEstado()) {
		
		case APAGADO_VERMELHO:
			//GPIO_escreveStringLCD (0x00, (uint8_t *)"VERMELHO");
			GPIO_escreveStringLCD (0x04, (uint8_t *)"  ");
			GPIO_escreveStringLCD (0x06, (uint8_t *)bells);
			SysTick_resetaCVR();
			ISR_resetaCounter();
			ISR_setaMaxPostScaler (7); //1.75s
			ISR_escreveEstado (VERMELHO);
			GPIO_ledRGB(ON, OFF, OFF);
			break;
			
		case VERMELHO_AMARELO:
			//GPIO_escreveStringLCD (0x00, (uint8_t *)"AMARELO ");
			GPIO_escreveStringLCD (0x04, (uint8_t *)hearts);
			SysTick_resetaCVR();
			ISR_resetaCounter();
			ISR_setaMaxPostScaler (4); //1s
			ISR_escreveEstado (AMARELO);
			GPIO_ledRGB(ON, ON, OFF);
			break;
			
		case AMARELO_VERDE:
			//GPIO_escreveStringLCD (0x00, (uint8_t *)"VERDE   ");
			GPIO_escreveStringLCD (0x04, (uint8_t *)"  ");
			GPIO_escreveStringLCD (0x06, (uint8_t *)"SIGA   ");
			SysTick_resetaCVR();
			ISR_resetaCounter();
			ISR_setaMaxPostScaler (13);     //3,25s
			ISR_escreveEstado (VERDE);
			GPIO_ledRGB(OFF, ON, OFF);
			break;
		
		case VERDE_APAGADO:
			//GPIO_escreveStringLCD (0x00, (uint8_t *)"APAGADO ");
			GPIO_escreveStringLCD (0x05, (uint8_t *)"APERTE ");
			SysTick_desativaInterrupt();
			ISR_escreveEstado (APAGADO);
			GPIO_ledRGB(OFF, OFF, OFF);
			break;
			
		default:
			
			break;
		}
	}

	/* Never leave main */
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
