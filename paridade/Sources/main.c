/*!
 * @file main.c
 * @brief Implementacao do projeto Paridade 
 * @date 05/05/2023
 * @author Mateus Henrique Silva Araujo
 */
#include "stdlib.h"
#include "string.h"
#include "derivative.h"
#include "ISR.h"
#include "UART.h"
#include "SIM.h"
#include "util.h"

uint8_t ExtraiString2Tokens(char *str, char **tokens){
	tokens[0]=strtok(str," ");
	tokens[1]=strtok(NULL," ");
	if (tokens[1] == NULL || strtok(NULL," ")!= NULL){ // Somente um ou mais de dois tokens na string
		return 1;
	}
	if (strcmp(tokens[0],"I")!= 0 && strcmp(tokens[0],"i")!= 0 && strcmp(tokens[0],"P")!= 0 && strcmp(tokens[0],"p") != 0){ // Primeiro token invalido
		return 3;
	}
	return 0;
}

uint8_t getValue(char *val_str,uint32_t *val){
	switch(val_str[0]){
	case 'H':
	case 'h':
		return ConvertHexStringtoUl32(val_str,val); // Caso de entrada hexadecimal
		break;
	case 'B':
	case 'b':
		return ConvertBitStringtoUl32(val_str,val); // Caso de entrada binaria
		break;
	default:
		return ConvertDecStringtoUl32(val_str,val); // Caso de entrada decimal. Tambem trata casos invalidos 
	}
}

int main(void)
{
	 //Habilitar MCGFLLCLK 20971520 Hz
	SIM_setaOUTDIV4 (0b000);
	SIM_setaFLLPLL (0);              //seta FLL
	
	 //Configurar sinais de relogio e pinos
	UART0_config_basica(0b1);
		
	//Configura o modulo UART0 
	//MCGFLLCLK em 20971520 Hz
	UART0_config_especifica (20971520, 38400, 0x0B); // baud_rate = 38400 ; amostragem = x12
	
	//Hablita interrupções
	UART0_habilitaNVICIRQ12(0b0);
	UART0_habilitaInterruptRxTerminal();
	
	//Inicializa buffer circular
	ISR_inicializaBC();
	
	char *tokens[2]; 	// Vetor de srings para guardar os tokens ( tokens[0] == op , tokens[1]== valor )
	char str_in[80]; 	// String de entrada
	char str_out[100]; 	// String de saida
	char bin[33]; 		// String para a conversao binaria

	uint32_t val;		// Variavel para receber o valor inteiro enviado como entrada
	uint8_t parity_bit;	// Variavel para guardar o bit de paridade calculado
	uint8_t error=0;	// Variavel para armazenar estados de erro
	
	//Inicia o programa
	ISR_escreveEstado (MENSAGEM);
	str_in[0] = '\0';
	str_out[0] = '\0';
	
	for(;;) {
		switch(ISR_LeEstado()) {
		case MENSAGEM:
			//Envio da mensagem de soliciacao de entrada
			ISR_EnviaString ("Entre <P/p/I/i> <tipo><valor> (tipo: b/B/h/H)\n\r");
			//Espera o esvaziamento do buffer
			while (!ISR_BufferSaidaVazio());
			//Chaveia para EXPRESSAO
			ISR_escreveEstado(EXPRESSAO);
			break;
		
		case EXPRESSAO:
			//Espera a entrada do usuario
			//Comportamento controlado por UART0_IRQHandler
			break;
			
		case TOKENS:
			//Extrai a string enviada e coloca em str_in
			ISR_ExtraiString(str_in);
			//Extrai os tokens da string de entrada, podendo retornar valores de erro
			error = ExtraiString2Tokens(str_in,tokens);
			if (error){ // Se erro == 1 ou erro == 3, a extracao foi mal suscedida. Se erro == 0, a extracao funcionou
				//Chaveia para ERRO
				ISR_escreveEstado(ERRO);
				break;
			}
			//Extrai o valor da string de entrada, podendo retornar indicadores de erro
			error = getValue(tokens[1],&val); 	
			if(error){ // Se erro == 2, a extracao foi mal suscedida. Se erro == 0, a extracao funcionou
				ISR_escreveEstado(ERRO);
				break;
			}
			//Procedimento do estado TOKENS finalizado. Chaveia para COMPUTO
			ISR_escreveEstado (COMPUTO);
			break;
			
		case COMPUTO:
			//Tokens validos e valor valido. Resta computar a saida
			//Encontra o bit de paridade para a pariade dada
			parity_bit = findParityBit(val,tokens[0]);
			//Converte o valor para binario e escreve na string bin
			ConvertUl32toBitString(val,bin);
			
			//Computacao finalizada. Chaveia para RESULTADO
			ISR_escreveEstado (RESULTADO);
			break;
		
		case RESULTADO:
			//Constroi a string de saida
			createOutputString(bin, tokens[0], parity_bit ,str_out);
			//Envia a string de saida
			ISR_EnviaString(str_out);
			//Espera ate que toda a string seja enviada
			while (!ISR_BufferSaidaVazio());
			//Impressao completa. Reiniciando o sistema
			ISR_escreveEstado (MENSAGEM);
			break;
		
		case ERRO:
			if(error == 1){
				//Mais de 2 tokens detectados. Envia mensagem de erro
				ISR_EnviaString("Quantidade de tokens incorreta. Digite novamente!\n\r");
				//Espera ate que toda a string seja enviada
				while (!ISR_BufferSaidaVazio());
				//Impressao completa. Reiniciando o sistema
				error=0;
				ISR_escreveEstado (MENSAGEM);
			}
			else if(error == 3){
				//Tipo de paridade errado. Envia mensagem de erro
				ISR_EnviaString("Tipo de paridade invalido. Digite novamente!\n\r");
				//Espera ate que toda a string seja enviada
				while (!ISR_BufferSaidaVazio());
				//Impressao completa. Reiniciando o sistema
				error=0;
				ISR_escreveEstado (MENSAGEM);
			} else {
				//Valor inteiro invalido. Envia mensagem de erro
				ISR_EnviaString("Valor inteiro invalido. Digite novamente!\n\r");
				//Espera ate que toda a string seja enviada
				while (!ISR_BufferSaidaVazio());
				//Impressao completa. Reiniciando o sistema
				error=0;
				ISR_escreveEstado (MENSAGEM);
			}
			break;
			
		default:
				break;
		}
	}
	
	return 0; // Never leaves main *-*
}
