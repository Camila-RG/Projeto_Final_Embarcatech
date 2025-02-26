#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
 
 // Configuração do pino do buzzer
 #define BUZZER 21
 // configura os pinos dos buzzers para PWM

 const uint16_t WRAP_PERIOD = 4000; //valor máximo do contador - WRAP
 const float PWM_DIVISER = 4.0; //divisor do clock para o PWM
 const uint16_t LED_STEP = 200; //passo de incremento/decremento para o duty cycle do LED
 uint16_t led_level = 200; //nível inicial do pwm (duty cycle)
 
 //função para configurar o módulo PWM
 void pwm_setup()
 {
     gpio_set_function(BUZZER, GPIO_FUNC_PWM); //habilitar o pino GPIO como PWM
 
     uint slice = pwm_gpio_to_slice_num(BUZZER); //obter o canal PWM da GPIO
 
     pwm_set_clkdiv(slice, PWM_DIVISER); //define o divisor de clock do PWM
 
     pwm_set_wrap(slice, WRAP_PERIOD); //definir o valor de wrap
 
     pwm_set_gpio_level(BUZZER, 100); //definir o cico de trabalho (duty cycle) do pwm
 
     pwm_set_enabled(slice, true); //habilita o pwm no slice correspondente
 }
 void beep(uint pin, uint duration_ms) {
     // Obter o slice do PWM associado ao pino
     uint slice_num = pwm_gpio_to_slice_num(pin);
 
     // Configurar o duty cycle para 50% (ativo)
     pwm_set_gpio_level(pin, 2048);
 
     // Temporização
     sleep_ms(duration_ms);
 
     // Desativar o sinal PWM (duty cycle 0)
     pwm_set_gpio_level(pin, 0);
 
     // Pausa entre os beeps
     sleep_ms(100); // Pausa de 100ms
 }