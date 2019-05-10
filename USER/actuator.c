#include "actuator.h"

uint8_t devIDList[ACTR_DEV_NUM] = {31, 3, 10, 8, 2};
//LM1-LM2-TM-RM2-RM1
float actrSpd[ACTR_DEV_NUM] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

float prev_actrPhase[ACTR_DEV_NUM] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
float actrPhase[ACTR_DEV_NUM] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
float actrRefPhase = 1.0f;

int16_t actrRevolution[ACTR_DEV_NUM] = {0, 0, 0, 0, 0};
int16_t actrRefRevolution = 0;

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

void InvertActrPos2Phase(int actr_index)
{
	static ActrParaTypedef *pActrParaDev = NULL;

	pActrParaDev = FindActrDevByID(devIDList[actr_index]);

	float k1 = 2.0f / (CtrlVal_ExpandRatio * 32);
	float k2 = 2.0f / ((2 - CtrlVal_ExpandRatio) * 32);
	float b1 = 4.0f - (CtrlVal_ExpandRatio - 1) * 16 * k1;
	float b2 = -(CtrlVal_ExpandRatio - 1) * 16 * k2;
	float pos_revised;

	if (pActrParaDev->actrPostion <= -64.0f)
		pos_revised = pActrParaDev->actrPostion + 128.0f;
	else if (pActrParaDev->actrPostion <= 0.0f)
		pos_revised = pActrParaDev->actrPostion + 64.0f;
	else if (pActrParaDev->actrPostion <= 64.0f)
		pos_revised = pActrParaDev->actrPostion + 0.0f;
	else if (pActrParaDev->actrPostion <= 128.0f)
		pos_revised = pActrParaDev->actrPostion - 64.0f;

	switch (actr_index)
	{
	case RM1_INDEX:
	case RM2_INDEX:

		pos_revised += 32.0f;
		if (pos_revised > 64)
			pos_revised -= 64;
		break;

	default:
		break;
	}

	if (pos_revised < (CtrlVal_ExpandRatio - 1) * 16)
		actrPhase[actr_index] = pos_revised * k1 + b1;
	else if (pos_revised < (CtrlVal_ExpandRatio - 1) * 16 + 2 / k2)
		actrPhase[actr_index] = pos_revised * k2 + b2;
	else
		actrPhase[actr_index] = (pos_revised - 64) * k1 + b1;

	switch (actr_index)
	{
	case LM1_INDEX:
	case RM2_INDEX:

		actrPhase[actr_index] = 4 - actrPhase[actr_index];
		break;

	default:
		break;
	}

	switch (actr_index)
	{
	case RM1_INDEX:
	case RM2_INDEX:
		if (actrPhase[actr_index] > 2)
			actrPhase[actr_index] -= 2;
		else
			actrPhase[actr_index] += 2;
		break;

	default:
		break;
	}
}

void CalcRefPhase(float spd)
{
	actrRefPhase += 8.0f * 4 * spd / 200;
	if (actrRefPhase > 4)
	{
		actrRefPhase -= 4;
		actrRefRevolution++;
	}
	else if (actrRefPhase < 0)
	{
		actrRefPhase += 4;
		actrRefRevolution--;
	}
}

void UpdateActrPhase(void)
{
	InvertActrPos2Phase(LM1_INDEX);
	InvertActrPos2Phase(LM2_INDEX);
	InvertActrPos2Phase(RM2_INDEX);
	InvertActrPos2Phase(RM1_INDEX);
}

void CountActrRevolution(void)
{
	for (int i = 0; i < ACTR_DEV_NUM; i++)
	{
		if (prev_actrPhase[i] < 1.1f && actrPhase[i] > 2.9f)
			actrRevolution[i]--;
		else if (prev_actrPhase[i] > 2.9f && actrPhase[i] < 1.1f)
			actrRevolution[i]++;
		prev_actrPhase[i] = actrPhase[i];
	}
}

void ClearActrPhase(void)
{
	for (int i = 0; i < ACTR_DEV_NUM; i++)
	{
		prev_actrPhase[i] = 0.0f;
		actrPhase[i] = 0.0f;
	}
	actrRefPhase = 0.0f;
}

void ClearActrRevolution(void)
{
	for (int i = 0; i < ACTR_DEV_NUM; i++)
	{
		actrRevolution[i] = 0;
	}
	actrRefRevolution = 0;
}

PID_Increment_t PID_LM1;
PID_Increment_t PID_LM2;
PID_Increment_t PID_RM2;
PID_Increment_t PID_RM1;

void InitActrPhasePID(void)
{
	PID_Increment_Reset(&PID_LM1, 1.2f, 0.03f, 0.0f, 0.01, 0.09f, 0.1f, 1.0f);
	PID_Increment_Reset(&PID_LM2, 1.2f, 0.03f, 0.0f, 0.01, 0.09f, 0.1f, 1.0f);
	PID_Increment_Reset(&PID_RM2, 1.2f, 0.03f, 0.0f, 0.01, 0.09f, 0.1f, 1.0f);
	PID_Increment_Reset(&PID_RM1, 1.2f, 0.03f, 0.0f, 0.01, 0.09f, 0.1f, 1.0f);
}

void CalcActrPhasePID(void)
{
	PID_LM1.Ref = actrRefPhase + actrRefRevolution * 4.0f;
	PID_LM1.Feedback = actrPhase[LM1_INDEX] + actrRevolution[LM1_INDEX] * 4.0f;
	PID_Increment_Calc(&PID_LM1);

	PID_LM2.Ref = actrRefPhase + actrRefRevolution * 4.0f;
	PID_LM2.Feedback = actrPhase[LM2_INDEX] + actrRevolution[LM2_INDEX] * 4.0f;
	PID_Increment_Calc(&PID_LM2);

	PID_RM2.Ref = actrRefPhase + actrRefRevolution * 4.0f;
	PID_RM2.Feedback = actrPhase[RM2_INDEX] + actrRevolution[RM2_INDEX] * 4.0f;
	PID_Increment_Calc(&PID_RM2);

	PID_RM1.Ref = actrRefPhase + actrRefRevolution * 4.0f;
	PID_RM1.Feedback = actrPhase[RM1_INDEX] + actrRevolution[RM1_INDEX] * 4.0f;
	PID_Increment_Calc(&PID_RM1);
}
