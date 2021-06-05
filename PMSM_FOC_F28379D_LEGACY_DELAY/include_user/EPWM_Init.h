/*
 * EPWM_Init.h
 *
 *  Created on: 2020年3月31日
 *      Author: YY
 */

#ifndef INCLUDE_USER_EPWM_INIT_H_
#define INCLUDE_USER_EPWM_INIT_H_

#include "F28x_Project.h"
#include "F2837xD_EPwm_defines.h"
#include "f2837xbmsk.h"

#ifndef MATH_TYPE
#define MATH_TYPE   1
#endif
#include "IQmathLib.h"
#include "math.h"

#include "Macro_Predefine.h"


/*----------------------------------------------------------------------------
Initialization constant for the F2833X Time-Base Control Registers for PWM Generation.
Sets up the timer to run free upon emulation suspend, count up-down mode
prescaler 1.
----------------------------------------------------------------------------*/
#define PWM_INIT_STATE ( FREE_RUN_FLAG 			+         	\
                         PRDLD_SHADOW  			+       	\
                         TIMER_CNT_UPDN 		+			\
                         HSPCLKDIV_PRESCALE_X_1 + 			\
                         CLKDIV_PRESCALE_X_1  	+			\
                         PHSDIR_CNT_UP    		+ 			\
                         CNTLD_DISABLE )

/*----------------------------------------------------------------------------
Initialization constant for the F2833X Compare Control Register.
----------------------------------------------------------------------------*/
#define CMPCTL_INIT_STATE_SINGLE  ( LOADAMODE_PRD 	    + 			\
                            		LOADBMODE_PRD 		+		 	\
									SHDWAMODE_SHADOW 	+		 	\
									SHDWBMODE_SHADOW )

#define CMPCTL_INIT_STATE_DOUBLE  ( LOADAMODE_ZRO_PRD 	+ 			\
                            		LOADAMODE_ZRO_PRD   +		 	\
									SHDWAMODE_SHADOW 	+		 	\
									SHDWBMODE_SHADOW )

/*----------------------------------------------------------------------------
Initialization constant for the F2833X Action Qualifier Output A Register.
----------------------------------------------------------------------------*/
#define AQCTLA_INIT_STATE ( CAU_CLEAR + CAD_SET )

/*----------------------------------------------------------------------------
Initialization constant for the F2833X Dead-Band Generator registers for PWM Generation.
Sets up the dead band for PWM and sets up dead band values.
DBCTL_INIT_STATE 被设置为 BP——ENABLE 和 Active high complementary (AHC). EPWMxB is inverted.
----------------------------------------------------------------------------*/
#define DBCTL_INIT_STATE  ( BP_ENABLE + POLSEL_ACTIVE_HI_CMP )

/*----------------------------------------------------------------------------
Initialization constant for the F2833X PWM Chopper Control register for PWM Generation.
----------------------------------------------------------------------------*/
#define  PCCTL_INIT_STATE  CHPEN_DISABLE

/*----------------------------------------------------------------------------
Initialization constant for the F2833X Trip Zone Select Register
这个地方根据需要 改为 TZ1和TZ2，以及DCA/BEVT的使能;
----------------------------------------------------------------------------*/
#if EPWM_XBAR_DC       //(其实该部分已经在EPWM_INit.c文件的DC子模块寄存器配置中配置过了)
//若还要使能EPWM的XBAR与DC模块配合输出的硬件TZ保护，使用以下配置
    #define  TZSEL_INIT_STATE  (ENABLE_TZ1_OST+ENABLE_DCAEVT1_OST+ENABLE_DCBEVT1_OST)
#else
    #if EPWM_TZ_OST
        #define  TZSEL_INIT_STATE  (ENABLE_TZ1_OST)
    #else
        #define  TZSEL_INIT_STATE  (DISABLE_TZ1_OST)
    #endif
#endif

/*----------------------------------------------------------------------------
Initialization constant for the F2833X Trip Zone Control Register
----------------------------------------------------------------------------*/
#if EPWM_XBAR_DC
//若还要使能EPWM的XBAR与DC模块配合输出的硬件TZ保护，使用以下配置
    #define  TZCTL_INIT_STATE (TZA_FORCE_LO+TZB_FORCE_LO+DCAEVT1_FORCE_LO+DCBEVT1_FORCE_LO)
#else
    #define  TZCTL_INIT_STATE ( TZA_FORCE_LO + TZB_FORCE_LO )
#endif
//------------------------------------------------------------------------------

//*****************************************************************************************
// 之前的F28335浮点版本好像设置为LOADAMODE_ZERO错了，占空比会相反；
#define PWMDAC_CMPCTL_INIT_STATE ( LOADAMODE_PRD 		+ 	\
                                   LOADBMODE_PRD 		+ 	\
                                   SHDWAMODE_IMMEDIATE 	+ 	\
                                   SHDWAMODE_IMMEDIATE )

#define PWMDAC_AQCTLA_INIT_STATE ( CAD_SET + CAU_CLEAR )
#define PWMDAC_AQCTLB_INIT_STATE ( CBD_SET + CBU_CLEAR )
#define PWMDAC_DBCTL_INIT_STATE  BP_DISABLE
#define PWMDAC_PCCTL_INIT_STATE  CHPEN_DISABLE
#define PWMDAC_TZSEL_INIT_STATE  DISABLE_TZSEL
#define PWMDAC_TZCTL_INIT_STATE ( TZA_HI_Z + TZB_HI_Z )

//------------------------------------------------------------------------------

//*****************************************************************************************
typedef struct {
                    Uint32 PeriodMax;     // Parameter: PWM Half-Period in CPU clock cycles (Q0)
                    Uint32 MfuncPeriod;    // Input: Period scaler (Q15)
                    _iq MfuncC1;        // Input: EPWM1 A&B Duty cycle ratio (Q15)
                    _iq MfuncC2;        // Input: EPWM2 A&B Duty cycle ratio (Q15)
                    _iq MfuncC3;        // Input: EPWM3 A&B Duty cycle ratio (Q15)
                    Uint16 PWM1out;
                    Uint16 PWM2out;
                    Uint16 PWM3out;

                    _iq Ta;
                    _iq Tb;
                    _iq Tc;
               } PWM ;

typedef PWM *PWM_handle;

#define PWM_DEFAULTS    {     5000,   \
                              0x7FFF, \
                              0,      \
                              0,      \
                              0,      \
                              0,      \
                              0,      \
                              0, 0,0,0  \
                           }
//------------------------------------------------------------------------------
typedef struct {
					_iq PwmDacCh1;   	// Input: Pointer to source data output on PWMDAC channel 0
					_iq PwmDacCh2;    // Input: Pointer to source data output on PWMDAC channel 1
					_iq PwmDacCh3;    // Input: Pointer to source data output on PWMDAC channel 2
					_iq PwmDacCh4;    // Input: Pointer to source data output on PWMDAC channel 3
					Uint16 PeriodMax;     		// Parameter: PWMDAC half period in number of clocks  (Q0)
				} PWMDAC;

typedef PWMDAC *PWMDAC_handle;
#define PWMDAC_DEFAULTS	{ 0,0,0,0,5000 }

#define INV_CH1_BASE INV_BASE_CURRENT
#define INV_CH2_BASE INV_BASE_CURRENT
#define INV_CH3_BASE INV_BASE_CURRENT
#define INV_CH4_BASE INV_BASE_SPEED
//------------------------------------------------------------------------------
extern void EPWM_Setup1(PWM_handle);
extern void EPWM_Duty_Calculate(PWM_handle);
extern void EPWM1_Interrupt_Set(PINT EPWM1_ISR);
extern void EPWM4_Interrupt_Set(PINT EPWM1_ISR);
extern void EPWMDAC_Setup(PWMDAC *v);
extern void EPWMDAC_VAR_WATCH(PWMDAC*);


#endif /* INCLUDE_USER_EPWM_INIT_H_ */
