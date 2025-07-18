#include "buzzer_modulo.h"
#include "../config.h"

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include <stdint.h>
#include <stdio.h>

#define BUZZER_FREQUENCY 100

void pwm_init_buzzer() {
    // Configurar o pino como saída de PWM
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096)); // Divisor de clock
    pwm_init(slice_num, &config, true);
    // Iniciar o PWM no nível baixo
    pwm_set_gpio_level(BUZZER_PIN, 0);

    printf("Buzzer Inicializado corretamente\n");
}

// Definição de uma função para emitir um beep com duração especificada
void play_buzzer(int duration_ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    // Configurar o duty cycle para 50% (ativo)
    pwm_set_gpio_level(BUZZER_PIN, 2048);
    // Temporização
    sleep_ms(duration_ms);
    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(BUZZER_PIN, 0);
    // Pausa entre os beeps
    sleep_ms(100); // Pausa de 100ms
}

/* Toca um bipe de erro – hoje 800 Hz/400 ms   */
void play_error_tone(void)                  // ★ NOVA FUNÇÃO
{
    play_buzzer(800);   // reutiliza API que já existia
    sleep_ms(400);      // mantém nota por 400 ms
    play_buzzer(0);     // silencia
}