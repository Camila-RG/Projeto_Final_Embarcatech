#include "matriz_leds.h"

PIO pio = pio0;
uint sm = 0;
uint led_r = 20;
uint led_g = 20;
uint led_b = 20;

// Função para definir a cor de um pixel específico
void set_pixel_color(uint32_t *pixels, int index, uint32_t color) {
    if (index >= 0 && index < NUM_PIXELS) {
        pixels[index] = color;
    }
}

// Função para exibir um frame da animação na matriz WS2812
void show_frame(const uint32_t frame[25]) {
    for (int row = 4; row >= 0; row--) {  // Inverte a ordem das linhas
        for (int col = 0; col < 5; col++) {
            int index = row * 5 + col;
            uint32_t color = frame[index];

            // Converte de RGB para GRB
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;
            uint32_t corrected_color = (g << 16) | (r << 8) | b;

            pio_sm_put_blocking(pio, sm, corrected_color);
        }
    }
}

void apagar_matriz_leds() {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, 0x000000); // Cor preta (desligado)
    }
}

// Função para percorrer os frames da animação e exibir na matriz de LEDs
void show_animation(const uint32_t animation[][25], int frame_count, int delay_ms) {
    for (int i = 0; i < frame_count; i++) {
        show_frame(animation[i]);
        sleep_ms(delay_ms);
    }
}

// Função para exibir uma cor sólida na matriz de LEDs
void display_static_color(uint32_t color) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, color);
    }
}

void setup_pio(){
    uint offset = pio_add_program(pio, &ws2812_program);
        ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
}

// Exemplo de chamada
void run_visual_mode() {
    static const uint32_t anim1[5][25] = {
        {
            0x00000000, 0x00000000, 0xff000ba2, 0x00000000, 0x00000000, 
            0x00000000, 0xff7341c8, 0xff7341c8, 0xff7341c8, 0x00000000, 
            0xff7341c8, 0xff7341c8, 0xff7341c8, 0xff7341c8, 0xff7341c8, 
            0x00000000, 0xff09104c, 0xff09104c, 0xff09104c, 0x00000000, 
            0x00000000, 0xff09104c, 0xff09104c, 0xff09104c, 0x00000000
            },
            {
            0x00000000, 0x00000000, 0xff000ba2, 0x00000000, 0x00000000, 
            0x00000000, 0xff41c864, 0xff41c864, 0xff41c864, 0x00000000, 
            0xff41c864, 0xff41c864, 0xff41c864, 0xff41c864, 0xff41c864, 
            0x00000000, 0xff09104c, 0xff09104c, 0xff09104c, 0x00000000, 
            0x00000000, 0xff09104c, 0xff09104c, 0xff09104c, 0x00000000
            },
            {
            0x00000000, 0x00000000, 0xff000ba2, 0x00000000, 0x00000000, 
            0x00000000, 0xffc66f3f, 0xffc66f3f, 0xffc66f3f, 0x00000000, 
            0xffc66f3f, 0xffc66f3f, 0xffc66f3f, 0xffc66f3f, 0xffc66f3f, 
            0x00000000, 0xff09104c, 0xff09104c, 0xff09104c, 0x00000000, 
            0x00000000, 0xff09104c, 0xff09104c, 0xff09104c, 0x00000000
            },
            {
            0x00000000, 0x00000000, 0xff000ba2, 0x00000000, 0x00000000, 
            0x00000000, 0xff0edbf1, 0xff0edbf1, 0xff0edbf1, 0x00000000, 
            0xff0edbf1, 0xff0edbf1, 0xff0edbf1, 0xff0edbf1, 0xff0edbf1, 
            0x00000000, 0xff09104c, 0xff09104c, 0xff09104c, 0x00000000, 
            0x00000000, 0xff09104c, 0xff09104c, 0xff09104c, 0x00000000
            },
            {
            0x00000000, 0x00000000, 0xff000ba2, 0x00000000, 0x00000000, 
            0x00000000, 0xff0066f3, 0xff0066f3, 0xff0066f3, 0x00000000, 
            0xff0066f3, 0xff0066f3, 0xff0066f3, 0xff0066f3, 0xff0066f3, 
            0x00000000, 0xff09104c, 0xff09104c, 0xff09104c, 0x00000000, 
            0x00000000, 0xff09104c, 0xff09104c, 0xff09104c, 0x00000000
            }
        };
    show_animation(anim1, 4, 200);
}
