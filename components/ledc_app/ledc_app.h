#ifndef LEDC_APP_H
#define LEDC_APP_H
#include "stdint.h"


void LedC_Init(long freq);
void LedC_Add_Pin(int pin, int channel);
void LedC_Set_Duty(int channel, uint32_t duty);

#endif
