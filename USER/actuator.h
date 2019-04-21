#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "sys.h"
#include "SCA_ctrl.h"
#include "usart.h"

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

int CheckActrTargetPosVal(void);
void PrintActrState(ActrReportDataTypedef actrReportDataType, uint32_t actrID);

#endif
