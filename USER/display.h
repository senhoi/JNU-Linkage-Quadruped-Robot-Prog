#ifndef DISPLAY_H
#define DISPLAY_H

#include "sys.h"
#include "lcd.h"
#include "SCA_ctrl.h"
#include "stdio.h"
#include "tasks.h"
#include "string.h"
#include "led.h"
#include "JY901.h"

#define INTERFACE_NUM 6

#define ROBOT_INTERFACE_MODE -1
#define ROBOT_INTERFACE_ACTR_PWR 1
#define ROBOT_INTERFACE_HOMING_CHECK 2
#define ROBOT_INTERFACE_ACTR_POS 3
#define ROBOT_INTERFACE_ACTR_PARA 4
#define ROBOT_INTERFACE_DEV_PARA 5

#define INFO_DISP_ABSOLUTE_X 0
#define INFO_DISP_ABSOLUTE_Y 10
#define INFO_DISP_FONTSIZE 24

#define INFO_DISP_STA_LABELLEN 10
#define INFO_DISP_STA_LABELHEIGHT (INFO_DISP_FONTSIZE)
#define INFO_DISP_STA_LABELWIDTH (INFO_DISP_FONTSIZE / 2 * INFO_DISP_STA_LABELLEN)

#define INFO_DISP_ROWLEDGE (INFO_DISP_FONTSIZE + INFO_DISP_FONTSIZE / 4)
#define INFO_DISP_SET_COL(COL) ((COL)*INFO_DISP_FONTSIZE / 2 + INFO_DISP_ABSOLUTE_X)
#define INFO_DISP_SET_ROW(ROW) ((ROW)*INFO_DISP_ROWLEDGE + INFO_DISP_ABSOLUTE_Y)

#define INFO_DISP_COL_TAB 2

#define ACTR_DISP_ABSOLUTE_X 0
#define ACTR_DISP_ABSOLUTE_Y 200
#define ACTR_DISP_FONTSIZE 24

#define ACTR_DISP_STA_LABELLEN 10
#define ACTR_DISP_STA_LABELHEIGHT (ACTR_DISP_FONTSIZE)
#define ACTR_DISP_STA_LABELWIDTH (ACTR_DISP_FONTSIZE / 2 * ACTR_DISP_STA_LABELLEN)

#define ACTR_DISP_DYN_STRINGLEN_L 16
#define ACTR_DISP_DYN_STRINGLEN_S 8
#define ACTR_DISP_DYN_STRINGHEIGHT (ACTR_DISP_FONTSIZE)
#define ACTR_DISP_DYN_STRINGWIDTH_L (ACTR_DISP_FONTSIZE / 2 * ACTR_DISP_DYN_STRINGLEN_L)
#define ACTR_DISP_DYN_STRINGWIDTH_S (ACTR_DISP_FONTSIZE / 2 * ACTR_DISP_DYN_STRINGLEN_S)

#define ACTR_DISP_ROWLEDGE (ACTR_DISP_FONTSIZE + ACTR_DISP_FONTSIZE / 4)
#define ACTR_DISP_SET_COL(COL) ((COL)*ACTR_DISP_FONTSIZE / 2 + ACTR_DISP_ABSOLUTE_X)
#define ACTR_DISP_SET_ROW(ROW) ((ROW)*ACTR_DISP_ROWLEDGE + ACTR_DISP_ABSOLUTE_Y)

#define ACRT_DISP_COL_TAB 2

extern int InterfaceIndex;

typedef enum mode_cursor
{
	MODECURSOR_ACTRON = 1,
	MODECURSOR_SETHMODE,
	MODECURSOR_SETPMODE,
	MODECURSOR_ACTRPARA,
	MODECURSOR_ACTROFF,
	MODECURSOR_DEVPARA,
} ModeCursorTypedef;

typedef enum homing_flag
{
	HOMING_FLAG_ERROR = 0,
	HOMING_FLAG_CORRECT
} HomingFlagTypedef;

void ShowInfo(void);
void ShowModeSelection(int index);
void ShowActrPwrStateTitle(void);
void ShowActrPwrState(void);
void ShowActrPosValTitle(void);
void ShowActrPosVal(void);
void ShowActrHomingPosVal(void);
void ShowActrHomingTitle(int index);
void ShowActrHoming(HomingFlagTypedef homingflag);
void ShowActrAllParaTitle(void);
void ShowActrAllPara(void);
void ShowDevParaTitle(void);
void ShowDevPara(void);

extern int ModeCursorIndex;
extern int ActrPwrCursorIndex;
extern int ActrPosCursorIndex;
extern int ActrHomingCursorIndex;
extern int ActrAllParaCursorIndex;
extern int DevParaCursorIndex;

void MoveModeCursor(void);
void MoveActrPwrCursor(void);
void MoveActrPosCursor(void);
void MoveActrHomingCursor(void);
void MoveDevParaCursor(void);

#endif
