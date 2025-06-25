#include "../config.h"
#include "joystick_modulo.h"
#include "hardware/adc.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>

int init_joystick() {
    // Inicializa entrada analógica
    adc_init();
    // Inicializa gpio adc
    adc_gpio_init(JOYSTICK_Y);
    adc_gpio_init(JOYSTICK_X);

    printf("Analogico Inicializado corretamente\n");
}

JoystickStruct* joystick_value () {
    JoystickStruct* Joystick_values = malloc(sizeof(JoystickStruct));

    if (Joystick_values == NULL) {
        return NULL;
    }

    adc_select_input(0);
    Joystick_values->y = adc_read();
    adc_select_input(1);
    Joystick_values->x = adc_read();

    return Joystick_values;
}
