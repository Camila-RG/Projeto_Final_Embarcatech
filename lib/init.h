#ifndef INIT_H
#define INIT_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

// Definição dos pinos
#define I2C_SDA 14
#define I2C_SCL 15
#define JOY_X 26
#define JOY_Y 27
#define JOY_BUTTON 22
#define BUTTON_A 5 
#define LED_G_PIN 11
#define LED_B_PIN 12
#define LED_R_PIN 13 
#define BUZZER_PIN 21

// Endereço I2C do display
#define SSD1306_ADDR 0x3C

void setup();

#endif // INIT_H