/**
 * @file upper_control.c
 * @author TITH (1023515576@qq.com)
 * @brief 上层控制
 * @version 0.1
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "main.h"
#include "DJI.h"
#include "Caculate.h"
#include "upper_control.h"
#include "cmsis_os.h"
#include "math.h"
#include "uart_device.h"
#include "tim.h"
#include <stdbool.h>

// int Claw_Steer_pw[2] = {0};
bool claw_open = false;

void UpperTask(void const *argument)
{
	const UC_Data_t *RxData = argument;
	double lift_speed;

	uint32_t PreviousWakeTime = osKernelSysTick();

	for (;;)
	{
		// 升降架
		lift_speed = RxData->Righty;

		if (lift_speed > 400)
			lift_speed -= 400;
		else if (lift_speed < -400)
			lift_speed += 400;
		else
			lift_speed = 0;

		speedServo(2 * lift_speed, &hDJI[0]);

		// 爪子夹具
		if ((RxData->buttons & (1 << 4)) || (RxData->buttons & (1 << 3))) // button 3,4
		{
			speedServo(500, &hDJI[2]);
		}
		else if ((RxData->buttons & (1 << 5)) || (RxData->buttons & (1 << 2)))
		{
			speedServo(-500, &hDJI[2]);
		}
		else
		{
			speedServo(0, &hDJI[2]);
		}

		// 爪子旋转
		positionServo(0, &hDJI[1]);

		// 爪子舵机
		if (claw_open)
		{
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 1715);	// 白，PD12
			__HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_2, 501); // 绿,PB15
		}
		else
		{
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 1050);	// 白，PD12
			__HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_2, 1100); // 绿,PB15
		}

		CanTransmit_DJI_1234(&hcan1,
							 hDJI[0].speedPID.output,
							 hDJI[1].speedPID.output,
							 hDJI[2].speedPID.output,
							 hDJI[3].speedPID.output);
		osDelayUntil(&PreviousWakeTime, 2);
	}
}

void UpperTaskInit()
{
	// 升降
	hDJI[0].motorType = M3508;
	
	// 爪子旋转
	hDJI[1].motorType = M2006;

	// 爪子夹具
	hDJI[2].motorType = M3508;
	hDJI[2].speedPID.outputMax = 2000;

	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_2);
}

void UpperTaskStart(UC_Data_t *RxData)
{
	osThreadDef(upper, UpperTask, osPriorityBelowNormal, 0, 1024);
	osThreadCreate(osThread(upper), RxData);

	// osThreadDef(upper_test, UpperTestTask, osPriorityBelowNormal, 0, 256);
	// osThreadCreate(osThread(chassis_test), NULL);
}
