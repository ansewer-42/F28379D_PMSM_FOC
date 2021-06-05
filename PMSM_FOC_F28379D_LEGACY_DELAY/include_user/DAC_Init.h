/*
 * DAC_Init.h
 *
 *  Created on: 2020��6��1��
 *      Author: PMSM
 */

#ifndef INCLUDE_USER_DAC_INIT_H_
#define INCLUDE_USER_DAC_INIT_H_

#include "F28x_Project.h"
#include "f2837xbmsk.h"

#ifndef MATH_TYPE
#define MATH_TYPE   1
#endif
#include "IQmathLib.h"
#include "math.h"

#define DACA_REF_VOL 3  // DACA�Ĳο���ѹֵ
#define DACB_REF_VOL 3  // DACB�Ĳο���ѹֵ
#define CMPSS_DIGITAL_FILTER  0 //CMPSSģ�����������˲���ʹ��

//*****************************************************************************************
typedef struct {
                    float  dutya;       //DACA������ռ�ձ�,����ڲο���ѹ�ı�����
                    float  dutyb;       //DACB������ռ�ձ�,����ڲο���ѹ�ı�����
                    float  VOL_A_Value; //DACA�������ѹֵ��ʵ��ֵ��
                    float  VOL_B_Value; //DACB�������ѹֵ��ʵ��ֵ��

                    Uint16 DACA_Value;  //DACA�������ѹ������ֵ��
                    Uint16 DACB_Value;  //DACB�������ѹ������ֵ��
               } DAC;

typedef DAC *DAC_handle;

#define DAC_DEFAULTS    {0,0,0,0,0,0}
//-------------------------------------------------------------------------------------------
extern void DAC_Setup(void);
extern void DAC_VOL_OUT(DAC_handle);
extern void DAC_VAR_WATCH(DAC_handle);
//-------------------------------------------------------------------------------------------


//*****************************************************************************************
typedef struct {
                    Uint16 DACH_Value1;
                    Uint16 DACL_Value1;
                    Uint16 DACH_Value2;
                    Uint16 DACL_Value2;
                    Uint16 DACH_Value3;
                    Uint16 DACL_Value3;
                    Uint16 DACH_Value6;
                    Uint16 DACL_Value6;
                    Uint16 DACH_Value7;
                    Uint16 DACL_Value7;
                    Uint16 DACH_Value8;
                    Uint16 DACL_Value8;
               } CMPSS;

typedef CMPSS *CMPSS_handle;

// ADCģ��ο���ѹΪ3V,DACģ��ο���ѹΪ3V,ģ��Ƚ���ģ��ο���ѹΪ3.3V;
// �趨5AΪ�����������ֵ������2048λ�����е㣬����ģ������ϵ��Ϊ0.00293��
// 2048+5/0.00293=3754�� 2048-5/0.00293=342��
// ADCģ��������뱣����������ֵΪ3754����Ӧ3*3755/4096=2.75V;������������ֵΪ342����Ӧ3*343/4096=0.2512V;
// ���㵽ģ��Ƚ���������ֵ,�Ƚ���������ֵΪ2.75/3.3*4096=3413�� �Ƚ���������ֵΪ0.251/3.3*4096=312��
#define CMPSS_DEFAULTS  { 3413,312, 3413,312, 3413,312, 3413,312, 3413,312, 3413,312 }
//-------------------------------------------------------------------------------------------
extern void CMPSS_Setup(void);
extern void CMPSS_PHASE_CUR_PRO(CMPSS_handle);
//-------------------------------------------------------------------------------------------


#endif /* INCLUDE_USER_DAC_INIT_H_ */
