#include "auto.h"

Node_t sLeftArea[MAX_STEP_NUM];
Node_t sRightArea[MAX_STEP_NUM];

float CtrlVal_Forward;
float CtrlVal_Turning;

uint8_t StepIndex = 0;
uint8_t StepIndex_Pause = 5;
uint8_t StepIndex_PauseFlag = STEP_PAUSED;

void InitAutoData(void)
{
	StepIndex = 0;

	sLeftArea[0].end_time = 5.0f;
	sLeftArea[0].forward_val = 80;
	sLeftArea[0].turning_val = 0;

	sLeftArea[1].end_time = 7.0f;
	sLeftArea[1].forward_val = 80;
	sLeftArea[1].turning_val = 115;

	sLeftArea[2].end_time = 10.5f;
	sLeftArea[2].forward_val = 60;
	sLeftArea[2].turning_val = 0;

	sLeftArea[3].end_time = 12.5f;
	sLeftArea[3].forward_val = 60;
	sLeftArea[3].turning_val = -105;

	sLeftArea[4].end_time = 16.0f;
	sLeftArea[4].forward_val = 80;
	sLeftArea[4].turning_val = 0;

	sLeftArea[5].end_time = 21.25f;
	sLeftArea[5].forward_val = 70;
	sLeftArea[5].turning_val = 0;

	sRightArea[0].end_time = 5.0f;
	sRightArea[0].forward_val = 80;
	sRightArea[0].turning_val = 0;

	sRightArea[1].end_time = 7.5f;
	sRightArea[1].forward_val = 80;
	sRightArea[1].turning_val = -90;

	sRightArea[2].end_time = 10.5f;
	sRightArea[2].forward_val = 80;
	sRightArea[2].turning_val = 0;

	sRightArea[3].end_time = 12.5f;
	sRightArea[3].forward_val = 80;
	sRightArea[3].turning_val = 100;

	sRightArea[4].end_time = 16.0f;
	sRightArea[4].forward_val = 80;
	sRightArea[4].turning_val = 0;

	sRightArea[5].end_time = 22.25f;
	sRightArea[5].forward_val = 60;
	sRightArea[5].turning_val = 0;
}

extern int AutoTime_Flag;

void CalcAutoData(int area)
{
	float current_time;

	current_time = actrRefPhase / 4.0f + actrRefRevolution;

	while (current_time > sLeftArea[StepIndex].end_time && StepIndex < MAX_STEP_NUM)
		StepIndex++;

	if (StepIndex == StepIndex_Pause)
	{
		if (StepIndex_PauseFlag == STEP_PAUSED)
		{
			AutoTime_Flag = 0;
			CtrlVal_Forward = 0;
			CtrlVal_Turning = 0;
			return;
		}
	}

	if (StepIndex == MAX_STEP_NUM)
	{
		CtrlVal_Forward = 0;
		CtrlVal_Turning = 0;
		AutoTime_Flag = 0;
		return;
	}

	if (area == CTRL_SRC_AUTO_L)
	{
		CtrlVal_Forward = sLeftArea[StepIndex].forward_val;
		CtrlVal_Turning = sLeftArea[StepIndex].turning_val;
	}
	else if (area == CTRL_SRC_AUTO_R)
	{
		CtrlVal_Forward = sRightArea[StepIndex].forward_val;
		CtrlVal_Turning = sRightArea[StepIndex].turning_val;
	}
}

void GetCtrlData(int data_src)
{
	switch (data_src)
	{
	case CTRL_SRC_AUTO_L:
		CalcAutoData(CTRL_SRC_AUTO_L);
		break;

	case CTRL_SRC_AUTO_R:
		CalcAutoData(CTRL_SRC_AUTO_R);
		break;

	case CTRL_SRC_REMOTE:
		if (RemoteData[4] != 1)
		{
			CtrlVal_Forward = RemoteData[1];
			CtrlVal_Turning = RemoteData[2];
		}
		break;

	default:
		break;
	}
}
