#ifndef _CHASSISMOTOR_H_
#define _CHASSISMOTOR_H_

#include "delay.h"
#define keypad_K 2u
#define remote_K 1.6f
#define limit 1012u

#define  CHASSIS_VX_RC_SEN 1.666f			//x��
#define CHASSIS_VY_RC_SEN 1.610f			//y��

#define VX_MAX 1100.0f
#define VY_MAX 1063.0f


#define CHASSIS_CONTROL_TIME_MS 1u

#ifndef PI
#define PI 3.14159265358979f
#endif

//����������Ƽ�� 0.001s
#define CHASSIS_CONTROL_TIME 0.001f
#define CHASSIS_ACCEL_X_NUM 0.33333333f
#define CHASSIS_ACCEL_Y_NUM 0.39999999f
#define CHASSIS_WZ_SET_SCALE 0.0f
#define MOTOR_DISTANCE_TO_CENTER 0.35f

//m3508ת���ɵ����ٶ�(m/s)�ı������������� ����Ϊ���ܻ������Ҫ��������
#define M3508_MOTOR_RPM_TO_VECTOR 0.000415809748903494517209f
#define CHASSIS_MOTOR_RPM_TO_VECTOR_SEN M3508_MOTOR_RPM_TO_VECTOR


void chassis_set_update(void);
void chassis_out_update(void);
void mecanum_Resolving(float *a ,float *b,float *c,float *d,int z);    //���׽���
void Chassis_fllow(float *x, float *y);
void remote_fifo(void);

// ���ڵ���һ���˲�����
typedef __packed struct
{
    float input;        //��������
    float out;          //�˲����������
    float num[1];       //�˲�����
    float frame_period; //�˲���ʱ���� ��λ s
} first_order_filter_type_t;

typedef struct Chassis_set
{
	
	float cm1_set;
	float cm2_set;
	float cm3_set;
	float cm4_set;
	
	float cm1_real;
	float cm2_real;
	float cm3_real;
	float cm4_real;
	float follow_set;
	float follow_real;
	float YAW_CENTRE;
	float YAW_CENTRE_Init;
	float YAW_DIR;
	
	float RAD_RC_last;
	float RAD_RC_now;
	float RAD_RC_diff;
	first_order_filter_type_t chassis_cmd_slow_set_vx;
	first_order_filter_type_t chassis_cmd_slow_set_vy;
}Chassis_set;
extern Chassis_set chassis_set;
void Set_ChassisMotor_Current(int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq);
void chassis_init(void);
void first_order_filter_init(first_order_filter_type_t *first_order_filter_type, float frame_period, const float num[1]);
void first_order_filter_cali(first_order_filter_type_t *first_order_filter_type, float input);
void Set_ChassisMotor_Current(int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq);


//volatile Chassis_set chassis_set;
#endif
