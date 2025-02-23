#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "lib/ssd1306.h"  // Certifique-se de incluir o cabeçalho correto
#include "lib/font.h"     // Inclua o cabeçalho da fonte (caso use fontes externas)

// Definição dos pinos
#define I2C_SDA 14
#define I2C_SCL 15
#define JOY_X 26
#define JOY_Y 27
#define JOY_BUTTON 22

// Endereço I2C do display
#define SSD1306_ADDR 0x3C

// Inicializa o display
ssd1306_t oled;

// Variáveis do menu
#define menu_size 3
int menu_option = 0;
const char *menu_items[] = {
    "1Sensivel",
    "2Estimulo",
    "3Modo Alerta"
};

// Função para desenhar o menu
void draw_menu() {
    // Limpa a tela
    ssd1306_fill(&oled, 0);
    ssd1306_rect(&oled, 0, 0, 127, 63, 1, 0); // Borda do display (não preenchido)
    ssd1306_rect(&oled, 10, 0, 127, 50, 1, 0); // Borda do menu (não preenchido)

    // Desenha o título do menu
    ssd1306_draw_string(&oled, "MENU", 48, 2); // Título do menu (ajustado para 48 para centralizar melhor)

    // Desenha as opções do menu
    for (int i = 0; i < menu_size; i++) {
        if (i == menu_option) {
            // Desenha o ">" antes da opção selecionada
            ssd1306_draw_string(&oled, "O", 10, i * 10 + 16);
            ssd1306_command(&oled, SET_NORM_INV);  // Inverte as cores para o texto da opção
            ssd1306_draw_string(&oled, menu_items[i], 24, i * 10 + 16); // Desenha a opção selecionada com cores invertidas
            ssd1306_command(&oled, SET_NORM_INV);  // Restaura as cores normais
        } else {
            // Desenha as opções não selecionadas
            ssd1306_draw_string(&oled, menu_items[i], 24, i * 10 + 16);
        }
    }

    // Atualiza o display
    ssd1306_send_data(&oled);
}

void setup() {
    stdio_init_all();

    // Inicializa I2C
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o display
    ssd1306_init(&oled, 128, 64, false, SSD1306_ADDR, i2c1);  // Corrija a largura e altura aqui
    ssd1306_config(&oled);

    // Inicializa ADC para o joystick
    adc_init();
    adc_gpio_init(JOY_X);
    adc_gpio_init(JOY_Y);

    // Inicializa botão do joystick
    gpio_init(JOY_BUTTON);
    gpio_set_dir(JOY_BUTTON, GPIO_IN);
    gpio_pull_up(JOY_BUTTON);
}

void loop() {
    // Leitura do eixo Y do joystick
    adc_select_input(1);
    uint16_t y_value = adc_read();

    // Leitura do botão do joystick
    bool button_pressed = !gpio_get(JOY_BUTTON);

    // Navegação no menu
    if (y_value < 1000) { // Para baixo
        menu_option = (menu_option + 1) % menu_size;
        draw_menu();
        sleep_ms(200); // Debounce
    } else if (y_value > 3000) { // Para cima
        menu_option = (menu_option - 1 + menu_size) % menu_size;
        draw_menu();
        sleep_ms(200); // Debounce
    }

    // Seleção de opção
    if (button_pressed) {
        ssd1306_fill(&oled, 0);
        ssd1306_draw_string(&oled, "Selecionado:", 10, 20);
        ssd1306_draw_string(&oled, menu_items[menu_option], 10, 35);
        ssd1306_send_data(&oled);
        printf("Selecionado: %s\n", menu_items[menu_option]);
        sleep_ms(1000); // Aguarda antes de voltar ao menu
        draw_menu();
    }
}

int main() {
    setup();
    draw_menu();
    while (1) {
        loop();
    }
}
