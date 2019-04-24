"# JNU-QuadrupedRobot-Prog-Slave" 

JNU-QuadrupedRobot-Prog-Slave
=====

Chip
-----
* STM32F407ZGT6

Peripherals
-----
`USART1` 与上位机通信串口，可使用Micro USB接口

`UART4`	遥控器接收串口

`TIM3`	主任务调度定时器

`CAN1`	与执行器通信CAN接口

`LED0`	红色LED，闪烁频率代表电机控制频率

`LED1`	绿色LED，闪烁频率代表电机角度帧接收频率