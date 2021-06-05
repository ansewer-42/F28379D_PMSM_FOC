/*
 * Discrete_Domain_Controller.c
 *
 *  Created on: 2019年12月27日
 *      Author: YY
 */

//头文件包含，已在"Global_Func...Call.h"中包含头文件"Discrete_Domain_Controller.h",此处不可重复包含，否则视为重定义；
// #include "Discrete_Domain_Controller.h"   
#include "Global_Functions_Variables_Call.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------
// 离散域复矢量同步坐标系PI电流调节器设计与数字实现
// Discrete-Time Complex Vector Synchronous Frame PI Current Regulator Design and Digital implementation
//参考文献《Discrete-Time Current Regulator Design for AC Machine Drives. #Hongrae Kim》
//*******************************************************************************************************************
void DiscreteTime_CV_Controller(DCV_CONTROLLER *vd, DCV_CONTROLLER *vq)
{
    _iq Ud_Temp1=0, Ud_Temp2=0,Ud_Temp3=0,Ud_Temp4=0;
    _iq Uq_Temp1=0, Uq_Temp2=0,Uq_Temp3=0,Uq_Temp4=0;  
    _iq We_Ts=0;
    
    We_Ts = _IQmpy(qep1.Speed_We,T_Sample);
       
    /* Compute and the input error */
	vd->Err = vd->Ref - vd->Fdb;
	vq->Err = vq->Ref - vq->Fdb;

    /*第一版实现过程有错误，不应该直接先展开再角度补偿，应该视为整体*/
    /*Compute the process temporary data*/              
    Ud_Temp1 = _IQmpy(vd->Err_Beat_1,_IQcos(We_Ts));
    Ud_Temp2 = _IQmpy(vd->Err_Beat_1,_IQsin(We_Ts)); 
    Uq_Temp1 = _IQmpy(vq->Err_Beat_1,_IQcos(We_Ts));
    Uq_Temp2 = _IQmpy(vq->Err_Beat_1,_IQsin(We_Ts)); 
    
    Ud_Temp3 = _IQmpy(EXP_AT,Ud_Temp1+Uq_Temp2);
    Ud_Temp4 = vd->Err - Ud_Temp3;    
    Uq_Temp3 = _IQmpy(EXP_AT,Uq_Temp1-Ud_Temp2);
    Uq_Temp4 = vq->Err - Uq_Temp3;
             
	/* Compute and Saturate the output */
	vd->Out = vd->Out + _IQmpy(vd->Kp, Ud_Temp4);
	vd->Out = _IQsat(vd->Out, vd->OutMax, vd->OutMin);
    vq->Out = vq->Out + _IQmpy(vq->Kp, Uq_Temp4);
    vq->Out = _IQsat(vq->Out, vq->OutMax, vq->OutMin);

	/* Refresh the error data of last beat*/
	vd->Err_Beat_1 = vd->Err;
    vq->Err_Beat_1 = vq->Err;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------
// 改进离散域复矢量同步坐标系PI电流调节器设计与数字实现
// Improved Discrete-Time Complex Vector Synchronous Frame PI Current Regulator  Implementation
//*******************************************************************************************************************
void Improved_DCV_Controller(DCV_CONTROLLER *vd, DCV_CONTROLLER *vq)
{    
    _iq Ud_Temp1=0, Ud_Temp2=0,Ud_Temp3=0,Ud_Temp4=0;
    _iq Uq_Temp1=0, Uq_Temp2=0,Uq_Temp3=0,Uq_Temp4=0;  
    _iq We_Ts=0, N=0;    
    
    We_Ts = _IQmpy(qep1.Speed_We,T_Sample);
       
    /* Compute and the input error */
	vd->Err = vd->Ref - vd->Fdb;
	vq->Err = vq->Ref - vq->Fdb;

    /*第一版实现过程有错误，不应该直接先展开再角度补偿，应该视为整体*/
    /*Compute the process temporary data*/              
    Ud_Temp1 = _IQmpy(vd->Err_Beat_1,_IQcos(We_Ts));
    Ud_Temp2 = _IQmpy(vd->Err_Beat_1,_IQsin(We_Ts)); 
    Uq_Temp1 = _IQmpy(vq->Err_Beat_1,_IQcos(We_Ts));
    Uq_Temp2 = _IQmpy(vq->Err_Beat_1,_IQsin(We_Ts)); 
    
    Ud_Temp3 = _IQmpy(EXP_K_AT,Ud_Temp1+Uq_Temp2);
    Ud_Temp4 = vd->Err - Ud_Temp3;    
    Uq_Temp3 = _IQmpy(EXP_K_AT,Uq_Temp1-Ud_Temp2);
    Uq_Temp4 = vq->Err - Uq_Temp3;
             
	/* Compute and Saturate the output of the Discrete Controller*/
	vd->Uc = vd->Uc + _IQmpy(vd->Kp, Ud_Temp4);
	vd->Uc = _IQsat(vd->Uc, vd->OutMax, vd->OutMin);
    vq->Uc = vq->Uc + _IQmpy(vq->Kp, Uq_Temp4);
    vq->Uc = _IQsat(vq->Uc, vq->OutMax, vq->OutMin);
    //---------------------------------------------------------------------------------------------------------------------------------------
    /* Compute and Saturate the output of the Active Damping Feedback*/
    N = _IQmpy(Rs_IQ, EXP_AT-EXP_K_AT);
    N= _IQdiv(N,1-EXP_AT);    
    Ud_Temp1 = _IQmpy(Process_Data.Id_Predict,_IQcos(We_Ts));
    Ud_Temp2 = _IQmpy(Process_Data.Id_Predict,_IQsin(We_Ts)); 
    Uq_Temp1 = _IQmpy(Process_Data.Iq_Predict,_IQcos(We_Ts));
    Uq_Temp2 = _IQmpy(Process_Data.Iq_Predict,_IQsin(We_Ts)); 
    
    vd->Ur = _IQmpy(N, Ud_Temp1 + Uq_Temp2);
    vq->Ur = _IQmpy(N, Uq_Temp1 -  Ud_Temp2);            
    
   /* Compute and Saturate the output of the Discrete Controller*/
	vd->Out = vd->Uc - vd->Ur;
	vd->Out = _IQsat(vd->Out, vd->OutMax, vd->OutMin);
    vq->Out = vq->Uc - vq->Ur;
    vq->Out = _IQsat(vq->Out, vq->OutMax, vq->OutMin);
    
	/* Refresh the error data of last beat*/
	vd->Err_Beat_1 = vd->Err;
    vq->Err_Beat_1 = vq->Err;
    
}
//-----------------------------------------------------------------------------------------------------------------------------------------------


