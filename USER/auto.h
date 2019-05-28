#ifndef AUTO_H
#define AUTO_H

#include "remote.h"
#include "actuator.h"

#define CTRL_SRC_REMOTE 0
#define CTRL_SRC_AUTO_L 1
#define CTRL_SRC_AUTO_R 2
#define CTRL_SRC_RESUME_L1 3
#define CTRL_SRC_RESUME_R1 4
#define CTRL_SRC_RESUME_L2 5
#define CTRL_SRC_RESUME_R2 6
#define CTRL_SRC_SLOPE 7

#define MAX_STEP_NUM 128

#define STEP_CONTINUE 0
#define STEP_PAUSED 1

typedef struct Node_t
{
	float end_time;	//本段运动停止时间（绝对）
	float forward_val; //0~128
	float turning_val; //转动速度，为0时为直线运动 -128~128
} Node_t;

extern float CtrlVal_Forward;
extern float CtrlVal_Turning;
extern float LiftPhaseThreshold;

void InitAutoData(void);
void GetCtrlData(int data_src);

#endif
