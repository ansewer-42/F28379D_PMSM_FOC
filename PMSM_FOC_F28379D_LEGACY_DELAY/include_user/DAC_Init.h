/*
 * DAC_Init.h
 *
 *  Created on: 2020年6月1日
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

#define DACA_REF_VOL 3  // DACA的参考电压值
#define DACB_REF_VOL 3  // DACB的参考电压值
#define CMPSS_DIGITAL_FILTER  0 //CMPSS模块的输出数字滤波器使能

//*****************************************************************************************
typedef struct {
                    float  dutya;       //DACA的输入占空比,相对于参考电压的比例；
                    float  dutyb;       //DACB的输入占空比,相对于参考电压的比例；
                    float  VOL_A_Value; //DACA的输出电压值，实际值；
                    float  VOL_B_Value; //DACB的输出电压值，实际值；

                    Uint16 DACA_Value;  //DACA的输出电压二进制值；
                    Uint16 DACB_Value;  //DACB的输出电压二进制值；
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

// ADC模块参考电压为3V,DAC模块参考电压为3V,模拟比较器模块参考电压为3.3V;
// 设定5A为相电流保护阈值，假设2048位采样中点，电流模数比例系数为0.00293；
// 2048+5/0.00293=3754， 2048-5/0.00293=342；
// ADC模块采样输入保护上限数字值为3754，对应3*3755/4096=2.75V;保护下限数字值为342，对应3*343/4096=0.2512V;
// 换算到模拟比较器的数字值,比较上限数字值为2.75/3.3*4096=3413， 比较下限数字值为0.251/3.3*4096=312；
#define CMPSS_DEFAULTS  { 3413,312, 3413,312, 3413,312, 3413,312, 3413,312, 3413,312 }
//-------------------------------------------------------------------------------------------
extern void CMPSS_Setup(void);
extern void CMPSS_PHASE_CUR_PRO(CMPSS_handle);
//-------------------------------------------------------------------------------------------


#endif /* INCLUDE_USER_DAC_INIT_H_ */
