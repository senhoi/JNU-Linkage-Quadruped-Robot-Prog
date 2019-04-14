/*
******************************************************************************************************
*                         INNFOS SCA Controller Ref Design
*
*   Copyright 2018 - 2022 INNFOS (Beijing) Technology Co., Ltd.
*   www.innfos.com
*   All rights reserved.
*	模块名称 : SCA控制模块头文件。
*	文件名称 : SCA_ctrl.h
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
******************************************************************************************************
*/

#ifndef _SCA_CTRL_H_
#define _SCA_CTRL_H_

#include "stdint.h"
#include "sys.h"
#include "delay.h"

#define CAN_BUSY_TIMEOUT 100000
#define CAN_WAIT_RECV_TIMEOUT 20
#define ACTR_DEV_NUM 12

#define CAN_BUS_STATE_FREE 0
#define CAN_BUS_STATE_RESET -1

#define ACTR_SET_MODE_SUCCESS 0
#define ACTR_SET_MODE_FIND_DEV_FAIL -1
#define ACTR_SET_MODE_SEND_FAIL -2
#define ACTR_SET_MODE_ACK_FAIL -3

#define CAN_RECV_UPDATE_SET 1
#define CAN_RECV_UPDATE_RESET 0

#define ACTR_CMD_SHAKE_HAND 0x00
#define ACTR_CMD_GET_CURRENT 0x04
#define ACTR_CMD_GET_SPEED 0x05
#define ACTR_CMD_GET_POSTION 0x06
#define ACTR_CMD_SET_MODE 0x07
#define ACTR_CMD_SET_CURRENT 0x08
#define ACTR_CMD_SET_SPEED 0x09
#define ACTR_CMD_SET_POSTION 0x0A
#define ACTR_CMD_SET_ON_OFF 0x2A
#define ACTR_CMD_GET_ON_OFF 0x2B
#define ACTR_CMD_SET_CURRENT_OUTPUT_LOWER_LIMIT 0x2E
#define ACTR_CMD_SET_CURRENT_OUTPUT_UPPER_LIMIT 0x2F
#define ACTR_CMD_SET_SPEED_OUTPUT_LOWER_LIMIT 0x30
#define ACTR_CMD_SET_SPEED_OUTPUT_UPPER_LIMIT 0x31
#define ACTR_CMD_SET_POSTION_OUTPUT_LOWER_LIMIT 0x32
#define ACTR_CMD_SET_POSTION_OUTPUT_UPPER_LIMIT 0x33
#define ACTR_CMD_GET_MOTOR_TEMP 0x5F
#define ACTR_CMD_GET_INVTR_TEMP 0x60
#define ACTR_CMD_GET_CUR_MODE 0x55
#define ACTR_CMD_GET_EXECPTION 0xFF
#define ACTR_CMD_SET_TSHAP_POS_MAX_SPEED 0x1F
#define ACTR_CMD_SET_TSHAP_POS_ACCELERATE 0x20
#define ACTR_CMD_SET_TSHAP_POS_DECELERATE 0x21
#define ACTR_CMD_GET_TSHAP_POS_MAX_SPEED 0x1C
#define ACTR_CMD_GET_TSHAP_POS_ACCELERATE 0x1D
#define ACTR_CMD_GET_TSHAP_POS_DECELERATE 0x1E
#define ACTR_CMD_GET_CURRENT_OUTPUT_LOWER_LIMIT 0x34
#define ACTR_CMD_GET_CURRENT_OUTPUT_UPPER_LIMIT 0x35
#define ACTR_CMD_GET_SPEED_OUTPUT_LOWER_LIMIT 0x36
#define ACTR_CMD_GET_SPEED_OUTPUT_UPPER_LIMIT 0x37
#define ACTR_CMD_GET_POSTION_OUTPUT_LOWER_LIMIT 0x38
#define ACTR_CMD_GET_POSTION_OUTPUT_UPPER_LIMIT 0x39
#define ACTR_CMD_GET_POSTION_LOWER_LIMIT 0x85
#define ACTR_CMD_GET_POSTION_UPPER_LIMIT 0x86
#define ACTR_CMD_GET_SHUTDOWN_STATE 0xB0

#define CAN_FRAME_BIT_CMD 0
#define CAN_FRAME_BIT_DAT_HH 1
#define CAN_FRAME_BIT_DAT_HL 2
#define CAN_FRAME_BIT_DAT_LH 3
#define CAN_FRAME_BIT_DAT_LL 4

#define CAN_FRAME_ANALYS_SUCCESS 0
#define CAN_FRAME_SET_MOD_FAIL -1
#define CAN_FRAME_GET_MOD_FAIL -2
#define CAN_FRAME_DAT_LEN_ERR -3

#define CAN_FRAME_ACK_SUCCESS 1
#define CAN_FRAME_ACK_FAIL 0
#define CAN_FRAME_ACK_CLEAR 0

#define SET_PARA_SUCCESS 0
#define SET_PARA_ERR_FIND_DEV -1
#define SET_PARA_ERR_OUT_RANGE -2
#define SET_PARA_ERR_CAN_T_ERR -3
#define SET_PARA_ERR_ACK_ERR -4

#define GET_PARA_SUCCESS 0
#define GET_PARA_ERR_FIND_DEV -1
#define GET_PARA_ERR_OUT_RANGE -2
#define GET_PARA_ERR_CAN_T_ERR -3
#define GET_PARA_ERR_ACK_ERR -4

#define ACTR_STATE_ON_LINE 1
#define ACTR_STATE_OFF_LINE 0
#define ACTR_OFF_LINE_LIMIT 10

typedef enum ActrRunModeTypedef
{
    ACTR_MODE_CUR = 1,   //电流模式
    ACTR_MODE_SPD,       //速度模式
    ACTR_MODE_POS,       //位置模式
    ACTR_MODE_TECH,      //示教模式
    ACTR_MODE_PLAY,      //播放模式
    ACTR_MODE_TSHAP_POS, //profile位置模式
    ACTR_MODE_TSHAP_SPD, //profile速度模式
    ACTR_MODE_HOMING     //homing模式
} ActrRunModeTypedef;

typedef enum ActrPwrStateTypedef
{
    PWR_OFF,
    PWR_ON
} ActrPwrStateTypedef;

typedef enum ActrShurdownStateTypedef
{
    SHUTDOWN_FALSE,
    SHUTDOWN_TURE
} ActrShutdownStateTypedef;

typedef struct ActrWarnStateBitTypedef
{
    uint16_t WARN_BIT_OVER_VOLT : 1;   //过压异常
    uint16_t WARN_BIT_UNDER_VOLT : 1;  //欠压异常
    uint16_t WARN_BIT_LOCK_ROTOR : 1;  //堵转异常
    uint16_t WARN_BIT_OVER_TEMP : 1;   //过热异常
    uint16_t WARN_BIT_RW_PARA : 1;     //读写参数异常
    uint16_t WARN_BIT_MUL_CIRCLE : 1;  //多圈计数异常
    uint16_t WARN_BIT_TEMP_SENSOR : 1; //温度传感器异常
    uint16_t WARN_BIT_CAN_BUS : 1;     //CAN通讯异常
    uint16_t WARN_BIT_RESERVE_B : 2;   //保留位
    uint16_t WARN_BIT_DRV_PROTEC : 1;  //DRV保护
    uint16_t WARN_BIT_RESERVE_C : 5;   //保留位
} ActrWarnStateBitTypedef;

typedef union ActrWarnStateTypedef {
    uint16_t ActrWarnStateAll;
    ActrWarnStateBitTypedef ActrWarnStateBit;
} ActrWarnStateTypedef;

typedef struct ActrParaTypedef
{
    uint8_t actrID;                             //执行器ID
    uint8_t actrParaUpdFlag;                    //参数更新标志
    uint8_t actrOnlineState;                    //执行器在线状态
    uint8_t actrRecvACKState;                   //接收到执行器应答状态
    uint8_t actrOfflineCounter;                 //执行器离线状态计数器
    ActrRunModeTypedef actrMode;                //执行器当前运行模式
    ActrPwrStateTypedef actrPwrState;           //执行器开关机状态
    ActrWarnStateTypedef actrWarnState;         //执行器报警状态，包含执行器各种异常报警
    ActrShutdownStateTypedef actrShutdownState; //执行器上次关机状态
    float actrCurrent;                          //执行器当前的电流
    float actrSpeed;                            //执行器当前的速度
    float actrPostion;                          //执行器当前的位置
    float actrDestCurrent;                      //执行器目标电流，电流模式有效
    float actrDestSpeed;                        //执行器目标速度，速度模式有效
    float actrDestPostion;                      //执行器目标位置，位置模式有效
    float actrMotorTemp;                        //执行器电机温度
    float actrInvterTemp;                       //执行器逆变器温度
    float actrTshapPosMaxSpeed;                 //执行器位置梯形曲线的最大速度
    float actrTshapPosAccelerate;               //执行器位置梯形曲线的加速度
    float actrTshapPosDecelerate;               //执行器位置梯形曲线的减速度
    float actrPosOutputUpperLimit;              //执行器位置环输出上限
    float actrPosOutputLowerLimit;              //执行器位置环输出下限
    float actrSpeedOutputUpperLimit;            //执行器速度环输出上限（修改此项可调整位置环模式下电机刚度）
    float actrSpeedOutputLowerLimit;            //执行器速度环输出下限（修改此项可调整位置环模式下电机刚度）
    float actrCurrentOutputUpperLimit;          //执行器电流环输出上限
    float actrCurrentOutputLowerLimit;          //执行器电流环输出下限
    float actrPosUpperLimit;                    //执行器位置环上限
    float actrPosLowerLimit;                    //执行器位置环下限
} ActrParaTypedef;

extern uint8_t devIDList[ACTR_DEV_NUM];

int SetActrMode(ActrRunModeTypedef actrMode, uint8_t actrID);
int SetActrPosition(float posSet, uint32_t actrID);
int SetActrSpeed(float speedSet, uint32_t actrID);
int SetActrSpeedOutputLowerLimit(float speedSetLowerLimit, uint32_t actrID);
int SetActrSpeedOutputUpperLimit(float speedSetUpperLimit, uint32_t actrID);
//int SetActrTshapPosMaxSpeed(float maxSpd, uint32_t actrID);	//不确定是否可用
//int SetActrTshapPosAccelerate(float acc, uint32_t actrID);	//不确定是否可用
//int SetActrTshapPosDecelerate(float dec, uint32_t actrID);	//不确定是否可用
int SetActrPwrState(ActrPwrStateTypedef PwrState, uint32_t actrID);
int GetActrPara(uint8_t actrGetParaCmd, uint32_t actrID);
int ActrHandShake(uint32_t actrID);

ActrParaTypedef *FindActrDevByID(uint8_t actrID);
void CanRecvFramAnalyse(CanRxMsg *pCanRxMsg, ActrParaTypedef *pActrParaDev);
int Can1BusyCheck(void);
void ActrDevInit(void);

#endif
