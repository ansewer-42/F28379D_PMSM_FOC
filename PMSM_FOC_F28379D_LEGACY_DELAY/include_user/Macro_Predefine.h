/*
 * Macro_Predefine.h
 * ----------------------------------------------------------------------------------
 * Description
 * This file is used for the macro definition;
 * ----------------------------------------------------------------------------------
 *  Created on: 2020年3月28日
 *      Author: YY
 * ----------------------------------------------------------------------------------
 */

#ifndef INCLUDE_USER_MACRO_PREDEFINE_H_
#define INCLUDE_USER_MACRO_PREDEFINE_H_


/*-------------------------------------------------------------------------------
Include files used in this file.
-------------------------------------------------------------------------------*/
#include "F28x_Project.h"

#ifndef MATH_TYPE
#define MATH_TYPE   1
#endif
#include "IQmathLib.h"
#include "math.h"
//------------------------------------------------------------------------------------


/************************************************************************************/
// TODO Macro defined for the control algorithm
/************************************************************************************/
// Here below, pick current controller mode of voltage update mode
#define  FCL_MODE       1
#define  DELAY_MODE     2

// This line sets the SAMPLING FREQUENCY to one of the available choices
#define  SINGLE_SAMPLING 	1
#define  DOUBLE_SAMPLING	2

// Here below, pick current loop controller option
#define  CMPLX_CNTLR     1
#define  PI_CNTLR        2

// Here below, pick controller code execution ISR
#define  ADC_ISR     1
#define  PWM_ISR     2

// User can select choices from available control configurations
#define  CUR_CONTROL_MODE    DELAY_MODE
#define  SAMPLING_METHOD     SINGLE_SAMPLING
#define  FCL_CNTLR 		     PI_CNTLR
#define  CONTROL_ISR_MODE    ADC_ISR
//------------------------------------------------------------------------------------
// Define the system frequency (MHz)
#define SYSTEM_FREQUENCY        200
#define PWM_FREQUENCY			5
#define PWMDAC_FREQ             50

#if (SAMPLING_METHOD == SINGLE_SAMPLING)
    #define ISR_FREQUENCY 			(PWM_FREQUENCY)
#endif
    #if (SAMPLING_METHOD == DOUBLE_SAMPLING)
#define ISR_FREQUENCY 			(2*PWM_FREQUENCY)
#endif

#define SAMPLING_FREQ   		(ISR_FREQUENCY*1000)
#define CUR_LOOP_BANDWIDTH     	(2.0F*PI*SAMPLING_FREQ / 18)

//FCL Computation time predetermined from library, tests on F2837xD
#define FCL_COMPUTATION_TIME    (1.00)  //in uS,可用于快速电流环中的最大占空比计算；

//#define T_Sample   0.0001
#define T_Sample  ((float)0.001/(float)ISR_FREQUENCY)
#define T_Offset    0
//------------------------------------------------------------------------------------


/************************************************************************************/
// TODO Macro defined for the peripheral registers operation
/************************************************************************************/
#define OC_PRO_DISABLE      0       // Overcurrent Protection Disable
#define OC_PRO_ENABLE       1       // Overcurrent Protection Enable
#define EPWM_XBAR_DC    OC_PRO_DISABLE  //选择是否使能EPWM模块的XBAR与DC子模块寄存器配置
#define EPWM_TZ_OST     OC_PRO_DISABLE  //选择是否配置EPWM的TZ_OST触发功能
#define DUAL_MOTOR_SYNC    0        // 选择是否双电机同步，用于一个电机阶跃，另一个电机动态阶跃数据；

// define the QEP&HALL Sensor connector
#define BOTTOM_BOARD    1
#define TRANSFER_BOARD  2
#define POSITION_CONNECTOR BOTTOM_BOARD
//------------------------------------------------------------------------------------
#define RED_LED_ON		 GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;	//低电平点亮LED
#define RED_LED_OFF		 GpioDataRegs.GPBSET.bit.GPIO34   = 1;	//高电平熄灭LED
#define RED_LED_TOGGLE   GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;	//翻转LED
#define BLUE_LED_ON		 GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;
#define BLUE_LED_OFF	 GpioDataRegs.GPASET.bit.GPIO31   = 1;
#define BLUE_LED_TOGGLE	 GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;

#if(POSITION_CONNECTOR == BOTTOM_BOARD)                    // motor1 三相UVW 霍尔信号读取
    #define HALL_U1      GpioDataRegs.GPBDAT.bit.GPIO32         // 光编与霍尔在底层调理板连接
    #define HALL_V1      GpioDataRegs.GPCDAT.bit.GPIO67
    #define HALL_W1      GpioDataRegs.GPDDAT.bit.GPIO111
#else
    #define HALL_U1      GpioDataRegs.GPCDAT.bit.GPIO94         // 光编与霍尔在中间层转接板连接
    #define HALL_V1      GpioDataRegs.GPDDAT.bit.GPIO97
    #define HALL_W1      GpioDataRegs.GPCDAT.bit.GPIO95
#endif

#define SWITCH1   GpioDataRegs.GPEDAT.bit.GPIO131         // 自锁开关S1输入状态读取
#define SWITCH2   GpioDataRegs.GPEDAT.bit.GPIO130         // 自锁开关S2输入状态读取
#define SWITCH3   GpioDataRegs.GPADAT.bit.GPIO27          // 自锁开关S3输入状态读取
#define SWITCH4   GpioDataRegs.GPADAT.bit.GPIO26          // 自锁开关S4输入状态读取

#define IPM_ERROR1	 GpioDataRegs.GPADAT.bit.GPIO22         //motor1 IPM故障信号读取

#define ENABLE_PWM1   GpioDataRegs.GPACLEAR.bit.GPIO29 = 1; //使能motor1三相PWM缓冲器芯片
#define DISABLE_PWM1  GpioDataRegs.GPASET.bit.GPIO29   = 1;
//------------------------------------------------------------------------------------
//通过软件强制触发 One-Shot Trip Event(单次TZ事件) 使得PWM引脚都置低，TZ中断使能则会产生中断
#define DISABLE_PWM_TZ_M1 						\
		EALLOW;									\
		EPwm1Regs.TZFRC.bit.OST=1;				\
		EPwm2Regs.TZFRC.bit.OST=1;				\
		EPwm3Regs.TZFRC.bit.OST=1;				\
		EDIS;

#define ENABLE_PWM_TZ_M1						\
		EALLOW;									\
		EPwm1Regs.TZCLR.bit.OST=1;				\
		EPwm2Regs.TZCLR.bit.OST=1;				\
		EPwm3Regs.TZCLR.bit.OST=1;				\
		EDIS;


//通过软件事件强制触发使得PWM拉低，不产生TZ中断,因为原先DB模块配置为互补输出，因此需要更改DBCTL寄存器的极性；
#define DISABLE_PWM_PINS_M1 					\
		EPwm1Regs.AQCSFRC.all=CSFA_LOW;			\
		EPwm2Regs.AQCSFRC.all=CSFA_LOW;			\
		EPwm3Regs.AQCSFRC.all=CSFA_LOW;			\
		EPwm1Regs.DBCTL.bit.POLSEL = 0;			\
		EPwm2Regs.DBCTL.bit.POLSEL = 0;			\
		EPwm3Regs.DBCTL.bit.POLSEL = 0;

#define ENABLE_PWM_PINS_M1 						\
		EPwm1Regs.AQCSFRC.all=0;				\
		EPwm2Regs.AQCSFRC.all=0;				\
		EPwm3Regs.AQCSFRC.all=0;				\
		EPwm1Regs.DBCTL.all |= 0x0008;			\
		EPwm2Regs.DBCTL.all |= 0x0008;			\
		EPwm3Regs.DBCTL.all |= 0x0008;
//---------------------------------------------------------------------------------------


/************************************************************************************/
// TODO Macro defined of constants for calculation and the motor parameters
/************************************************************************************/
#ifndef TRUE
#define FALSE 0
#define TRUE  1
#endif

#define PI 3.14159265358979
#define pi 3.14159265358979

// Define the PMSM motor parameters
#define MOTOR_PARAM_VAR     1           //MOTOR_PARAM_VAR为1代表使用变量，为0表示使用宏定义
#if (!MOTOR_PARAM_VAR)
#define Rs_IQ        0.31               // Stator resistance (ohm)
#define Ld_IQ        0.00092            // D Axis inductance (H)
#define Lq_IQ        0.00092            // Q Axis inductance (H)
#define Flux_Mag_IQ  0.01428            //永磁体磁链幅值，实际测量得到,线反电势5.4V/krpm
#define Inertia_IQ   0.0004808          //电机转子转动惯量，根据手册给定参数得到，不一定准确
#endif

#define POLES       4                   // Number of poles
#define POLE_PAIRS  2                   // Number of pole pairs of motor
//------------------------------------------------------------------------------

//物理量基值计算与预定义，Define the base quantites;
//Voltage = Speed_we*Flux,  Flux=Inductance*Current;
//Speed_we=POLE_PAIRS*2*pi*n/60=0.20944*n
//Theta_Elec=Speed_we*Time=1;//Time=1/Speed_we;
//Voltage=d(FLux)/d(Time);Time=Flux/Voltage=Flux/(Speed_we*Flux)=1/Speed_we;
//Inertia*d(Speed)/d(Time_spd)=Delat_Torque;Inertia=Flux*Current*Time_spd/speed;
#define BASE_VOLTAGE        30          // Base peak phase voltage (volt), maximum measurable DC Bus(66.32V)/sqrt(3)   -----need to be modified
#define BASE_CURRENT        10          // Base peak phase current (amp) , maximum measurable peak current
#define BASE_SPEED          5000        // pn=60f,f=5000*2/60=166.66,上述电频率的基准值要与转速的基准值相对应
#define BASE_SPEED_WE       1047        // POLE_PAIRS*2*pi*BASE_SPEED/60=0.20944*5000=1047.2
#define BASE_SPEED_WR       524         // BASE_SPEED_WE/POLE_PAIRS=1047/2=524
#define BASE_FLUX           0.028653    // BASE_VOLTAGE/BASE_SPEED_WE=30/1047=0.028653
#define BASE_INDUCTANCE     0.002865    // BASE_FLUX/BASE_CURRENT=0.028653/10=0.0028653
#define BASE_Rs             3           // BASE_VOLTAGE/BASE_CURRENT=30/10=3
#define BASE_TIME           0.0009546   // 1/BASE_SPEED_WE=1/1047=0.0009546
//对于微分运算，分母为采样时间Ts=100us=0.0001s,其标幺值为0.0001/0009546,取倒数为9.546;


#define INV_BASE_VOLTAGE    0.0333333   // 1/BASE_VOLTAGE
#define INV_BASE_CURRENT    0.1         // 1/BASE_CURRENT
#define INV_BASE_SPEED      0.0002      // 1/BASE_SPEED
#define INV_BASE_SPEED_WE   0.0009546   // 1/BASE_SPEED_WE
#define INV_BASE_Rs         0.333333     // 1/BASE_Rs
#define INV_BASE_TIME       1047        // 1/BASE_TIME

//---------------------------------------------------------------------------------------
//用于控制算法实现中常用的与电机参数相关项预定义,_一个下划线表示乘法，__两个下划线表示除法
//该处使用的是宏定义，调用的时候才进行寻址替代，在Ld_IQ/Lq_IQ定义为变量时，在调用下列语句之前已经在Global_Functions_Variables_Call.h中声明；
//本文件内不需要对变量Ld_IQ/Lq_IQ进行extern声明即可使用，在调用前已将Global_Functions_Variables_Call.h声明编译

//*通用宏定义声明*//
#define R_Ts__Ld  _IQdiv(_IQmpy(Rs_IQ,T_Sample),Ld_IQ)
#define Ts__Ld    _IQdiv(T_Sample,Ld_IQ)
#define Ld__Ts    _IQdiv(Ld_IQ,T_Sample)

#define R_Ts__Lq  _IQdiv(_IQmpy(Rs_IQ,T_Sample),Lq_IQ)
#define Ts__Lq    _IQdiv(T_Sample,Lq_IQ)
#define Lq__Ts    _IQdiv(Lq_IQ,T_Sample)

//*专于电流预测的宏定义声明*//
#define Tsam_Predict     _IQmpy(T_Sample,Process_Data.Current_Predict_Beats)
#define R_Ts__Ld_P       _IQdiv(_IQmpy(Rs_IQ,Tsam_Predict),Ld_IQ)
#define Ts__Ld_P         _IQdiv(Tsam_Predict,Ld_IQ)
#define Ld__Ts_P         _IQdiv(Ld_IQ,Tsam_Predict)

#define R_Ts__Lq_P       _IQdiv(_IQmpy(Rs_IQ,Tsam_Predict),Lq_IQ)
#define Ts__Lq_P         _IQdiv(Tsam_Predict,Lq_IQ)
#define Lq__Ts_P         _IQdiv(Lq_IQ,Tsam_Predict)
//---------------------------------------------------------------------------------------

#endif /* INCLUDE_USER_MACRO_PREDEFINE_H_ */
