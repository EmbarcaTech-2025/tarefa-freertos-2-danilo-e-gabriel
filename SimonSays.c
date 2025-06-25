#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"

#include "config.h"

#include "buzzer/buzzer_modulo.h"
#include "joystick/joystick_modulo.h"
#include "matriz_led/matriz_led_modulo.h"

#include <stdint.h>



void tarefa1(void *pvParameters) {
    while (1) {
        printf("Tarefa 1 rodando\n");
        JoystickStruct *Valores = joystick_value();
        printf("Valor X: %d, Valor Y: %d", Valores->x, Valores->y);
        turn_red();
        sleep_ms(2000);
        turn_green();
        sleep_ms(2000);
        turn_blue();
        sleep_ms(2000);
        turn_white();
        sleep_ms(2000);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    stdio_init_all();
    init_joystick();
    inicializa_matriz_led();
    xTaskCreate(tarefa1, "Tarefa1", 2048, NULL, 1, NULL);

    vTaskStartScheduler();

    return 0;
}
