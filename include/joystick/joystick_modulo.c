#include "../config.h"
#include "joystick_modulo.h"
#include "hardware/adc.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>

#define THRESHOLD_HIGH 3500
#define THRESHOLD_LOW 500   
#define THRESHOLD_CENTER_MIN 1800
#define THRESHOLD_CENTER_MAX 2200

enum cores {
    VERMELHO,
    VERDE,
    AZUL,
    BRANCO,
    ZERO 
};

int init_joystick() {
    
    adc_init();
    
    adc_gpio_init(JOYSTICK_Y);
    adc_gpio_init(JOYSTICK_X);

    printf("Analogico Inicializado corretamente\n");
    return 0; 
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


int get_joystick_color(int x_value, int y_value) {
   
    if (y_value > THRESHOLD_HIGH) {
       
        if (x_value < THRESHOLD_LOW) {
            return VERMELHO;
        }
       
        else if (x_value > THRESHOLD_HIGH) {
            return AZUL;
        }
    }
    
    else if (y_value < THRESHOLD_LOW) {
       
        if (x_value < THRESHOLD_LOW) {
            return VERDE;
        }
        
        else if (x_value > THRESHOLD_HIGH) {
            return BRANCO;
        }
    }
    
    return ZERO;
}