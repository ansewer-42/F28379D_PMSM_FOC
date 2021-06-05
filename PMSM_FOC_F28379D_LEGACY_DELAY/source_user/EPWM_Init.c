/*
 * EPWM_Init.c
 *
 *  Created on: 2020年5月29日
 *      Author: YY
 */


#include "EPWM_Init.h"

// TODO EPWM1~3 Configuration
// 关键在于对于单采样来说，在CNT=PRD时触发中断,占空比加载模式为就要在CNT=PRD时加载，这两个要同步；否则为双采样配置；
//配置为CAU_CLEAR,CAD_SET,EPwm1Regs.ETSEL.bit.INTSEL = 2;
//在CNT=PRD时候触发EPWM1中断和SOCA信号，且CMPCTL配置成在PRD时CMPA映射寄存器装载,ETSEL寄存器在CNT=PRD时触发PWM中断
//配置模式与蓝石项目配置不一样，是在CNT=PRD时进行占空比刷新，只是通过配置CAU_CLEAR+CAD_SET实现相同效果，周期仍为10k;
void EPWM_Setup1(PWM *v1)	//对应motor1的EPWM1~3模块初始化配置
{
     float temp1=0;

	 // Initialize PWM module, 在配置EPWM模块的时候要遵循一下步骤：
	 // 1、使能各个模块的时钟,已在InitSysCtrl()函数中完成;
	 // 2、对TBCLKSYNC位清零，停止所有ePWM模块的时钟；
	 // 3、对ePWM模块进行配置
	 // 4、将TBCLKSYNC位置位，开启所有ePWM模块的时钟
	 EALLOW;
	 CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;

	 /* Init Timer-Base Control Register for EPWM1-EPWM3*/
	 // 计数器自由运行FREE_RUN_FLAG，不会在一次计数后停止而是继续循环周期计数;
	 // 默认为映射模式PRDLD_SHADOW，最大需要等待一个载波周期更新载波频率，即更新TBPRD寄存器;
	 // 计数器增减计数模式 TIMER_CNT_UPDN;
	 // 高速外设时钟分频系数HSPCLKDIV_PRESCALE_X_1*HSPCLKDIV_PRESCALE_X_1;
	 // 即默认不分频TBCLK=EPWMCLK/(HSPCLKDIV * CLKDIV)=SYSCLKOUT/2/(1*1)=100MHz;
	 // 相位方向位PHSDIR_CNT_UP同步事件发生后计数器的计数方向是向上还是向下（增减计数模式有效）,初始化为向上增计数；
	 /* TBCTL中 PHSEN和SYNCOSEL位， ABC三相中，A相为主模块，需要单独考虑 */
	 EPwm1Regs.TBCTL.all = PWM_INIT_STATE;
	 EPwm2Regs.TBCTL.all = PWM_INIT_STATE;
	 EPwm3Regs.TBCTL.all = PWM_INIT_STATE;

     // EPWM模块主频默认经过PERCLKDIVSEL.EPWMCLKDIV位二分频，为CPU主频的1/2，也即为EPWMCLK=200/2=100MHz,
     // 16位TBCTR的半周期计数值PeriodMax=TBPRD,up-down增减计数模式
	 // 要注意不用float强制类型转换可能会造成小数点向下舍去，例如PWM_FREQUENCY=20计算出来PeriodMax = 2000,正确为2500,0.5被取整舍去;
	 temp1 = ((float)(SYSTEM_FREQUENCY/2)/(PWM_FREQUENCY))/2*1000;  //PRD=200/2*1e6/2/10000=5000;
	 v1->PeriodMax = temp1;

	 /*-----------------------------------------------------------------------------
 	   各PWM模块以串行同步信号的模式触发，ePWM2/ePWM3的触发信号为前级的同步信号输出,ePWM1的同步触发信号输入为外部GPIO输入(上升沿)；
 	 本次配置不易ePWM1的外部输入同步信号作为同步基准，而是以ePWM1的同步信号输出作为后级各模块的同步基准；
 	 ePWM1的同步信号输出条件为CTR=0,为后级同步信触发号输入，触发后其余EPWM模块将TBPHS的值装载到CTR中
 	   对ePWM1模块PHSEN位disbale，不以外部同步信号触发，以ePWM1的输出同步信号为整个ePWM模块的同步信号基准；
	 -----------------------------------------------------------------------------*/
	 /* Allow each timer to be sync'ed*/     //配置相位同步功能使能位
	 EPwm1Regs.TBCTL.bit.PHSEN = 0;			//EPWM1不使能相位同步装载功能，即EPWM1不受外部同步信号控制；
	 EPwm2Regs.TBCTL.bit.PHSEN = 1;			//使能相位同步装载功能
	 EPwm3Regs.TBCTL.bit.PHSEN = 1;			//使能相位同步装载功能

	 //TI的FCL例程中的PWM_1ch_UpDwnCnt_CNF函数中PHSEN,SYNCOSEL,TBPHS配置不一样，但是后面又补上去了，最后与本文件配置相同!
	 EPwm1Regs.TBCTL.bit.SYNCOSEL = 1;       //输出同步信号触发信号为TB_CTR_ZERO
	 EPwm2Regs.TBCTL.bit.SYNCOSEL = 0;       //输出同步信号触发信号为EPWMxSYNCI，2的输入信号为1的输出信号
	 EPwm3Regs.TBCTL.bit.SYNCOSEL = 0;       //输出同步信号触发信号为EPWMxSYNCI，3的输入信号为2的输出信号

	 /* Init Timer-Base Phase Register for EPWM1-EPWM3*/
	 //配置相位同步装载相位,根据数据手册，TBCLK！=SYSCLKOUT,相位值装载过程有1个TBCLK的延时，因此配置相位装载值为1
	 EPwm1Regs.TBPHS.bit.TBPHS = 0;
	 EPwm2Regs.TBPHS.bit.TBPHS = 1;
	 EPwm3Regs.TBPHS.bit.TBPHS = 1;

	 /* Init Timer-Base Period Register for EPWM1-EPWM3*/  //配置PWM周期,采用上下计数方式fpwm=fsysclkout/2/PeriodMax
	 EPwm1Regs.TBPRD = v1->PeriodMax;
	 EPwm2Regs.TBPRD = v1->PeriodMax;
	 EPwm3Regs.TBPRD = v1->PeriodMax;

	 /* Init Timer-Counter Register for EPWM1-EPWM3*/
	 EPwm1Regs.TBCTR = 0;
	 EPwm2Regs.TBCTR = 0;
	 EPwm3Regs.TBCTR = 0;

	 /*-----------------------------------------------------------------------------
	 通过对LOADAMODE_PRD/LOADBMODE_PRD  在CTR=PRD时装载CMPA的值
	SHDWAMODE_SHADOW/SHDWBMODE_SHADOW 使能CMPA的映射寄存器（缓冲）
	----------------------------------------------------------------------------*/
	 /* Init Compare Control Register for EPWM1-EPWM3*/
	 // 常规延迟模式，CMPA寄存器的读写操作作用到CMPA映射寄存器
	 #if (CUR_CONTROL_MODE == DELAY_MODE)
		EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW; //映射模式，CMPA寄存器的读写操作作用于CMPA映射寄存器；
		EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
		EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	 #endif
	 // 快速电流环模式，CMPA寄存器的读写操作立即作用到CMPA当前寄存器
	 #if (CUR_CONTROL_MODE == FCL_MODE)
		EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;  //立即模式，CMPA寄存器的读写操作直接作用于CMPA当前寄存器；
		EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;
		EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;
	 #endif
	 //----------------------------------------------------------------------------

	 //单采样模式，仅在TBCTR=TBPRD时装载CMPA映射寄存器值到当前CMPA寄存器
	 #if (SAMPLING_METHOD == SINGLE_SAMPLING)
		EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
		EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
		EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
	 #endif
	 //双采样模式，在TBCTR=0与TBCTR=TBPRD时都装载CMPA映射寄存器值到当前CMPA寄存器
	 #if (SAMPLING_METHOD == DOUBLE_SAMPLING)
		EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
		EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
		EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
	 #endif
	 //----------------------------------------------------------------------------

	 /*-----------------------------------------------------------------------------
	 AQ模块限定CTR=CMPA时PWMA引脚如何动作（set or clear）**** 关键配置模块
	 CAU_SET:增计数时CTR=CMPA置高，CAD_ClEAR:减计数时CTR=CMPA置低——一般对应LOADAMODE=ZERO;
	 CAU_CLEAR:增计数时CTR=CMPA置低，CAD_SET:减计数时CTR=CMPA置高——一般对应LOADAMODE=PRD；
	 -----------------------------------------------------------------------------*/
	 /* Init Action Qualifier Output A Register for EPWM1-EPWM3*/ //配置占空比波形如何产生(CTR=CMPA时该置高置低还是翻转)
	 EPwm1Regs.AQCTLA.all = AQCTLA_INIT_STATE;
	 EPwm2Regs.AQCTLA.all = AQCTLA_INIT_STATE;
	 EPwm3Regs.AQCTLA.all = AQCTLA_INIT_STATE;

	 /*配置动作限定模块的软件触发模式寄存器AQCSFRC从映射寄存器装载的时刻为立即装载immediate mode*/
	 EPwm1Regs.AQSFRC.bit.RLDCSF = 3;
	 EPwm2Regs.AQSFRC.bit.RLDCSF = 3;
	 EPwm3Regs.AQSFRC.bit.RLDCSF = 3;
	 //----------------------------------------------------------------------------

	 /* Init Dead-Band Generator Control Register for EPWM1-EPWM3*/
	 /* 这里需要将PWM设置成互补模式 */
	 //设置为互补模式，则之前AQ模块的EPWMB输出被旁路，DB输出的EPWMB信号直接来自于AQ模块的EPWMA信号加死区反相，与AQ的EPWMB输出无关；
	 EPwm1Regs.DBCTL.all = DBCTL_INIT_STATE;
	 EPwm2Regs.DBCTL.all = DBCTL_INIT_STATE;
	 EPwm3Regs.DBCTL.all = DBCTL_INIT_STATE;

	 /* 设置互补模式的死区时间 */  //死区时间设置为DBFED*TBCLK=100/100MHz=1us
	 EPwm1Regs.DBFED.bit.DBFED = 100;
	 EPwm1Regs.DBRED.bit.DBRED = 100;
	 EPwm2Regs.DBFED.bit.DBFED = 100;
	 EPwm2Regs.DBRED.bit.DBRED = 100;
	 EPwm3Regs.DBFED.bit.DBFED = 100;
	 EPwm3Regs.DBRED.bit.DBRED = 100;;
	 //----------------------------------------------------------------------------

	 /* Init PWM Chopper Control Register for EPWM1-EPWM3*/  //——可删除该段配置
	 /* 这是 PWM-chopper，是一个斩波功能，可用于低频PWM变为高频PWM，例如高频变压器 */
	 EPwm1Regs.PCCTL.all = PCCTL_INIT_STATE;
	 EPwm2Regs.PCCTL.all = PCCTL_INIT_STATE;
	 EPwm3Regs.PCCTL.all = PCCTL_INIT_STATE;
	 //----------------------------------------------------------------------------

	 /* DC子模块寄存器配置，配合EPWM XBAR与CMPSS模块实现硬件过流保护功能，可选功能*/
	 // 若是CMPSS比较器输入信号噪声较大，还可以使能event filter滤波功能，与GPIO的采样窗限定差不多，这里暂时不配置；
	 /* DCACTL/DCBCTL控制寄存器默认选择DCAEVT1/2,DCBEVT1/2作为输入, 将滤波后的信号DCEVTFILT旁路*/
	 #if EPWM_XBAR_DC
	 // 因为DCAEVT1,DCBEVT1才可用作OST触发TZ，DCAEVT2,DCBEVT2仅可用作CSC触发TZ，因此要将1个TRIP触发输出到两个事件force信号;
     // 在Device_Init.c文件中配置了EPWM XBAR功能，将多个相电流采样ADC引脚复用为CMPSS输入引脚，单电机三个输出经过或运算后综合到TRIP4(motor1)与TRIP5(motor2)引脚;
     // motor1对应TRIP4输入，对应EPWM1~3寄存器配置;
     EPwm1Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 3;	//3,TRIP4
     EPwm1Regs.DCTRIPSEL.bit.DCBHCOMPSEL = 3;  //3,TRIP4
	 EPwm1Regs.TZDCSEL.bit.DCAEVT1 = 2;		//010:DCAH = high,DCAL = don't care, DC A Event 1 Selection
	 EPwm1Regs.TZDCSEL.bit.DCBEVT1 = 2;     //010:DCBH = high,DCBL = don't care, DC B Event 1 Selection
	 EPwm1Regs.DCACTL.bit.EVT1FRCSYNCSEL = 1; //1, Source is passed through asynchronously

     EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 3;  //3,TRIP4
     EPwm2Regs.DCTRIPSEL.bit.DCBHCOMPSEL = 3;  //3,TRIP4
     EPwm2Regs.TZDCSEL.bit.DCAEVT1 = 2;     //010:DCAH = high,DCAL = don't care, DC A Event 1 Selection
     EPwm2Regs.TZDCSEL.bit.DCBEVT1 = 2;     //010:DCBH = high,DCBL = don't care, DC B Event 1 Selection
     EPwm2Regs.DCACTL.bit.EVT1FRCSYNCSEL = 1; //1, Source is passed through asynchronously

     EPwm3Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 3;  //3,TRIP4
     EPwm3Regs.DCTRIPSEL.bit.DCBHCOMPSEL = 3;  //3,TRIP4
     EPwm3Regs.TZDCSEL.bit.DCAEVT1 = 2;     //010:DCAH = high,DCAL = don't care, DC A Event 1 Selection
     EPwm3Regs.TZDCSEL.bit.DCBEVT1 = 2;     //010:DCBH = high,DCBL = don't care, DC B Event 1 Selection
     EPwm3Regs.DCACTL.bit.EVT1FRCSYNCSEL = 1; //1, Source is passed through asynchronously
     #endif

     /* Init Trip Zone Select Register*/
     /***这个地方是使能 Enable TZ1  TZ2 as a one-shot trip source for this ePWM module*/
     /* 当外部TZ被触发时，TZCTL将把相关引脚拉低 */
     // 利用INPUT XBAR功能将IPM Error1引脚连接到内部TZ1引脚上，IPM Error2引脚连接到内部TZ2引脚上；
     // IPM Error低电平有效，TZ信号也是低电平触发；即使有IPM_Error引脚悬空未接，使能GPIO上拉电阻默认为高电平，不会触发TZ；
     // 通过TZCLR[OST]位清除TZFLG[OST]标志位，已经宏定义为Enable_PWMx_TZ
     // 此外，条件编译可选使用EPWM XBAR与DC子模块做硬件电流保护方案；
     EPwm1Regs.TZSEL.all = TZSEL_INIT_STATE;
     EPwm2Regs.TZSEL.all = TZSEL_INIT_STATE;
     EPwm3Regs.TZSEL.all = TZSEL_INIT_STATE;

     /* Init Trip Zone Control Register*/
     EPwm1Regs.TZCTL.all = TZCTL_INIT_STATE;    //TZA_FORCE_LO + TZB_FORCE_LO
     EPwm2Regs.TZCTL.all = TZCTL_INIT_STATE;    //TZA_FORCE_LO + TZB_FORCE_LO
     EPwm3Regs.TZCTL.all = TZCTL_INIT_STATE;    //TZA_FORCE_LO + TZB_FORCE_LO
     //----------------------------------------------------------------------------

	 // Clear TZ Flag
	 EPwm1Regs.TZCLR.bit.DCAEVT1 = 1;
	 EPwm1Regs.TZCLR.bit.DCBEVT1 = 1;
	 EPwm1Regs.TZCLR.bit.OST = 1;
	 EPwm2Regs.TZCLR.bit.DCAEVT1 = 1;
	 EPwm2Regs.TZCLR.bit.DCBEVT1 = 1;
	 EPwm2Regs.TZCLR.bit.OST = 1;
	 EPwm3Regs.TZCLR.bit.DCAEVT1 = 1;
	 EPwm3Regs.TZCLR.bit.DCBEVT1 = 1;
	 EPwm3Regs.TZCLR.bit.OST = 1;
     //----------------------------------------------------------------------------

	/* Set up Event Trigger with CNT_PRD enable for Time-base of EPWM1 */
	EPwm1Regs.ETSEL.bit.SOCAEN  = 1;    /* Enable SOCA */
	EPwm1Regs.ETPS.bit.SOCAPRD  = 1;    /* Generate SOCA on the 1st event */
	EPwm1Regs.ETCLR.bit.SOCA    = 1;    /* Clear SOCA Flag */
	#if (SAMPLING_METHOD == SINGLE_SAMPLING)
		EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;    /* Enable CNT_PRD event for SOCA */
	#endif
	#if (SAMPLING_METHOD == DOUBLE_SAMPLING)
		EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRDZERO;   /* Enable CNT_ZERO_PRD event for SOCA */
	#endif
	//----------------------------------------------------------------------------

	/* Init Compare Modelu Register*/
	// 初始化为0.5的占空比，而不是下三管短路，下三管短路会有转矩产生；
	EPwm1Regs.CMPA.bit.CMPA= v1->PeriodMax>>1;	// PWM 1A - PhaseA
	EPwm2Regs.CMPA.bit.CMPA= v1->PeriodMax>>1;	// PWM 2A - PhaseB
	EPwm3Regs.CMPA.bit.CMPA= v1->PeriodMax>>1;	// PWM 3A - PhaseC
	EPwm1Regs.CMPB.bit.CMPB=0;
	EPwm2Regs.CMPB.bit.CMPB=0;
	EPwm3Regs.CMPB.bit.CMPB=0;
    //---------------------------------------------------------------------------

	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	EDIS;                         /* Disable EALLOW*/
}

void EPWM_Duty_Calculate(PWM *u)
{
    Uint32 MPeriod1;

    MPeriod1 = u->PeriodMax;
    /*Compute the compare value (Q0) from the related duty cycle ratio (Q15)*/
    u->Ta = (u->MfuncC1*0.5+0.5);
    u->PWM1out = MPeriod1*(u->Ta);
    u->Tb = (u->MfuncC2*0.5+0.5);
    u->PWM2out = MPeriod1*(u->Tb);
    u->Tc = (u->MfuncC3*0.5+0.5);
    u->PWM3out = MPeriod1*(u->Tc);
}

// TODO EPWM1 Interrupt Configuration
void EPWM1_Interrupt_Set(PINT EPWM1_ISR)
{
	EALLOW;	// This is needed to write to EALLOW protected registers

	// Enable CNT_PRD interrupt using EPWM1 Time-base
    EPwm1Regs.ETSEL.bit.INTEN  = 1;   // Enable EPWM1INT generation
    EPwm1Regs.ETPS.bit.INTPRD  = 1;   // Generate interrupt on the 1st event
	EPwm1Regs.ETCLR.bit.INT    = 1;   // Enable more interrupts

	#if (SAMPLING_METHOD == SINGLE_SAMPLING)
    	EPwm1Regs.ETSEL.bit.INTSEL = 2;   // Enable interrupt CNT_PRD event
	#endif
	#if (SAMPLING_METHOD == DOUBLE_SAMPLING)
        EPwm1Regs.ETSEL.bit.INTSEL = 3;   // Enable interrupt CNT_ZERO_PRD event
	#endif

	// Enable PIE group 3 interrupt 1 for EPWM1_INT
	// EPWM1_INT match INT3.1, and have the highest priority in PIE group3.
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

    // Enable CPU INT3 for EPWM1_INT:
	IER |= M_INT3;

	PieVectTable.EPWM1_INT = EPWM1_ISR;

	EDIS;
}


void EPWMDAC_Setup(PWMDAC *v)    //在GPIO引脚复用功能的初始化中使能PWM输出功能，否则不会产生PWM
{
	//将PWM_DAC的模拟输出频率设置为PWM载波频率相等，即10kHz
	 v->PeriodMax = SYSTEM_FREQUENCY/2/PWMDAC_FREQ/2*1000;  //PRD=200/2*1e6/2/50000=1000;

    /* Init Timer-Base Control Register for EPWM7/8*/
	EPwm7Regs.TBCTL.all = PWM_INIT_STATE;
	EPwm8Regs.TBCTL.all = PWM_INIT_STATE;

    /* Init Timer-Base Period Register for EPWM7/8*/
    EPwm7Regs.TBPRD = v->PeriodMax;
    EPwm8Regs.TBPRD = v->PeriodMax;

	/* Init Timer-Counter Register for EPWM7-EPWM8*/
	EPwm7Regs.TBCTR = 0;
	EPwm8Regs.TBCTR = 0;

    /* Init Compare Control Register for EPWM7/8*/  //这个地方之前配置为LOADMODE_ZERO好像不对；
    EPwm7Regs.CMPCTL.all = PWMDAC_CMPCTL_INIT_STATE;
    EPwm8Regs.CMPCTL.all = PWMDAC_CMPCTL_INIT_STATE;

    /* Init Action Qualifier Output A Register for EPWM7/8*/
    EPwm7Regs.AQCTLA.all = PWMDAC_AQCTLA_INIT_STATE;
    EPwm7Regs.AQCTLB.all = PWMDAC_AQCTLB_INIT_STATE;
    EPwm8Regs.AQCTLA.all = PWMDAC_AQCTLA_INIT_STATE;
    EPwm8Regs.AQCTLB.all = PWMDAC_AQCTLB_INIT_STATE;

    /* Init Dead-Band Generator Control Register for EPWM7/8*/
    // 只需要设置OUT_MODE = 0x00,即将IN_MODE与POLSEL直接旁路，AQ的EPWMA/B分别与DB的输出RPWMA/B连接;
    // DBM is fully disabled or by-passed. In this mode the POLSEL
    // and IN-MODE bits have no effect.
    EPwm7Regs.DBCTL.all = PWMDAC_DBCTL_INIT_STATE;
    EPwm8Regs.DBCTL.all = PWMDAC_DBCTL_INIT_STATE;

    /* Init PWM Chopper Control Register for EPWM6*/
    EPwm7Regs.PCCTL.all = PWMDAC_PCCTL_INIT_STATE;
    EPwm8Regs.PCCTL.all = PWMDAC_PCCTL_INIT_STATE;

    EALLOW;                           /* Enable EALLOW */

    /* Init Trip Zone Select Register*/
    EPwm7Regs.TZSEL.all = PWMDAC_TZSEL_INIT_STATE;
    EPwm8Regs.TZSEL.all = PWMDAC_TZSEL_INIT_STATE;

    /* Init Trip Zone Control Register*/
    EPwm7Regs.TZCTL.all = PWMDAC_TZCTL_INIT_STATE;
    EPwm8Regs.TZCTL.all = PWMDAC_TZCTL_INIT_STATE;

    EDIS;                             /* Disable EALLOW*/
}

//PwmDacCh1/2/3/4 是在调用占空比输出函数前被观测变量（实际值）赋值的中间变量，通过除以基值标幺化；
//duty1,duty2,duty3,duty4是标幺化的区间（0,1）内的占空比,分别对应4A(SCLK)/5A(RCLK)/6A(DIO)/6B
//Variables Watch by PWMDAC
void EPWMDAC_VAR_WATCH(PWMDAC *v)
{
	/* Update Timer-Base period Registers*/
	EPwm7Regs.TBPRD = v->PeriodMax;
	EPwm8Regs.TBPRD = v->PeriodMax;

	//CMPA的寄存器为32位，高16位为实际用到的CMPA值，低16位为HRPWM的CMPA值,CMPB值则为16位
	//经过测试，浮点转换为无符号整型为去除浮点尾数赋值，若浮点数小于-1则会溢出，但一般在-1~0范围内，转换后为0，无碍；
	EPwm7Regs.CMPA.bit.CMPA = (Uint16)(v->PwmDacCh3*v->PeriodMax);
	EPwm7Regs.CMPB.bit.CMPB = (Uint16)(v->PwmDacCh4*v->PeriodMax);
	EPwm8Regs.CMPA.bit.CMPA = (Uint16)(v->PwmDacCh1*v->PeriodMax);
	EPwm8Regs.CMPB.bit.CMPB = (Uint16)(v->PwmDacCh2*v->PeriodMax);
}


