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
                    Uint32  GAME_ON:1;        // 1���ڳ���������PWM���ؿ���λ����1����PWM��
                    Uint32  GAME_RESTART:1;   // 2���ڹ��Ϸ������ֶ���������������̣���1������
                    Uint32  GAME_OVER:1;      // 3�����ֶ�����ֱ�ӹر�PWM����ͣ��״̬,��1�ر�PWM��
                    Uint32  VAR_CLEAR:1;      // 4�����ֶ����ȫ�ֱ���ֵ����1ȫ�ֱ��������
                    Uint32  Is_Angle_ON:1;    // 5����ת�ؿ���ģʽ�����Ƿ����ת�ؽǷ����������1����ת�ؽǿ��ƣ�
                    Uint32  FAULT_ON:1;       // 6������ʾ�����Ƿ�������1���Ϸ�����
                    Uint32  Data_Record_ON:1; // 7�����ֶ������Ƿ������ݼ�¼��
                    Uint32  User_Input_ON:1;  // 8�����ֶ������Ƿ�ʹ���û�������������1ʹ��

                    Uint32  ANGLE_COMPEN_ON:1;      // 1�����ֶ�����1.5�ĽǶȲ�����
                    Uint32  VOL_FDB_DECOUPLE:1;     // 2�����ֶ�ʹ�ܵ�ѹǰ������ڣ���1ʹ��ǰ�����
                    Uint32  COMPL_VECT_DECOUPLE:1;  // 3�����ֶ�ʹ�ܸ�ʸ������ڣ���1ʹ�ܸ�ʸ�����
                    Uint32  VI_CV_DECOUPLE:1;       // 4�����ֶ�ʹ�����迹ǰ�����ڣ���1���������迹���ܣ�
                    Uint32  CUR_PREDICTIVE_ON:1;    // 5�����ֶ�������k+1�ĵ���ֵԤ�⣬��1��������Ԥ����£�
                    Uint32  DISCRETE_CUR_ON:1;      // 6�����ֶ�����ԭʼ��ɢ����ƿ���������1������ɢ����ƿ�������
                    Uint32  DCV_R_CUR_ON:1;         // 7�����ֶ������Ľ���ɢ����ƿ���������1������ɢ����ƿ�������

                    Uint32  CUR_CNT_Kp_ON:1;        // 1�����ֶ������Ƿ�ʹ��kp_Ratio�����޸ı���������
                    Uint32  CUR_PREDICT_COMPEN:1;   // 2�����ֶ�����Ԥ����������㷨

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

                  //OpenLoop Mode����ģʽ�������������
                  float  UdRef_OpenL;               // Input: Ud (Mode_Openloop/VF)
                  float  UqRef_OpenL;               // Input: Uq (Mode_Openloop/VF)
                  float  IdRef_OpenL;               // Input: Id (Mode_Openloop/IF)
                  float  IqRef_OpenL;               // Input: Iq (Mode_Openloop/IF)
                  float  Theta_Step ;               // Input: Theta Step (Mode_Openloop)
                  Uint16  IF_Enable;            // Input: Open_Loop Mode Control Select:VF(0,Ĭ��) or IF(1),
                  Uint16  Angle_Enable;         // Input: Open_Loop Mode Angle Select:Custom(0,Ĭ��) or Sensor(1)

                  //Torque Mode����ģʽ�������������
                  float  Is_Ref;                    // Input: Is (Mode_Torque),While "FLAG1.bit.Is_Angle_ON=1" Enable;
                  float  Id_Ref;                    // Input: Id (Mode_Torque)
                  float  Iq_Ref;                    // Input: Iq (Mode_Torque)

                  //Speed Mode����ģʽ�������������
                  float Speed_Ref;              // Input: Speed (Mode_Speed)

                  //�㷨��֤���ⲿ�����ѹ��������
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
                    PWMDAC_MODE Show_Mode;  //ʾ������ʾ����

                    float SpeedRef;         //����ת�ٸ���
                    float IsRef;            //�����������ֵ���������FLAG1.bit.Is_Angle_ON=1����ת�ؽǷ���ģʽ
                    float Is_Angle;         //���̵���ת�ؽǸ��������FLAG1.bit.Is_Angle_ON=1����ת�ؽǷ���ģʽ
                    float IdRef;            //����D��������������FLAG1.bit.Is_Angle_ON=0����ֱ�ӷ���ģʽ
                    float IqRef;            //����Q��������������FLAG1.bit.Is_Angle_ON=0����ֱ�ӷ���ģʽ

                    float Ud_ACR_Out;      //���ֵ���������D�����, (PI Controller  /Discrete-Domain Controller)
                    float Uq_ACR_Out;      //���ֵ���������Q�����, (PI Controller  /Discrete-Domain Controller)
                    float Ud_Decouple;     //���ֽ������D����������ܵ�ѹ������
                    float Uq_Decouple;     //���ֽ������Q����������ܵ�ѹ������
                    float Ud_CV_Decouple;  //�����迹+��ʸ�������и�ʸ��ֱ�����D���ѹ����
                    float Uq_CV_Decouple;  //�����迹+��ʸ�������и�ʸ��ֱ�����Q���ѹ����
                    float Ud_VI_Decouple;  //�����迹+��ʸ�������������迹ǰ�����D���ѹ����
                    float Uq_VI_Decouple;  //�����迹+��ʸ�������������迹ǰ�����Q���ѹ����
                    float Ud_PreSat;       //D���ѹ���ʹ���ǰ��������ʹ�������뵽SVPWM����ipark.Ds
                    float Uq_PreSat;       //Q���ѹ���ʹ���ǰ��������ʹ�������뵽SVPWM����ipark.Qs
                    float Uref_Max;        //SV�����������������ѹ��ֵ�����ڡ�����������������޷���SV����ǰ�����ƴ���
                    float Uref_Temp;       //SV����ǰ��ѹ�����ƴ����ݴ����
                    float Ud_Disturbance;  //D���ⲿ�Ŷ���ѹ���룻
                    float Uq_Disturbance;  //Q���ⲿ�Ŷ���ѹ���룻

                    float Theta_Elec_OpenL;      //�������е��Ը�����Ƕ�
                    float Theta_Compen;          //�ǶȲ����Ƕ���ֵ�������ƣ���Theta_Compen=Angle_Compen_Ratio*Speed_We*T_Sample
                    float Angle_Compen_Ratio;    //�ǶȲ�������
                    float VI_CV_c1;              //�����迹����ϵ��
                    float VI_CV_c2;              //�����迹���ϵ��
                    float Current_Predict_Beats; //����Ԥ������

                    //��ɢ��ʾ��ʽ��U�����ѹ��dq����DQ�ᣬ0�����k+0�ĵ����õ�ѹ������ǰ�ģ�,����k+0-1�ļ��������ѹ
                    //���»��߱�ʾk-,�����»��߱�ʾk+,����Ud_Beat1��ʾUd(k+1),Ud_Beat_1��ʾ(k-1);
                    float Ud_Beat0;            //��һ��D����������ѹ����Ϊ��ǰ��D�����õ�ѹ
                    float Uq_Beat0;            //��һ��Q����������ѹ����Ϊ��ǰ��Q�����õ�ѹ
                    float Id_Predict;          // D��Ԥ�����ֵ,���ڽ���ǰ����ѹ���㣻
                    float Iq_Predict;          // Q��Ԥ�����ֵ,���ڽ���ǰ����ѹ���㣻
                    float Id_Cal;              // ���ڽ�����е�D������������
                    float Iq_Cal;              // ���ڽ�����е�Q������������

                    float Counter;             // ���ڽ����е����ڼ�����
                    float Kp_Ratio;            // ����Kp�ı�������ϵ�����ڣ�

                    // ���ڵ����±ջ�����Ԥ��ʵ���������
                    //�ݴ浱ǰ�ĵĵ���Ԥ������������һ�ļ��㣻
                    float Current_Predict_Mode;  //����Ԥ���㷨ѡ��
                    float Id_Predict_1;     //��һ��D��Ԥ�����ֵ(��k-1��Ԥ������Ϊ��ǰ��k��Ԥ��ֵ)
                    float Iq_Predict_1;     //��һ��Q��Ԥ�����ֵ(��k-1��Ԥ������Ϊ��ǰ��k��Ԥ��ֵ)
                    float Ud_Compen;        //����Ԥ��������d���ѹ
                    float Uq_Compen;        //����Ԥ��������q���ѹ
                    float Ki_Predict;       //����Ԥ�������Ļ���ϵ��
                } PROCESS_DATA;

#define PROCESS_DATA_DEFAULTS { PWMDAC_MODE_DEFAULTS, 0,0,_IQ(0.25),0,0,  0,0,0,0,  \
                                0,0,0,0,  0,0,0,0,0,0,  0,0,0,0,0,0,  0,0,0,0,0,0,  \
                                0,0, 0,0,0,0,0,0   \
                              }

//------------------------------------------------------------------------------------
// DATA_RECORD���ڼ�¼���й�������
#define DATA_MAX 500        // �������궨��
//#define DATA_N1  300        // ��̬�����ݴ�������Ŀ�궨��
#define DATA_N2  100        // �û�����������ʱ�����ڴ洢��̬����,DATA_N2���С��DATA_N1

typedef enum    {
                    Mode_Continuous_enm = 0,
                    Mode_Single_enm
                }DATA_MODE;

typedef struct {
                    DATA_MODE Dlog_Mode;
                    Uint16 Counter;
                    Uint16 FLAG;

                    float Ud_Dlog;           //  ���ڽ�Ծ�����ı����ݴ�
                    float Uq_Dlog;           //  ���ڽ�Ծ�����ı����ݴ�
                    float Id_Dlog;           //  ���ڽ�Ծ�����ı����ݴ�
                    float Iq_Dlog;           //  ���ڽ�Ծ�����ı����ݴ�

                    float Dlog_State;
                    float DlogCh1[DATA_MAX];
                    float DlogCh2[DATA_MAX];
                    float DlogCh3[DATA_MAX];
                    float DlogCh4[DATA_MAX];

                    float Ud_Dlog_old;           //  ���ڽ�Ծ�����ı����ݴ�
                    float Uq_Dlog_old;           //  ���ڽ�Ծ�����ı����ݴ�
                    float Id_Dlog_old;           //  ���ڽ�Ծ�����ı����ݴ�
                    float Iq_Dlog_old;           //  ���ڽ�Ծ�����ı����ݴ�
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


/*���̱�������*/
extern DATA_RECORD Data_Record;

#if MOTOR_PARAM_VAR
/*����ʵ����̵��������еȲ�������궨��ͬ��*/
extern _iq Ld_IQ;
extern _iq Lq_IQ;
extern _iq Rs_IQ;
extern _iq Flux_Mag_IQ;
extern _iq Inertia_IQ;

/*����ʵ����������迹������ƣ���궨��ͬ��*/
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
