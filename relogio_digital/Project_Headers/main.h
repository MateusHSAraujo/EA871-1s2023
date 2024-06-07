/*!
 * @file main.h
 * @brief Declaracao dos prototipos, macros e tipos de dados relacionados com main
 * @date 14/04/2023
 * @author Wu Shin-Ting 
 * @author Mateus Henrique Silva Araujo
 */

#ifndef MAIN_H_
#define MAIN_H_

/**
 * @brief Atualiza horario no LCD
 * @param[in] segundos 
 */
void atualizaHorarioLCD (uint32_t segundos);

/**
 * @brief Fluxo de controle principal do projeto relogio_com_ajustes
 */
int main(void);

#endif /* MAIN_H_ */
