#ifndef MATRIZ_LED_MODULO_H
#define MATRIZ_LED_MODULO_H

#include "pico/stdlib.h"

void turn_led_color(uint x, uint y, uint8_t r, uint8_t g, uint8_t b);
void reseta_coluna_leds(uint coluna);
void inicializa_matriz_led();
void turn_red();
void turn_green();
void turn_blue();
void turn_white();

#endif
