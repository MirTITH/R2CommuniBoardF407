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

typedef enum
{
	block_1,
	block_2,
	block_3,
	block_4,
	block_5
}BlockNum_e;


void UpperTask(void const *argument)
{
	const UC_Data_t *RxData = argument;
	double lift_speed;
	uint32_t PreviousWakeTime = osKernelSysTick();

	for (;;)
	{
		lift_speed = RxData->Righty;

		if (lift_speed > 400)
			lift_speed -= 400;
		else if (lift_speed < -400)
			lift_speed += 400;
		else
			lift_speed = 0;

		speedServo(2*lift_speed, &hDJI[0]);

		CanTransmit_DJI_1234(&hcan1,
						 hDJI[0].speedPID.output,
						 hDJI[1].speedPID.output,
		
						 hDJI[2].speedPID.output,
						 hDJI[3].speedPID.output);
		osDelayUntil(&PreviousWakeTime, 2);
	}	
}

void UpperTaskStart(UC_Data_t *RxData)
{
	osThreadDef(upper, UpperTask, osPriorityBelowNormal, 0, 1024);
	osThreadCreate(osThread(upper), RxData);

	// osThreadDef(upper_test, UpperTestTask, osPriorityBelowNormal, 0, 256);
	// osThreadCreate(osThread(chassis_test), NULL);
}
