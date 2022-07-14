/**
 * @file user_code.c
 * @author TITH (1023515576@qq.com)
 * @brief 用户代码主文件
 * @version 0.1
 * @date 2022-01-12
 *
 * @copyright Copyright (c) 2022
 *
 */

/*
osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
*/

#include "user_code.h"
#include "CLI.h"
#include "uart_device.h"
#include <math.h>
#include <stdio.h>

#include "DJI.h"
#include "wtr_can.h"
#include "Caculate.h"
#include "uart_com.h"
#include "string.h"
#include <stdbool.h>
#include "upper_control.h"
#include "HWT101CT.h"

void TestTask(void const *argument);

UC_Data_t RxData = {0};

int can_rx_count = 0;

HWT_Handle_t hhwt1;

void StartDefaultTask(void const *argument)
{
	osDelay(500);

	CLI_Init(&huart6);
	UD_SetPrintfDevice(UD_Find(&huart6));

	osThreadDef(testTask, TestTask, osPriorityNormal, 0, 256);
	osThreadCreate(osThread(testTask), NULL);

	// HWT_Init(&hhwt1, &huart3);

	DJI_PID_Init();

	UpperTaskInit();

	DJI_motorType_Init();
	CANFilterInit(&hcan1);


	UC_Receive_Start(1, &huart3, &RxData);
	UpperTaskStart(&RxData);

	// ADS1256_Init();

	while (1)
	{
		// UD_printf("YAW:%6d Wz_Raw:%6d Wz_C:%6d V:%6d\n", hhwt1.raw_data.Yaw, hhwt1.raw_data.Wz_Raw, hhwt1.raw_data.Wz_Corrected, hhwt1.raw_data.V);
		// UD_printf("YAW:%f Wz:%f YAW_rad:%f Wz_rad:%f \n", HWT_ReadYawDeg(&hhwt1), HWT_ReadWzDeg(&hhwt1), HWT_ReadYawRad(&hhwt1), HWT_ReadWzRad(&hhwt1));
		osDelay(1000);
	}
}

bool pnt_UC_Debug_Data = false;
bool pnt_RxData = false;
bool pnt_can_rx_count = false;
void TestTask(void const *argument)
{
	for (;;)
	{
		if (pnt_UC_Debug_Data)
		{
			UC_print_debug_data();
		}

		if (pnt_RxData)
		{
			UD_printf("lx:%5d ly:%5d rx:%5d ry:%5d ", RxData.Leftx, RxData.Lefty, RxData.Rightx, RxData.Righty);
			UD_printf("but:%x\n", RxData.buttons);
		}

		if(pnt_can_rx_count)
		{
			UD_printf("can_rx_count:%d\n", can_rx_count);
		}

		osDelay(500);
	}
}
