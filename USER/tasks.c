#include "tasks.h"

int ActivateFlag = STOP;
int AutoManualFlag = IDLE;

float Spd_Factor = 0.52f;
float Spd_Mu = 0.015f;

void InitTask(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	USART1_Init(115200);
	UART4_Init(100000);

	LED_Init();
	BEEP_Init();
	KEY_Init();
	LCD_Init();
	IO_Init();
	CAN1_Init();
	TIM3_Init();
	TIM7_Init();
	TIM14_PWM_Init(20000 - 1, 84 - 1);

	ActrDevInit();

	ShowModeSelection(-1);

	InitActrPhasePID();
}

void ControlTask(void)
{
	switch (AutoManualFlag)
	{
	case AUTO_L:
		GetCtrlData(CTRL_SRC_AUTO_L);
		break;

	case AUTO_R:
		GetCtrlData(CTRL_SRC_AUTO_R);
		break;

	case RESUME_L1:
		GetCtrlData(CTRL_SRC_RESUME_L1);
		break;

	case RESUME_R1:
		GetCtrlData(CTRL_SRC_RESUME_R1);
		break;

	case RESUME_L2:
		GetCtrlData(CTRL_SRC_RESUME_L2);
		break;

	case RESUME_R2:
		GetCtrlData(CTRL_SRC_RESUME_R2);
		break;
	
	case SLOPE:
		GetCtrlData(CTRL_SRC_SLOPE);
		break;

	case MANUAL:
		GetCtrlData(CTRL_SRC_REMOTE);
		break;
	}

	UpdateActrPhase();
	CountActrRevolution();

	if (ActivateFlag == RUN)
	{
		if (CtrlVal_Forward > 0.0f)
		{
			if (actrRefPhase < 2.0f)
			{
				actrSpd[LM1_INDEX] = -Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 1.0f, -Spd_Mu, 1 / sqrt(2 * PI));
				actrSpd[LM2_INDEX] = Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 1.0f, -Spd_Mu, 1 / sqrt(2 * PI)) + PID_LM2.Output;
				actrSpd[RM2_INDEX] = -Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 1.0f, -Spd_Mu, 1 / sqrt(2 * PI)) - PID_RM2.Output;
				actrSpd[RM1_INDEX] = Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 1.0f, -Spd_Mu, 1 / sqrt(2 * PI)) + PID_RM1.Output;
			}
			else
			{
				actrSpd[LM1_INDEX] = -Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 3.0f, -Spd_Mu, 1 / sqrt(2 * PI)) - PID_LM1.Output;
				actrSpd[LM2_INDEX] = Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 3.0f, -Spd_Mu, 1 / sqrt(2 * PI));
				actrSpd[RM2_INDEX] = -Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 3.0f, -Spd_Mu, 1 / sqrt(2 * PI)) - PID_RM2.Output;
				actrSpd[RM1_INDEX] = Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 3.0f, -Spd_Mu, 1 / sqrt(2 * PI)) + PID_RM1.Output;
			}
		}
		else
		{
			if (actrRefPhase < 2.0f)
			{
				actrSpd[LM1_INDEX] = -Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 1.0f, Spd_Mu, 1 / sqrt(2 * PI));
				actrSpd[LM2_INDEX] = Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 1.0f, Spd_Mu, 1 / sqrt(2 * PI)) + PID_LM2.Output;
				actrSpd[RM2_INDEX] = -Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 1.0f, Spd_Mu, 1 / sqrt(2 * PI)) - PID_RM2.Output;
				actrSpd[RM1_INDEX] = Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 1.0f, Spd_Mu, 1 / sqrt(2 * PI)) + PID_RM1.Output;
			}
			else
			{
				actrSpd[LM1_INDEX] = -Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 3.0f, Spd_Mu, 1 / sqrt(2 * PI)) - PID_LM1.Output;
				actrSpd[LM2_INDEX] = Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 3.0f, Spd_Mu, 1 / sqrt(2 * PI));
				actrSpd[RM2_INDEX] = -Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 3.0f, Spd_Mu, 1 / sqrt(2 * PI)) - PID_RM2.Output;
				actrSpd[RM1_INDEX] = Spd_Factor * CtrlVal_Forward / 128.0f * normpdf_revised(actrRefPhase - 3.0f, Spd_Mu, 1 / sqrt(2 * PI)) + PID_RM1.Output;
			}
		}

		CalcActrPhasePID();

		if (fabs(actrRefPhase - 1.0f) < 0.5f)
		{
			SetActrPosition(TM_LIMIT * CtrlVal_Turning / 128.0f, devIDList[TM_INDEX]);
		}
		else if (fabs(actrRefPhase - 3.0f) < 0.5f)
		{
			SetActrPosition(-TM_LIMIT * CtrlVal_Turning / 128.0f, devIDList[TM_INDEX]);
		}
	}
	else
	{
		actrSpd[LM1_INDEX] = 0;
		actrSpd[LM2_INDEX] = 0;
		actrSpd[RM2_INDEX] = 0;
		actrSpd[RM1_INDEX] = 0;

		SetActrPosition(0.0f, devIDList[TM_INDEX]);
	}
	SetActrSpeed(actrSpd[LM1_INDEX], devIDList[LM1_INDEX]);
	SetActrSpeed(actrSpd[LM2_INDEX], devIDList[LM2_INDEX]);
	SetActrSpeed(actrSpd[RM2_INDEX], devIDList[RM2_INDEX]);
	SetActrSpeed(actrSpd[RM1_INDEX], devIDList[RM1_INDEX]);
}

/**
 * 函数定义: DisplayTask
 * 描   述:	执行需要重复刷新的低优先级显示任务
 * 入口参数: 
 * 出口参数: 
 * 备   注:
 */
void DisplayTask(void)
{
	ShowInfo();
	switch (InterfaceIndex)
	{
	case ROBOT_INTERFACE_MODE:
		break;
	case ROBOT_INTERFACE_ACTR_PWR:
		break;
	case ROBOT_INTERFACE_HOMING_CHECK:
		ShowActrHomingPosVal();
		break;
	case ROBOT_INTERFACE_ACTR_POS:
		ShowActrPosVal();
		break;
	case ROBOT_INTERFACE_ACTR_PARA:
		ShowActrAllPara();
		break;
	case ROBOT_INTERFACE_DEV_PARA:
		ShowDevPara();
		break;
	}
}

int ReportTaskFlag;

/**
 * 函数定义: ReportTask
 * 描   述:	读取执行器参数
 * 入口参数: 
 * 出口参数: 
 * 备   注:
 */
void ReportTask(TaskFlagTypedef taskflag)
{
	for (int i = 0; i < ACTR_DEV_NUM; i++)
	{
		if (Can1BusyCheck() == CAN_BUS_STATE_FREE)
		{
			switch (taskflag)
			{
			case TASK_FLAG_REPORT_POS:
				GetActrPara(ACTR_CMD_GET_POSTION, devIDList[i]);
				break;
			case TASK_FLAG_REPORT_VEL:
				GetActrPara(ACTR_CMD_GET_SPEED, devIDList[i]);
				break;
			case TASK_FLAG_REPORT_CUR:
				GetActrPara(ACTR_CMD_GET_CURRENT, devIDList[i]);
				break;
			case TASK_FLAG_REPORT_EXECPTION:
				GetActrPara(ACTR_CMD_GET_EXECPTION, devIDList[i]);
				break;
			case TASK_FLAG_REPORT_ALL:
				GetActrPara(ACTR_CMD_GET_ON_OFF, devIDList[i]);
				GetActrPara(ACTR_CMD_GET_CUR_MODE, devIDList[i]);
				GetActrPara(ACTR_CMD_GET_CURRENT, devIDList[i]);
				GetActrPara(ACTR_CMD_GET_SPEED, devIDList[i]);
				GetActrPara(ACTR_CMD_GET_POSTION, devIDList[i]);
				GetActrPara(ACTR_CMD_GET_EXECPTION, devIDList[i]);
				GetActrPara(ACTR_CMD_GET_TSHAP_POS_MAX_SPEED, devIDList[i]);
				GetActrPara(ACTR_CMD_GET_TSHAP_POS_ACCELERATE, devIDList[i]);
				GetActrPara(ACTR_CMD_GET_TSHAP_POS_DECELERATE, devIDList[i]);
				GetActrPara(ACTR_CMD_GET_POSTION_LOWER_LIMIT, devIDList[i]);
				GetActrPara(ACTR_CMD_GET_POSTION_UPPER_LIMIT, devIDList[i]);
				GetActrPara(ACTR_CMD_GET_SPEED_OUTPUT_LOWER_LIMIT, devIDList[i]);
				GetActrPara(ACTR_CMD_GET_SPEED_OUTPUT_UPPER_LIMIT, devIDList[i]);
				GetActrPara(ACTR_CMD_GET_SHUTDOWN_STATE, devIDList[i]);
				break;
			default:
				break;
			}
		}
	}
}

unsigned int LowPirorityTaskFlag = 0;
//	[0]	:REPORT_FLAG
//	[1]	:KEY_FLAG
//	[31-2]:	IDLE

void CheckIOTask(void);
void ScanIOTask(void);

/**
 * 函数定义: HandleLowPirorityTask
 * 描   述:	根据标志位处理低优先级任务
 * 入口参数: 
 * 出口参数: 
 * 备   注:
 */
void HandleLowPirorityTask(void)
{
	KeyTask();
	if (TASK_SEARCH_FLAG(TASK_FLAG_CONTROL) == 0) //如果未处理ControlTask和UploadTask，不进行显示和请求电机数据
	{
		DisplayTask();
		if (TASK_SEARCH_FLAG(TASK_FLAG_REPORT_POS))
		{
			TASK_RESET_FLAG(TASK_FLAG_REPORT_POS);
			ReportTask(TASK_FLAG_REPORT_POS);
		}
		if (TASK_SEARCH_FLAG(TASK_FLAG_REPORT_VEL))
		{
			TASK_RESET_FLAG(TASK_FLAG_REPORT_VEL);
			ReportTask(TASK_FLAG_REPORT_VEL);
		}
		if (TASK_SEARCH_FLAG(TASK_FLAG_REPORT_CUR))
		{
			TASK_RESET_FLAG(TASK_FLAG_REPORT_CUR);
			ReportTask(TASK_FLAG_REPORT_CUR);
		}
		if (TASK_SEARCH_FLAG(TASK_FLAG_REPORT_EXECPTION))
		{
			TASK_RESET_FLAG(TASK_FLAG_REPORT_EXECPTION);
			ReportTask(TASK_FLAG_REPORT_EXECPTION);
		}
		if (TASK_SEARCH_FLAG(TASK_FLAG_REPORT_ALL))
		{
			TASK_RESET_FLAG(TASK_FLAG_REPORT_ALL);
			ReportTask(TASK_FLAG_REPORT_ALL);
		}
	}
	else
	{
		TASK_RESET_FLAG(TASK_FLAG_CONTROL);
		ControlTask();
	}

	//CheckFootGroundingTask();
	HandleDevDataTask();
	//CheckIOTask();
	ScanIOTask();

	if (CheckActrRunningStateTask() == 0)
		ActivateFlag = ERROR;
}

/**
 * 函数定义: KeyTask
 * 描   述:	根据当前界面判断按键功能，并执行无需刷新的低优先级显示任务
 * 入口参数: 
 * 出口参数: 
 * 备   注:
 */
void KeyTask(void)
{
	uint8_t KeyVal;
	KeyVal = KEY_Scan(0);

	void SubKeyTask_MODE(uint8_t KeyVal);
	void SubKeyTask_ACTR_PWR(uint8_t KeyVal);
	void SubKeyTask_ACTR_POS(uint8_t KeyVal);
	void SubKeyTask_HOMING_CHECK(uint8_t KeyVal);
	void SubKeyTask_ACTR_PARA(uint8_t KeyVal);
	void SubKeyTask_DEV_PARA(uint8_t KeyVal);

	switch (InterfaceIndex)
	{
	case ROBOT_INTERFACE_MODE:
		SubKeyTask_MODE(KeyVal);
		break;
	case ROBOT_INTERFACE_ACTR_PWR:
		SubKeyTask_ACTR_PWR(KeyVal);
		break;
	case ROBOT_INTERFACE_HOMING_CHECK:
		SubKeyTask_HOMING_CHECK(KeyVal);
		break;
	case ROBOT_INTERFACE_ACTR_POS:
		SubKeyTask_ACTR_POS(KeyVal);
		break;
	case ROBOT_INTERFACE_ACTR_PARA:
		SubKeyTask_ACTR_PARA(KeyVal);
		break;
	case ROBOT_INTERFACE_DEV_PARA:
		SubKeyTask_DEV_PARA(KeyVal);
		break;
	}
}

/**
 * 函数定义: SubKeyTask_MODE
 * 描   述:	模式选择界面下的按键处理函数
 * 入口参数: 
 * 		KeyVal	键值
 * 出口参数: 
 * 备   注:
 */
void SubKeyTask_MODE(uint8_t KeyVal)
{
	static ActrParaTypedef *pActrParaDev = NULL;

	switch (KeyVal)
	{
	case KEY0_PRES:
		TIM_SetCompare1(TIM14, 1500);
		break;
	case KEY1_PRES:
		switch (ModeCursorIndex)
		{
		case MODECURSOR_ACTRON:
			InterfaceIndex = ROBOT_INTERFACE_ACTR_PWR;
			TASK_STOP_REGULAR_REPORT;
			ShowActrPwrStateTitle();
			for (int i = 0; i < ACTR_DEV_NUM; i++)
			{
				SetActrPwrState(PWR_ON, devIDList[i]);
				BEEP_Normal(1);
			}
			ShowActrPwrState();
			TASK_BEGIN_REGULAR_REPORT;
			break;
		case MODECURSOR_SETHMODE:
			TASK_STOP_REGULAR_REPORT;
			InterfaceIndex = ROBOT_INTERFACE_HOMING_CHECK;
			ShowActrHomingTitle(-1);
			break;
		case MODECURSOR_SETPMODE:
			TASK_STOP_REGULAR_REPORT;
			for (int i = 0; i < ACTR_DEV_NUM; i++)
			{
				GetActrPara(ACTR_CMD_GET_CUR_MODE, devIDList[i]);
				pActrParaDev = FindActrDevByID(devIDList[i]);
				if (i != 2)
				{
					if (pActrParaDev->actrMode != ACTR_MODE_TSHAP_SPD)
					{
						SetActrMode(ACTR_MODE_TSHAP_SPD, devIDList[i]);
					}
				}
				else
				{
					if (pActrParaDev->actrMode != ACTR_MODE_TSHAP_POS)
					{
						SetActrMode(ACTR_MODE_TSHAP_POS, devIDList[i]);
					}
				}
				delay_us(100);
			}
			InterfaceIndex = ROBOT_INTERFACE_ACTR_POS;
			ShowActrPosValTitle();
			break;
		case MODECURSOR_ACTRPARA:
			TASK_BEGIN_REGULAR_REPORT;
			InterfaceIndex = ROBOT_INTERFACE_ACTR_PARA;
			ShowActrAllParaTitle();
			break;
		case MODECURSOR_ACTROFF:
			InterfaceIndex = ROBOT_INTERFACE_ACTR_PWR;
			TASK_STOP_REGULAR_REPORT;
			ShowActrPwrStateTitle();
			for (int i = 0; i < ACTR_DEV_NUM; i++)
			{
				SetActrPwrState(PWR_OFF, devIDList[i]);
				BEEP_Normal(1);
				//GetActrPara(ACTR_CMD_GET_ON_OFF, devIDList[i]);
			}
			ShowActrPwrState();
			TASK_BEGIN_REGULAR_REPORT;
			break;
		case MODECURSOR_DEVPARA:
			InterfaceIndex = ROBOT_INTERFACE_DEV_PARA;
			TASK_STOP_REGULAR_REPORT;
			ShowDevParaTitle();
			break;
		default:
			break;
		}
		break;
	case KEY2_PRES:
		TIM_SetCompare1(TIM14, 2500);
		break;
	case WKUP_PRES:
		ModeCursorIndex++;
		MoveModeCursor();
		break;
	}
}

/**
 * 函数定义: SubKeyTask_ACTR_PWR
 * 描   述:	电源状态界面下的按键处理函数
 * 入口参数: 
 * 		KeyVal	键值
 * 出口参数: 
 * 备   注:
 */
void SubKeyTask_ACTR_PWR(uint8_t KeyVal)
{
	switch (KeyVal)
	{
	case KEY0_PRES:
		break;
	case KEY1_PRES:
		switch (ActrPwrCursorIndex)
		{
		case 0:
			InterfaceIndex = ROBOT_INTERFACE_MODE;
			ShowModeSelection(-1);
			break;
		default:
			break;
		}
		break;
	case KEY2_PRES:
		break;
	case WKUP_PRES:
		MoveActrPwrCursor();
		break;
	}
}

extern int AutoTime_Flag;
extern int AutoTime_S;
extern int AutoTime_MS;
extern uint8_t StepIndex;
extern uint8_t StepIndex_Pause;
extern uint8_t StepIndex_PauseFlag;

uint8_t ActrEmergeResetFlag;
/**
 * 函数定义: SubKeyTask_ACTR_POS
 * 描   述:	位置显示界面下的按键处理函数
 * 入口参数: 
 * 		KeyVal	键值
 * 出口参数: 
 * 备   注:
 */
void SubKeyTask_ACTR_POS(uint8_t KeyVal)
{
	//static ActrParaTypedef *pActrParaDev = NULL;
	switch (KeyVal)
	{
	case KEY0_PRES:
		if (IO_SOFT_EMERGE == 1)
		{
			switch (AutoManualFlag)
			{
			case AUTO_R:
			case AUTO_L:
				for (int i = 0; i < ACTR_DEV_NUM; i++)
				{
					SetActrMode(ACTR_MODE_TSHAP_POS, devIDList[i]);
					delay_us(100);
					switch (i)
					{
					case LM1_INDEX:
						SetActrPosition(-16.0f, devIDList[i]);
						break;

					case LM2_INDEX:
						SetActrPosition(16.0f, devIDList[i]);
						break;

					case TM_INDEX:
						SetActrPosition(0.0f, devIDList[i]);
						break;

					case RM2_INDEX:
						SetActrPosition(-16.0f, devIDList[i]);
						break;

					case RM1_INDEX:
						SetActrPosition(16.0f, devIDList[i]);
						break;
					}
				}
				break;

			case RESUME_L1:
			case RESUME_R1:
			case RESUME_L2:
			case RESUME_R2:
				for (int i = 0; i < ACTR_DEV_NUM; i++)
				{
					SetActrMode(ACTR_MODE_TSHAP_POS, devIDList[i]);
					delay_us(100);
					switch (i)
					{
					case LM1_INDEX:
						SetActrPosition(0.0f, devIDList[i]);
						break;

					case LM2_INDEX:
						SetActrPosition(0.0f, devIDList[i]);
						break;

					case TM_INDEX:
						SetActrPosition(0.0f, devIDList[i]);
						break;

					case RM2_INDEX:
						SetActrPosition(0.0f, devIDList[i]);
						break;

					case RM1_INDEX:
						SetActrPosition(0.0f, devIDList[i]);
						break;
					}
				}
				break;
			}
			ActrEmergeResetFlag = 1;
		}
		else
		{
			BEEP_Alert(1);
			ActivateFlag = STOP;
			AutoManualFlag++;
			if (AutoManualFlag == 9)
				AutoManualFlag = IDLE;
		}

		break;

	case KEY1_PRES:
		ActivateFlag = STOP;
		ClearActrPhase();
		ClearActrRevolution();
		AutoTime_S = 0;
		AutoTime_MS = 0;
		AutoTime_Flag = 0;
		switch (ActrPosCursorIndex)
		{
		case 0:
			InterfaceIndex = ROBOT_INTERFACE_MODE;
			ShowModeSelection(-1);
			break;
		default:
			break;
		}
		break;

	case KEY2_PRES:
		if (IO_SOFT_EMERGE == 1)
		{
			switch (AutoManualFlag)
			{
			case AUTO_L:
				AutoManualFlag = RESUME_L1;
				break;

			case RESUME_L1:
				AutoManualFlag = RESUME_L2;
				break;

			case RESUME_L2:
				AutoManualFlag = AUTO_L;
				break;

			case AUTO_R:
				AutoManualFlag = RESUME_R1;
				break;

			case RESUME_R1:
				AutoManualFlag = RESUME_R2;
				break;

			case RESUME_R2:
				AutoManualFlag = AUTO_R;
				break;
			}
		}
		BEEP_Alert(1);

		break;

	case WKUP_PRES:
		if (RemoteData[5] != 2)
		{
			if (AutoManualFlag == AUTO_L || AutoManualFlag == AUTO_R || AutoManualFlag == RESUME_L1 || AutoManualFlag == RESUME_R1 || AutoManualFlag == RESUME_L2 || AutoManualFlag == RESUME_R2)
			{
				//BEEP_Alert(1);
				MoveActrPosCursor();
				AutoTime_Flag = 1;
				ActivateFlag = RUN;
				InitAutoData();
			}
			else
			{
				ActivateFlag = RUN;
			}
		}
		else
			BEEP_Normal(1);
		break;
	}
}

/**
 * 函数定义: SubKeyTask_HOMING_CHECK
 * 描   述:	零位检查界面下的按键处理函数
 * 入口参数: 
 * 		KeyVal	键值
 * 出口参数: 
 * 备   注:
 */
void SubKeyTask_HOMING_CHECK(uint8_t KeyVal)
{
	char id_index;
	static ActrParaTypedef *pActrParaDev = NULL;

	id_index = ActrHomingCursorIndex - 1;

	switch (KeyVal)
	{
	case KEY0_PRES:
		BEEP_Alert(2);
		for (int i = 0; i < ACTR_DEV_NUM; i++)
		{
			SetActrMode(ACTR_MODE_TSHAP_POS, devIDList[i]);
			delay_us(100);
			switch (i)
			{
			case LM1_INDEX:
				SetActrPosition(-16.0f, devIDList[i]);
				break;

			case LM2_INDEX:
				SetActrPosition(16.0f, devIDList[i]);
				break;

			case TM_INDEX:
				SetActrPosition(0.0f, devIDList[i]);
				break;

			case RM2_INDEX:
				SetActrPosition(-16.0f, devIDList[i]);
				break;

			case RM1_INDEX:
				SetActrPosition(16.0f, devIDList[i]);
				break;

			default:
				break;
			}
		}
		break;
	case KEY1_PRES:
		InterfaceIndex = ROBOT_INTERFACE_MODE;
		ShowModeSelection(-1);
		break;
	case KEY2_PRES:

		GetActrPara(ACTR_CMD_GET_CUR_MODE, devIDList[id_index]);

		if (pActrParaDev->actrMode != ACTR_MODE_TSHAP_POS)
		{
			SetActrMode(ACTR_MODE_TSHAP_POS, devIDList[id_index]);
		}

		switch (id_index)
		{
		case LM1_INDEX:
			SetActrPosition(-16.0f, devIDList[id_index]);
			break;

		case LM2_INDEX:
			SetActrPosition(16.0f, devIDList[id_index]);
			break;

		case TM_INDEX:
			SetActrPosition(0.0f, devIDList[id_index]);
			break;

		case RM2_INDEX:
			SetActrPosition(-16.0f, devIDList[id_index]);
			break;

		case RM1_INDEX:
			SetActrPosition(16.0f, devIDList[id_index]);
			break;

		default:
			break;
		}

		BEEP_Normal(1);

		ShowActrHoming(HOMING_FLAG_CORRECT);
		break;
	case WKUP_PRES:
		ActrHomingCursorIndex++;
		MoveActrHomingCursor();
		break;
	}
}

/**
 * 函数定义: SubKeyTask_ACTR_PARA
 * 描   述:	参数界面下的按键处理函数
 * 入口参数: 
 * 		KeyVal	键值
 * 出口参数: 
 * 备   注:
 */
void SubKeyTask_ACTR_PARA(uint8_t KeyVal)
{
	switch (KeyVal)
	{
	case KEY0_PRES:
		break;
	case KEY1_PRES:
		switch (DevParaCursorIndex)
		{
		case 0:
			InterfaceIndex = ROBOT_INTERFACE_MODE;
			ShowModeSelection(-1);
			break;
		default:
			break;
		}
		break;
	case KEY2_PRES:
		break;
	case WKUP_PRES:
		ActrAllParaCursorIndex++;
		if (ActrAllParaCursorIndex == ACTR_DEV_NUM)
		{
			ActrAllParaCursorIndex = 0;
		}
		MoveActrPosCursor();
		break;
	}
}

/**
 * 函数定义: SubKeyTask_DEV_PARA
 * 描   述:	外设参数显示界面下的按键处理函数
 * 入口参数: 
 * 		KeyVal	键值
 * 出口参数: 
 * 备   注:
 */
void SubKeyTask_DEV_PARA(uint8_t KeyVal)
{
	switch (KeyVal)
	{
	case KEY0_PRES:
		break;
	case KEY1_PRES:
		switch (ActrPwrCursorIndex)
		{
		case 0:
			InterfaceIndex = ROBOT_INTERFACE_MODE;
			ShowModeSelection(-1);
			break;
		default:
			break;
		}
		break;
	case KEY2_PRES:
		break;
	case WKUP_PRES:
		MoveDevParaCursor();
		break;
	}
}

const float Cur_Threshold = 1.2f;
uint8_t FootGrounding;
/**
 * 函数定义: CheckFootGroundingTask
 * 描   述:	足部主动柔顺控制
 * 入口参数: 
 * 		
 * 出口参数: 
 * 备   注:	通过检测关节电流实现落地检测
 */
void CheckFootGroundingTask(void)
{
	static ActrParaTypedef *pActrParaDev = NULL;

	pActrParaDev = FindActrDevByID(devIDList[0]);
	if (fabs(pActrParaDev->actrCurrent * 33.0f) > Cur_Threshold)
	{
		FootGrounding |= 0x01 << 0;
	}
	else
	{
		FootGrounding &= ~(0x01 << 0);
	}

	pActrParaDev = FindActrDevByID(devIDList[1]);
	if (fabs(pActrParaDev->actrCurrent * 33.0f) > Cur_Threshold)
	{
		FootGrounding |= 0x01 << 1;
	}
	else
	{
		FootGrounding &= ~(0x01 << 1);
	}

	pActrParaDev = FindActrDevByID(devIDList[3]);
	if (fabs(pActrParaDev->actrCurrent * 33.0f) > Cur_Threshold)
	{
		FootGrounding |= 0x01 << 2;
	}
	else
	{
		FootGrounding &= ~(0x01 << 2);
	}

	pActrParaDev = FindActrDevByID(devIDList[4]);
	if (fabs(pActrParaDev->actrCurrent * 33.0f) > Cur_Threshold)
	{
		FootGrounding |= 0x01 << 3;
	}
	else
	{
		FootGrounding &= ~(0x01 << 3);
	}
}

void HandleDevDataTask(void)
{
	Remote_Get();
}

int CheckActrRunningStateTask(void)
{
	if (fabs((actrRefPhase + 4.0f * actrRefRevolution) - (actrPhase[LM1_INDEX] + 4.0f * actrRevolution[LM1_INDEX])) > 1.0f)
		return 0;
	if (fabs((actrRefPhase + 4.0f * actrRefRevolution) - (actrPhase[LM2_INDEX] + 4.0f * actrRevolution[LM2_INDEX])) > 1.0f)
		return 0;
	if (fabs((actrRefPhase + 4.0f * actrRefRevolution) - (actrPhase[RM2_INDEX] + 4.0f * actrRevolution[RM2_INDEX])) > 1.0f)
		return 0;
	if (fabs((actrRefPhase + 4.0f * actrRefRevolution) - (actrPhase[RM1_INDEX] + 4.0f * actrRevolution[RM1_INDEX])) > 1.0f)
		return 0;

	if (RemoteData[5] == 2)
		ActivateFlag = STOP;

	return 1;
}

void ScanIOTask(void)
{
	static uint8_t shawdow_io_microswitch;
	static uint8_t shawdow_io_soft_emerge;
	static uint8_t shawdow_io_start1;
	static uint8_t shawdow_io_start2;
	static uint16_t beep_times;
	static ActrParaTypedef *pActrParaDev = NULL;

	if (IO_SPEEDUP1 == 1 && IO_SPEEDUP2 == 1)
	{
		Spd_Factor = 1.0f;
		Spd_Mu = 0.10f;
	}
	else if (IO_SPEEDUP1 == 1 && IO_SPEEDUP2 == 0)
	{
		Spd_Factor = 0.9f;
		Spd_Mu = 0.080f;
	}
	else if (IO_SPEEDUP1 == 0 && IO_SPEEDUP2 == 1)
	{
		Spd_Factor = 0.8f;
		Spd_Mu = 0.07f;
	}
	else if (IO_SPEEDUP1 == 0 && IO_SPEEDUP2 == 0)
	{
		Spd_Factor = 0.70f;
		Spd_Mu = 0.05f;
	}

	if (IO_SOFT_EMERGE == 1)
	{
		AutoTime_Flag = 0;
		ActivateFlag = STOP;
		InitAutoData();

		if (shawdow_io_soft_emerge == 0)
		{

			if (AutoManualFlag == AUTO_L)
			{
				if (actrRefPhase / 4.0f + actrRefRevolution > 8.0f && actrRefPhase / 4.0f + actrRefRevolution < 10.0f)
					AutoManualFlag = RESUME_L1;
				else if (actrRefPhase / 4.0f + actrRefRevolution >= 10.0f)
					AutoManualFlag = RESUME_L2;
				else
					AutoManualFlag = AUTO_L;
			}
			else if (AutoManualFlag == AUTO_R)
			{
				if (actrRefPhase / 4.0f + actrRefRevolution > 8.0f && actrRefPhase / 4.0f + actrRefRevolution < 10.0f)
					AutoManualFlag = RESUME_R1;
				else if (actrRefPhase / 4.0f + actrRefRevolution >= 10.0f)
					AutoManualFlag = RESUME_R2;
				else
					AutoManualFlag = AUTO_R;
			}
			else if (AutoManualFlag == RESUME_L1)
			{
				if (actrRefPhase / 4.0f + actrRefRevolution > 1.5f && actrRefPhase / 4.0f + actrRefRevolution < 3.0f)
					AutoManualFlag = RESUME_L1;
				else if (actrRefPhase / 4.0f + actrRefRevolution >= 3.0f)
					AutoManualFlag = RESUME_L2;
				else
					AutoManualFlag = AUTO_L;
			}
			else if (AutoManualFlag == RESUME_L2)
			{
				if (actrRefPhase / 4.0f + actrRefRevolution > 0.0f && actrRefPhase / 4.0f + actrRefRevolution < 1.0f)
					AutoManualFlag = RESUME_L1;
				else if (actrRefPhase / 4.0f + actrRefRevolution >= 1.0f)
					AutoManualFlag = RESUME_L2;
				else
					AutoManualFlag = AUTO_L;
			}
			else if (AutoManualFlag == RESUME_R1)
			{
				if (actrRefPhase / 4.0f + actrRefRevolution > 1.5f && actrRefPhase / 4.0f + actrRefRevolution < 3.0f)
					AutoManualFlag = RESUME_R1;
				else if (actrRefPhase / 4.0f + actrRefRevolution >= 3.0f)
					AutoManualFlag = RESUME_R2;
				else
					AutoManualFlag = AUTO_R;
			}
			else if (AutoManualFlag == RESUME_R2)
			{
				if (actrRefPhase / 4.0f + actrRefRevolution > 0.0f && actrRefPhase / 4.0f + actrRefRevolution < 1.0f)
					AutoManualFlag = RESUME_R1;
				else if (actrRefPhase / 4.0f + actrRefRevolution >= 1.0f)
					AutoManualFlag = RESUME_R2;
				else
					AutoManualFlag = AUTO_R;
			}
			else if (AutoManualFlag == SLOPE)
			{
				AutoManualFlag = SLOPE;
			}
		}

		beep_times++;
		if (beep_times == 1000)
		{
			beep_times = 0;
			//BEEP_Error(3);
		}
	}
	else
	{
		if (shawdow_io_soft_emerge == 1 && ActrEmergeResetFlag == 1)
		{
			ActrEmergeResetFlag = 0;

			ClearActrPhase();
			ClearActrRevolution();

			for (int i = 0; i < ACTR_DEV_NUM; i++)
			{
				GetActrPara(ACTR_CMD_GET_CUR_MODE, devIDList[i]);
				pActrParaDev = FindActrDevByID(devIDList[i]);
				if (i != 2)
				{
					if (pActrParaDev->actrMode != ACTR_MODE_TSHAP_SPD)
					{
						SetActrMode(ACTR_MODE_TSHAP_SPD, devIDList[i]);
					}
				}
				else
				{
					if (pActrParaDev->actrMode != ACTR_MODE_TSHAP_POS)
					{
						SetActrMode(ACTR_MODE_TSHAP_POS, devIDList[i]);
					}
				}
				delay_us(100);
			}
			if (RemoteData[5] != 2)
			{
				if (AutoManualFlag == RESUME_L1 || AutoManualFlag == RESUME_R1 || AutoManualFlag == RESUME_L2 || AutoManualFlag == RESUME_R2)
				{
					BEEP_Alert(1);
					AutoTime_Flag = 1;
					ActivateFlag = RUN;
				}
				else
				{
					ActivateFlag = RUN;
				}
			}
			else
				BEEP_Normal(1);
		}

		if (AutoManualFlag == AUTO_L || AutoManualFlag == AUTO_R || AutoManualFlag == RESUME_L1 || AutoManualFlag == RESUME_R1 || AutoManualFlag == RESUME_L2 || AutoManualFlag == RESUME_R2)
		{
			if (IO_MICROSWITCH != shawdow_io_microswitch && IO_MICROSWITCH == 1)
			{
				AutoTime_Flag = 1;
				ActivateFlag = RUN;
				//BEEP_Normal(1);
				InitAutoData();
			}
			if (!IO_ENSURANCE1 && !IO_ENSURANCE2 && ActivateFlag == RUN)
			{
				AutoTime_Flag = 0;
				ActivateFlag = STOP;
				//BEEP_Error(10);
			}
			if (ActivateFlag == RUN && actrRefPhase / 4.0f + actrRefRevolution > LiftPhaseThreshold)
				TIM_SetCompare1(TIM14, 1500);
			if ((IO_START1 != shawdow_io_start1 && IO_START1 == 1) || (IO_START2 != shawdow_io_start2 && IO_START1 == 2))
			{
				if (StepIndex == StepIndex_Pause)
					StepIndex_PauseFlag = STEP_CONTINUE;
			}
		}
	}
	shawdow_io_microswitch = IO_MICROSWITCH;
	shawdow_io_soft_emerge = IO_SOFT_EMERGE;
	shawdow_io_start1 = IO_START1;
	shawdow_io_start2 = IO_START2;
}
