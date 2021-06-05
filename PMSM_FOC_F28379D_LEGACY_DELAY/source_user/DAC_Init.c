/*
 * DAC_Init.c
 *
 *  Created on: 2020年6月1日
 *      Author: PMSM
 */

#include "DAC_Init.h"

// TODO DAC Module Configuration
void DAC_Setup(void)
{
    EALLOW;
    DacaRegs.DACCTL.bit.DACREFSEL  = 1; // 1, ADC VREFHI/VSSA are the reference voltages;
    DacaRegs.DACCTL.bit.LOADMODE   = 0; // 0, Load on next SYSCLK;
    DacaRegs.DACVALS.all           = 0; // 0~4096,Shadow output code to be loaded into DACVALA;
    DacaRegs.DACOUTEN.bit.DACOUTEN = 1; // 1, DAC output is enabled;
    DELAY_US(10); // Delay for buffered DAC to power up,Power-up time (DACOUTEN to DAC output valid)


    DacbRegs.DACCTL.bit.DACREFSEL  = 1; // 1, ADC VREFHI/VSSA are the reference voltages;
    DacbRegs.DACCTL.bit.LOADMODE   = 0; // 0, Load on next SYSCLK;
    DacbRegs.DACVALS.all           = 0; // 0~4096,Shadow output code to be loaded into DACVALA;
    DacbRegs.DACOUTEN.bit.DACOUTEN = 1; // 1, DAC output is enabled;
    DELAY_US(10); // Delay for buffered DAC to power up,Power-up time (DACOUTEN to DAC output valid)
    EDIS;
}

// Variables Watch by DAC, 输入参数为占空比；
void DAC_VAR_WATCH(DAC* w)
{
    float a=0, b=0;

    a= w->dutya*4096-1;
    b= w->dutyb*4096-1;
    a=_IQsat(a,4095,0);     //限幅处理
    b=_IQsat(b,4095,0);     //限幅处理

    w->DACA_Value = a;
    w->DACB_Value = b;
    DacaRegs.DACVALS.bit.DACVALS  = w->DACA_Value; // 0~4096,Shadow output code to be loaded into DACVALA;
    DacbRegs.DACVALS.bit.DACVALS  = w->DACB_Value; // 0~4096,Shadow output code to be loaded into DACVALA;

    w->VOL_A_Value = DACA_REF_VOL*(w->dutya);
    w->VOL_B_Value = DACB_REF_VOL*(w->dutyb);
}

// 根据实际电压值输入进行输出,3V为参考电压值，输入参数为实际输出电压值;分辨率为3/4096=0.000732V
void DAC_VOL_OUT(DAC* w)
{
    float c=0,d=0;

    w->dutya= w->VOL_A_Value/DACA_REF_VOL;
    w->dutyb= w->VOL_B_Value/DACB_REF_VOL;

    c= w->dutya*4096-1;
    d= w->dutyb*4096-1;
    c=_IQsat(c,4095,0);     //限幅处理
    d=_IQsat(d,4095,0);     //限幅处理

    w->DACA_Value = c;
    w->DACB_Value = d;
    DacaRegs.DACVALS.bit.DACVALS  = w->DACA_Value; // 0~4096,Shadow output code to be loaded into DACVALA;
    DacbRegs.DACVALS.bit.DACVALS  = w->DACB_Value; // 0~4096,Shadow output code to be loaded into DACVALA;
}


//-----------------------------------------------------------------------------------------------
// TODO CMPSS Module Configuration
// A2-CMP1P,B2-CMP3P,A4-CMP2P; C2-CMP6P,D2-CMP8P,D0-CMP7P;
// 若是不设置ramp generator,digital filter
void CMPSS_Setup(void)
{
    EALLOW;

    // COMPDACCTL与COMPCTL寄存器默全认为0，基本是最简单应用，选择直接异步输出，基本符合本应用需求，不需要额外配置；
    Cmpss1Regs.COMPDACCTL.bit.DACSOURCE = 0;  // 0 DACHVALA is updated from DACHVALS, 1 updated from e ramp generator;
    Cmpss1Regs.COMPDACCTL.bit.SWLOADSEL = 0;  // 0,DACxVALA is updated from DACxVALS on SYSCLK;
    Cmpss1Regs.COMPDACCTL.bit.SELREF    = 0;  // 0,VDDA is the voltage reference for the DAC;
    Cmpss1Regs.COMPCTL.bit.COMPHSOURCE  = 0;  // 0,Inverting input of comparator driven by internal DAC;

    //上比较器同相输出，下比较器反相输出，组成上下门限比器；
    Cmpss1Regs.COMPCTL.bit.COMPLINV     = 1;  // 1, Output of comparator is inverted;
    Cmpss3Regs.COMPCTL.bit.COMPLINV     = 1;  // 1, Output of comparator is inverted;
    Cmpss7Regs.COMPCTL.bit.COMPLINV     = 1;  // 1, Output of comparator is inverted;

    //CMPSS Comparator Hysteresis Control Register
    // ADC输入电压与实际电流比例是0.25V/A (电压范围0~3V对应-6~6A的采样电流，0.25=3/12)
    // COMPHYS=1对应宽度为0.0097V的滞环，对应实际相电流大小大概为0.0097/0.25=0.0376A,根据实际噪声大小设置滞环大小；
    Cmpss1Regs.COMPHYSCTL.bit.COMPHYS   = 2;  // 1 Set to typical hysteresis, 12 CMPSS DAC LSB;
    Cmpss3Regs.COMPHYSCTL.bit.COMPHYS   = 2;  // 1 Set to typical hysteresis, 12 CMPSS DAC LSB;
    Cmpss7Regs.COMPHYSCTL.bit.COMPHYS   = 2;  // 1 Set to typical hysteresis, 12 CMPSS DAC LSB;

    // 比较器输出数字滤波器功能digital filter,判断SAMPWIN个采样周期内的多数值，若多数值大于门限值THRESH则比较器输出值更新为多数值；
    // 0.5*SAMPWIN<THRESH<=SAMPWIN，相等即要求在SAMPWIN个采样周期内输出值皆不变才进行更新；
    // 与GPIO引脚的输入采样窗采样限定功能差不多，但是更加灵活，可以自定义采样窗内不变的次数
    #if CMPSS_DIGITAL_FILTER
        Cmpss1Regs.CTRIPHFILCLKCTL.bit.CLKPRESCALE = 0x4;  //采样时钟对系统时钟进行分频，SAMPLE_CLOCK =SYSCLK/(CLKPRESCALE+1)=25ns
        Cmpss1Regs.CTRIPHFILCTL.bit.SAMPWIN = 0x28;        // 40*25=1us
        Cmpss1Regs.CTRIPHFILCTL.bit.THRESH  = 0x1E;        // 30*25=750ns,四分之三次即可判断输出；
        Cmpss1Regs.CTRIPLFILCLKCTL.bit.CLKPRESCALE = 0x4;  //采样时钟对系统时钟进行分频，SAMPLE_CLOCK =SYSCLK/(CLKPRESCALE+1)
        Cmpss1Regs.CTRIPLFILCTL.bit.SAMPWIN = 0x28;
        Cmpss1Regs.CTRIPLFILCTL.bit.THRESH  = 0x1E;
    #endif

    // 默认所有模拟比较器模块皆关闭，清零关闭，置1开启；
    Cmpss1Regs.COMPCTL.bit.COMPDACE     = 1;  // 1,Comparator/DAC enabled;
    Cmpss3Regs.COMPCTL.bit.COMPDACE     = 1;  // 1,Comparator/DAC enabled;
    Cmpss7Regs.COMPCTL.bit.COMPDACE     = 1;  // 1,Comparator/DAC enabled;

    EDIS;
}

// 利用CMPSS模块实现相电流过流硬件保护，设置CMPSS内部DAC的比较阈值；
// TODO Phase Current Protection in CMPSS
void CMPSS_PHASE_CUR_PRO(CMPSS* w)
{
    EALLOW;

    //可在先更改上下比较器DAC比较值，可将该函数放在循环或PWM中断中运行
    Cmpss1Regs.DACHVALS.bit.DACVAL = w->DACH_Value1; // High DAC shadow value.
    Cmpss1Regs.DACLVALS.bit.DACVAL = w->DACL_Value1; // Low  DAC shadow value.
    Cmpss3Regs.DACHVALS.bit.DACVAL = w->DACH_Value3; // High DAC shadow value.
    Cmpss3Regs.DACLVALS.bit.DACVAL = w->DACL_Value3; // Low  DAC shadow value.
    Cmpss7Regs.DACHVALS.bit.DACVAL = w->DACH_Value7; // High DAC shadow value.
    Cmpss7Regs.DACLVALS.bit.DACVAL = w->DACL_Value7; // Low  DAC shadow value.

    EDIS;
}

//-----------------------------------------------------------------------------------------------
