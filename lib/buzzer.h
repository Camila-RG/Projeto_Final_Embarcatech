#ifndef BUZZER_H
#define BUZZER_H

// Pino do buzzer
#define BUZZER_PIN 21

void pwm_init_buzzer(uint pin);
void play_tone(uint pin, uint frequency, uint duration_ms);
void play_song();

#endif // BUZZER_H