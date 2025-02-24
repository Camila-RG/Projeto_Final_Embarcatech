#ifndef BUZZER_H
#define BUZZER_H

#include <stdio.h>
#include "hardware/pwm.h"
#include "hardware/clocks.h"

void pwm_init_buzzer(uint pin);
void play_tone(uint pin, uint frequency, uint duration_ms);
void play_star_wars(uint pin);

#endif // BUZZER_H