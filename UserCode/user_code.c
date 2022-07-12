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

bool pnt_UC_Debug_Data = true;

void TestTask(void const *argument);

UC_Data_t RxData = {0};

int can_rx_count = 0;

void StartDefaultTask(void const *argument)
{
	CLI_Init(&huart6);
	UD_SetPrintfDevice(UD_Find(&huart6));

	osThreadDef(testTask, TestTask, osPriorityNormal, 0, 256);
	osThreadCreate(osThread(testTask), NULL);

	//大疆电机初始化
	CANFilterInit(&hcan1);
	hDJI[0].motorType = M3508; // 升降
	DJI_Init();
	UC_Receive_Start(1, &huart3, &RxData);
	// ChassisTaskStart(&RxData);
	UpperTaskStart(&RxData);

	// ADS1256_Init();

	while (1)
	{
		osDelay(10000);
	}
}

void TestTask(void const *argument)
{
	pnt_UC_Debug_Data = false;
	for (;;)
	{
		if (pnt_UC_Debug_Data)
		{
			UC_print_debug_data();
		}

		UD_printf("lx:%5d ly:%5d rx:%5d ry:%5d ", RxData.Leftx, RxData.Lefty, RxData.Rightx, RxData.Righty);
		UD_printf("but:%x\n", RxData.buttons);

		UD_printf("can_rx_count:%d\n", can_rx_count);

		// UD_printf("fix counter: %d\n", fix_counter);
		osDelay(500);
	}
}
