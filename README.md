
# Tarefa: Roteiro de FreeRTOS #2 - EmbarcaTech 2025

Autores: **Danilo Naves do Nascimento** e **Gabriel Martins Ribeiro**

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Brasília, Junho de 2025

---

# 🎮 Simon Says — RP2040 + FreeRTOS   
_Recriação do clássico jogo da memória usando a placa **BitDogLab (RP2040)**_

---
## ▶ Vídeo de Demonstração

Assista ao protótipo funcionando no YouTube :

[![Veja o demo no YouTube](https://img.youtube.com/vi/aJsjYD89ZdE/0.jpg)](https://youtube.com/shorts/aJsjYD89ZdE?feature=share)

Clique na imagem acima ou, se preferir, use o link de texto:

[▶️ Demo no YouTube](https://youtube.com/shorts/aJsjYD89ZdE?feature=share)


---

## ✨ Visão Geral
Projeto educacional que demonstra:
* **Programação bare-metal em C** no RP2040  
* **FreeRTOS SMP** (duplo core)  
* Drivers para **WS2812**, **pwm buzzer**, **joystick analógico**, **botões** e **display OLED SSD1306**  
* Estruturação **modular** (cada periférico no próprio módulo)

O objetivo é reproduzir (e aumentar) a sequência de cores que aparece na matriz de LEDs, usando o joystick + Botão A para selecionar cores. Quando errar, LED central fica vermelho, buzzer toca, OLED mostra “ERROR!” e surge o prompt “PRESS B TO RESTART”.

---

## 📦 Hardware

| Item | Pino | Descrição |
|------|------|-----------|
| Matriz WS2812 (8×8) | GP7 (PIO) | Exibe sequência de cores |
| LED RGB central | GP11/12/13 (PWM) | Verde = acerto · Vermelho = erro · Azul (blink) = seleção |
| Buzzer passivo | GP21 (PWM) | Sons de acerto/erro |
| Joystick analógico | GP26 (X) · GP27 (Y) (ADC) | Seleciona cor |
| Botão A | GP5  | Confirma cor |
| **Botão B** | GP6  | Reinicia jogo |
| OLED SSD1306 (I²C) | GP14 (SDA) · GP15 (SCL) (I²C1) | Score + mensagens |

> Todos os pinos estão centralizados em **[`config.h`](include/config.h)**.

---

## ✔️ Funcionalidades

- Alternância de cores: **Vermelho · Verde · Azul · Branco**
- Pontuação em tempo real no OLED  
- Buzzer toda som quando usuário errar a sequência
- LED RGB central indica estado  
- **Blink** de confirmação ao selecionar cor  
- Mensagens de status no OLED (“COR OK!”, “ERROU!”, “PRESS B TO RESTART”)  
- Pausa inicial de 1,5 s antes da 1ª cor  
- Código 100 % modular + FreeRTOS (2 tasks, semáforos binários)

---

## 🗂️ Estrutura de Diretórios
```text
.
├── include/
│ ├── buzzer/ # driver PWM do buzzer
│ ├── joystick/
│ ├── matriz_led/ # WS2812 + PIO
│ ├── rgb_led/
│ ├── botao/
│ ├── oled/ # wrapper SSD1306
│ ├── ssd1306.c|h # driver base
│ └── config.h # pinos do projeto
├── FreeRTOS-Kernel/ # submódulo
├── SimonSays.c # main + tasks
└── CMakeLists.txt
```

---

## 🛠️ Requisitos

| Software | Versão |
|----------|--------|
| **Pico SDK** | 2.1.1 |
| **Toolchain Arm GCC** | 14-₂ Rel 1 |
| **CMake** | ≥ 3.13 |
| **FreeRTOS-Kernel** | tag `V10.5.1` (submódulo)

---

## 🚀 Build & Flash

```bash
# clone com submódulos
git clone --recurse-submodules https://github.com/<usuario>/SimonSays-RP2040.git
cd SimonSays-RP2040

# gerar build Release
cmake -B build -DPICO_BOARD=pico
cmake --build build -j

# entrar no modo UF2 da placa e copiar
cp build/SimonSays.uf2 /media/$USER/RPI-RP2
```
### 🎮 Como Jogar

| Ação                | Controles                                                                                 |
|---------------------|-------------------------------------------------------------------------------------------|
| Selecionar cor      | Mover joystick até a cor + pressionar **Botão A**                                         |
| Confirmar sequência | Última cor correta → LED verde + score sobe                                               |
| Erro                | LED vermelho + som; OLED mostra **ERROU!**                                                |
| Recomeçar           | Pressionar **Botão B** quando OLED pedir “PRESS B TO RESTART”                             |

---

### 🗓️ Roadmap / TODO

- [ ] Salvar **high-score** em flash  
- [ ] Animação “attract mode” na matriz durante inatividade  
- [ ] Ajuste fino de dead-zone do joystick  
- [ ] Tradução multilíngue BR/EN via `#define`

---
## 📜 Licença
GNU GPL-3.0.
