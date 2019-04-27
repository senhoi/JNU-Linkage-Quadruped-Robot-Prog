#include "auto.h"

Node_t sLeftArea[MAX_STEP_NUM];
Node_t sRightArea[MAX_STEP_NUM];
Node_t sResumeLeftPoint[MAX_STEP_NUM];
Node_t sResumeRightPoint[MAX_STEP_NUM];

float CtrlVal_Forward;
float CtrlVal_Turning;

uint8_t StepIndex = 0;
uint8_t StepIndex_Pause = 7;

uint8_t StepIndex_PauseFlag = STEP_PAUSED;

void InitAutoData(void)
{
	StepIndex = 0;

	sLeftArea[0].end_time = 5.0f;
	sLeftArea[0].forward_val = 80;
	sLeftArea[0].turning_val = 0;

	sLeftArea[1].end_time = 7.0f;
	sLeftArea[1].forward_val = 80;
	sLeftArea[1].turning_val = 112;

	sLeftArea[2].end_time = 9.0f;
	sLeftArea[2].forward_val = 60;
	sLeftArea[2].turning_val = 0;

	sLeftArea[3].end_time = 9.5f;
	sLeftArea[3].forward_val = 60;
	sLeftArea[3].turning_val = 100;

	sLeftArea[4].end_time = 10.5f;
	sLeftArea[4].forward_val = 80;
	sLeftArea[4].turning_val = 0;

	sLeftArea[5].end_time = 12.5f;
	sLeftArea[5].forward_val = 60;
	sLeftArea[5].turning_val = -128;

	sLeftArea[6].end_time = 15.75f;
	sLeftArea[6].forward_val = 80;
	sLeftArea[6].turning_val = 0;

	sLeftArea[7].end_time = 20.65f;
	sLeftArea[7].forward_val = 70;
	sLeftArea[7].turning_val = 0;

	sRightArea[0].end_time = 5.0f;
	sRightArea[0].forward_val = 80;
	sRightArea[0].turning_val = 0;

	sRightArea[1].end_time = 7.0f;
	sRightArea[1].forward_val = 80;
	sRightArea[1].turning_val = -112;

	sRightArea[2].end_time = 9.0f;
	sRightArea[2].forward_val = 60;
	sRightArea[2].turning_val = 0;

	sRightArea[3].end_time = 9.5f;
	sRightArea[3].forward_val = 60;
	sRightArea[3].turning_val = -100;

	sRightArea[4].end_time = 10.5f;
	sRightArea[4].forward_val = 80;
	sRightArea[4].turning_val = 0;

	sRightArea[5].end_time = 12.5f;
	sRightArea[5].forward_val = 60;
	sRightArea[5].turning_val = 128;

	sRightArea[6].end_time = 15.75f;
	sRightArea[6].forward_val = 80;
	sRightArea[6].turning_val = 0;

	sRightArea[7].end_time = 20.65f;
	sRightArea[7].forward_val = 70;
	sRightArea[7].turning_val = 0;

	sResumeLeftPoint[0].end_time = 2.5f;
	sResumeLeftPoint[0].forward_val = 60;
	sResumeLeftPoint[0].turning_val = 0;

	sResumeLeftPoint[1].end_time = 3.0f;
	sResumeLeftPoint[1].forward_val = 60;
	sResumeLeftPoint[1].turning_val = 100;

	sResumeLeftPoint[2].end_time = 4.0f;
	sResumeLeftPoint[2].forward_val = 80;
	sResumeLeftPoint[2].turning_val = 0;

	sResumeLeftPoint[3].end_time = 6.0f;
	sResumeLeftPoint[3].forward_val = 60;
	sResumeLeftPoint[3].turning_val = -128;

	sResumeLeftPoint[4].end_time = 9.25f;
	sResumeLeftPoint[4].forward_val = 80;
	sResumeLeftPoint[4].turning_val = 0;

	sResumeLeftPoint[5].end_time = 14.15f;
	sResumeLeftPoint[5].forward_val = 70;
	sResumeLeftPoint[5].turning_val = 0;
	
	sResumeRightPoint[0].end_time = 2.5f;
	sResumeRightPoint[0].forward_val = 60;
	sResumeRightPoint[0].turning_val = 0;

	sResumeRightPoint[1].end_time = 3.0f;
	sResumeRightPoint[1].forward_val = 60;
	sResumeRightPoint[1].turning_val = -100;

	sResumeRightPoint[2].end_time = 4.0f;
	sResumeRightPoint[2].forward_val = 80;
	sResumeRightPoint[2].turning_val = 0;

	sResumeRightPoint[3].end_time = 6.0f;
	sResumeRightPoint[3].forward_val = 60;
	sResumeRightPoint[3].turning_val = 128;

	sResumeRightPoint[4].end_time = 9.25f;
	sResumeRightPoint[4].forward_val = 80;
	sResumeRightPoint[4].turning_val = 0;

	sResumeRightPoint[5].end_time = 14.15f;
	sResumeRightPoint[5].forward_val = 70;
	sResumeRightPoint[5].turning_val = 0;
	
}

extern int AutoTime_Flag;

void CalcAutoData(int area)
{
	float current_time;

	current_time = actrRefPhase / 4.0f + actrRefRevolution;

	if (area == CTRL_SRC_AUTO_L)
	{
		while (current_time > sLeftArea[StepIndex].end_time && StepIndex < MAX_STEP_NUM)
			StepIndex++;
	}
	else if (area == CTRL_SRC_AUTO_R)
	{
		while (current_time > sRightArea[StepIndex].end_time && StepIndex < MAX_STEP_NUM)
			StepIndex++;
	}
	else if (area == CTRL_SRC_RESUME_L)
	{
		while (current_time > sResumeLeftPoint[StepIndex].end_time && StepIndex < MAX_STEP_NUM)
			StepIndex++;
	}
	else if (area == CTRL_SRC_RESUME_R)
	{
		while (current_time > sResumeRightPoint[StepIndex].end_time && StepIndex < MAX_STEP_NUM)
			StepIndex++;
	}

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
	else if (area == CTRL_SRC_RESUME_L)
	{
		CtrlVal_Forward = sResumeLeftPoint[StepIndex].forward_val;
		CtrlVal_Turning = sResumeLeftPoint[StepIndex].turning_val;
	}
	else if (area == CTRL_SRC_RESUME_R)
	{
		CtrlVal_Forward = sResumeRightPoint[StepIndex].forward_val;
		CtrlVal_Turning = sResumeRightPoint[StepIndex].turning_val;
	}
}

void GetCtrlData(int data_src)
{
	switch (data_src)
	{
	case CTRL_SRC_AUTO_L:
		StepIndex_Pause = 7;
		CalcAutoData(CTRL_SRC_AUTO_L);
		break;

	case CTRL_SRC_AUTO_R:
		StepIndex_Pause = 7;
		CalcAutoData(CTRL_SRC_AUTO_R);
		break;
	
	case CTRL_SRC_RESUME_L:
		StepIndex_Pause = 5;
		CalcAutoData(CTRL_SRC_RESUME_L);
		break;

	case CTRL_SRC_RESUME_R:
		StepIndex_Pause = 5;
		CalcAutoData(CTRL_SRC_RESUME_R);
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

int16_t actrStopRevolution;
float actrStopPhase;

uint8_t actrResumePoint;

void ResumeAutoData(void)
{
	if (actrRefRevolution + actrStopPhase / 4.0f < 6.0f)
	{
		actrResumePoint = 0;
	}
	else if (actrRefRevolution + actrStopPhase / 4.0f > 6.0f && actrRefRevolution + actrStopPhase / 4.0f <= 9.0f)
	{
		actrResumePoint = 1;
	}
	else if (actrRefRevolution + actrStopPhase / 4.0f > 9.0f && actrRefRevolution + actrStopPhase / 4.0f < 12.0f)
	{
		actrResumePoint = 2;
	}
	else
	{
		actrResumePoint = 3;
	}
}
