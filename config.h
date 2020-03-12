#ifndef CONFIG_H
#define	CONFIG_H


#include <xc.h> 
#include "Global_Definitions.h"
#include <math.h>
#include <stdbool.h>


void configPIC();
void wait_in_ms(unsigned int time);
void all_ok();
void PWM_Init();
int setPeriodTo(unsigned long FPWM);
void SetDuty1CycleTo(float Duty_cycle, int Period);
void SetDuty2CycleTo(float Duty_cycle, int Period);
void motorder_avanzar(int value);
void motorizq_avanzar(int value);
void enable(int x);
unsigned int ADC_Read (unsigned int channel);
void motorder_atras(int value);
void motorizq_atras(int value);
long map(long x, long in_min, long in_max, long out_min, long out_max);
int act_position(long van0, long van1, long van2, long van3, long van4, long van5);
void freno (bool left, bool right, int value);
void Motor(int left, int right);
void Trigger_Pulse_10us();
void reversa(int left, int right);
void Trigger_Pulse2_10us();
void Trigger_Pulse3_10us();
#endif	

