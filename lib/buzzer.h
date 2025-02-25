#ifndef BUZZER_H
#define BUZZER_H

#include <stdio.h>
#include "hardware/pwm.h"
#include "hardware/clocks.h"

void init_buzzer();
void play_note(uint32_t note, uint32_t duration);
void play_calming_music();

#endif // BUZZER_H