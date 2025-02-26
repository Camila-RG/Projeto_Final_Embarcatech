#ifndef MATRIZ_LEDS_H
#define MATRIZ_LEDS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "lib/init.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7

extern PIO pio;
extern uint sm;
extern uint led_r;
extern uint led_g;
extern uint led_b;

void apagar_matriz_leds();
void set_pixel_color(uint32_t *pixels, int index, uint32_t color);
void show_frame(const uint32_t frame[25]);
void show_animation(const uint32_t animation[][25], int frame_count, int delay_ms);
void display_static_color(uint32_t color);
void run_visual_mode();
void setup_pio();

void run_visual_mode0();
void run_visual_mode1();
void run_visual_mode2();
void run_visual_mode3();
void run_visual_mode4();

void run_visual_mode00();
void run_visual_mode01();
void run_visual_mode02();
void run_visual_mode03();


#endif // MATRIZ_LEDS_H