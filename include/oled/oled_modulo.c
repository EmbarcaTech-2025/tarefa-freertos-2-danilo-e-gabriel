#include "oled/oled_modulo.h"
#include "config.h"
#include "ssd1306.h"
#include <stdio.h>

static ssd1306_t disp;

void init_oled(void)
{
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    /* 128×64, endereço padrão 0x3C */
    if (!ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT)) {
        printf("SSD1306 init FAILED\n");
        return;
    }
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 0, 0, 1, "SIMON SAYS");
    ssd1306_show(&disp);
}

void oled_show_score(int pontos)
{
    char buf[20];
    sprintf(buf, "SCORE: %d", pontos);
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 0, 0, 1, buf);
    ssd1306_show(&disp);
}

void oled_show_status(const char *msg)   //  ← NOVA FUNÇÃO
{
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 0, 0, 1, msg);
    ssd1306_show(&disp);
}

void oled_show_status2(const char *l1, const char *l2)
{
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 0,  0, 1, l1);
    ssd1306_draw_string(&disp, 0, 16, 1, l2);   // linha abaixo (~16 px)
    ssd1306_show(&disp);
}

void oled_show_status3(const char *l1, const char *l2,
                       const char *l3)
{
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 0,  0, 1, l1);
    ssd1306_draw_string(&disp, 0, 16, 1, l2);
    ssd1306_draw_string(&disp, 0, 32, 1, l3);
    ssd1306_show(&disp);
}
