/*
 * Discrete_Domain_Controller.h
 *
 *  Created on: 2019年12月27日
 *      Author: YY
 */

#include "F28x_Project.h"

#ifndef MATH_TYPE
#define MATH_TYPE   1
#endif
#include "IQmathLib.h"
#include "math.h"

#include "Macro_Predefine.h"



//****************************************************************************************************************************
// 离散域电流调节器数字实现函数中用到的幂运算宏定义
//--------------------------------------------------------------------------------------------------------------------------------------------------
// 在线计算精度 EXP_AT=EXP_AT1>EXP_AT4>EXP_AT3>EXP_AT2
// 二阶帕德近似的运算精度与exp幂级数运算函数相当，具体哪个看运算时长；
#define EXP_AT        expf(-R_Ts__Ld)        //C语言math.h中定义的自然底数e幂运算库函数
#define EXP_AT1       0.69101                      // 离线计算，R=0.34,L=0.00092,Ts=0.001;
#define EXP_AT2       (1- R_Ts__Ld)               //无穷级数展开至一次项累加
#define EXP_AT3       (1- R_Ts__Ld+0.5*R_Ts__Ld*R_Ts__Ld)              //无穷级数展开至二次项累加
#define EXP_AT4       (1- R_Ts__Ld/(1+ 0.5*R_Ts__Ld+R_Ts__Ld*R_Ts__Ld/12))        //二阶帕德近似
//--------------------------------------------------------------------------------------------------------------------------------------------------
#define K_R_Ts_Ld    _IQmpy(R_Ts__Ld, dcv_r_id.Kr)   
#define EXP_K_AT     expf(-K_R_Ts_Ld)        //C语言math.h中定义的自然底数e幂运算库函数
#define EXP_K_AT4    (1- K_R_Ts_Ld/(1+ 0.5*K_R_Ts_Ld+K_R_Ts_Ld*K_R_Ts_Ld/12))  //二阶帕德近似
//--------------------------------------------------------------------------------------------------------------------------------------------------

//****************************************************************************************************************************
// 离散域复矢量DCV同步坐标系PI电流调节器设计与数字实现
// Discrete-Time Complex Vector Synchronous Frame PI Current Regulator Design and Digital implementation
//--------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct {   
                              float  Ref;   			// Input: Reference input
                              float  Fdb;   			// Input: Feedback input
                              float  Out;   			// Output: DCV output
                              
                              float  Err;				         // Data Variable: Error,x(k)
                              float  Err_Beat_1;         // Data Variable:Error of last beat,x(k-1)
                              float  Uc;                        // Discrete Controller Output, only for DCV_R
                              float  Ur;                         // Active Damping Feedback Output, only for DCV_R
                              
                              float  Kr;                  // Parameter: Active Damping ratio, default value: 1;
                              float  Kp;                // Parameter: Proportional Gain
                              float  OutMax;	    // Parameter: Maximum output
                              float  OutMin;	     // Parameter: Minimum output+
                    	} DCV_CONTROLLER;

typedef DCV_CONTROLLER *DCV_CONTROLLER_handle;
#define DCV_CONTROLLER_DEFAULTS { 0,0,0,  0,0,0,0,  1,0,0,0 }
//-----------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
extern void DiscreteTime_CV_Controller(DCV_CONTROLLER_handle,DCV_CONTROLLER_handle);
extern void Improved_DCV_Controller(DCV_CONTROLLER_handle,DCV_CONTROLLER_handle);
//-----------------------------------------------------------------------------------------------
     



