#ifndef CONFIG_H
#define CONFIG_H

// ==========================================================
// CONFIGURAÇÕES GERAIS DO PROJETO
// ==========================================================

// Pinagem
#define BUZZER_PIN 21
// Botões
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
// Leds
#define LED_VERDE_PIN 11
#define LED_AZUL_PIN 11
#define LED_VERMELHO_PIN 13
// Display Oled
#define I2C_PORT i2c1
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15
// Joystick
#define JOYSTICK_Y 26
#define JOYSTICK_X 27
// Configuração Matriz de Leds
// Definicao de Numero de Leds e pino da matriz
#define MATRIZ_LED_PIN 7

#endif // CONFIG_H