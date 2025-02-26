#include "init.h"
#include "lib/ssd1306.h"
#include "hardware/adc.h"

// Inicializa o display
extern ssd1306_t oled;

// Função para configurar os pinos e iniciar o display
void setup() {
    stdio_init_all();

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(LED_G_PIN);  // Inicializa o LED verde
    gpio_set_dir(LED_G_PIN, GPIO_OUT);  // Configura como saída

    gpio_init(LED_B_PIN);  // Inicializa o LED azul
    gpio_set_dir(LED_B_PIN, GPIO_OUT);  // Configura como saída

    gpio_init(LED_R_PIN);  // Inicializa o LED vermelho
    gpio_set_dir(LED_R_PIN, GPIO_OUT);  // Configura como saída

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
}