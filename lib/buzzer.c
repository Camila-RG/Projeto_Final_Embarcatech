#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "buzzer.h"

// Definição das frequências das notas musicais (em Hz)
const uint notes[] = {
    262, 294, 330, 349, 392, 440, 494, 523  // C4, D4, E4, F4, G4, A4, B4, C5
};

// Definição das durações das notas (em milissegundos)
const uint note_duration[] = {
    500, 500, 500, 500, 500, 500, 500, 500  // Duração de cada nota
};

// Definição das frequências das notas musicais (em Hz)
const uint notes1[] = {
    262, 294, 330, 349, 392, 440, 494, 523, 440, 392, 349, 330, 294, 262, 523, 494, // C4, D4, E4, F4, G4, A4, B4, C5, A4, G4, F4, E4, D4, C4, C5, B4
    330, 349, 392, 440, 392, 349, 330, 294, 262, 440, 523, 494, 440, 392, 349, 330, // E4, F4, G4, A4, G4, F4, E4, D4, C4, A4, C5, B4, A4, G4, F4, E4
    262, 330, 349, 392, 440, 494, 523, 440, 392, 349, 330, 294, 262, 330, 349, 392  // C4, E4, F4, G4, A4, B4, C5, A4, G4, F4, E4, D4, C4, E4, F4, G4
}; 

// Definição das durações das notas (em milissegundos)
const uint note_duration1[] = {
    300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 600, 600, // Parte 1
    300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 600, 600, // Parte 2
    300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 600, 600  // Parte 3
}; 

// Definição das frequências das notas musicais (em Hz)
const uint notes2[] = {
    440, 466, 493, 523, 523, 493, 440, 440, 493, 523, 523, 493, 440, 440, 493, 523, 523, 493, // A4, A#4, B4, C5, C5, B4, A4
    440, 466, 493, 523, 523, 493, 440, 440, 493, 523, 523, 493, 440, 440, 493, 523, 523, 493, // A4, A#4, B4, C5, C5, B4, A4
    440, 466, 493, 523, 523, 493, 440, 440, 493, 523, 523, 493, 440, 440, 493, 523, 523, 493  // A4, A#4, B4, C5, C5, B4, A4
};

// Definição das durações das notas (em milissegundos)
const uint note_duration2[] = {
    400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, // Tempo para cada nota
    400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, // Tempo para cada nota
    400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400  // Tempo para cada nota
};

// Função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 2.0f);  // Divisão de clock ajustada
    pwm_init(slice_num, &config, true);
}

// Função para tocar uma nota (frequência e duração)
void play_tone(uint pin, uint frequency, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint32_t clock_freq = clock_get_hz(clk_sys);  // Obtendo a frequência do clock
    uint32_t top = clock_freq / (frequency * 2) - 1;  // Cálculo da frequência de PWM

    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(pin, top / 2);  // Ajuste do ciclo de trabalho para controlar a intensidade
    pwm_set_enabled(slice_num, true);  // Ativa o PWM para produzir o som

    sleep_ms(duration_ms);  // Duração da nota

    pwm_set_gpio_level(pin, 0);  // Desativa o som
    pwm_set_enabled(slice_num, false);  // Desativa o PWM
}

// Função para tocar a música (sequência de notas)
void play_song() {
    int num_notes = sizeof(notes) / sizeof(uint);  // Calcula o número de notas
    for (int i = 0; i < num_notes; i++) {
        play_tone(BUZZER_PIN, notes[i], note_duration[i]);
    }
}

void play_song1() {
    int num_notes = sizeof(notes1) / sizeof(uint);  // Calcula o número de notas
    for (int i = 0; i < num_notes; i++) {
        play_tone(BUZZER_PIN, notes1[i], note_duration1[i]);
    }
}

void play_song2() {
    int num_notes = sizeof(notes2) / sizeof(uint);  // Calcula o número de notas
    for (int i = 0; i < num_notes; i++) {
        play_tone(BUZZER_PIN, notes2[i], note_duration2[i]);
    }
}