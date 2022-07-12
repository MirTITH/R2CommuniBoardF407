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
#include "vesc.h"
#include "uart_com.h"
#include "string.h"
#include <stdbool.h>
#include "chassis_control.h"
#include "upper_control.h"

extern int fix_counter;

void TestTask(void const *argument);

UC_Data_t RxData = {0};

int can_rx_count = 0;

void StartDefaultTask(void const *argument)
{
	osDelay(500);

	CLI_Init(&huart6);
	UD_SetPrintfDevice(UD_Find(&huart6));

	osThreadDef(testTask, TestTask, osPriorityNormal, 0, 256);
	osThreadCreate(osThread(testTask), NULL);

	DJI_PID_Init();

	UpperTaskInit();

	DJI_motorType_Init();
	CANFilterInit(&hcan1);

	UC_Receive_Start(1, &huart3, &RxData);
	UpperTaskStart(&RxData);

	// ChassisTaskStart(&RxData);
	// ADS1256_Init();

	while (1)
	{
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
