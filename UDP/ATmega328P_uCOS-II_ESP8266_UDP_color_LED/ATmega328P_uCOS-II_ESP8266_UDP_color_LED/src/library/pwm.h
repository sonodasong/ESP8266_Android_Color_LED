#ifndef __PWM_H__
#define __PWM_H__

#include "define.h"

void pwmInit(void);
void pwm0A(uint8 dutyCycle);
void pwm0B(uint8 dutyCycle);
void pwm2A(uint8 dutyCycle);
void pwm2B(uint8 dutyCycle);

#endif
