#ifndef JOYSTICK_MODULO_H
#define JOYSTICK_MODULO_H

typedef struct {
    int x;
    int y;
} JoystickStruct;

int init_joystick();
JoystickStruct* joystick_value();
int get_joystick_color(int x_value, int y_value);

#endif