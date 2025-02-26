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
#include "hardware/timer.h"

int animacao_contador = 0; // Contador para as repetições das animações
int animacao_atual = 0;

#define microfone 28

// Tempo de debouncing
static volatile uint32_t last_joy_button_time = 0;
#define DEBOUNCE_TIME 100000 // 100ms de debounce

int estado_atual_botao_a = 1;  // Estado inicial do botão A
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)
ssd1306_t oled;

// Flags para controlar o estado das ações
bool visual_mode_active = false;
bool sound_mode_active = false;

// Estrutura para os menus
typedef struct {
    const char *title;
    const char *options[3];
} Menu;

// Menus e submenus
Menu main_menu = {"MENU", {"Sensivel", "Estimulo", "Modo Alerta"}};
Menu submenu1 = {"Sensivel", {"Modo Calmo", "Detector ruido", "Voltar"}};
Menu submenu2 = {"Estimulo", {"Visuais", "Sonoros", "Voltar"}};
Menu submenu3 = {"Falante", {"Sentimentos", "Acoes", "Voltar"}};

Menu *current_menu = &main_menu;
int menu_option = 0;

// Função de leitura do eixo Y do joystick (ajustada para corrigir o eixo)
int read_joy_y() {
    adc_select_input(0); // Altere para o canal correto para o eixo Y
    uint16_t y_value = adc_read();
    return y_value;
}

// Função de leitura do eixo X do joystick (caso precise também)
int read_joy_x() {
    adc_select_input(1); // Altere para o canal correto para o eixo X
    uint16_t x_value = adc_read();
    return x_value;
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

#define DEADZONE 500   // Zona morta para evitar leituras imprecisas
#define NAV_DELAY 200

void joy_navigation() {
    static uint32_t last_move_time = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    int y_value = read_joy_y();

    if (current_time - last_move_time > NAV_DELAY) {
        if (y_value < 1000 - DEADZONE) {  // Movimento para cima (corrigido)
            menu_option = (menu_option - 1 + 3) % 3;
            last_move_time = current_time;
            draw_menu();
        } 
        else if (y_value > 3000 + DEADZONE) {  // Movimento para baixo (corrigido)
            menu_option = (menu_option + 1) % 3;
            last_move_time = current_time;
            draw_menu();
        }
    }
}

// Função principal do loop, onde as ações são chamadas de forma não bloqueante

#define MAX_REPETICOES 5     // Número máximo de repetições por animação

bool animacao_em_execucao = false;

void monitoramento_mic();

void button_callback(uint gpio, uint32_t events) {
    static uint32_t last_button_time = 0;
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_button_time > DEBOUNCE_TIME) {
        last_button_time = current_time;

        if (gpio == JOY_BUTTON) {
            // Lógica de navegação nos menus
            if (current_menu == &main_menu) {
                switch (menu_option) {
                    case 0: current_menu = &submenu1;
                        printf("Monitoramento ligado!");
                        monitoramento_mic();
                        break;
                    case 1: current_menu = &submenu2; break;
                    case 2: current_menu = &submenu3; break;
                }
            } else if (current_menu == &submenu2) {
                switch (menu_option) {
                    case 0:
                        printf("Modo visual ativado!\n");
                        visual_mode_active = true;
                        break;
                    case 1:
                        printf("Modo sonoro ativado!\n");
                        sound_mode_active = true;
                        break;
                    case 2:
                        current_menu = &main_menu;
                        break;
                }
            }
            menu_option = 0;
            draw_menu();
        }
    }
}

void main_loop() {
    // Executa animações no modo visual
    if (visual_mode_active && !animacao_em_execucao) {
        animacao_em_execucao = true;
        for (int i = 0; i < MAX_REPETICOES; i++) {
            switch (animacao_atual) {
                case 0: run_visual_mode0(); break;
                case 1: run_visual_mode1(); break;
                case 2: run_visual_mode2(); break;
                case 3: run_visual_mode3(); break;
                case 4: run_visual_mode4(); break;
            }
            sleep_ms(500);
        }
        animacao_atual = (animacao_atual + 1) % 5;
        visual_mode_active = false;
        animacao_em_execucao = false;
    }

    // Executa o som no modo sonoro
    if (sound_mode_active) {
        void play_song();
        printf("Tocando som de 'Clair de Lune'!\n");
          // Toca o som
        sound_mode_active = false;  // Reseta o estado do modo sonoro
    }

    joy_navigation();
}

const uint limiar_1 = 700;     // Limiar para monitoramento
const uint limiar_2 = 3000;    // Limiar para alerta

// Define o intervalo entre amostras
#define INTERVALO_US 100000

void monitoramento_mic() {
    adc_select_input(2);  // Certifica que está lendo o canal correto
    uint16_t mic_value = adc_read(); 

    if (mic_value > limiar_1 && mic_value < limiar_2) {
        ssd1306_draw_string(&oled, "Ruidos suportaveis", 10, 20);
    } else if (mic_value > limiar_2) {
        ssd1306_draw_string(&oled, "Ruidos excessivos", 10, 20);
        ssd1306_draw_string(&oled, "Modo Calmo recomendado", 10, 35);
        gpio_put(LED_R_PIN, 1);
        gpio_put(LED_B_PIN, 0);
        gpio_put(LED_G_PIN, 0);
    }

    ssd1306_send_data(&oled);  // Atualiza o display
    sleep_us(INTERVALO_US);    // Delay para taxa de amostragem correta
}

int main() {
    setup_pio();
    pwm_init_buzzer(BUZZER_PIN); 
    
    setup();
       
    // Configura a interrupção para o botão B e o botão do joystick na mesma função de callback
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(JOY_BUTTON, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    draw_menu();

    
    while (1) {
        joy_navigation();
        main_loop(); // Chama o loop principal
    }

    return 0;
}