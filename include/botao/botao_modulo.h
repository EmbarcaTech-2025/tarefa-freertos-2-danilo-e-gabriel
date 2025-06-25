#ifndef BOTAO_MODULO_H
#define BOTAO_MODULO_H

#include <stdbool.h> // Inclua para poder usar o tipo 'bool'

/**
 * @brief Inicializa os pinos GPIO para os botões.
 */
void inicializa_botoes();

/**
 * @brief Verifica o estado do botão 'A'.
 * @return true se o botão estiver pressionado, false caso contrário.
 */
bool is_button_a_pressed();

/**
 * @brief Verifica o estado do botão 'B'.
 * @return true se o botão estiver pressionado, false caso contrário.
 */
bool is_button_b_pressed();

#endif // BOTAO_MODULO_H