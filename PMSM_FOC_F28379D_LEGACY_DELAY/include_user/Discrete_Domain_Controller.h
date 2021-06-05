/*
 * Discrete_Domain_Controller.h
 *
 *  Created on: 2019��12��27��
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
// ��ɢ���������������ʵ�ֺ������õ���������궨��
//--------------------------------------------------------------------------------------------------------------------------------------------------
// ���߼��㾫�� EXP_AT=EXP_AT1>EXP_AT4>EXP_AT3>EXP_AT2
// �������½��Ƶ����㾫����exp�ݼ������㺯���൱�������ĸ�������ʱ����
#define EXP_AT        expf(-R_Ts__Ld)        //C����math.h�ж������Ȼ����e������⺯��
#define EXP_AT1       0.69101                      // ���߼��㣬R=0.34,L=0.00092,Ts=0.001;
#define EXP_AT2       (1- R_Ts__Ld)               //�����չ����һ�����ۼ�
#define EXP_AT3       (1- R_Ts__Ld+0.5*R_Ts__Ld*R_Ts__Ld)              //�����չ�����������ۼ�
#define EXP_AT4       (1- R_Ts__Ld/(1+ 0.5*R_Ts__Ld+R_Ts__Ld*R_Ts__Ld/12))        //�������½���
//--------------------------------------------------------------------------------------------------------------------------------------------------
#define K_R_Ts_Ld    _IQmpy(R_Ts__Ld, dcv_r_id.Kr)   
#define EXP_K_AT     expf(-K_R_Ts_Ld)        //C����math.h�ж������Ȼ����e������⺯��
#define EXP_K_AT4    (1- K_R_Ts_Ld/(1+ 0.5*K_R_Ts_Ld+K_R_Ts_Ld*K_R_Ts_Ld/12))  //�������½���
//--------------------------------------------------------------------------------------------------------------------------------------------------

//****************************************************************************************************************************
// ��ɢ��ʸ��DCVͬ������ϵPI�������������������ʵ��
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
     



