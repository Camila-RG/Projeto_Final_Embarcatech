#include "buzzer.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define BUZZER_PIN 21  // Pino onde o buzzer está conectado

// Função para inicializar o PWM para o buzzer
void init_buzzer() {
    // Configura o pino como saída de PWM
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    
    // Obtém o número do slice para o pino do buzzer
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    
    // Configura a frequência do PWM para o valor base (1 Hz ou o valor inicial desejado)
    pwm_set_clkdiv(slice_num, 125.f);  // Ajuste o divisor conforme necessário
    
    // Define o valor de wrap (o número de ciclos do PWM)
    pwm_set_wrap(slice_num, 1000);  // Ajuste o wrap para a frequência desejada
    
    // Define o ciclo de trabalho inicial (duty cycle)
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 500);  // 50% duty cycle
    
    // Ativa o PWM
    pwm_set_enabled(slice_num, true);
}


// Função para tocar uma nota no buzzer
void play_note(uint32_t note, uint32_t duration) {
    // Configura o pino para PWM
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    
    // Obtenha o número do canal PWM
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    
    // Configura a frequência da nota (em Hz)
    pwm_set_clkdiv(slice_num, 125.f);  // Divisor de clock para controlar a frequência
    
    // Configura o ciclo de trabalho (duty cycle)
    pwm_set_wrap(slice_num, 1000);  // Define o valor de wrap, que vai afetar a frequência
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 500);  // 50% duty cycle
    
    // Define a frequência da nota
    pwm_set_clkdiv_int_frac(slice_num, 125000000 / note, 0);
    
    // Inicia o PWM
    pwm_set_enabled(slice_num, true);
    
    // Espera o tempo da duração da nota
    sleep_ms(duration);
    
    // Desliga o PWM
    pwm_set_enabled(slice_num, false);
}

// Função para tocar uma música calma
void play_calming_music() {
    // Exemplo de notas com suas frequências (Hz) e duração (ms)
    play_note(262, 400); // Dó (C4)
    play_note(294, 400); // Ré (D4)
    play_note(330, 400); // Mi (E4)
    play_note(349, 400); // Fá (F4)
    play_note(392, 400); // Sol (G4)
    play_note(440, 400); // Lá (A4)
    play_note(494, 400); // Si (B4)
    play_note(523, 400); // Dó (C5)
}