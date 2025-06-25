#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include <stdint.h>
// Bibliotecas criadas
#include "config.h"
#include "buzzer/buzzer_modulo.h"
#include "joystick/joystick_modulo.h"
#include "matriz_led/matriz_led_modulo.h"
#include "botao/botao_modulo.h"

enum cores {
    VERMELHO,
    VERDE,
    AZUL,
    BRANCO,
    ZERO
};

int sequencia_fases[] = {0,3,0,1,0,1,2,3,0,1};
volatile int fase_atual = 2;

void tarefa1(void *pvParameters) {
    while (1) {
        printf("Tarefa 1 rodando\n");
        JoystickStruct *Valores = joystick_value();
        printf("Valor X: %d, Valor Y: %d\n", Valores->x, Valores->y);

        turn_off_all();
        printf("Apagou tudo\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// void tarefa2(void *pvParameters) {

//     for (int i = 0; i <= fase_atual; i++) {
//         printf("Inicio: %d\n", i);
//         if (sequencia_fases[i] == VERMELHO) turn_red();
//         if (sequencia_fases[i] == VERDE) turn_green();
//         if (sequencia_fases[i] == AZUL) turn_blue();
//         if (sequencia_fases[i] == BRANCO) turn_white();

//         vTaskDelay(pdMS_TO_TICKS(1000));

//         printf("Passou daqui %d\n", i);
//     }

//     vTaskDelete(NULL);

//     for (;;) vTaskDelay(pdMS_TO_TICKS(1000));
    
// }

void play_sequencia() {
    printf("Iniciando a reprodução da sequência...\n");
    // Este loop irá percorrer a sequência de cores até a fase atual do jogo.
    for (int i = 0; i <= fase_atual; i++) {
        printf("Mostrando cor da fase %d: %d\n", i, sequencia_fases[i]);
        
        // Use um switch-case para acender o LED da cor correta.
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
            default:
                printf("Cor inválida na sequência: %d\n", sequencia_fases[i]);
                break;
        }

        sleep_ms(1000); 
        // Apaga todos os LEDs para que o próximo se destaque.
        turn_off_all();
        // Aguarda um curto intervalo entre as cores.
        sleep_ms(250); 
    }

    printf("Terminou de reproduzir a sequência. É sua vez!\n");
}

int main() {
    stdio_init_all();
    init_joystick();
    inicializa_matriz_led();
    inicializa_botoes();
    sleep_ms(2000);
    // xTaskCreate(tarefa2, "Tarefa_mostrar_sequencia", 2048, NULL, 1, NULL);
    play_sequencia();
    sleep_ms(3000);
    play_sequencia();

    vTaskStartScheduler();

    return 0;
}
