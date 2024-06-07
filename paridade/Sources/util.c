/**
 * @file util.c
 * @brief Definicao das funcoes uteis do projeto
 * @date Jan 11, 2023
 * @author Wu Shin-Ting   
 */
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"
#include "string.h"

uint8_t ConvertBitStringtoUl32 (char* str, uint32_t *valor){
	uint8_t i=1;
	*valor=0;
	uint32_t prev=*valor;
	uint8_t len = strlen(str);
	while(str[i]!='\0'){
		if (str[i] < '0' || str[i]>'1' || *valor<prev){
			return 2;	// Valor invalido
		}
		prev=*valor;
		*valor += ((str[i]-'0')<<(len - i - 1));
		i++;
	}
	return 0;
}

uint8_t ConvertDecStringtoUl32 (char* str, uint32_t *valor){
	uint8_t i=0;
	*valor=0;
	uint32_t prev=*valor;
	uint32_t aux;
	uint8_t len = strlen(str);
	uint8_t exp;
	while(str[i]!='\0'){
		if (str[i] < '0' || str[i]>'9' || *valor<prev){
			return 2;	// Valor invalido
		}
		prev=*valor;
		aux=1;
		exp = len - i - 1;
		while (exp!=0){
			aux*=10;
			exp--;
		}
		*valor += (str[i]-'0')*aux;
		i++;
	}
	return 0;
}

uint8_t ConvertHexStringtoUl32 (char* str, uint32_t *valor){
	uint8_t i=1;
	*valor=0;
	uint32_t prev=*valor;
	uint8_t len = strlen(str);
	while(str[i]!='\0'){
		if ( !( (str[i] >= '0' && str[i]<='9') || (str[i] >= 'A' && str[i]<='F') || (str[i] >= 'a' && str[i]<='f') ) || *valor<prev ){
			return 2;	// Valor invalido
		}
		prev=*valor;
		if(str[i] >= '0' && str[i]<='9'){
			*valor += (str[i]-'0')<<(4*(len - i - 1));
		}
		else if (str[i] >= 'A' && str[i]<='F'){
			*valor += (str[i]-'A'+10)<<(4*(len - i - 1));
		}
		else if (str[i] >= 'a' && str[i]<='f'){
			*valor += (str[i]-'a'+10)<<(4*(len - i - 1));
		}
		i++;
	}
	return 0;
}

uint8_t findParityBit(uint32_t x, char* str){
	// divide recursivamentemente o inteiro (32 bits) em dois
	// reduz pela metade e pega seu XOR até restar apenas 1 bit
	
	x = (x & 0x0000FFFF) ^ (x >> 16);
	x = (x & 0x000000FF) ^ (x >> 8);
	x = (x & 0x0000000F) ^ (x >> 4);
	x = (x & 0x00000003) ^ (x >> 2);
	x = (x & 0x00000001) ^ (x >> 1);
	// A partir daqui, x == 0 indica que x tem numero par de 1s e x == 1 indica que x tem um numero impar de 1s
	
	if (str[0] == 'I' || str[0]=='i'){ // Se a paridade desejada e impar. O bit de paridade sera 1 se  x == 0 e 0 se x == 1 
		return !x;
	}
	else { // Se a paridade desejada e par. O bit de paridade sera 0 se  x == 0 e 1 se x == 1
		return x;
	}
	
}

char* ConvertUl32toBitString (uint32_t value, char *bin){
	uint8_t i = 0;
	while (i<=32){
		bin[i] = '0' + ((value>>(32-i))&1);
		i++;
	}
	bin[i]='\0';
	return bin;
}

void createOutputString(char* bin, char* parity, uint8_t parity_bit ,char* out_str){
	strcpy(out_str,bin);
	strcat(out_str," tem uma quantidade");
	if (parity[0]=='I' || parity [0] == 'i'){
		if(parity_bit){
			strcat(out_str," par de 1. O digito de paridade impar: ");
		} else {
			strcat(out_str," impar de 1. O digito de paridade impar: ");
		}
	} else {
		if(parity_bit){
			strcat(out_str," impar de 1. O digito de paridade par: ");
		} else {
			strcat(out_str," par de 1. O digito de paridade par: ");
		}
	}
	char aux[]={'\0','\n','\r','\0'};
	aux[0] = (parity_bit)? '1':'0';
	strcat(out_str, aux);
}
