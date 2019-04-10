#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "sys.h"
#include "SCA_ctrl.h"
#include "usart.h"

#define ACTR_DEV_NUM 12

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

int CheckActrTargetPosVal(void);
void PrintActrState(ActrReportDataTypedef actrReportDataType, uint32_t actrID);

#endif
