#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "lib/ssd1306.h"
#include "lib/font.h"
#include "hardware/timer.h"
#include "lib/init.h"
#include "ws2812.pio.h"
#include "lib/buzzer.h"
#include "lib/matriz_leds.h"

// Tempo de debouncing
static volatile uint32_t last_joy_button_time = 0;
#define DEBOUNCE_TIME 200000 // 200ms de debounce

int estado_atual_botao_a = 1;  // Estado inicial do botão A
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)
ssd1306_t oled;

// Estrutura para os menus
typedef struct {
    const char *title;
    const char *options[3];
} Menu;

// Menus e submenus
Menu main_menu = {"MENU", {"1Sensivel", "2Estimulo", "3Modo Alerta"}};
Menu submenu1 = {"Sensivel", {"Modo tranquilizante", "Monitoramento ruidos", "Voltar"}};
Menu submenu2 = {"Estimulo", {"Visuais", "Sonoros", "Voltar"}};
Menu submenu3 = {"Falante", {"Sentimentos", "Acoes", "Voltar"}};

Menu *current_menu = &main_menu;
int menu_option = 0;

// Função de leitura do eixo Y do joystick
int read_joy_y() {
    adc_select_input(1);
    uint16_t y_value = adc_read();
    return y_value;
}

// Função para desenhar o menu
void draw_menu() {
    ssd1306_fill(&oled, 0);
    ssd1306_rect(&oled, 0, 0, 127, 63, 1, 0); // Moldura externa
    ssd1306_rect(&oled, 10, 0, 127, 50, 1, 0); // Moldura interna
    ssd1306_draw_string(&oled, current_menu->title, 48, 2);

    for (int i = 0; i < 3; i++) {
        if (i == menu_option) {
            ssd1306_draw_string(&oled, "0", 10, i * 10 + 16); // Setinha para indicar a opção
            ssd1306_command(&oled, SET_NORM_INV);
            ssd1306_draw_string(&oled, current_menu->options[i], 24, i * 10 + 16);
            ssd1306_command(&oled, SET_NORM_INV);
        } else {
            ssd1306_draw_string(&oled, current_menu->options[i], 24, i * 10 + 16);
        }
    }
    ssd1306_send_data(&oled);
}

// Callback da interrupção do botão do joystick com debounce
void button_callback(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se passou tempo suficiente para evitar bouncing (200ms)
    if (current_time - last_joy_button_time > DEBOUNCE_TIME) {
        last_joy_button_time = current_time;  // Atualiza o tempo do último evento

        if (gpio == JOY_BUTTON) {
            if (current_menu == &main_menu) {
                switch (menu_option) {
                    case 0: current_menu = &submenu1; break;
                    case 1: current_menu = &submenu2; break;
                    case 2: current_menu = &submenu3; break;
                }
            } else if (current_menu == &submenu1) {
                switch (menu_option) {
                    case 0: // Ação para "Modo tranquilizante"
                        gpio_put(LED_G_PIN, true);  // Acende o LED verde para demonstrar a ação
                        gpio_put(LED_R_PIN, false); // Desliga o LED vermelho (opcional)
                        gpio_put(LED_B_PIN, false); // Desliga o LED azul (opcional)
                        draw_menu();  // Atualiza o display
                        break;
                    case 1: // Ação para "Monitoramento de ruidos"
                        // Outra ação pode ser realizada, como emitir um som
                        printf("Monitoramento de ruídos ativado!\n");
                        draw_menu();
                        break;
                    case 2: current_menu = &main_menu; break; // Volta ao menu principal
                }
            } else if (current_menu == &submenu2) {
                switch (menu_option) {
                    case 0: // Ação para "Visuais"
                        printf("Modo visual ativado!\n");
                        draw_menu();
                        break;
                    case 1: // Ação para "Sonoros"
                        printf("Modo sonoro ativado!\n");
                        draw_menu();
                        break;
                    case 2: current_menu = &main_menu; break; // Volta ao menu principal
                }
            } else if (current_menu == &submenu3) {
                switch (menu_option) {
                    case 0: // Ação para "Sentimentos"
                        printf("Sentimentos ativados!\n");
                        draw_menu();
                        break;
                    case 1: // Ação para "Ações"
                        printf("Ações ativadas!\n");
                        draw_menu();
                        break;
                    case 2: current_menu = &main_menu; break; // Volta ao menu principal
                }
            }
            menu_option = 0; // Reseta a opção do menu
            draw_menu(); // Desenha o menu após a ação
        }
    }
}

// Navegação joystick
void joy_navigation() {
    int y_value = read_joy_y();

    if (y_value < 1000) {
        menu_option = (menu_option + 1) % 3;
        draw_menu();
        sleep_ms(200);
    } else if (y_value > 3000) {
        menu_option = (menu_option - 1 + 3) % 3;
        draw_menu();
        sleep_ms(200);
    }
}

int main() {
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    setup();
    pwm_init_buzzer(BUZZER_PIN);
    // Configuração da interrupção para o botão do joystick com debounce
    gpio_set_irq_enabled_with_callback(JOY_BUTTON, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    draw_menu();
    while (1) {
        joy_navigation();
    }
    return 0;
}
