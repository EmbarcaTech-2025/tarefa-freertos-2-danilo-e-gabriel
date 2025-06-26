#ifndef OLED_MODULO_H
#define OLED_MODULO_H
void init_oled(void);
void oled_show_score(int pontos); 
void oled_show_status(const char *msg);
void oled_show_status2(const char *l1, const char *l2);
void oled_show_status3(const char *l1, const char *l2,
                       const char *l3);
                       
#endif
