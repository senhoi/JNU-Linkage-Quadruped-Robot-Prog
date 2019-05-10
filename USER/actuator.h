#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "sys.h"
#include "SCA_ctrl.h"
#include "usart.h"
#include "pid.h"

typedef enum ActrReportDataTypedef
{
	ACTR_REPORT_ALL,
	ACTR_REPORT_ON_OFF,
	ACTR_REPORT_CUR_MODE,
	ACTR_REPORT_CURRENT,
	ACTR_REPORT_SPEED,
	ACTR_REPORT_POSTION,
	ACTR_REPORT_MOTOR_TEMP,
	ACTR_REPORT_INVTR_TEMP,
	ACTR_REPORT_EXECPTION,
} ActrReportDataTypedef;

#define LM1_INDEX 0
#define LM2_INDEX 1
#define TM_INDEX 2
#define RM2_INDEX 3
#define RM1_INDEX 4

#define TM_LIMIT 2.2f

extern float actrSpd[ACTR_DEV_NUM];
extern float actrPhase[ACTR_DEV_NUM];
extern float actrRefPhase;
extern int16_t actrRevolution[ACTR_DEV_NUM];
extern int16_t actrRefRevolution;

extern PID_Increment_t PID_LM1;
extern PID_Increment_t PID_LM2;
extern PID_Increment_t PID_RM2;
extern PID_Increment_t PID_RM1;

int CheckActrTargetPosVal(void);
void PrintActrState(ActrReportDataTypedef actrReportDataType, uint32_t actrID);
void UpdateActrPhase(void);
void CountActrRevolution(void);
void CalcRefPhase(float spd);

void ClearActrPhase(void);
void ClearActrRevolution(void);

void InitActrPhasePID(void);
void CalcActrPhasePID(void);

#endif
