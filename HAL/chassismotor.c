#include "main.h"
#include "math.h"

Chassis_set chassis_set = {0, 0, 0, 0, 0, 0, 0, 0};
#define ACC_STEP_NUM 335
unsigned short AccStep[ACC_STEP_NUM] = 
{
	0,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,11,11,11,12,12,12,13,13,13,14,14,15,15,16,16,16,17,17,18,18,19,20,20,21,21,22,23,23,24,25,25,26,27,28,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,46,47,48,49,51,52,54,55,57,58,60,61,63,64,66,68,70,72,73,75,77,79,81,83,85,88,90,92,94,97,99,101,104,106,109,111,114,117,119,122,125,128,131,133,136,139,142,146,149,152,155,158,162,165,168,171,175,178,182,185,189,192,196,199,203,207,210,214,218,221,225,229,233,236,240,244,248,251,255,259,262,266,270,274,277,281,285,288,292,296,299,303,307,310,314,317,321,324,327,331,334,337,341,344,347,350,353,357,360,363,366,368,371,374,377,380,382,385,388,390,393,395,398,400,403,405,407,410,412,414,416,418,420,422,424,426,428,430,431,433,435,437,438,440,441,443,444,446,447,449,450,451,453,454,455,456,458,459,460,461,462,463,464,465,466,467,468,469,470,471,471,472,473,474,474,475,476,477,477,478,478,479,480,480,481,481,482,482,483,483,484,484,485,485,486,486,486,487,487,488,488,488,489,489,489,490,490,490,490,491,491,491,492,492,492,492,492,493,493,493,493,494,494,494,494,494,494,495,495,495,495,495,495,495,496,496,496,496,496,496,496,496,497,497
};
//int16_t A[20] = {65,55,45,60,	60,50,40,55,	55,50,35,50,	60,50,40,55,	55,50,35,50};
//float T[20] = {1200.0f,900.0f,800.0f,1100.0f,		1100.0f,800.0f,700.0f,1000.0f,		1000.0f,700.0f,600.0f,900.0f,	1100.0f,800.0f,700.0f,1000.0f,		1000.0f,700.0f,600.0f,900.0f};

void chassis_init(void)
{
	  const static float chassis_x_order_filter[1] = {CHASSIS_ACCEL_X_NUM};
    const static float chassis_y_order_filter[1] = {CHASSIS_ACCEL_Y_NUM};

		    //用一阶滤波代替斜波函数生成
    first_order_filter_init(&chassis_set.chassis_cmd_slow_set_vx, CHASSIS_CONTROL_TIME, chassis_x_order_filter);
    first_order_filter_init(&chassis_set.chassis_cmd_slow_set_vy, CHASSIS_CONTROL_TIME, chassis_y_order_filter);
		
}


void first_order_filter_init(first_order_filter_type_t *first_order_filter_type, float frame_period, const float num[1])
{
    first_order_filter_type->frame_period = frame_period;
    first_order_filter_type->num[0] = num[0];
    first_order_filter_type->input = 0.0f;
    first_order_filter_type->out = 0.0f;
}

void first_order_filter_cali(first_order_filter_type_t *first_order_filter_type, float input)
{
    first_order_filter_type->input = input;
    first_order_filter_type->out =
        first_order_filter_type->num[0] / (first_order_filter_type->num[0] + first_order_filter_type->frame_period) * first_order_filter_type->out + first_order_filter_type->frame_period / (first_order_filter_type->num[0] + first_order_filter_type->frame_period) * first_order_filter_type->input;
}


float deal_W, deal_A, deal_S, deal_D ;
int t_W, t_A, t_S, t_D;
void remote_fifo()
{
	if(rc.W!=0)
		t_W++;
	else
		t_W--;
	
	if(rc.A!=0)
		t_A++;
	else
		t_A--;
	
	if(rc.S!=0)
		t_S++;
	else
		t_S--;
	
	if(rc.D!=0)
		t_D++;
	else
		t_D--;

	t_W = t_W > 334 ? 334 : t_W;
	t_W = t_W < 0 ? 0 : t_W;
	t_A = t_A > 334 ? 334 : t_A;
	t_A = t_A < 0 ? 0 : t_A;
	t_S = t_S > 334 ? 334 : t_S;
	t_S = t_S < 0 ? 0 : t_S;
	t_D = t_D > 334 ? 334 : t_D;
	t_D = t_D < 0 ? 0 : t_D;

	deal_W=AccStep[t_W];
	deal_A=AccStep[t_A];
	deal_S=AccStep[t_S];
	deal_D=AccStep[t_D];
}

uint8_t NIUYAO;
void chassis_set_update()
{
//	float delta;
//	static uint16_t i;
//	static uint64_t j;
//	static uint16_t k;

	chassis_set.cm1_real = CM1Encoder.filter_rate;
	chassis_set.cm2_real = CM2Encoder.filter_rate;
	chassis_set.cm3_real = CM3Encoder.filter_rate;
	chassis_set.cm4_real = CM4Encoder.filter_rate;
	
//	if(((rc.Ctrl ==1) && (rc.Q == 1)) || (feed_state.if_dance == 1))
//	{
//		NIUYAO = 1;
//		j = 0;
//		i = 0;
//	}
//	else if(((rc.Ctrl ==1) && (rc.E == 1)) || (feed_state.if_dance == 2))
//		NIUYAO = 0;
	if((rc.Ctrl ==1) && (rc.Q == 1) && (rc.Shift == 0))
	{
		NIUYAO = 1;
////		j = 0;
////		i = 0;
	}
	else if((rc.Ctrl ==1) && (rc.E == 1))
	{
		NIUYAO = 0;
	}
		
//	if(feed_state.dance_flag == 1)
//	{
//		NIUYAO = 1;
//	}
//	else if(feed_state.dance_flag == 0)
//	{
//		NIUYAO = 0;
//	}
//	if(NIUYAO == 1)
//	{
		//扭腰
//		j++;
//		if(j >= T[i] / 2.0f)
//		{
//			i++;
//			j = 0;
//			k = ~k;
//			if(i >= 20)
//				i = 0;
//		}
//		if(k==0)
//			chassis_set.follow_set = YAW_MECHANICAL_CENTRE + A[i] * sin(2.0f * M_PI / T[i] * j);
//		else
//			chassis_set.follow_set = YAW_MECHANICAL_CENTRE + A[i] * sin(2.0f * M_PI / T[i] * (j + T[i] / 2.0f));

//	}
//	else
//		chassis_set.follow_set = YAW_MECHANICAL_CENTRE;
	if(NIUYAO == 1)
	{
		//小陀螺
		chassis_set.RAD_RC_now = t_angle.yaw_set;
		chassis_set.RAD_RC_diff = chassis_set.RAD_RC_now - chassis_set.RAD_RC_last ;
		chassis_set.RAD_RC_last = chassis_set.RAD_RC_now ;
		
		chassis_set.YAW_CENTRE += (0.3f + chassis_set.RAD_RC_diff );
		chassis_set.follow_set = chassis_set.YAW_CENTRE;
	}
	else
	{
		chassis_set.RAD_RC_last = t_angle.yaw_set;
		if(GMYawEncoder.ecd_angle < (int16_t)(GMYawEncoder.ecd_angle / 360.0f) * 360.0f + chassis_set.YAW_CENTRE_Init + 180.0f)
			chassis_set.follow_set = (int16_t)(GMYawEncoder.ecd_angle / 360.0f) * 360.0f + chassis_set.YAW_CENTRE_Init;
		else if(GMYawEncoder.ecd_angle >= (int16_t)(GMYawEncoder.ecd_angle / 360.0f) * 360.0f + chassis_set.YAW_CENTRE_Init + 180.0f)
			chassis_set.follow_set = (int16_t)(GMYawEncoder.ecd_angle / 360.0f + 1) * 360.0f + chassis_set.YAW_CENTRE_Init;
		chassis_set.YAW_CENTRE = chassis_set.follow_set;
		chassis_set.YAW_DIR = chassis_set.follow_set;
	}
	

	out[CHASSIS_FOLLOW] = Calculate_Current_Value(&pid[CHASSIS_FOLLOW], chassis_set.follow_set, GMYawEncoder.ecd_angle);
	mecanum_Resolving(&chassis_set.cm1_set, &chassis_set.cm2_set, &chassis_set.cm3_set, &chassis_set.cm4_set, -(int)out[CHASSIS_FOLLOW]);

	
	
}
void chassis_out_update()
{
	chassis_set_update();
//	
	out_incr[FR] = pid_incr_calc(&pid_incr[FR], chassis_set.cm1_real, chassis_set.cm1_set);
	out_incr[FL] = pid_incr_calc(&pid_incr[FL], chassis_set.cm2_real, chassis_set.cm2_set);
	out_incr[BL] = pid_incr_calc(&pid_incr[BL], chassis_set.cm3_real, chassis_set.cm3_set);
	out_incr[BR] = pid_incr_calc(&pid_incr[BR], chassis_set.cm4_real, chassis_set.cm4_set);
	if(fly_flag == 0)
		Set_ChassisMotor_Current((int16_t)out_incr[FR], (int16_t)out_incr[FL], (int16_t)out_incr[BL], (int16_t)out_incr[BR]);
	else 
		Set_ChassisMotor_Current(0, 0, 0, 0);
}
#define PI_mu 0.01745f 
float rc_x, rc_y, X, Y;  
void Chassis_fllow(float *x, float *y)
{ 
		float raw_x = *x, raw_y = *y;
		float ecd_angle_chazhi;
		ecd_angle_chazhi=-(GMYawEncoder.ecd_angle - chassis_set.YAW_DIR);//(云台yaw轴的真实值减去编码器的值)
		*x=raw_x*cos(ecd_angle_chazhi*PI_mu)+raw_y*sin(ecd_angle_chazhi*PI_mu); 
		*y=raw_y*cos(ecd_angle_chazhi*PI_mu)-raw_x*sin(ecd_angle_chazhi*PI_mu);
}
void mecanum_Resolving(float *a ,float *b,float *c,float *d,int z)
{
//	float rc_x, rc_y, X, Y;  //云台的坐标系
//	int W = 0,A = 0,S = 0,D = 0;

	 rc_x = rc.R_x * CHASSIS_VX_RC_SEN;
	 rc_y = rc.R_y * CHASSIS_VY_RC_SEN;
	
	if(rc.W == 1 && rc.S == 0 && rc.Ctrl == 0)
	{
		rc_y = -VY_MAX;
	}
	else if(rc.W == 0 && rc.S == 1 && rc.Ctrl == 0)
	{
		rc_y = VY_MAX;
	}
	if(rc.A == 1 && rc.D == 0 && rc.Ctrl == 0)
	{
		rc_x = VX_MAX;
	}
	else if(rc.A == 0 && rc.D == 1 && rc.Ctrl == 0)
	{
		rc_x = -VX_MAX;
	}

    //一阶低通滤波代替斜波作为底盘速度输入	
	 first_order_filter_cali(&chassis_set.chassis_cmd_slow_set_vx, rc_x);
   first_order_filter_cali(&chassis_set.chassis_cmd_slow_set_vy, rc_y);
 
//	if((rc.W == 0 && rc.S == 0) && ((fabs((float)rc.R_y)) <= 2.0f))
//	{
//		chassis_set.chassis_cmd_slow_set_vy.out = 0.0f;
//	}
//	
//	if((rc.A == 0 && rc.D == 0) && ((fabs((float)rc.R_x)) <= 2.0f))
//	{
//		chassis_set.chassis_cmd_slow_set_vx.out = 0.0f;
//	}
	 X = chassis_set.chassis_cmd_slow_set_vx.out;
	 Y = chassis_set.chassis_cmd_slow_set_vy.out;
	 Chassis_fllow(&X, &Y);

	*a = Y - X - z;
	*b = -Y - X - z;
	*c = -Y + X - z;
	*d = Y + X - z;    //遥控器无法控制	
	 
}

void Set_ChassisMotor_Current(int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq)		//地盘电机电流发送函数
{
    CanTxMsg tx_message;
    tx_message.StdId = 0x200;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = (uint8_t)(cm1_iq >> 8);
    tx_message.Data[1] = (uint8_t)cm1_iq;
    tx_message.Data[2] = (uint8_t)(cm2_iq >> 8);
    tx_message.Data[3] = (uint8_t)cm2_iq;
    tx_message.Data[4] = (uint8_t)(cm3_iq >> 8);
    tx_message.Data[5] = (uint8_t)cm3_iq;
    tx_message.Data[6] = (uint8_t)(cm4_iq >> 8);
    tx_message.Data[7] = (uint8_t)cm4_iq;
    CAN_Transmit(CAN1,&tx_message);
}
