#include "../config.h"
#include "matriz_led_modulo.h"
#include "ws2818b.pio.h"

#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define LED_WIDTH 5
#define LED_HEIGHT 5
#define LED_COUNT (LED_WIDTH * LED_HEIGHT)

struct pixel_t {
    uint8_t G, R, B;
};

typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t;
    
npLED_t leds[LED_COUNT];
 
PIO np_pio;
uint sm;
  
static void npInit() {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
  
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
      np_pio = pio1;
      sm = pio_claim_unused_sm(np_pio, true);
    }
  
    ws2818b_program_init(np_pio, sm, offset, MATRIZ_LED_PIN, 800000.f);
  
    for (uint i = 0; i < LED_COUNT; ++i) {
      leds[i].R = 0;
      leds[i].G = 0;
      leds[i].B = 0;
    }
  }
  
static void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
  }
  
static void npClear() {
    for (uint i = 0; i < LED_COUNT; ++i)
      npSetLED(i, 0, 0, 0);
  }
  
static void npWrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {
      pio_sm_put_blocking(np_pio, sm, leds[i].G);
      pio_sm_put_blocking(np_pio, sm, leds[i].R);
      pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100);
}

void inicializa_matriz_led() {
    npInit();
    npClear();
    npWrite();

    printf("Matriz inicializada\n");
}


uint calculate_led_index(uint x, uint y) {
    uint index;
    if (y % 2 == 0)
    {
        index = (4 - y) * 5 + (4 - x);
    }
    else
    {
        index = (4 - y) * 5 + x;
    }
    return index;
}

void turn_led_color_in_memory(uint x, uint y, uint8_t r, uint8_t g, uint8_t b)
{
    uint index = calculate_led_index(x, y);
    npSetLED(index, r, g, b);
}

void reseta_coluna_leds(uint coluna)
{
    for (int i = 0; i < 5; i++)
    {
        turn_led_color_in_memory(i, coluna, 0, 0, 0);
    }
    npWrite();
}

void turn_off_all () {
    npClear();
    npWrite();
}

void turn_red() {
    npClear();
    turn_led_color_in_memory(0, 0, 255, 0, 0);
    turn_led_color_in_memory(1, 0, 255, 0, 0);
    turn_led_color_in_memory(0, 1, 255, 0, 0);
    turn_led_color_in_memory(1, 1, 255, 0, 0);
    npWrite();
}

void turn_green() {
    npClear();
    turn_led_color_in_memory(0, 3, 0, 255, 0);
    turn_led_color_in_memory(0, 4, 0, 255, 0);
    turn_led_color_in_memory(1, 3, 0, 255, 0);
    turn_led_color_in_memory(1, 4, 0, 255, 0);
    npWrite();
}

void turn_blue() {
    npClear();
    turn_led_color_in_memory(3, 0, 0, 0, 255);
    turn_led_color_in_memory(4, 1, 0, 0, 255);
    turn_led_color_in_memory(3, 1, 0, 0, 255);
    turn_led_color_in_memory(4, 0, 0, 0, 255);
    npWrite();
}

void turn_white() {
    npClear();
    turn_led_color_in_memory(3, 3, 255, 255, 255);
    turn_led_color_in_memory(4, 3, 255, 255, 255);
    turn_led_color_in_memory(3, 4, 255, 255, 255);
    turn_led_color_in_memory(4, 4, 255, 255, 255);
    npWrite();
}

void turn_white_zero() {
    npClear();
    turn_led_color_in_memory(3, 3, 0, 0, 0);
    turn_led_color_in_memory(4, 3, 0, 0, 0);
    turn_led_color_in_memory(3, 4, 0, 0, 0);
    turn_led_color_in_memory(4, 4, 0, 0, 0);
    npWrite();
}