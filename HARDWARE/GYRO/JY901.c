#include "JY901.h"

struct STime stcTime;
struct SAcc stcAcc;
struct SGyro stcGyro;
struct SAngle stcAngle;
struct SMag stcMag;
struct SDStatus stcDStatus;
struct SPress stcPress;
struct SLonLat stcLonLat;
struct SGPSV stcGPSV;
struct SQ stcQ;

//CopeSerialData为串口2中断调用函数，串口每收到一个数据，调用一次这个函数。
void AnalyzeGyroData(unsigned char ucData)
{
	/*static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;

	ucRxBuffer[ucRxCnt++] = ucData; //将收到的数据存入缓冲区中
	if (ucRxBuffer[0] != 0x55)		//数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt = 0;
		return;
	}
	if (ucRxCnt < 11)
	{
		return;
	} //数据不满11个，则返回
	else
	{
		switch (ucRxBuffer[1]) //判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
		{
		case 0x50:
			memcpy(&stcTime, &ucRxBuffer[2], 8);
			break; //memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
		case 0x51:
			memcpy(&stcAcc, &ucRxBuffer[2], 8);
			break;
		case 0x52:
			memcpy(&stcGyro, &ucRxBuffer[2], 8);
			break;
		case 0x53:
			memcpy(&stcAngle, &ucRxBuffer[2], 8);
			break;
		case 0x54:
			memcpy(&stcMag, &ucRxBuffer[2], 8);
			break;
		case 0x55:
			memcpy(&stcDStatus, &ucRxBuffer[2], 8);
			break;
		case 0x56:
			memcpy(&stcPress, &ucRxBuffer[2], 8);
			break;
		case 0x57:
			memcpy(&stcLonLat, &ucRxBuffer[2], 8);
			break;
		case 0x58:
			memcpy(&stcGPSV, &ucRxBuffer[2], 8);
			break;
		case 0x59:
			memcpy(&stcQ, &ucRxBuffer[2], 8);
			break;
		}
		ucRxCnt = 0; //清空缓存区
	}*/
}

void PrintfGyroData(void)
{
	char str[100];
	//输出时间
	sprintf(str, "Time:20%d-%d-%d %d:%d:%.3f\r\n", stcTime.ucYear, stcTime.ucMonth, stcTime.ucDay, stcTime.ucHour, stcTime.ucMinute, (float)stcTime.ucSecond + (float)stcTime.usMiliSecond / 1000);
	printf(str);
	delay_ms(10);
	//输出加速度
	sprintf(str, "Acc:%.3f %.3f %.3f\r\n", (float)stcAcc.a[0] / 32768 * 16, (float)stcAcc.a[1] / 32768 * 16, (float)stcAcc.a[2] / 32768 * 16);
	printf(str);
	delay_ms(10);
	//输出角速度
	sprintf(str, "Gyro:%.3f %.3f %.3f\r\n", (float)stcGyro.w[0] / 32768 * 2000, (float)stcGyro.w[1] / 32768 * 2000, (float)stcGyro.w[2] / 32768 * 2000);
	printf(str);
	delay_ms(10);
	//输出角度
	sprintf(str, "Angle:%.3f %.3f %.3f\r\n", (float)stcAngle.Angle[0] / 32768 * 180, (float)stcAngle.Angle[1] / 32768 * 180, (float)stcAngle.Angle[2] / 32768 * 180);
	printf(str);
	delay_ms(10);
	//输出磁场
	sprintf(str, "Mag:%d %d %d\r\n", stcMag.h[0], stcMag.h[1], stcMag.h[2]);
	printf(str);
	delay_ms(10);
	//输出气压、高度
	sprintf(str, "Pressure:%ld Height%.2f\r\n", stcPress.lPressure, (float)stcPress.lAltitude / 100);
	printf(str);
	delay_ms(10);
	//输出端口状态
	sprintf(str, "DStatus:%d %d %d %d\r\n", stcDStatus.sDStatus[0], stcDStatus.sDStatus[1], stcDStatus.sDStatus[2], stcDStatus.sDStatus[3]);
	printf(str);
	delay_ms(10);
	//输出经纬度
	sprintf(str, "Longitude:%ldDeg%.5fm Lattitude:%ldDeg%.5fm\r\n", stcLonLat.lLon / 10000000, (double)(stcLonLat.lLon % 10000000) / 1e5, stcLonLat.lLat / 10000000, (double)(stcLonLat.lLat % 10000000) / 1e5);
	printf(str);
	delay_ms(10);
	//输出地速
	sprintf(str, "GPSHeight:%.1fm GPSYaw:%.1fDeg GPSV:%.3fkm/h\r\n", (float)stcGPSV.sGPSHeight / 10, (float)stcGPSV.sGPSYaw / 10, (float)stcGPSV.lGPSVelocity / 1000);
	printf(str);
	delay_ms(10);
	//输出四元素
	sprintf(str, "Four elements:%.5f %.5f %.5f %.5f\r\n\r\n", (float)stcQ.q[0] / 32768, (float)stcQ.q[1] / 32768, (float)stcQ.q[2] / 32768, (float)stcQ.q[3] / 32768);
	printf(str);
	delay_ms(10); //等待传输完成
}
