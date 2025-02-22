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

#define MENU_X 10
#define MENU_Y 20
#define MENU_WIDTH 128
#define MENU_HEIGHT 16
#define CURSOR_WIDTH 10
#define CURSOR_HEIGHT 10

int main()
{
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
    ssd1306_fill(&ssd, false); // Limpa o display
    ssd1306_send_data(&ssd);

    // Inicializando os ADCs para leitura do Joystick
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    uint16_t adc_value_x;
    uint16_t adc_value_y;
    char str_x[5];
    char str_y[5];

    bool cor = true;
    int menu_cursor = 0;  // Índice do item selecionado

    while (true)
    {
        adc_select_input(0); // Seleciona o ADC para eixo X
        adc_value_x = adc_read();
        adc_select_input(1); // Seleciona o ADC para eixo Y
        adc_value_y = adc_read();
        sprintf(str_x, "%d", adc_value_x);  // Converte o valor de X em string
        sprintf(str_y, "%d", adc_value_y);  // Converte o valor de Y em string

        // Limpa o display e desenha o menu
        ssd1306_fill(&ssd, false); // Limpa a tela
        ssd1306_send_data(&ssd);

        // Desenha o título
        ssd1306_draw_string(&ssd, "Menu Principal", 30, 0);

        // Desenha as opções do menu
        ssd1306_draw_string(&ssd, "1. Opcao 1", MENU_X, MENU_Y + (MENU_HEIGHT * 0));  // Primeira opção
        ssd1306_draw_string(&ssd, "2. Opcao 2", MENU_X, MENU_Y + (MENU_HEIGHT * 1));  // Segunda opção
        ssd1306_draw_string(&ssd, "3. Opcao 3", MENU_X, MENU_Y + (MENU_HEIGHT * 2));  // Terceira opção

        // Desenha o cursor (apenas a borda)
        ssd1306_rect(&ssd, MENU_X - 10, MENU_Y + menu_cursor * MENU_HEIGHT, CURSOR_WIDTH, CURSOR_HEIGHT, true, false); // Cursor vazio (bordas)

        // Atualiza o display
        ssd1306_send_data(&ssd);

        // Lógica para navegação no menu usando o joystick
        if (adc_value_y < 2000) // Joystick para cima
        {
            menu_cursor--;
            if (menu_cursor < 0)
                menu_cursor = 2; // Vai para a última opção se subir no topo
            sleep_ms(150); // Debounce do joystick
        }
        else if (adc_value_y > 3000) // Joystick para baixo
        {
            menu_cursor++;
            if (menu_cursor > 2)
                menu_cursor = 0; // Vai para a primeira opção se descer no fundo
            sleep_ms(150); // Debounce do joystick
        }

        // Ação do botão (apertando o joystick)
        if (!gpio_get(JOYSTICK_PB)) // Se o botão for pressionado
        {
            // Adicione aqui a lógica de ação ao pressionar o botão
            // Exemplo: Display da opção selecionada
            char option[20];
            snprintf(option, sizeof(option), "Opção %d Selecionada", menu_cursor + 1);
            ssd1306_fill(&ssd, false); // Limpa a tela
            ssd1306_draw_string(&ssd, option, 10, 30); // Exibe a opção selecionada
            ssd1306_send_data(&ssd);
            sleep_ms(1000); // Mostra por um tempo e volta ao menu
        }

        sleep_ms(50); // Delay para evitar leitura excessiva
    }
}