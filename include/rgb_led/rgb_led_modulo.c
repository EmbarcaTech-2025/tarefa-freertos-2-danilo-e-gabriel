#include "rgb_led/rgb_led_modulo.h"
#include "config.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

#ifndef PWM_TOP
#define PWM_TOP 255
#endif

static uint slice_r, slice_g, slice_b;

void init_rgb_led(void)
{
   
    gpio_set_function(LED_VERMELHO_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LED_VERDE_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LED_AZUL_PIN, GPIO_FUNC_PWM);

    slice_r = pwm_gpio_to_slice_num(LED_VERMELHO_PIN);
    slice_g = pwm_gpio_to_slice_num(LED_VERDE_PIN);
    slice_b = pwm_gpio_to_slice_num(LED_AZUL_PIN);

    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_wrap(&cfg, PWM_TOP);         
    pwm_config_set_clkdiv(&cfg, 4.f);         

    pwm_init(slice_r, &cfg, true);
    if (slice_g != slice_r) pwm_init(slice_g, &cfg, true);
    if (slice_b != slice_r && slice_b != slice_g) pwm_init(slice_b, &cfg, true);

   
    rgb_led_set(0, 0, 0);
}


static inline void channel_set(uint pin, uint8_t value)
{
    pwm_set_gpio_level(pin, value);
}

void rgb_led_set(uint8_t r, uint8_t g, uint8_t b)
{
    channel_set(LED_VERMELHO_PIN, r);
    channel_set(LED_VERDE_PIN, g);
    channel_set(LED_AZUL_PIN, b);
}

void show_success(void) { rgb_led_set(0, 255, 0);  }  
void show_error  (void) { rgb_led_set(255, 0, 0);  }  

void blink_select(void)                  
{
    rgb_led_set(255, 255, 255);          
    sleep_ms(120);
    rgb_led_set(0,   0,   0);
}