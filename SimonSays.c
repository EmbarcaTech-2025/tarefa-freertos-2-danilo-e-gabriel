#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "pico/stdlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "buzzer/buzzer_modulo.h"
#include "joystick/joystick_modulo.h"
#include "matriz_led/matriz_led_modulo.h"
#include "botao/botao_modulo.h"
#include "rgb_led/rgb_led_modulo.h"
#include "oled/oled_modulo.h"

#define START_PAUSE_MS 1500

enum cores {
    VERMELHO,
    VERDE,
    AZUL,
    BRANCO,
    ZERO
};
int sequencia_fases[100]; 
volatile int fase_atual = 0;
volatile bool jogo_ativo = true;
SemaphoreHandle_t xPlayerTurnSemaphore;
SemaphoreHandle_t xGameStartSemaphore;

void generate_new_sequence_step() {
    sequencia_fases[fase_atual] = rand() % 4;
    printf("Nova cor adicionada: %d (para a Fase %d)\n", sequencia_fases[fase_atual], fase_atual + 1);
}

void play_sequencia() {
    printf("Reproduzindo a sequencia da Fase %d...\n", fase_atual + 1);
    for (int i = 0; i <= fase_atual; i++) {
        switch (sequencia_fases[i]) {
            case VERMELHO:
                turn_red();
                break;
            case VERDE:
                turn_green();
                break;
            case AZUL:
                turn_blue();
                break;
            case BRANCO:
                turn_white();
                break;
        }
        sleep_ms(500);
        turn_off_all();
        sleep_ms(250);
    }
}

void game_over(void) {
    printf("--- FIM DE JOGO ---\n");
    jogo_ativo = false;

    
    play_error_tone();                         
    show_error();                               
    char score_buf[20];
    sprintf(score_buf, "SCORE: %d", fase_atual);
    oled_show_status3(score_buf, "ERROR!", "PRESS B TO RESTART");
    turn_white();
    sleep_ms(1000);
    turn_off_all();
    sleep_ms(500);
    turn_red();
    sleep_ms(1000);
    turn_off_all();
    printf("Sua pontuacao foi: %d fases\n", fase_atual);
    printf("Aperte um botao para comecar um novo jogo.\n");

}
void level_up_effect() {
    printf("--- PASSOU DE FASE! ---\n");
    for (int i = 0; i < 4; i++) {
        turn_white();
        sleep_ms(150);
        turn_off_all();
        sleep_ms(150);
    }
}

 /* ---------- TAREFA PRINCIPAL ---------- */
void tarefa_jogo_principal(void *pvParameters) {
    srand(time(NULL)); 
    while (1) {
        if (!jogo_ativo) {
            printf("Aperte o BOTÃO B para recomeçar o jogo.\n");   
            while (!is_button_b_pressed()) {
                vTaskDelay(pdMS_TO_TICKS(100));
            }
            while (!is_button_b_pressed());   

            fase_atual = 0;
            jogo_ativo = true;
            oled_show_score(0);       
 
            vTaskDelay(pdMS_TO_TICKS(START_PAUSE_MS));   

        }
        
        generate_new_sequence_step();
        
        if (fase_atual == 0) {                    
            vTaskDelay(pdMS_TO_TICKS(START_PAUSE_MS));
        }

        play_sequencia();
        
        xSemaphoreGive(xPlayerTurnSemaphore);
        
        xSemaphoreTake(xGameStartSemaphore, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void tarefa_jogador(void *pvParameters)
{
    printf("Tarefa do Jogador rodando, esperando a vez...\n");
    while (1)
    {
        if (xSemaphoreTake(xPlayerTurnSemaphore, portMAX_DELAY) == pdPASS)
        {
            printf("Sua vez! Reproduza a sequencia de %d cores.\n", fase_atual + 1);
            for (int i = 0; i <= fase_atual; i++)
            {
                bool input_received = false;
                while (!input_received)
                {
                    JoystickStruct *valores = joystick_value();
                    int cor_selecionada = get_joystick_color(valores->x, valores->y);
                    if (cor_selecionada != ZERO && is_button_a_pressed())
                    {
                        blink_select();
                        printf("Jogador selecionou a cor: %d\n", cor_selecionada);
            
            if (cor_selecionada == sequencia_fases[i])
            {
                if (i < fase_atual)             
                    oled_show_status("COLOR OK!"); 
                else {
                   
                    show_success();
                    oled_show_score(fase_atual + 1);  
                }

                input_received = true;
                sleep_ms(100);
                turn_off_all();
                while (is_button_a_pressed());
            }
            else
            {
                show_error();
                play_error_tone();           
                game_over();
                input_received = true;
                i = fase_atual;
                while (is_button_a_pressed());
            }
        }

        free(valores);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

            if (jogo_ativo)
            {
                printf("Sequencia da Fase %d completa e correta! Avancando para a proxima fase.\n", fase_atual + 1);
                show_success();
                oled_show_score(fase_atual + 1);
                level_up_effect();
                fase_atual++;
                sleep_ms(1000);
            }
            xSemaphoreGive(xGameStartSemaphore);
        }
    }
}
int main() {
    stdio_init_all();
    init_rgb_led();
    pwm_init_buzzer();
    init_oled();                
    oled_show_score(0);
    init_joystick();
    inicializa_matriz_led();
    inicializa_botoes();
    show_success();
    play_buzzer(60);
    xPlayerTurnSemaphore = xSemaphoreCreateBinary();
    xGameStartSemaphore = xSemaphoreCreateBinary();
    
    if (xPlayerTurnSemaphore == NULL || xGameStartSemaphore == NULL) {
        printf("Erro na criacao dos semaforos\n");
        while(1);
    }
    xTaskCreate(tarefa_jogo_principal, "Jogo Principal", 4096, NULL, 1, NULL);
    xTaskCreate(tarefa_jogador, "Entrada Jogador", 4096, NULL, 2, NULL);
    vTaskStartScheduler();
    return 0;
}