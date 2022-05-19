#include "main.h"

uint64_t fireFlag = 0;
int16_t last_shift = 0;
int16_t now_shift = 0;

Ramp_Start ramp_start = {0};
Fire_Enconder fire_enconderL = {0};
Fire_Enconder fire_enconderR = {0};
 
void ramp_init(void)
{
	ramp_start.frame_period = 0.001;
	ramp_start.add_each_second = 100;
	ramp_start.add_real = ramp_start.frame_period * ramp_start.add_each_second;
	ramp_start.out_L = FIRE_PWM_INPUT_OFF;
	ramp_start.out_R = FIRE_PWM_INPUT_OFF;
	
	
	ramp_start.pid_out_L = FIRE_PWM_INPUT_ON;
	ramp_start.pid_out_R = FIRE_PWM_INPUT_ON;
	
//	enconderSys_Configure();//编码器初始化
}


void ramp_calc(void)
{

	if((int16_t)ramp_start.out_L != (int16_t)ramp_start.input_L)
	{
		ramp_start.back_flag = 0;
		if(ramp_start.out_L < ramp_start.input_L)
			ramp_start.out_L += ramp_start.add_real;
		else if(ramp_start.out_L > ramp_start.input_L)
			ramp_start.out_L -= ramp_start.add_real;
	}
	else 
	{
		ramp_start.out_L = ramp_start.input_L;
	}
	
	if((int16_t)ramp_start.out_R != (int16_t)ramp_start.input_R)
	{
		ramp_start.back_flag = 0;
		if(ramp_start.out_R < ramp_start.input_R)
			ramp_start.out_R += ramp_start.add_real;
		else if(ramp_start.out_R > ramp_start.input_R)
			ramp_start.out_R -= ramp_start.add_real;
	}
	else 
	{
		ramp_start.out_R = ramp_start.input_R;
	}
	
	
	if(((int16_t)ramp_start.out_L == FIRE_PWM_INPUT_ON) && ((int16_t) ramp_start.out_R == FIRE_PWM_INPUT_ON))
		ramp_start.back_flag = 1;
	if(((int16_t)ramp_start.out_L == FIRE_PWM_INPUT_OFF) && ((int16_t) ramp_start.out_R == FIRE_PWM_INPUT_OFF))
		ramp_start.back_flag = 2;


}

void fireMotor_stop()
{
	ramp_start.input_L = FIRE_PWM_INPUT_OFF;
	ramp_start.input_R = FIRE_PWM_INPUT_OFF;
	TIM1->CCR1 = ramp_start.out_L;
	TIM1->CCR4 = ramp_start.out_R;
}

void fireMotor_fire()
{	
//	if(rc.off_line_flag == 0)
//	{
		ramp_start.input_L = FIRE_PWM_INPUT_ON;
		ramp_start.input_R = FIRE_PWM_INPUT_ON;
		TIM1->CCR1 = ramp_start.out_L;
		TIM1->CCR4 = ramp_start.out_R;
//	}
//	else 
//	{
//		ramp_start.input_L = FIRE_PWM_INPUT_OFF;
//		ramp_start.input_R = FIRE_PWM_INPUT_OFF;
//		TIM1->CCR1 = ramp_start.out_L;
//		TIM1->CCR4 = ramp_start.out_R;
	
//	}


}

void fireMotor()//1000~1450~1900
{	
	now_shift = ((rc.Shift == 1) && (rc.Ctrl == 0));
	if(now_shift == 1 && last_shift == 0)
		fireFlag++;
	last_shift = now_shift;
	
	if(fireFlag % 2 == 1)
	{
		if(ramp_start.back_flag != 1)
			fireMotor_fire();
	}
	else
	{
		fireMotor_stop();
	}
		
//	fireMotor_fire();
	
}

//void fireMotor_keep()
//{
//	out[FIRE_MOTOR_SPEED_L] = Calculate_Current_Value(&pid[FIRE_MOTOR_SPEED_L], FIRE_SET_SPEED, fire_enconderL.filter_rate);
//	out[FIRE_MOTOR_SPEED_R] = Calculate_Current_Value(&pid[FIRE_MOTOR_SPEED_R], FIRE_SET_SPEED, fire_enconderR.filter_rate);
//	
//	ramp_start.pid_out_L = FIRE_PWM_INPUT_ON - out[FIRE_MOTOR_SPEED_L];
//	ramp_start.pid_out_R = FIRE_PWM_INPUT_ON - out[FIRE_MOTOR_SPEED_R];
//	
//	ramp_start.pid_out_L = ramp_start.pid_out_L > 1000 ? ramp_start.pid_out_L : 1000;
//	ramp_start.pid_out_L = ramp_start.pid_out_L < 1450 ? ramp_start.pid_out_L : 1450;
//	ramp_start.pid_out_R = ramp_start.pid_out_R > 1000 ? ramp_start.pid_out_R : 1000;
//	ramp_start.pid_out_R = ramp_start.pid_out_R < 1450 ? ramp_start.pid_out_R : 1450;
//	
//	TIM1->CCR1 = (int16_t)ramp_start.pid_out_L;
//	TIM1->CCR4 = (int16_t)ramp_start.pid_out_R;
//}

//void enconderSys_Configure()
//{
//	GPIO_InitTypeDef gpio;
//	
///******************编码器L初始化*******************************/
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

//	gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
//    gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	gpio.GPIO_Mode = GPIO_Mode_AF;
//	gpio.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOD,&gpio);
//	
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
//	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
//	TIM_Cmd(TIM4,ENABLE);
//	
//	/******************编码器R初始化*******************************/
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

//	gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
//    gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	gpio.GPIO_Mode = GPIO_Mode_AF;
//	gpio.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOH,&gpio);
//	
//	GPIO_PinAFConfig(GPIOH,GPIO_PinSource10,GPIO_AF_TIM5);
//	GPIO_PinAFConfig(GPIOH,GPIO_PinSource11,GPIO_AF_TIM5);
//	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
//	TIM_Cmd(TIM5,ENABLE);
//	
//}

//void getEnconderData_fire(Fire_Enconder *Enc, char TIM_temp)
//{
//	int i = 0;
//	int32_t temp_sum = 0;  
//	
//	Enc->value_last = Enc->value;
//	
//	if(TIM_temp == 'L')
//		Enc->value = TIM4->CNT;
//	else if(TIM_temp == 'R')
//		Enc->value = -TIM5->CNT;//摩擦轮方向相反
//	
//	Enc->diff = Enc->value - Enc->value_last;
//	
//	
//	
//    if(Enc->diff > 32767)
//		Enc->ecd_raw_rate = Enc->diff - 65535;
//    else if( Enc->diff < -32767)
//		Enc->ecd_raw_rate = Enc->diff + 65535;
//	else
//		Enc->ecd_raw_rate = Enc->diff;
//	
//	
//	Enc->rate_buf[Enc->buf_count++] = Enc->ecd_raw_rate;
//    if(Enc->buf_count == FIRE_RATE_BUF_SIZE)
//	{
//		Enc->buf_count = 0;
//	}
//	//计算速度平均值
//	for(i = 0;i < FIRE_RATE_BUF_SIZE; i++)
//	{
//		temp_sum += Enc->rate_buf[i];
//	}
//	Enc->filter_rate = (int32_t)(temp_sum/FIRE_RATE_BUF_SIZE);
//}


