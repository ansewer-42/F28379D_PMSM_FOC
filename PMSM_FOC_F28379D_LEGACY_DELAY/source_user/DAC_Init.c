/*
 * DAC_Init.c
 *
 *  Created on: 2020��6��1��
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

// Variables Watch by DAC, �������Ϊռ�ձȣ�
void DAC_VAR_WATCH(DAC* w)
{
    float a=0, b=0;

    a= w->dutya*4096-1;
    b= w->dutyb*4096-1;
    a=_IQsat(a,4095,0);     //�޷�����
    b=_IQsat(b,4095,0);     //�޷�����

    w->DACA_Value = a;
    w->DACB_Value = b;
    DacaRegs.DACVALS.bit.DACVALS  = w->DACA_Value; // 0~4096,Shadow output code to be loaded into DACVALA;
    DacbRegs.DACVALS.bit.DACVALS  = w->DACB_Value; // 0~4096,Shadow output code to be loaded into DACVALA;

    w->VOL_A_Value = DACA_REF_VOL*(w->dutya);
    w->VOL_B_Value = DACB_REF_VOL*(w->dutyb);
}

// ����ʵ�ʵ�ѹֵ����������,3VΪ�ο���ѹֵ���������Ϊʵ�������ѹֵ;�ֱ���Ϊ3/4096=0.000732V
void DAC_VOL_OUT(DAC* w)
{
    float c=0,d=0;

    w->dutya= w->VOL_A_Value/DACA_REF_VOL;
    w->dutyb= w->VOL_B_Value/DACB_REF_VOL;

    c= w->dutya*4096-1;
    d= w->dutyb*4096-1;
    c=_IQsat(c,4095,0);     //�޷�����
    d=_IQsat(d,4095,0);     //�޷�����

    w->DACA_Value = c;
    w->DACB_Value = d;
    DacaRegs.DACVALS.bit.DACVALS  = w->DACA_Value; // 0~4096,Shadow output code to be loaded into DACVALA;
    DacbRegs.DACVALS.bit.DACVALS  = w->DACB_Value; // 0~4096,Shadow output code to be loaded into DACVALA;
}


//-----------------------------------------------------------------------------------------------
// TODO CMPSS Module Configuration
// A2-CMP1P,B2-CMP3P,A4-CMP2P; C2-CMP6P,D2-CMP8P,D0-CMP7P;
// ���ǲ�����ramp generator,digital filter
void CMPSS_Setup(void)
{
    EALLOW;

    // COMPDACCTL��COMPCTL�Ĵ���Ĭȫ��Ϊ0�����������Ӧ�ã�ѡ��ֱ���첽������������ϱ�Ӧ�����󣬲���Ҫ�������ã�
    Cmpss1Regs.COMPDACCTL.bit.DACSOURCE = 0;  // 0 DACHVALA is updated from DACHVALS, 1 updated from e ramp generator;
    Cmpss1Regs.COMPDACCTL.bit.SWLOADSEL = 0;  // 0,DACxVALA is updated from DACxVALS on SYSCLK;
    Cmpss1Regs.COMPDACCTL.bit.SELREF    = 0;  // 0,VDDA is the voltage reference for the DAC;
    Cmpss1Regs.COMPCTL.bit.COMPHSOURCE  = 0;  // 0,Inverting input of comparator driven by internal DAC;

    //�ϱȽ���ͬ��������±Ƚ����������������������ޱ�����
    Cmpss1Regs.COMPCTL.bit.COMPLINV     = 1;  // 1, Output of comparator is inverted;
    Cmpss3Regs.COMPCTL.bit.COMPLINV     = 1;  // 1, Output of comparator is inverted;
    Cmpss7Regs.COMPCTL.bit.COMPLINV     = 1;  // 1, Output of comparator is inverted;

    //CMPSS Comparator Hysteresis Control Register
    // ADC�����ѹ��ʵ�ʵ���������0.25V/A (��ѹ��Χ0~3V��Ӧ-6~6A�Ĳ���������0.25=3/12)
    // COMPHYS=1��Ӧ���Ϊ0.0097V���ͻ�����Ӧʵ���������С���Ϊ0.0097/0.25=0.0376A,����ʵ��������С�����ͻ���С��
    Cmpss1Regs.COMPHYSCTL.bit.COMPHYS   = 2;  // 1 Set to typical hysteresis, 12 CMPSS DAC LSB;
    Cmpss3Regs.COMPHYSCTL.bit.COMPHYS   = 2;  // 1 Set to typical hysteresis, 12 CMPSS DAC LSB;
    Cmpss7Regs.COMPHYSCTL.bit.COMPHYS   = 2;  // 1 Set to typical hysteresis, 12 CMPSS DAC LSB;

    // �Ƚ�����������˲�������digital filter,�ж�SAMPWIN�����������ڵĶ���ֵ��������ֵ��������ֵTHRESH��Ƚ������ֵ����Ϊ����ֵ��
    // 0.5*SAMPWIN<THRESH<=SAMPWIN����ȼ�Ҫ����SAMPWIN���������������ֵ�Բ���Ž��и��£�
    // ��GPIO���ŵ���������������޶����ܲ�࣬���Ǹ����������Զ���������ڲ���Ĵ���
    #if CMPSS_DIGITAL_FILTER
        Cmpss1Regs.CTRIPHFILCLKCTL.bit.CLKPRESCALE = 0x4;  //����ʱ�Ӷ�ϵͳʱ�ӽ��з�Ƶ��SAMPLE_CLOCK =SYSCLK/(CLKPRESCALE+1)=25ns
        Cmpss1Regs.CTRIPHFILCTL.bit.SAMPWIN = 0x28;        // 40*25=1us
        Cmpss1Regs.CTRIPHFILCTL.bit.THRESH  = 0x1E;        // 30*25=750ns,�ķ�֮���μ����ж������
        Cmpss1Regs.CTRIPLFILCLKCTL.bit.CLKPRESCALE = 0x4;  //����ʱ�Ӷ�ϵͳʱ�ӽ��з�Ƶ��SAMPLE_CLOCK =SYSCLK/(CLKPRESCALE+1)
        Cmpss1Regs.CTRIPLFILCTL.bit.SAMPWIN = 0x28;
        Cmpss1Regs.CTRIPLFILCTL.bit.THRESH  = 0x1E;
    #endif

    // Ĭ������ģ��Ƚ���ģ��Թرգ�����رգ���1������
    Cmpss1Regs.COMPCTL.bit.COMPDACE     = 1;  // 1,Comparator/DAC enabled;
    Cmpss3Regs.COMPCTL.bit.COMPDACE     = 1;  // 1,Comparator/DAC enabled;
    Cmpss7Regs.COMPCTL.bit.COMPDACE     = 1;  // 1,Comparator/DAC enabled;

    EDIS;
}

// ����CMPSSģ��ʵ�����������Ӳ������������CMPSS�ڲ�DAC�ıȽ���ֵ��
// TODO Phase Current Protection in CMPSS
void CMPSS_PHASE_CUR_PRO(CMPSS* w)
{
    EALLOW;

    //�����ȸ������±Ƚ���DAC�Ƚ�ֵ���ɽ��ú�������ѭ����PWM�ж�������
    Cmpss1Regs.DACHVALS.bit.DACVAL = w->DACH_Value1; // High DAC shadow value.
    Cmpss1Regs.DACLVALS.bit.DACVAL = w->DACL_Value1; // Low  DAC shadow value.
    Cmpss3Regs.DACHVALS.bit.DACVAL = w->DACH_Value3; // High DAC shadow value.
    Cmpss3Regs.DACLVALS.bit.DACVAL = w->DACL_Value3; // Low  DAC shadow value.
    Cmpss7Regs.DACHVALS.bit.DACVAL = w->DACH_Value7; // High DAC shadow value.
    Cmpss7Regs.DACLVALS.bit.DACVAL = w->DACL_Value7; // Low  DAC shadow value.

    EDIS;
}

//-----------------------------------------------------------------------------------------------
