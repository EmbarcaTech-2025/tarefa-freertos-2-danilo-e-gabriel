/**
 *  Simon Says – BitDogLab + FreeRTOS (RP2040)
 *  Autor: Gabriel Martins – 2025
 *
 *  ▸ LED RGB nos pinos GP10/11/12 (R,G,B) via PWM
 *  ▸ Buzzer passivo em GP13 (PWM)
 *  ▸ Botões A e B em GP5 / GP6 (pull-ups internos)
 *  ▸ Quatro cores possíveis: RED, GREEN, BLUE, YELLOW
 *    - use apenas RED e GREEN se possuir só dois botões.
 *
 *  Tasks (prioridade):
 *    3  taskPlayerInput   – leitura hard-real de botões
 *    2  taskSequence      – gera sequência & cria nova rodada
 *    2  taskPlayback      – “toca” a sequência (LED + beep)
 *    1  taskFeedback      – efeitos de vitória / derrota
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

#include <stdlib.h>
#include <hardware/clocks.h>

/* ---------- Configuração de pinos ---------- */
#define PIN_LED_R   13
#define PIN_LED_G   11
#define PIN_LED_B   12
#define PIN_BUZZER  10
#define PIN_BTN_A   5
#define PIN_BTN_B   6

/* ---------- Constantes de jogo ---------- */
typedef enum { RED = 0, GREEN } color_t;
#define MAX_LEVELS      32
#define FLASH_MS        500     // duração de cada flash na reprodução
#define INPUT_TIMEOUT   pdMS_TO_TICKS(800)   // modo “hardcore”: 800 ms

/* ---------- Recursos FreeRTOS ---------- */
static QueueHandle_t xQueueSteps;
static EventGroupHandle_t eGameState;
static TaskHandle_t xHandleSequence, xHandleFeedback;

/* Bits do event-group */
#define BIT_PLAYER  (1U << 0)

/* ---------- Variáveis de jogo ---------- */
static color_t seq[MAX_LEVELS];
static uint8_t level = 0;      // nível atual (= n° de cores-1)
static uint8_t pos   = 0;      // posição dentro da rodada

/* =======================================================================
   Hardware helpers
   =======================================================================*/
static void rgb_off(void) { gpio_put(PIN_LED_R, 0); gpio_put(PIN_LED_G, 0); }

static void rgb_color(color_t c, bool on)
{
    uint8_t r = 0, g = 0, b = 0;
    if (on) {
        switch (c) {
            case RED:    r = 1; break;
            case GREEN:  g = 1; break;
        //    case BLUE:   b = 1; break;
        //     case YELLOW: r = g = 1; break;
         }
    }
    gpio_put(PIN_LED_R, r);
    gpio_put(PIN_LED_G, g);
   // gpio_put(PIN_LED_B, b);
}

static void buzzer_tone(color_t c, uint32_t ms)
{
    /* Mapear cor -> frequência simples */
    const uint16_t freq_tbl[] = { 523, 659 };     // C5, E5, G5, B5
    uint16_t freq = freq_tbl[c % 2];

    uint slice = pwm_gpio_to_slice_num(PIN_BUZZER);
    uint channel = pwm_gpio_to_channel(PIN_BUZZER);
    uint32_t sysclk = clock_get_hz(clk_sys);
    uint32_t top = sysclk / freq - 1;
    pwm_set_wrap(slice, top);
    pwm_set_chan_level(slice, channel, top / 2);
    pwm_set_enabled(slice, true);

    vTaskDelay(pdMS_TO_TICKS(ms));

    pwm_set_enabled(slice, false);
}

static void flash_led(color_t c, uint32_t ms)
{
    rgb_color(c, true);
    // buzzer_tone(c, ms);
    rgb_off();
}

/* ---------- Leitura bloquante dos botões ---------- */
static color_t read_buttons(void)
{
    /* Bloqueia até qualquer botão ↓ */
    while (true) {
        if (!gpio_get(PIN_BTN_A)) {      // ativo-baixo
            while (!gpio_get(PIN_BTN_A)) taskYIELD();   // espera soltar
            return RED;                  // mapear Botão A -> RED
        }
        if (!gpio_get(PIN_BTN_B)) {
            while (!gpio_get(PIN_BTN_B)) taskYIELD();
            return GREEN;                // mapear Botão B -> GREEN
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

/* =======================================================================
   Tasks
   =======================================================================*/
static void taskSequence(void *p)
{
    srand(time_us_32());
    for (;;) {
        /* Adiciona cor nova */
        seq[level] = rand() % 2;
        /* Envia sequência inteira (0..level) para o Playback */
        for (uint8_t i = 0; i <= level; ++i)
            xQueueSend(xQueueSteps, &seq[i], portMAX_DELAY);

        /* Sinaliza que é a vez do jogador */
        xEventGroupSetBits(eGameState, BIT_PLAYER);

        /* Aguarda sucesso ou falha (taskPlayerInput ou game_over) */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        /* Pequena pausa entre rodadas */
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

static void taskPlayback(void *p)
{
    color_t step;
    for (;;) {
        /* Bloqueia aguardando um passo vindo da tarefa Sequence */
        if (xQueueReceive(xQueueSteps, &step, portMAX_DELAY) == pdTRUE) {
            flash_led(step, FLASH_MS);

            /* Se a fila acabou (vazia) voltamos ao início do loop
               para esperar mais passos – ou a próxima rodada. */
            vTaskDelay(pdMS_TO_TICKS(150));
        }
    }
}

static void taskPlayerInput(void *p)
{
    for (;;) {
        /* Espera fase PLAYER_TURN */
        xEventGroupWaitBits(eGameState, BIT_PLAYER, pdFALSE, pdTRUE,
                            portMAX_DELAY);

        /* Timeout opcional (hardcore) */
        color_t choice;
        BaseType_t ok = xTaskNotifyWait(0, 0, NULL, INPUT_TIMEOUT);
        (void) ok; // não usado, mas compila

        choice = read_buttons();

        /* Verifica escolha */
        if (choice == seq[pos]) {
            pos++;
            if (pos > level) {           /* Venceu nível */
                level++;  pos = 0;
                xEventGroupClearBits(eGameState, BIT_PLAYER);
                /* Acende verde curto para feedback de sucesso */
                xTaskNotifyGive(xHandleFeedback);    // efeito visual
                vTaskDelay(pdMS_TO_TICKS(200));
                xTaskNotifyGive(xHandleSequence);    // libera próxima rodada
            }
        } else {
            /* Errou: zera jogo */
            xTaskNotifyGive(xHandleFeedback);        // efeito vermelho
            level = pos = 0;
            xEventGroupClearBits(eGameState, BIT_PLAYER);
            vTaskDelay(pdMS_TO_TICKS(400));
            xTaskNotifyGive(xHandleSequence);        // reinicia
        }
    }
}

static void taskFeedback(void *p)
{
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);     // bloqueia até notificação

        if (pos == 0 && level == 0) {   /* Erro */
            rgb_color(RED, true);
            buzzer_tone(RED, 600);
            rgb_off();
        } else {                        /* Sucesso de nível  */
            rgb_color(GREEN, true);
            buzzer_tone(GREEN, 150);
            rgb_off();
        }
    }
}

/* =======================================================================
   main – setup de hardware, filas, tasks e scheduler
   =======================================================================*/
int main(void)
{
    stdio_init_all();

    /* GPIOs */
    const uint8_t leds[] = { PIN_LED_R, PIN_LED_G };
    for (uint i = 0; i < 2; ++i) { gpio_init(leds[i]); gpio_set_dir(leds[i], GPIO_OUT); }
    gpio_init(PIN_BUZZER); gpio_set_function(PIN_BUZZER, GPIO_FUNC_PWM);
    gpio_init(PIN_BTN_A);   gpio_pull_up(PIN_BTN_A);
    gpio_init(PIN_BTN_B);   gpio_pull_up(PIN_BTN_B);

    /* RTOS objects */
    xQueueSteps = xQueueCreate(MAX_LEVELS, sizeof(color_t));
    eGameState  = xEventGroupCreate();

    /* Tasks */
    xTaskCreate(taskSequence,    "Sequence", 512, NULL, 2, &xHandleSequence);
    xTaskCreate(taskPlayback,    "Playback", 512, NULL, 2, NULL);
    xTaskCreate(taskPlayerInput, "Input",    512, NULL, 3, NULL);
    xTaskCreate(taskFeedback,    "Feedback", 384, NULL, 1, &xHandleFeedback);

    /* “Pronto” – pisca LED azul enquanto espera */
  //  rgb_color(BLUE, true);  vTaskDelay(pdMS_TO_TICKS(200));
    rgb_off();

    vTaskStartScheduler();
    for(;;);   /* nunca deve chegar aqui */
}