/*
 * EQEP_Init.h
 *
 *  Created on: 2020年6月6日
 *      Author: PMSM
 */

#ifndef INCLUDE_USER_EQEP_INIT_H_
#define INCLUDE_USER_EQEP_INIT_H_

#include "F28x_Project.h"
#include "f2837xbmsk.h"
#include "math.h"
#include "Macro_Predefine.h"

#ifndef MATH_TYPE
#define MATH_TYPE   1
#endif
#include "IQmathLib.h"
#include "math.h"

//*****************************************************************************************
typedef struct {
                        Uint16 Device_Index;     //外设在芯片内的标号，QEP1位1，QEP2为2;
                        Uint32 Encoder_N;       //光编码盘一周脉冲数的N倍，N=4,Encoder_N=4000;
                        Uint32 Utimer_PRD;      //QEP模块的Utimer定时器的计数周期
                        Uint16 Speed_Cal_CNT;   //转速计算周期计数器
                        Uint16 Rotation_DIR;    //电机转向，0位正向，1为反向
                        int32  POSCNT_Temp;     //用于暂存本此PWM周期读取的QEP光栅计数值
                        int16  POSCNT_Offset;   //转子起始点偏置值，实际转子位置需要加上该偏置；
                        int16  POSCNT_New;
                        int16  POSCNT_Old;

                        float  POSCNT_Fractional;
                        float  POSCNT_New_Real;
                        float  POSCNT_Old_Real;
                        float  POSCNT_Diff_New;
                        float  POSCNT_Diff_Old;
                        float  Speed_Real;
                        float  Speed_Real_PU;

                        float Theta_Offset;     //电角度偏置
                        float Theta_Mech;       //电机转子机械角度
                        float Theta_Elec;       //电机转子电角度
                        float Theta_Comp;       //电压变换的电角度补偿
                        float Filter0_Speed;    //转速新值滤波系数
                        float Filter1_Speed;    //转速旧值滤波系数
                        float Speed_PU0;        //滤波前的转速标幺值
                        float Speed_PU;         //滤波后的转速标幺值
                        float Speed_We;         //电角速度标幺值ֵ

                        Uint16 Speed_CNT_ON;    //转速调节器计算作用的标志位
                } QEP;
typedef QEP *QEP_handle;

#define QEP_DEFAULTS   {1,4000,2000000,761,0.38055555, 0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0.1,0.9,0,0,0 ,0}

#define INV_ENCODER_N  0.00025  //  1/Encoder_N=1/4000
//-------------------------------------------------------------------------------------------
extern void EQEP_Setup1(QEP_handle);
extern void EQEP_Speed_Cal(QEP_handle);
extern void EQEP_Angle_Cal(QEP_handle);
extern void Hall_Angle_Cal(QEP *p);

#endif /* INCLUDE_USER_EQEP_INIT_H_ */

