#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "config.h"

#include "buzzer/buzzer_modulo.h"

void tarefa1(void *pvParameters) {
    while (1) {
        printf("Tarefa 1 rodando\n");
        pwm_init_buzzer();
        play_buzzer(500);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo
    }
}

int main() {
    xTaskCreate(tarefa1, "Tarefa1", 2048, NULL, 1, NULL);

    vTaskStartScheduler();

    return 0;
}
