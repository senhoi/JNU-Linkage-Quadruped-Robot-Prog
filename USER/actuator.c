#include "actuator.h"

uint8_t devIDList[ACTR_DEV_NUM] = {21, 78, 88, 8, 3, 33, 55, 20, 31, 2, 188, 1};
//LF-RB-RF-LB
float actrAngle[12] = {0.0f, 10.0f, 3.0f, 0.0f, -10.0f, -3.0f, 0.0f, -10.0f, -3.0f, 0.0f, 10.0f, 3.0f};

const struct actuator_limit
{
	float Joint0_Max_PosLimit;
	float Joint0_Min_PosLimit;
	float Joint1_Max_PosLimit;
	float Joint1_Min_PosLimit;
	float Joint2_Max_PosLimit;
	float Joint2_Min_PosLimit;

	// float Joint0_Max_VelLimit;
	// float Joint0_Min_VelLimit;
	// float Joint1_Max_VelLimit;
	// float Joint1_Min_VelLimit;
	// float Joint2_Max_VelLimit;
	// float Joint2_Min_VelLimit;

	// float Joint0_Max_CurrLimit;
	// float Joint0_Min_CurrLimit;
	// float Joint1_Max_CurrLimit;
	// float Joint1_Min_CurrLimit;
	// float Joint2_Max_CurrLimit;
	// float Joint2_Min_CurrLimit;
} ACTUATOR_LIMIT = {2.5f, -2.5f, 15.0f, -15.0f, 7.0f, -7.0f};

int CheckActrTargetPosVal(void)
{
	extern float actrAngle[12];

	for (int i = 0; i < 12; i++)
	{
		switch (i)
		{
		case 0:
		case 3:
		case 6:
		case 9:
			if (actrAngle[i] <= ACTUATOR_LIMIT.Joint0_Min_PosLimit || actrAngle[i] >= ACTUATOR_LIMIT.Joint0_Max_PosLimit)
				return 0;
			break;
		case 1:
		case 4:
		case 7:
		case 10:
			if (actrAngle[i] <= ACTUATOR_LIMIT.Joint1_Min_PosLimit || actrAngle[i] >= ACTUATOR_LIMIT.Joint1_Max_PosLimit)
				return 0;
			break;
		case 2:
		case 5:
		case 8:
		case 11:
			if (actrAngle[i] <= ACTUATOR_LIMIT.Joint2_Min_PosLimit || actrAngle[i] >= ACTUATOR_LIMIT.Joint2_Max_PosLimit)
				return 0;
			break;
		}
	}
	return 1;
}

/**
 * 函数定义: PrintActrState
 * 描    述：获取执行器的全部参数信息，将跟新的参数值保存再执行器参数结构体中
 * 入口参数: 
 * 		actrID	执行器ID
 * 		actrReportDataType	要发送的参数类型，为0时输出全部参数
 * 出口参数: 程序执行结果
 * 备    注：
 * Editor：  zhangchunyu
 */
void PrintActrState(ActrReportDataTypedef actrReportDataType, uint32_t actrID)
{
	static ActrParaTypedef *pActrParaDev = NULL;
	pActrParaDev = FindActrDevByID(actrID);

	switch (actrReportDataType)
	{
	case ACTR_REPORT_ALL:
		PrintActrState(ACTR_REPORT_ON_OFF, actrID);
		PrintActrState(ACTR_REPORT_CUR_MODE, actrID);
		PrintActrState(ACTR_REPORT_CURRENT, actrID);
		PrintActrState(ACTR_REPORT_SPEED, actrID);
		PrintActrState(ACTR_REPORT_POSTION, actrID);
		PrintActrState(ACTR_REPORT_MOTOR_TEMP, actrID);
		PrintActrState(ACTR_REPORT_INVTR_TEMP, actrID);
		break;
	case ACTR_REPORT_ON_OFF:
		if (pActrParaDev->actrPwrState == PWR_ON)
			printf("Motor %d is ON.\r\n", actrID);
		else
			printf("Motor %d is OFF.\r\n", actrID);
		break;
	case ACTR_REPORT_CUR_MODE:
		switch (pActrParaDev->actrMode)
		{
		case ACTR_MODE_CUR:
			printf("Motor %d is in CURRENT mode.\r\n", actrID);
			break;
		case ACTR_MODE_SPD:
			printf("Motor %d is in SPEED mode.\r\n", actrID);
			break;
		case ACTR_MODE_POS:
			printf("Motor %d is in POSITION mode.\r\n", actrID);
			break;
		case ACTR_MODE_TECH:
			printf("Motor %d is in TEACHING mode.\r\n", actrID);
			break;
		case ACTR_MODE_PLAY:
			printf("Motor %d is in PLAY mode.\r\n", actrID);
			break;
		case ACTR_MODE_TSHAP_POS:
			printf("Motor %d is in PROFILE-POSITION mode.\r\n", actrID);
			break;
		case ACTR_MODE_TSHAP_SPD:
			printf("Motor %d is in PROFILE-VELOCITY mode.\r\n", actrID);
			break;
		case ACTR_MODE_HOMING:
			printf("Motor %d is in HOMING mode.\r\n", actrID);
			break;
		default:
			printf("Motor %d is in UNKOWEN mode.\r\n", actrID);
		}
		break;
	case ACTR_REPORT_CURRENT:
		printf("Motor %d \tCUR:\t%.3f A\r\n", actrID, pActrParaDev->actrCurrent);
		break;
	case ACTR_REPORT_SPEED:
		printf("Motor %d \tSPD:\t%.3f RPM\r\n", actrID, pActrParaDev->actrSpeed);
		break;
	case ACTR_REPORT_POSTION:
		printf("Motor %d \tPOS:\t%.3f R\r\n", actrID, pActrParaDev->actrPostion);
		break;
	case ACTR_REPORT_MOTOR_TEMP:
		printf("Motor %d \tM-TEMP:\t%.3f\r\n", actrID, pActrParaDev->actrMotorTemp);
		break;
	case ACTR_REPORT_INVTR_TEMP:
		printf("Motor %d \tI-TEMP:\t%.3f\r\n", actrID, pActrParaDev->actrInvterTemp);
		break;
	default:
		break;
	}
}
