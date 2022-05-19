#ifndef _FEEDMOTOR_H_
#define _FEEDMOTOR_H_
#include "stm32f4xx.h"

typedef struct Feed_Set
{
	float feed_speed_set;
	float feed_speed_real;
	int16_t feed_speed_max;
}Feed_set;
;

extern Feed_set feed_set;

void Shoot(void);
void fireMotorPidReal_update(void);
void fireMotorSingleLoopPid_out_update(void);
void Set_FeedMotor_Current(int16_t feed_motor_iq);//拨弹电机电流发送函数//CAN2发送函数

#endif




