#include "auto.h"

Node_t sLeftArea[MAX_STEP_NUM];
Node_t sRightArea[MAX_STEP_NUM];
Node_t sResumeLeftPoint_1[MAX_STEP_NUM];
Node_t sResumeRightPoint_1[MAX_STEP_NUM];
Node_t sResumeLeftPoint_2[MAX_STEP_NUM];
Node_t sResumeRightPoint_2[MAX_STEP_NUM];

float CtrlVal_Forward;
float CtrlVal_Turning;

uint8_t StepIndex = 0;
uint8_t StepIndex_Pause = 8;

uint8_t StepIndex_PauseFlag = STEP_PAUSED;

void InitAutoData(void)
{
	StepIndex = 0;

	sLeftArea[0].end_time = 0.5f;
	sLeftArea[0].forward_val = 45;
	sLeftArea[0].turning_val = 0;

	sLeftArea[1].end_time = 5.5f;
	sLeftArea[1].forward_val = 90;
	sLeftArea[1].turning_val = 0;

	sLeftArea[2].end_time = 7.5f;
	sLeftArea[2].forward_val = 80;
	sLeftArea[2].turning_val = 118;

	sLeftArea[3].end_time = 9.5f;
	sLeftArea[3].forward_val = 60;
	sLeftArea[3].turning_val = 0;

	sLeftArea[4].end_time = 10.0f;
	sLeftArea[4].forward_val = 70;
	sLeftArea[4].turning_val = 90;

	sLeftArea[5].end_time = 11.0f;
	sLeftArea[5].forward_val = 80;
	sLeftArea[5].turning_val = 0;

	sLeftArea[6].end_time = 13.0f;
	sLeftArea[6].forward_val = 75;
	sLeftArea[6].turning_val = -128;

	sLeftArea[7].end_time = 16.50f;
	sLeftArea[7].forward_val = 80;
	sLeftArea[7].turning_val = -10;

	sLeftArea[8].end_time = 21.60f;
	sLeftArea[8].forward_val = 72;
	sLeftArea[8].turning_val = 0;

	sRightArea[0].end_time = 0.5f;
	sRightArea[0].forward_val = 40;
	sRightArea[0].turning_val = 0;

	sRightArea[1].end_time = 5.5f;
	sRightArea[1].forward_val = 80;
	sRightArea[1].turning_val = 0;

	sRightArea[2].end_time = 7.5f;
	sRightArea[2].forward_val = 80;
	sRightArea[2].turning_val = -112;

	sRightArea[3].end_time = 9.5f;
	sRightArea[3].forward_val = 60;
	sRightArea[3].turning_val = 0;

	sRightArea[4].end_time = 10.5f;
	sRightArea[4].forward_val = 60;
	sRightArea[4].turning_val = -100;

	sRightArea[5].end_time = 11.0f;
	sRightArea[5].forward_val = 80;
	sRightArea[5].turning_val = 0;

	sRightArea[6].end_time = 13.5f;
	sRightArea[6].forward_val = 60;
	sRightArea[6].turning_val = 128;

	sRightArea[7].end_time = 16.5f;
	sRightArea[7].forward_val = 80;
	sRightArea[7].turning_val = 0;

	sRightArea[8].end_time = 21.10f;
	sRightArea[8].forward_val = 80;
	sRightArea[8].turning_val = 0;

	sResumeLeftPoint_1[0].end_time = 2.5f;
	sResumeLeftPoint_1[0].forward_val = 60;
	sResumeLeftPoint_1[0].turning_val = 0;

	sResumeLeftPoint_1[1].end_time = 3.0f;
	sResumeLeftPoint_1[1].forward_val = 60;
	sResumeLeftPoint_1[1].turning_val = 100;

	sResumeLeftPoint_1[2].end_time = 4.0f;
	sResumeLeftPoint_1[2].forward_val = 80;
	sResumeLeftPoint_1[2].turning_val = 0;

	sResumeLeftPoint_1[3].end_time = 6.0f;
	sResumeLeftPoint_1[3].forward_val = 60;
	sResumeLeftPoint_1[3].turning_val = -128;

	sResumeLeftPoint_1[4].end_time = 9.5f;
	sResumeLeftPoint_1[4].forward_val = 80;
	sResumeLeftPoint_1[4].turning_val = 0;

	sResumeLeftPoint_1[5].end_time = 14.6f;
	sResumeLeftPoint_1[5].forward_val = 70;
	sResumeLeftPoint_1[5].turning_val = 0;

	sResumeRightPoint_1[0].end_time = 2.5f;
	sResumeRightPoint_1[0].forward_val = 60;
	sResumeRightPoint_1[0].turning_val = 0;

	sResumeRightPoint_1[1].end_time = 3.0f;
	sResumeRightPoint_1[1].forward_val = 60;
	sResumeRightPoint_1[1].turning_val = -100;

	sResumeRightPoint_1[2].end_time = 4.0f;
	sResumeRightPoint_1[2].forward_val = 80;
	sResumeRightPoint_1[2].turning_val = 0;

	sResumeRightPoint_1[3].end_time = 6.0f;
	sResumeRightPoint_1[3].forward_val = 60;
	sResumeRightPoint_1[3].turning_val = 128;

	sResumeRightPoint_1[4].end_time = 9.75f;
	sResumeRightPoint_1[4].forward_val = 80;
	sResumeRightPoint_1[4].turning_val = 0;

	sResumeRightPoint_1[5].end_time = 13.60f;
	sResumeRightPoint_1[5].forward_val = 70;
	sResumeRightPoint_1[5].turning_val = 0;

	sResumeLeftPoint_2[0].end_time = 0.0f;
	sResumeLeftPoint_2[0].forward_val = 60;
	sResumeLeftPoint_2[0].turning_val = 0;

	sResumeLeftPoint_2[1].end_time = 0.5f;
	sResumeLeftPoint_2[1].forward_val = 60;
	sResumeLeftPoint_2[1].turning_val = 90;

	sResumeLeftPoint_2[2].end_time = 1.5f;
	sResumeLeftPoint_2[2].forward_val = 80;
	sResumeLeftPoint_2[2].turning_val = 0;

	sResumeLeftPoint_2[3].end_time = 3.5f;
	sResumeLeftPoint_2[3].forward_val = 60;
	sResumeLeftPoint_2[3].turning_val = -128;

	sResumeLeftPoint_2[4].end_time = 6.5f;
	sResumeLeftPoint_2[4].forward_val = 80;
	sResumeLeftPoint_2[4].turning_val = -10;

	sResumeLeftPoint_2[5].end_time = 11.6f;
	sResumeLeftPoint_2[5].forward_val = 70;
	sResumeLeftPoint_2[5].turning_val = 0;

	sResumeRightPoint_2[0].end_time = 0.0f;
	sResumeRightPoint_2[0].forward_val = 60;
	sResumeRightPoint_2[0].turning_val = 0;

	sResumeRightPoint_2[1].end_time = 0.5f;
	sResumeRightPoint_2[1].forward_val = 60;
	sResumeRightPoint_2[1].turning_val = -100;

	sResumeRightPoint_2[2].end_time = 1.5f;
	sResumeRightPoint_2[2].forward_val = 80;
	sResumeRightPoint_2[2].turning_val = 0;

	sResumeRightPoint_2[3].end_time = 3.5f;
	sResumeRightPoint_2[3].forward_val = 60;
	sResumeRightPoint_2[3].turning_val = 128;

	sResumeRightPoint_2[4].end_time = 6.5f;
	sResumeRightPoint_2[4].forward_val = 80;
	sResumeRightPoint_2[4].turning_val = 0;

	sResumeRightPoint_2[5].end_time = 11.75f;
	sResumeRightPoint_2[5].forward_val = 70;
	sResumeRightPoint_2[5].turning_val = 0;
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
	else if (area == CTRL_SRC_RESUME_L1)
	{
		while (current_time > sResumeLeftPoint_1[StepIndex].end_time && StepIndex < MAX_STEP_NUM)
			StepIndex++;
	}
	else if (area == CTRL_SRC_RESUME_R1)
	{
		while (current_time > sResumeRightPoint_1[StepIndex].end_time && StepIndex < MAX_STEP_NUM)
			StepIndex++;
	}
	else if (area == CTRL_SRC_RESUME_L2)
	{
		while (current_time > sResumeLeftPoint_2[StepIndex].end_time && StepIndex < MAX_STEP_NUM)
			StepIndex++;
	}
	else if (area == CTRL_SRC_RESUME_R2)
	{
		while (current_time > sResumeRightPoint_2[StepIndex].end_time && StepIndex < MAX_STEP_NUM)
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
	else if (area == CTRL_SRC_RESUME_L1)
	{
		CtrlVal_Forward = sResumeLeftPoint_1[StepIndex].forward_val;
		CtrlVal_Turning = sResumeLeftPoint_1[StepIndex].turning_val;
	}
	else if (area == CTRL_SRC_RESUME_R1)
	{
		CtrlVal_Forward = sResumeRightPoint_1[StepIndex].forward_val;
		CtrlVal_Turning = sResumeRightPoint_1[StepIndex].turning_val;
	}
	else if (area == CTRL_SRC_RESUME_L2)
	{
		CtrlVal_Forward = sResumeLeftPoint_2[StepIndex].forward_val;
		CtrlVal_Turning = sResumeLeftPoint_2[StepIndex].turning_val;
	}
	else if (area == CTRL_SRC_RESUME_R2)
	{
		CtrlVal_Forward = sResumeRightPoint_2[StepIndex].forward_val;
		CtrlVal_Turning = sResumeRightPoint_2[StepIndex].turning_val;
	}
}

void GetCtrlData(int data_src)
{
	switch (data_src)
	{
	case CTRL_SRC_AUTO_L:
		StepIndex_Pause = 8;
		CalcAutoData(CTRL_SRC_AUTO_L);
		break;

	case CTRL_SRC_AUTO_R:
		StepIndex_Pause = 8;
		CalcAutoData(CTRL_SRC_AUTO_R);
		break;

	case CTRL_SRC_RESUME_L1:
		StepIndex_Pause = 5;
		CalcAutoData(CTRL_SRC_RESUME_L1);
		break;

	case CTRL_SRC_RESUME_R1:
		StepIndex_Pause = 5;
		CalcAutoData(CTRL_SRC_RESUME_R1);
		break;

	case CTRL_SRC_RESUME_L2:
		StepIndex_Pause = 5;
		CalcAutoData(CTRL_SRC_RESUME_L2);
		break;

	case CTRL_SRC_RESUME_R2:
		StepIndex_Pause = 5;
		CalcAutoData(CTRL_SRC_RESUME_R2);
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
