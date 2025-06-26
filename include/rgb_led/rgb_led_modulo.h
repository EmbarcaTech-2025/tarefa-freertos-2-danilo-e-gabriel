#ifndef RGB_LED_MODULO_H
#define RGB_LED_MODULO_H

#include <stdint.h>

/*  Inicializa o LED RGB nos pinos definidos em config.h  */
void init_rgb_led(void);

/*  Define o valor PWM de cada canal (0–255)              */
void rgb_led_set(uint8_t r, uint8_t g, uint8_t b);

/*  Conveniências para feedback rápido                    */
void show_success(void);   /* verde  */
void show_error(void);     /* vermelho */

void blink_select(void);                 //  ← NOVA API

#endif /* RGB_LED_MODULO_H */
