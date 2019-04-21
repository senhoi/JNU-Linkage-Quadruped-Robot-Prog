#include "actuator.h"

uint8_t devIDList[ACTR_DEV_NUM] = {55, 3, 31, 8, 2};
//M1-M2-T-M3-M4
float actrAngle[ACTR_DEV_NUM] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

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
