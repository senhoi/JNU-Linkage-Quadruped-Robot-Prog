#ifndef AUTO_H
#define AUTO_H

#include "remote.h"
#include "actuator.h"

#define CTRL_SRC_REMOTE 0
#define CTRL_SRC_AUTO_L 1
#define CTRL_SRC_AUTO_R 2

#define MAX_STEP_NUM 128

#define STEP_CONTINUE 0
#define STEP_PAUSED 1

typedef struct Node_t
{
	float end_time;	//�����˶�ֹͣʱ�䣨���ԣ�
	float forward_val; //0~128
	float turning_val; //ת���ٶȣ�Ϊ0ʱΪֱ���˶� -128~128
} Node_t;

extern float CtrlVal_Forward;
extern float CtrlVal_Turning;

void InitAutoData(void);
void GetCtrlData(int data_src);

#endif