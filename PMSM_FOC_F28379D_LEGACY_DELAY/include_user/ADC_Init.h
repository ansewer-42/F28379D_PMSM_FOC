/*
 * ADC_Init.h
 *
 *  Created on: 2020.06.18
 *      Author: PMSM
 */

#ifndef INCLUDE_USER_ADC_INIT_H_
#define INCLUDE_USER_ADC_INIT_H_

#include "F28x_Project.h"
#include "f2837xbmsk.h"

#include "Macro_Predefine.h"

#ifndef MATH_TYPE
#define MATH_TYPE   1
#endif
#include "IQmathLib.h"
#include "math.h"

#define ADC_OVER_SAMPLE	    1
#define ADC_SAMPLE_ALL      1

//*****************************************************************************************
// 浠ヤ笅瀹忓畾涔変负鐢垫祦鐢靛帇ADC閲囨牱鐨勬瘮渚嬬郴鏁帮紝閲囨牱鏁板瓧閲忓噺鍘婚浂鍋忔暟瀛楅噺锛屽啀涔樹互涓嬬郴鏁板嵆涓哄疄闄呮ā鎷熼噺澶у皬;
// The macro defined below is ADC sample ratio
#define Current_A_Ratio     0.00293
#define Current_B_Ratio     0.00293
#define Current_C_Ratio     0.00293
#define Current_Phase_Ratio 0.00293     //12/4096=0.00293, ADC閲忕▼涓烘ā鎷熼噺姝ｈ礋6A,瀵瑰簲鏁板瓧閲忎负0~4096;
#define Voltage_A_Ratio     0.007324
#define Voltage_B_Ratio     0.007324
#define Voltage_C_Ratio     0.007324
#define Voltage_Phase_Ratio 0.007324    //鐩哥數鍘婣DC閲囨牱鏁板瓧閲�-瀹為檯妯℃嫙閲忔瘮渚嬬郴鏁帮紱
#define Current_DC_Ratio    0.001465    //5/3413.33=5/(5*0.5/3*4096)
#define Voltage_DC_Ratio    0.007324    //24/3276.8=24/(24*0.1/3*4096)
/* 鐢垫祦閲囨牱鍋忕疆閲廼a_offset=2280,ib_offset=2273,ic_offset=2321;
        鎸夌収涓婅堪鐢垫祦閲囨牱绯绘暟鍜岄浂鍋忔暟瀛楅噺锛岀浉鐢垫祦鐨勬祴閲忚寖鍥村ぇ姒備负(4095-2280)*0.00293=5.32A,鐢垫祦浼犳劅鍣ㄩ噰鏍疯寖鍥寸暐灏忥紱*/
//----------------------------------------------------------------------------------


//****************************************************************************
struct ADC_RESULT {
                        //鐢ㄤ簬琛ㄧず瀹為檯鐨勯噰鏍锋暟瀛楅噺鐩稿澶у皬
                        int16 Ia;
                        int16 Ib;
                        int16 Ic;
                        int16 Ua;
                        int16 Ub;
                        int16 Uc;
                        int16 Idc;
                        int16 Udc;

                        //鐢ㄤ簬姹侫DC閲囨牱鐨勯浂鍋忕疮鍔犲拰
                        Uint32 Ia_Sum;
                        Uint32 Ib_Sum;
                        Uint32 Ic_Sum;
                        Uint32 Ua_Sum;
                        Uint32 Ub_Sum;
                        Uint32 Uc_Sum;
                        Uint32 Idc_Sum;
                        Uint32 Udc_Sum;

                        //鐢ㄤ簬鏆傚瓨ADC鐨勯噰鏍峰亸缃暟瀛楅噺0~4095锛寈x_zero_offset涓烘渶鍚庢眰寰楀钩鍧囧��
                        int16 Ia_Zero_Offset;
                        int16 Ib_Zero_Offset;
                        int16 Ic_Zero_Offset;
                        int16 Ua_Zero_Offset;
                        int16 Ub_Zero_Offset;
                        int16 Uc_Zero_Offset;
                        int16 Idc_Zero_Offset;
                        int16 Udc_Zero_Offset;

                        //鐢ㄤ簬鏆傚瓨褰撳墠ADC閲囨牱鏁板瓧閲忕嚎鎬ф崲绠楀悗寰楀埌鐨勫疄闄呭��,鐢ㄤ簬褰撳墠鐢垫祦鍙嶉鐨凜lark鍙樻崲
                        float Current_A;
                        float Current_B;
                        float Current_C;
                        float Voltage_A;
                        float Voltage_B;
                        float Voltage_C;
                        float Current_DC;
                        float Voltage_DC;

                        //鐢ㄤ簬鏆傚瓨涓婃ADC閲囨牱鏁板瓧閲忕嚎鎬ф崲绠楀悗寰楀埌鐨勫疄闄呭��
                        float Current_A_Last;
                        float Current_B_Last;
                        float Current_C_Last;
                        float Voltage_A_Last;
                        float Voltage_B_Last;
                        float Voltage_C_Last;
                        float Current_DC_Last;
                        float Voltage_DC_Last;
                  };

struct State_FlAG_BITS {
                            Uint16 Zero_Offset_End:1;
                            Uint16 Over_Fault_Flag:1;
                            Uint16 OverTime_Flag:1;
                            Uint16 OverCurrent_A_Flag:1;
                            Uint16 OverCurrent_B_Flag:1;
                            Uint16 OverCurrent_C_Flag:1;
                            Uint16 OverCurrent_DC_Flag:1;
                            Uint16 OverVoltage_DC_Flag:1;
                            Uint16 rsvd1:8;                 // 15:9   reserved
                      };

union  State_FlAG {
                       Uint16                all;
                       struct State_FlAG_BITS  bit;
                  };
//--------------------------------------------------------------------------------------
typedef struct {
                    float Filter0_Idc;
                    float Filter1_Idc;
                    float Filter0_Udc;
                    float Filter1_Udc;

                    Uint16 Zero_Offset_Count;
                    Uint16 Fault_Current_A_Count;
                    Uint16 Fault_Current_B_Count;
                    Uint16 Fault_Current_DC_Count;
                    Uint16 Fault_Voltage_DC_Count;

                    volatile union  State_FlAG Flag;
                    volatile struct ADC_RESULT Result;
               } ADC;

typedef ADC *ADC_handle;

#define ADC_DEFAULTS    { 0.1,0.9,  0.1,0.9, 0,0,0,0,0,0,                       \
                          { 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,  \
                            0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 }                  \
                        }

//--------------------------------------------------------------------------------------
extern void ADC_Setup1(void);
extern void ADC_Setup2(void);
extern void ADCA1_Interrupt_Set(PINT ADCA1_ISR);
extern void ADCC1_Interrupt_Set(PINT ADCC1_ISR);
extern void ADC_Zero_Offset(ADC_handle);
extern void ADC_Result_Read(ADC_handle);
extern void ADC_Result_Handle(ADC_handle);
//--------------------------------------------------------------------------------------


#endif /* INCLUDE_USER_ADC_INIT_H_ */
