#ifndef _FIREMOTOR_H_
#define _FIREMOTOR_H_
#include "stm32f4xx.h"

#define FIRE_RATE_BUF_SIZE  5
#define FIRE_PWM_INPUT_OFF 1450
#define FIRE_PWM_INPUT_ON 1200
#define FIRE_SET_SPEED 49.0f

typedef struct 
{
	float input_L;
	float input_R;
	float out_L;
	float out_R;
	float pid_out_L;
	float pid_out_R;
	uint8_t back_flag;
	
	float frame_period;
	uint8_t add_each_second;
	float add_real;
}Ramp_Start;

typedef struct encoder__
{
    int32_t value;
    int32_t value_last;
    int32_t diff;
    uint8_t buf_count;						 //滤波更新buf用
	int32_t rate_buf[FIRE_RATE_BUF_SIZE];	     //buf，for filter
	int32_t ecd_raw_rate;					 //通过编码器计算得到的速度原始值
	int32_t filter_rate;					 //速度
	
}Fire_Enconder;     //编码器

extern Ramp_Start ramp_start;
extern Fire_Enconder fire_enconderL;
extern Fire_Enconder fire_enconderR;


void fireMotor_stop(void);
void fireMotor_fire(void);
void fireMotor(void);
void ramp_calc(void);
void ramp_init(void);
//void fireMotor_keep(void);
//void enconderSys_Configure(void);
//void getEnconderData_fire(Fire_Enconder *Enc, char TIM_temp);

#endif






