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
	// CLI_New_Command(sc, shengjiang zhuazi, F_Set_shangceng, -1);
	CLI_New_Command(joystkl, joystick left_x left_y, F_Set_joystickL, 2);
	CLI_New_Command(joystkr, joystick right_x right_y, F_Set_joystickR, 2);
	CLI_New_Command(kamimadoka, kami.im, F_kamimadoka, 0);
	CLI_New_Command(pnt_uart_com, print uart com message, F_pnt, 0);
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

extern bool pnt_UC_Debug_Data;
BaseType_t F_pnt(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) 
{
	pnt_UC_Debug_Data = !pnt_UC_Debug_Data;
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