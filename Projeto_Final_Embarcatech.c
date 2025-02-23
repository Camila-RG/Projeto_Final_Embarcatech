#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "lib/ssd1306.h"
#include "lib/font.h"

// Definição dos pinos
#define I2C_SDA 14
#define I2C_SCL 15
#define JOY_X 26
#define JOY_Y 27
#define JOY_BUTTON 22
#define BACK_BUTTON 6  // Botão para voltar ao menu principal

// Endereço I2C do display
#define SSD1306_ADDR 0x3C

// Inicializa o display
ssd1306_t oled;

// Estrutura para os menus
typedef struct {
    const char *title;
    const char *options[3];
} Menu;

// Menus e submenus
Menu main_menu = {"MENU", {"1Sensivel", "2Estimulo", "3Modo Alerta"}};
Menu submenu1 = {"Sensivel", {"Modo tranquilizante", "Monitoramento de ruidos", "Voltar"}};
Menu submenu2 = {"Estimulo", {"Visuais", "Sonoros", "Voltar"}};
Menu submenu3 = {"Falante", {"Sentimentos", "Acoes", "Voltar"}};

Menu *current_menu = &main_menu;
int menu_option = 0;

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

// Callback da interrupção do botão do joystick
void button_callback(uint gpio, uint32_t events) {
    if (gpio == JOY_BUTTON) {
        if (current_menu == &main_menu) {
            switch (menu_option) {
                case 0: current_menu = &submenu1; break;
                case 1: current_menu = &submenu2; break;
                case 2: current_menu = &submenu3; break;
            }
        } else {
            // Se for um submenu e a opção selecionada for "Voltar", volta ao menu principal
            if (menu_option == 2) { 
                current_menu = &main_menu;
            }
        }
        menu_option = 0;
        draw_menu();
    }
}

// Função de leitura do eixo Y do joystick
int read_joy_y() {
    adc_select_input(1);
    uint16_t y_value = adc_read();
    return y_value;
}

void setup() {
    stdio_init_all();
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&oled, 128, 64, false, SSD1306_ADDR, i2c1);
    ssd1306_config(&oled);
    adc_init();
    adc_gpio_init(JOY_X);
    adc_gpio_init(JOY_Y);
    gpio_init(JOY_BUTTON);
    gpio_set_dir(JOY_BUTTON, GPIO_IN);
    gpio_pull_up(JOY_BUTTON);
    gpio_set_irq_enabled_with_callback(JOY_BUTTON, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    draw_menu();
}

// Navegação joystick
void loop() {
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
    setup();
    while (1) {
        loop();
    }
}
