
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define JOYSTICK_X_PIN 26  // GPIO para eixo X
#define JOYSTICK_Y_PIN 27  // GPIO para eixo Y
#define JOYSTICK_PB 22 // GPIO para botão do Joystick
#define Botao_A 5 // GPIO para botão A

#define MENU_OPTION_COUNT 3
#define MENU_X 10
#define MENU_Y 10
#define MENU_HEIGHT 12
#define CURSOR_WIDTH 60
#define CURSOR_HEIGHT 10

int main() {
    gpio_init(JOYSTICK_PB);
    gpio_set_dir(JOYSTICK_PB, GPIO_IN);
    gpio_pull_up(JOYSTICK_PB); 

    gpio_init(Botao_A);
    gpio_set_dir(Botao_A, GPIO_IN);
    gpio_pull_up(Botao_A);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line

    ssd1306_t ssd; // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);  

    uint16_t adc_value_x;
    uint16_t adc_value_y;  
    char str_x[5];  // Buffer para armazenar a string
    char str_y[5];  // Buffer para armazenar a string  

    int menu_cursor = 0; // Índice da opção do menu selecionada
    bool cor = true;

    while (true) {
        adc_select_input(0); // Seleciona o ADC para eixo X. O pino 26 como entrada analógica
        adc_value_x = adc_read();
        adc_select_input(1); // Seleciona o ADC para eixo Y. O pino 27 como entrada analógica
        adc_value_y = adc_read();    

        sprintf(str_x, "%d", adc_value_x);  // Converte o inteiro em string
        sprintf(str_y, "%d", adc_value_y);  // Converte o inteiro em string

        // Atualiza o conteúdo do display com animações
        ssd1306_fill(&ssd, !cor); // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor); // Desenha um retângulo

        // Desenhar o menu
        ssd1306_draw_string(&ssd, "MENU", 50, 0);  // Título do menu

        // Desenhar as opções do menu
        char* menu_options[MENU_OPTION_COUNT] = {"Opção 1", "Opção 2", "Opção 3"};
        for (int i = 0; i < MENU_OPTION_COUNT; i++) {
            ssd1306_draw_string(&ssd, menu_options[i], MENU_X, MENU_Y + i * MENU_HEIGHT);
        }

        // Desenhar o cursor como um retângulo
        ssd1306_rect(&ssd, MENU_X - 10, MENU_Y + menu_cursor * MENU_HEIGHT, CURSOR_WIDTH, CURSOR_HEIGHT, true, true); // Cursor


        // Navegação no menu com o joystick
        if (adc_value_y > 3500 && menu_cursor > 0) {
            menu_cursor--; // Move para cima
        }
        if (adc_value_y < 1000 && menu_cursor < MENU_OPTION_COUNT - 1) {
            menu_cursor++; // Move para baixo
        }

        // Seleção de opção com o botão
        if (gpio_get(JOYSTICK_PB) == 0) {
            // Aqui você pode adicionar o código para executar ações dependendo da opção selecionada
            printf("Opção selecionada: %d\n", menu_cursor + 1); // Para depuração
        }

        ssd1306_send_data(&ssd); // Atualiza o display

        sleep_ms(100);  // Aguarda um pouco para a próxima iteração
    }
}