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

// Contador para as repetições das animações
int animacao_contador = 0; 
int animacao_atual = 0;
int emocao_atual = 0;

const uint limiar_1 = 700;     // Limiar para monitoramento
const uint limiar_2 = 2800;    // Limiar para alerta

#define microfone 28
#define DEADZONE 500   // Zona morta para evitar leituras imprecisas
#define NAV_DELAY 200
#define INTERVALO_US 100000 // Define o intervalo entre amostras

// Tempo de debouncing
static volatile uint32_t last_joy_button_time = 0;
#define DEBOUNCE_TIME 100000 // 100ms de debounce
int estado_atual_botao_a = 1;  // Estado inicial do botão A
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)
ssd1306_t oled;

// Flags para controlar o estado das ações
bool visual_mode_active = false;
bool visual_mode_active1 = false;
bool sound_mode_active = false;

#define MAX_REPETICOES 5     // Número máximo de repetições por animação
bool sound_mode_active2 = false;
bool sound_mode_active1 = false;
bool animacao_em_execucao = false;
bool monitor_ative = false;
bool modo_ajuda = false;
bool emocao_em_execucao = false;
bool modo_sentimentos_ativo = false;  // Flag para o modo Sentimentos


// Estrutura para os menus
typedef struct {
    const char *title;
    const char *options[3];
} Menu;

// Menus e submenus
Menu main_menu = {"MENU", {"Sensivel", "Estimulo", "Modo Alerta"}};
Menu submenu1 = {"Sensivel", {"Modo Calmo", "Detector ruido", "Voltar"}};
Menu submenu2 = {"Estimulo", {"Visual", "Sonoro", "Voltar"}};
Menu submenu3 = {"Falante", {"Emocoes", "AJUDA", "Voltar"}};

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


void monitoramento_mic();

void modoajuda(){
    gpio_put(LED_B_PIN, 0);
    gpio_put(LED_R_PIN, 1);
    gpio_put(LED_G_PIN, 0);
    for(int a=0; a < 7; a++){
        gpio_put(LED_R_PIN, 0);
        sleep_ms(500);
        gpio_put(LED_R_PIN, 1);
    }
}

void button_callback(uint gpio, uint32_t events) {
    static uint32_t last_button_time = 0;
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_button_time > DEBOUNCE_TIME) {
        last_button_time = current_time;

        if (gpio == JOY_BUTTON) {
            // Lógica de navegação no menu
            if (current_menu == &main_menu) {
                switch (menu_option) {
                    case 0: current_menu = &submenu1; break;
                    case 1: current_menu = &submenu2; break;
                    case 2: current_menu = &submenu3; break;
                }
            } else if (current_menu == &submenu1) {
                switch (menu_option) {
                    case 0:
                        printf("Modo Calmo ativado!\n");
                        sound_mode_active2 = true;
                        break;
                    case 1:
                        printf("Monitoramento de ruido ativado\n");
                        monitor_ative = true; // Ativar corretamente
                        break;
                    case 2:
                        current_menu = &main_menu;
                        break;
                }
            } else if (current_menu == &submenu2) {
                switch (menu_option) {
                    case 0: 
                        visual_mode_active = true;
                        printf("Modo Visual ativo!\n");
                     break;
                    case 1: 
                        sound_mode_active1 = true;
                        printf("Modo Sonoro ativo!\n");
                    break;
                    case 2: current_menu = &main_menu; break;
                }
            } else if (current_menu == &submenu3) {
                switch (menu_option) {
                    case 0: 
                        modo_sentimentos_ativo = true;
                        printf("Modo Visual ativo!\n");
                     break;
                    case 1: 
                        modo_ajuda = false;
                        printf("Modo Sonoro ativo!\n");
                    break;
                    case 2: current_menu = &main_menu; break;
                    
                }
                // Alterna a animação de emoção
            if (modo_sentimentos_ativo) {
                emocao_atual = (emocao_atual + 1) % 5;  // Alterna entre 0 a 4
            }

            }
            menu_option = 0;
            draw_menu();
        }

        if (gpio_get(BUTTON_A) == 0) {
            printf("Monitoramento encerrado!\n");
            monitor_ative = false;
            current_menu = &main_menu;  
            menu_option = 0;
        }
    }
}

void main_loop() {
    joy_navigation();
    if (visual_mode_active && !animacao_em_execucao) {
        animacao_em_execucao = true;
        switch (animacao_atual) {
            case 0: run_visual_mode0();
            animacao_em_execucao = false; 
            break;
            case 1: run_visual_mode1();
            animacao_em_execucao = false; 
            break;
            case 2: run_visual_mode2(); 
            animacao_em_execucao = false; 
            break;
            case 3: run_visual_mode3(); 
            animacao_em_execucao = false; 
            break;
            case 4: run_visual_mode4(); 
            animacao_em_execucao = false; 
            break;
        }
        sleep_ms(500);  // Tempo para a animação ser visível antes de alternar
        animacao_atual = (animacao_atual + 1) % 5;  // Alterna a animação
        animacao_em_execucao = false;  // Permite que a animação mude novamente no próximo ciclo
    
    }  // Exibir as animações de emoções uma por vez a cada pressão de botão
    if (modo_sentimentos_ativo) {  // Verifique se o modo Sentimentos está ativo
        switch (emocao_atual) {
            case 0: 
                run_visual_mode00(); 
                break;  // Animação 0
            case 1: 
                run_visual_mode01(); 
                break;  // Animação 1
            case 2: 
                run_visual_mode02(); 
                break;  // Animação 2
            case 3: 
                run_visual_mode03(); 
                break;  // Animação 3
            case 4: 
                run_visual_mode04(); 
                break;  // Animação 4
        }
        
        // Depois de exibir uma animação, desativamos o modo de sentimentos para aguardar a próxima pressão do botão
        modo_sentimentos_ativo = false;
    }
    if (sound_mode_active2) {
        printf("Tocando 'Clair de Lune'!\n");
        play_song2();
        sound_mode_active = false;  
    }
    if (sound_mode_active1) {
        printf("Tocando música!\n");
        play_song1();
        sound_mode_active1 = false;  
    }

    if (sound_mode_active) {
        printf("Tocando música!\n");
        play_song();
        sound_mode_active = false;  
    }
    if (modo_ajuda) {
        modoajuda();
        printf("Modo ajuda ativo!\n");
        play_song();
        sound_mode_active = false;  
    }

    if (monitor_ative) {  // Somente chama a função se estiver ativado
        monitoramento_mic();
    }

}


void monitoramento_mic() {
    if (!monitor_ative) {
        return;
    }

    static uint32_t last_sample_time = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    while (monitor_ative) {
        if (current_time - last_sample_time >= 100) {
            last_sample_time = current_time;
            adc_select_input(2);  
            uint16_t mic_value = adc_read();
            ssd1306_fill(&oled, 0);

            if (mic_value > limiar_1 && mic_value < limiar_2) {
                ssd1306_draw_string(&oled, "Ruídos suportáveis", 10, 20);
                gpio_put(LED_G_PIN, 1);
                gpio_put(LED_R_PIN, 0);
                gpio_put(LED_B_PIN, 0);
            } else if (mic_value > limiar_2) {
                ssd1306_draw_string(&oled, "Ruído excessivo!", 10, 20);
                ssd1306_draw_string(&oled, "Modo Calmo recomendado", 10, 35);
                gpio_put(LED_R_PIN, 1);
                gpio_put(LED_B_PIN, 0);
                gpio_put(LED_G_PIN, 0);
            } else {
                gpio_put(LED_R_PIN, 0);
                gpio_put(LED_B_PIN, 0);
                gpio_put(LED_G_PIN, 0);
            }

            ssd1306_send_data(&oled);
        }

        // Se o botão A for pressionado, sai do monitoramento
        if (gpio_get(BUTTON_A) == 0) {
            printf("Monitoramento encerrado!\n");
            monitor_ative = false;
            current_menu = &main_menu;  
            menu_option = 0;
            gpio_put(LED_B_PIN, 0);
            gpio_put(LED_G_PIN, 0);
            gpio_put(LED_R_PIN, 0);
        }

        sleep_ms(100);
    }

    // Agora o menu será desenhado após a execução
    draw_menu();
}


int main() {
    setup_pio();
    pwm_init_buzzer(BUZZER_PIN); 
    
    setup();
       
    // Configura a interrupção para o botão B e o botão do joystick na mesma função de callback
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(JOY_BUTTON, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    draw_menu();
    
    while (1) {
        joy_navigation();
        main_loop(); // Chama o loop principal
    }

    return 0;
}