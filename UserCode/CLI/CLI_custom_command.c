/**
 * @file CLI_custom_command.c
 * @author TITH (1023515576@qq.com)
 * @brief 在此处添加自定义命令，具体方法可以参考Sample-CLI-commands.c
 * @version 2.0
 * @date 2022-01-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "CLI_custom_command.h"
#include "uart_device.h"
#include "stdbool.h"

/**
 * @brief 自定义命令在这里注册
 * 
 */
void vRegisterCustomCLICommands(void)
{
	CLI_New_Command(testvar, set CLI_test_var, F_Set_CLI_test_var, -1);
	CLI_New_Command(claw_sw, claw open and close, F_claw_sw, 0);
	// CLI_New_Command(claw_steer, claw_steer <0|1> pause width, F_claw_steer, -1);
	// CLI_New_Command(sc, shengjiang zhuazi, F_Set_shangceng, -1);
	CLI_New_Command(joystkl, joystick left_x left_y, F_Set_joystickL, 2);
	CLI_New_Command(joystkr, joystick right_x right_y, F_Set_joystickR, 2);
	CLI_New_Command(kamimadoka, kami.im, F_kamimadoka, 0);
	CLI_New_Command(prnt, print, F_pnt, -1);
	CLI_New_Command(reboot, reboot, F_reboot, 0);
}

//-------------------------------示例命令--------------------------

double CLI_test_var = 2.78;
/**
 * @brief 查看与更改浮点数变量
 * 
 * @param pcWriteBuffer 
 * @param xWriteBufferLen 
 * @param pcCommandString 命令行输入的完整字符串
 * @return BaseType_t pdFALSE 结束执行，pdTrue 循环执行
 */
BaseType_t F_Set_CLI_test_var(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) 
{
	BaseType_t xParameterStringLength;
	const char *pcParameter;
	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);

	if (pcParameter == NULL) // 说明没有带参数
	{
		UD_printf("CLI_test_var == %lg\n", CLI_test_var);
	}
	else
	{
		CLI_test_var = atof(pcParameter);

		UD_printf("Set CLI_test_var = %lg\n", CLI_test_var);
	}
	return pdFALSE; // 结束执行
}

/**
 * @brief 用于演示返回 pdPASS 可以循环执行
 * 
 * @param pcWriteBuffer 
 * @param xWriteBufferLen 
 * @param pcCommandString 命令行输入的完整字符串
 * @return BaseType_t 
 */
BaseType_t F_kamimadoka(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) 
{
	const char poem[] = "Darkness cannot drive out darkness\r\n\r\nonly light can do that\r\n\r\nHate cannot drive out hate\r\n\r\nonly love can do that.\r\n\r\nYour sin , I bear\r\n\r\n";
	const uint16_t length = strlen(poem);
	static uint16_t i = 0;

	UD_printf("%c", poem[i++]);//输出一个字符，然后 i++

	osDelay(50);

	if (i >= length)
	{
		i = 0;
		return pdFALSE; // 结束执行
	}

	return pdPASS; // 循环执行
}

//-------------------------------自定义命令写在下面（记得在上面的vRegisterCustomCLICommands()中注册）--------------------------
// extern int Claw_Steer_pw[2];
// BaseType_t F_claw_steer(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) 
// {
// 	BaseType_t xParameterStringLength;
// 	const char *pcParameter;
// 	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);
// 	int id = 0;

// 	if (pcParameter == NULL) // 说明没有带参数
// 	{
// 		UD_printf("Claw_Steer_pw[0] == %d\n", Claw_Steer_pw[0]);
// 		UD_printf("Claw_Steer_pw[1] == %d\n", Claw_Steer_pw[1]);
// 		return pdFALSE;
// 	}
// 	else
// 	{
// 		id = atoi(pcParameter);
// 	}

// 	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameterStringLength);

// 	if (pcParameter == NULL) // 说明没有带参数
// 	{
// 		UD_printf("Claw_Steer_pw[%d] == %d\n", id, Claw_Steer_pw[id]);
// 	}
// 	else
// 	{
// 		Claw_Steer_pw[id] = atoi(pcParameter);
// 		UD_printf("set Claw_Steer_pw[%d] = %d\n", id, Claw_Steer_pw[id]);
// 	}

// 	return pdFALSE; // 结束执行
// }

extern bool claw_open;
BaseType_t F_claw_sw(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) 
{
	claw_open = !claw_open;
	UD_printf("claw_open:%d\n", claw_open);
	return pdFALSE;
}

extern bool pnt_UC_Debug_Data;
extern bool pnt_RxData;
extern bool pnt_can_rx_count;
extern int fix_counter;
BaseType_t F_pnt(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) 
{
	BaseType_t xParameterStringLength;
	const char *pcParameter;
	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);

	if (strcmp(pcParameter, "uc_debug_data") == 0)
	{
		pnt_UC_Debug_Data = !pnt_UC_Debug_Data;
	}
	else if (strcmp(pcParameter, "fix_counter") == 0)
	{
		UD_printf("fix_counter: %d\n", fix_counter);
	}
	else if (strcmp(pcParameter, "rx_data") == 0)
	{
		pnt_RxData = !pnt_RxData;
	}
	else if (strcmp(pcParameter, "can_rx_count") == 0)
	{
		pnt_can_rx_count = !pnt_can_rx_count;
	}
	else
	{
		UD_printf("pnt uc_debug_data | fix_counter | rx_data | can_rx_count\n");
	}
	
	return pdFAIL;
}

BaseType_t F_reboot(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) 
{
	HAL_NVIC_SystemReset();
	return pdFAIL;
}

// extern double speed_shengjiang;
// extern double speed_zhuazi;
// BaseType_t F_Set_shangceng(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) 
// {
// 	BaseType_t xParameterStringLength;
// 	const char *pcParameter;
// 	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);

// 	// UD_printf("enter\n", speed_zhuazi);

// 	// HAL_Delay(2);

// 	if (pcParameter != NULL) // 说明没有带参数
// 	{
// 		speed_zhuazi = atof(pcParameter);
// 		// UD_printf("z %g ", speed_zhuazi);
// 	}
// 	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameterStringLength);

// 	if (pcParameter != NULL) // 说明没有带参数
// 	{
// 		speed_shengjiang = atof(pcParameter);
// 		// UD_printf("s %g\n", speed_shengjiang);
// 	}
// 	return pdFALSE; // 结束执行
// }

#include "uart_com.h"
extern UC_Data_t RxData;
BaseType_t F_Set_joystickL(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) 
{
	BaseType_t xParameterStringLength;
	const char *pcParameter;
	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);

	if (pcParameter != NULL)
	{
		RxData.Leftx = atof(pcParameter);
	}
	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameterStringLength);

	if (pcParameter != NULL)
	{
		RxData.Lefty = atof(pcParameter);
	}

	return pdFALSE; // 结束执行
}

BaseType_t F_Set_joystickR(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) 
{
	BaseType_t xParameterStringLength;
	const char *pcParameter;
	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);

	if (pcParameter != NULL)
	{
		RxData.Rightx = atof(pcParameter);
	}
	pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameterStringLength);

	if (pcParameter != NULL)
	{
		RxData.Righty = atof(pcParameter);
	}

	return pdFALSE; // 结束执行
}
