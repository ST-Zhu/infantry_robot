#include "main.h"

Feed_set feed_set = {0};

void Shoot()
{
	if((rc.E == 1) && (rc.Ctrl ==0))
	{
		feed_set.feed_speed_max = 800;
	}
	else
	{
		feed_set.feed_speed_max = 500;
	}
	
	
	if((rc.Q == 1) && (rc.Ctrl == 1) && (rc.Shift == 1))
	{
		Set_FeedMotor_Current(-(int16_t)2000);
	}
	else
	{
		if(rc.mouse_l == 1) //射
		{
			feed_set.feed_speed_set = feed_set.feed_speed_max;
		}
		else if(rc.mouse_l == 0) //停
		{
			feed_set.feed_speed_set = 0.0f;
		}

		fireMotorSingleLoopPid_out_update();

	}

}

void fireMotorPidReal_update(void)
{
	feed_set.feed_speed_real = CMFeedEncoder.filter_rate;
}
void fireMotorSingleLoopPid_out_update(void)
{
	fireMotorPidReal_update();//真实值更新
	out[FEED_MOTOR_SINGLE] = Calculate_Current_Value(&pid[FEED_MOTOR_SINGLE], feed_set.feed_speed_set, feed_set.feed_speed_real);
	Set_FeedMotor_Current((int16_t)out[FEED_MOTOR_SINGLE]);
}

void Set_FeedMotor_Current(int16_t feed_motor_iq)//拨弹电机电流发送函数//CAN2发送函数
{
    CanTxMsg tx_message;    
    tx_message.StdId = 0x1FF;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = 0x00;
    tx_message.Data[1] = 0x00;
    tx_message.Data[2] = 0x00;
    tx_message.Data[3] = 0x00;
		tx_message.Data[4] = (unsigned char)(feed_motor_iq >> 8);
    tx_message.Data[5] = (unsigned char)feed_motor_iq;
    tx_message.Data[6] = 0x00;
    tx_message.Data[7] = 0x00;
    CAN_Transmit(CAN2,&tx_message);
}

