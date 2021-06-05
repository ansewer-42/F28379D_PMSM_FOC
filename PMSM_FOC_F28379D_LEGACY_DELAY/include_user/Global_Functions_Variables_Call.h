/*
 * Global_Functions_Variables_Call.h
 *
 *  Created on: 2020.3.28
 *      Author: YY
 */

#ifndef INCLUDE_USER_GLOBAL_FUNCTIONS_VARIABLES_CALL_H_
#define INCLUDE_USER_GLOBAL_FUNCTIONS_VARIABLES_CALL_H_

/************************************************************************************/
// Include files used for the global functions and variables definition
/************************************************************************************/
#include "F28x_Project.h"

#ifndef MATH_TYPE
#define MATH_TYPE   1
#endif
#include "IQmathLib.h"
#include "math.h"
#include "Macro_Predefine.h"

#include "EPWM_Init.h"
#include "DAC_Init.h"
#include "ADC_Init.h"
#include "EQEP_Init.h"

#include "SVPWM_Gen.h"
#include "PID_Controller.h"
#include "Coordinate_Trans.h"
#include "Ramp_Function.h"

#include "Discrete_Domain_Controller.h"
//------------------------------------------------------------

/************************************************************************************/
// Structure Definition
/************************************************************************************/
struct FLAG_BITS {                            // bits  description
                    Uint32  GAME_ON:1;        // 1用于程序流程中PWM开关控制位，置1开启PWM；
                    Uint32  GAME_RESTART:1;   // 2用于故障发生后手动重启电机控制流程，置1重启；
                    Uint32  GAME_OVER:1;      // 3用于手动控制直接关闭PWM处于停机状态,置1关闭PWM；
                    Uint32  VAR_CLEAR:1;      // 4用于手动清除全局变量值，置1全局变量清除；
                    Uint32  Is_Angle_ON:1;    // 5用于转矩控制模式给定是否采用转矩角分配电流，置1开启转矩角控制；
                    Uint32  FAULT_ON:1;       // 6用于显示故障是否发生，读1故障发生；
                    Uint32  Data_Record_ON:1; // 7用于手动控制是否开启数据记录；
                    Uint32  User_Input_ON:1;  // 8用于手动控制是否使能用户电流给定，置1使能

                    Uint32  ANGLE_COMPEN_ON:1;      // 1用于手动开启1.5拍角度补偿；
                    Uint32  VOL_FDB_DECOUPLE:1;     // 2用于手动使能电压前馈解耦环节，置1使能前馈解耦；
                    Uint32  COMPL_VECT_DECOUPLE:1;  // 3用于手动使能复矢量解耦环节，置1使能复矢量解耦；
                    Uint32  VI_CV_DECOUPLE:1;       // 4用于手动使虚拟阻抗前馈环节，置1开启虚拟阻抗功能；
                    Uint32  CUR_PREDICTIVE_ON:1;    // 5用于手动开启第k+1拍电流值预测，置1开启电流预测更新；
                    Uint32  DISCRETE_CUR_ON:1;      // 6用于手动开启原始离散域设计控制器，置1开启离散域设计控制器；
                    Uint32  DCV_R_CUR_ON:1;         // 7用于手动开启改进离散域设计控制器，置1开启离散域设计控制器；

                    Uint32  CUR_CNT_Kp_ON:1;        // 1用于手动开启是否使能kp_Ratio变量修改比例参数；
                    Uint32  CUR_PREDICT_COMPEN:1;   // 2用于手动开启预测电流误差补偿算法

                    Uint32  rsvd:15;                //  reserved
                 };

typedef union {
                   Uint32            all;
                   struct FLAG_BITS  bit;
              }FLAG_VAR;

#define FLAG_DEFAULTS  {0}
//------------------------------------------------------------------------------------
typedef enum    {
                    Mode_Stop_enm = 0,
                    Mode_Torque_enm,
                    Mode_Speed_enm,
                    Mode_Openloop_enm,
                    Mode_Initial_enm
                }CONTROL_MODE;
#define MODE_DEFAULTS   { Mode_Stop_enm }

typedef struct {  CONTROL_MODE GAME_MODE;           // Input: Control Mode
                  Uint16  Speed_DIR;                // Input: Rotation Direction,POS:0,NEG:1

                  //OpenLoop Mode开环模式的输入变量定义
                  float  UdRef_OpenL;               // Input: Ud (Mode_Openloop/VF)
                  float  UqRef_OpenL;               // Input: Uq (Mode_Openloop/VF)
                  float  IdRef_OpenL;               // Input: Id (Mode_Openloop/IF)
                  float  IqRef_OpenL;               // Input: Iq (Mode_Openloop/IF)
                  float  Theta_Step ;               // Input: Theta Step (Mode_Openloop)
                  Uint16  IF_Enable;            // Input: Open_Loop Mode Control Select:VF(0,默认) or IF(1),
                  Uint16  Angle_Enable;         // Input: Open_Loop Mode Angle Select:Custom(0,默认) or Sensor(1)

                  //Torque Mode开环模式的输入变量定义
                  float  Is_Ref;                    // Input: Is (Mode_Torque),While "FLAG1.bit.Is_Angle_ON=1" Enable;
                  float  Id_Ref;                    // Input: Id (Mode_Torque)
                  float  Iq_Ref;                    // Input: Iq (Mode_Torque)

                  //Speed Mode开环模式的输入变量定义
                  float Speed_Ref;              // Input: Speed (Mode_Speed)

                  //算法验证的外部输入电压干扰作用
                  float Ud_Disturbance;
                  float Uq_Disturbance;
                } INPUT;

typedef struct {  float  Rotation_Speed;        // Output: Rotation_Speed
                  float  Voltage_DC_BUS;            // Output: Voltage_DC_BUS
                  float  Current_DC_BUS;        // Output: Current_DC_BUS
                  float  Current_Phase_Mag;     // Output: Current_Phase_Mag
                  float  Current_Phase_RMS;     // Output: Current_Phase_RMS
                  float  Torque_Elec;               // Output: Torque_Elec
                } OUTPUT;

typedef struct {  INPUT  in;            // User Input Parameters
                  OUTPUT out;           // User Output Parameters
                } USER_TERMINALS;

#define USER_TERMINALS_DEFAULTS { {MODE_DEFAULTS,0, 0,0,0,0,0,0,0, 0,0,0, 0, 0,0},  \
                                  {0,0,0,0,0,0}                         \
                                }
//------------------------------------------------------------------------------------
typedef enum    {
                    Show_Iq_enm = 0,
                    Show_Id_enm,
                    Show_Speed_enm,
                    Show_Torque_enm
                }PWMDAC_MODE;
#define PWMDAC_MODE_DEFAULTS   { Show_Iq_enm }

typedef struct {
                    PWMDAC_MODE Show_Mode;  //示波器显示变量

                    float SpeedRef;         //进程转速给定
                    float IsRef;            //进程相电流幅值给定，配合FLAG1.bit.Is_Angle_ON=1开启转矩角分配模式
                    float Is_Angle;         //进程电流转矩角给定，配合FLAG1.bit.Is_Angle_ON=1开启转矩角分配模式
                    float IdRef;            //进程D轴电流给定，配合FLAG1.bit.Is_Angle_ON=0开启直接分配模式
                    float IqRef;            //进程Q轴电流给定，配合FLAG1.bit.Is_Angle_ON=0开启直接分配模式

                    float Ud_ACR_Out;      //各种电流调节器D轴输出, (PI Controller  /Discrete-Domain Controller)
                    float Uq_ACR_Out;      //各种电流调节器Q轴输出, (PI Controller  /Discrete-Domain Controller)
                    float Ud_Decouple;     //各种解耦策略D轴解耦最终总电压补偿量
                    float Uq_Decouple;     //各种解耦策略Q轴解耦最终总电压补偿量
                    float Ud_CV_Decouple;  //虚拟阻抗+复矢量解耦中复矢量直接输出D轴电压部分
                    float Uq_CV_Decouple;  //虚拟阻抗+复矢量解耦中复矢量直接输出Q轴电压部分
                    float Ud_VI_Decouple;  //虚拟阻抗+复矢量解耦中虚拟阻抗前馈输出D轴电压部分
                    float Uq_VI_Decouple;  //虚拟阻抗+复矢量解耦中虚拟阻抗前馈输出Q轴电压部分
                    float Ud_PreSat;       //D轴电压饱和处理前输出，饱和处理后输入到SVPWM给定ipark.Ds
                    float Uq_PreSat;       //Q轴电压饱和处理前输出，饱和处理后输入到SVPWM给定ipark.Qs
                    float Uref_Max;        //SV调制允许最大输出相电压幅值，用于——电流控制器输出限幅和SV输入前过调制处理
                    float Uref_Temp;       //SV输入前电压过调制处理暂存变量
                    float Ud_Disturbance;  //D轴外部扰动电压输入；
                    float Uq_Disturbance;  //Q轴外部扰动电压输入；

                    float Theta_Elec_OpenL;      //开环运行的自给定电角度
                    float Theta_Compen;          //角度补偿角度数值（弧度制），Theta_Compen=Angle_Compen_Ratio*Speed_We*T_Sample
                    float Angle_Compen_Ratio;    //角度补偿拍数
                    float VI_CV_c1;              //虚拟阻抗电阻系数
                    float VI_CV_c2;              //虚拟阻抗电感系数
                    float Current_Predict_Beats; //电流预测拍数

                    //离散表示格式，U代表电压，dq代表DQ轴，0代表第k+0拍的作用电压（即当前拍）,即第k+0-1拍计算输出电压
                    //带下划线表示k-,不带下划线表示k+,例如Ud_Beat1表示Ud(k+1),Ud_Beat_1表示(k-1);
                    float Ud_Beat0;            //上一拍D轴计算输出电压，即为当前拍D轴作用电压
                    float Uq_Beat0;            //上一拍Q轴计算输出电压，即为当前拍Q轴作用电压
                    float Id_Predict;          // D轴预测电流值,用于解耦前馈电压计算；
                    float Iq_Predict;          // Q轴预测电流值,用于解耦前馈电压计算；
                    float Id_Cal;              // 用于解耦函数中的D轴电流计算变量
                    float Iq_Cal;              // 用于解耦函数中的Q轴电流计算变量

                    float Counter;             // 用于进程中的周期计数；
                    float Kp_Ratio;            // 用于Kp的比例带宽系数调节；

                    // 用于第三章闭环电流预测实验变量定义
                    //暂存当前拍的电流预测结果，用于下一拍计算；
                    float Current_Predict_Mode;  //电流预测算法选择
                    float Id_Predict_1;     //上一拍D轴预测电流值(第k-1拍预测结果，为当前第k拍预测值)
                    float Iq_Predict_1;     //上一拍Q轴预测电流值(第k-1拍预测结果，为当前第k拍预测值)
                    float Ud_Compen;        //电流预测误差补偿的d轴电压
                    float Uq_Compen;        //电流预测误差补偿的q轴电压
                    float Ki_Predict;       //电流预测误差补偿的积分系数
                } PROCESS_DATA;

#define PROCESS_DATA_DEFAULTS { PWMDAC_MODE_DEFAULTS, 0,0,_IQ(0.25),0,0,  0,0,0,0,  \
                                0,0,0,0,  0,0,0,0,0,0,  0,0,0,0,0,0,  0,0,0,0,0,0,  \
                                0,0, 0,0,0,0,0,0   \
                              }

//------------------------------------------------------------------------------------
// DATA_RECORD用于记录运行过程数据
#define DATA_MAX 500        // 最大数组宏定义
//#define DATA_N1  300        // 动态过程暂存数组数目宏定义
#define DATA_N2  100        // 用户给定作用延时，用于存储稳态数据,DATA_N2务必小于DATA_N1

typedef enum    {
                    Mode_Continuous_enm = 0,
                    Mode_Single_enm
                }DATA_MODE;

typedef struct {
                    DATA_MODE Dlog_Mode;
                    Uint16 Counter;
                    Uint16 FLAG;

                    float Ud_Dlog;           //  用于阶跃给定的变量暂存
                    float Uq_Dlog;           //  用于阶跃给定的变量暂存
                    float Id_Dlog;           //  用于阶跃给定的变量暂存
                    float Iq_Dlog;           //  用于阶跃给定的变量暂存

                    float Dlog_State;
                    float DlogCh1[DATA_MAX];
                    float DlogCh2[DATA_MAX];
                    float DlogCh3[DATA_MAX];
                    float DlogCh4[DATA_MAX];

                    float Ud_Dlog_old;           //  用于阶跃给定的变量暂存
                    float Uq_Dlog_old;           //  用于阶跃给定的变量暂存
                    float Id_Dlog_old;           //  用于阶跃给定的变量暂存
                    float Iq_Dlog_old;           //  用于阶跃给定的变量暂存
                } DATA_RECORD;

#define DATA_RECORD_DEFAULTS { Mode_Continuous_enm, 0,0, 0,0,0,0, 0}
//------------------------------------------------------------------------------------


/************************************************************************************/
// Global variables used in this system
/************************************************************************************/
extern PWM pwm1;
extern PWMDAC pwmdac1;
extern DAC dac1;
extern CMPSS cmpss1;
extern ADC adc1;
extern QEP qep1;

extern RMPCNTL ramp1;
extern RMPCNTL ramp2;
extern RMPCNTL ramp3;
extern RMPCNTL ramp4;

extern PARK park1;
extern IPARK ipark1;
extern CLARKE clarke1;
extern SVPWM  svpwm1;

extern PID_CONTROLLER pid_id;
extern PID_CONTROLLER pid_iq;
extern PID_CONTROLLER pid_spd;

extern DCV_CONTROLLER dcv_id;
extern DCV_CONTROLLER dcv_iq;

extern DCV_CONTROLLER dcv_r_id;
extern DCV_CONTROLLER dcv_r_iq;

extern FLAG_VAR FLAG1;
extern PROCESS_DATA Process_Data;
extern USER_TERMINALS User_Terminal;


/*过程变量定义*/
extern DATA_RECORD Data_Record;

#if MOTOR_PARAM_VAR
/*用于实验过程调整电机电感等参数，与宏定义同名*/
extern _iq Ld_IQ;
extern _iq Lq_IQ;
extern _iq Rs_IQ;
extern _iq Flux_Mag_IQ;
extern _iq Inertia_IQ;

/*用于实验过程虚拟阻抗参数设计，与宏定义同名*/
extern _iq Lad_IQ;
extern _iq Laq_IQ;
extern _iq Ra_IQ;
#endif
//------------------------------------------------------------------------------------


/************************************************************************************/
// Global Functions Defined in this file
/************************************************************************************/
extern void Global_Variables_Init(void);
extern void Global_Variables_Clear(void);
extern void Controller_Params_Preset(void);
//---------------------------------------------------------------------------------------
extern void Complex_Vector_Decouple(_iq *,_iq *);
extern void Voltage_Feedforward_Decouple(_iq *,_iq *);
extern void Virtual_Impedance_CV_Decouple(_iq *,_iq *);
//---------------------------------------------------------------------------------------------------------
extern void Voltage_OutMax_Update(void);
extern void Current_Predictive(_iq *,_iq *);
//---------------------------------------------------------------------------------------------------------
#endif /* SOURCE_GLOBAL_FUNCTIONS_VARIABLES_CALL_H_ */
