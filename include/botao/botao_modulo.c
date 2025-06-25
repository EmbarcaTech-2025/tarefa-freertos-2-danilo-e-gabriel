#include "botao_modulo.h"
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "../config.h"

#include <stdio.h>

/**
 * @brief Inicializa os pinos GPIO para os botões e configura como entrada com pull-up.
 */
void inicializa_botoes() {
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
    
    printf("Botoes inicializados corretamente\n");
}

/**
 * @brief Verifica se o botão 'A' foi pressionado.
 * @return true se o botão estiver pressionado (nível baixo), false caso contrário.
 */
bool is_button_a_pressed() {
    return !gpio_get(BUTTON_A_PIN); 
}

/**
 * @brief Verifica se o botão 'B' foi pressionado.
 * @return true se o botão estiver pressionado (nível baixo), false caso contrário.
 */
bool is_button_b_pressed() {
    return !gpio_get(BUTTON_B_PIN);
}