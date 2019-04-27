#ifndef TASK_H
#define TASK_H

#include "sys.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "can.h"
#include "beep.h"
#include "key.h"
#include "timer.h"
#include "actuator.h"
#include "display.h"
#include "sbus.h"
#include "remote.h"
#include "math.h"
#include "usr_math.h"
#include "auto.h"
#include "io.h"

extern unsigned int LowPirorityTaskFlag;
extern int ActrHomingError;
extern int ActrHomingCorrect;
extern int ActrHomingErrorID[ACTR_DEV_NUM];
extern int ReportTaskFlag; //在执行器发送控制命令前关闭此标志位以禁止定时报告任务，防止总线干扰

typedef enum task_flag
{
	TASK_FLAG_KEY = 0,
	TASK_FLAG_REPORT_POS,
	TASK_FLAG_REPORT_VEL,
	TASK_FLAG_REPORT_CUR,
	TASK_FLAG_REPORT_EXECPTION,
	TASK_FLAG_REPORT_ALL,
	TASK_FLAG_UPLOAD,
	TASK_FLAG_CONTROL,
} TaskFlagTypedef;

#define TASK_SET_FLAG(flag) (LowPirorityTaskFlag |= (0x0001 << (unsigned int)flag))
#define TASK_RESET_FLAG(flag) (LowPirorityTaskFlag &= (~(0x0001 << (unsigned int)flag)))
#define TASK_SEARCH_FLAG(flag) (LowPirorityTaskFlag & (0x0001 << (unsigned int)flag))
#define TASK_STOP_REGULAR_REPORT (ReportTaskFlag = 0)
#define TASK_BEGIN_REGULAR_REPORT (ReportTaskFlag = 1)
#define TASK_CHECK_REGULAR_REPORT_FLAG (ReportTaskFlag == 1)

#define IDLE 0
#define MANUAL 1
#define AUTO_L 2
#define AUTO_R 3
#define RESUME_L 4
#define RESUME_R 5

#define ERROR -1
#define STOP 0
#define RUN 1

void InitTask(void);
void KeyTask(void);
void ControlTask(void);
void HandleLowPirorityTask(void);
void CheckFootGroundingTask(void);
void HandleDevDataTask(void);
int CheckActrRunningStateTask(void);

#endif
