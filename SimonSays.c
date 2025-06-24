#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"

#include "config.h"

#include "buzzer/buzzer_modulo.h"
#include "joystick/joystick_modulo.h"

void tarefa1(void *pvParameters) {
    while (1) {
        printf("Tarefa 1 rodando\n");
        JoystickStruct *Valores = joystick_value();
        printf("Valor X: %d, Valor Y: %d", Valores->x, Valores->y);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo
    }
}

int main() {
    stdio_init_all();
    init_joystick();
    xTaskCreate(tarefa1, "Tarefa1", 2048, NULL, 1, NULL);

    vTaskStartScheduler();

    return 0;
}
