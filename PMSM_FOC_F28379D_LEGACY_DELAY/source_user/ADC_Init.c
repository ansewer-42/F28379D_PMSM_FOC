/*
 * ADC_Init.c
 *
 *  Created on: 2020��6��3��
 *      Author: PMSM
 */

#include "ADC_Init.h"

/*  F28379D��ADCģ����������  Analog to Digital Conversion channels
    ia1/ib1/ic1 (motor1)  --->  ADC B2/A2/D0
    ua1/ub1/uc1 (motor1)  --->  ADC B3/A3/C2
    udc/idc (motor1)      --->  ADC C3/IN14
*/

#if ADC_OVER_SAMPLE
// TODO ADC_A~B Module Configuration
void ADC_Setup1(void)
{
    EALLOW;

    //********************************************
    // ADCA Module Registers Configuration
    //********************************************
    AdcaRegs.ADCCTL2.bit.PRESCALE    = 6;       //set ADCCLK divider to /4�� ADCCLK=SYSCLK/4=200MHz/4=50MHz;
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;       //Set pulse positions to late
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ    = 1;       //power up the ADC
    DELAY_US(1000);                             //delay for 1ms to allow ADC time to power up

    //��������������ͬһ������ͨ�����ж�β���
    AdcaRegs.ADCSOC0CTL.bit.CHSEL     = 2;       //SOC0 will convert pin A2
    AdcaRegs.ADCSOC0CTL.bit.ACQPS     = 20;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA
    AdcaRegs.ADCSOC1CTL.bit.CHSEL     = 2;       //SOC1 will convert pin A2
    AdcaRegs.ADCSOC1CTL.bit.ACQPS     = 20;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA
    AdcaRegs.ADCSOC2CTL.bit.CHSEL     = 2;       //SOC2 will convert pin A2
    AdcaRegs.ADCSOC2CTL.bit.ACQPS     = 20;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA
    AdcaRegs.ADCSOC3CTL.bit.CHSEL     = 2;       //SOC3 will convert pin A2
    AdcaRegs.ADCSOC3CTL.bit.ACQPS     = 20;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA

    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 3;       //3,end of SOC33 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E   = 1;       //1,enable ADCINT1 ;0, disable ADCINT1;��ʹ��λ��ʹ��ADC EOC�ж���Ϊ�����㷨�жϳ���ʱ��λΪ1��
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;       //make sure INT1 flag is cleared

    #if ADC_SAMPLE_ALL
        // ADCA3-ub1
        AdcaRegs.ADCSOC4CTL.bit.CHSEL     = 3;       //SOC0 will convert pin A3
        AdcaRegs.ADCSOC4CTL.bit.ACQPS     = 20;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
        AdcaRegs.ADCSOC4CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA
    #endif
    //-----------------------------------------------------------

    //********************************************
    // ADCB Module Registers Configuration
    //********************************************
    AdcbRegs.ADCCTL2.bit.PRESCALE    = 6;       //set ADCCLK divider to /4�� ADCCLK=SYSCLK/4=200MHz/4=50MHz;
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;       //Set pulse positions to late
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ    = 1;       //power up the ADC
    DELAY_US(1000);                             //delay for 1ms to allow ADC time to power up

    AdcbRegs.ADCSOC0CTL.bit.CHSEL     = 2;       //SOC0 will convert pin B2
    AdcbRegs.ADCSOC0CTL.bit.ACQPS     = 20;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA
    AdcbRegs.ADCSOC1CTL.bit.CHSEL     = 2;       //SOC1 will convert pin B2
    AdcbRegs.ADCSOC1CTL.bit.ACQPS     = 20;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA
    AdcbRegs.ADCSOC2CTL.bit.CHSEL     = 2;       //SOC2 will convert pin B2
    AdcbRegs.ADCSOC2CTL.bit.ACQPS     = 20;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA
    AdcbRegs.ADCSOC3CTL.bit.CHSEL     = 2;       //SOC3 will convert pin B2
    AdcbRegs.ADCSOC3CTL.bit.ACQPS     = 20;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA
    // ��Ҫ��λINT1Eʹ��ADCAINT1�жϣ�����AdcbRegs.ADCINTFLG.bit.ADCINT1��־λ������λ��
//    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 0;       //0,end of SOC0 will set INT1 flag
//    AdcbRegs.ADCINTSEL1N2.bit.INT1E   = 1;       //1,enable ADCINT1 ;0, disable ADCINT1;��ʹ��λ��ʹ��ADC EOC�ж���Ϊ�����㷨�жϳ���ʱ��λΪ1��
//    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;       //make sure INT1 flag is cleared

    #if ADC_SAMPLE_ALL
        // ADCB3-ua1
        AdcbRegs.ADCSOC4CTL.bit.CHSEL     = 3;       //SOC0 will convert pin B3
        AdcbRegs.ADCSOC4CTL.bit.ACQPS     = 14;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
        AdcbRegs.ADCSOC4CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA
    #endif
    //-----------------------------------------------------------

    EDIS;
}
#else    // end of "#if ADC_OVER_SAMPLE"
void ADC_Setup1(void)
{
    EALLOW;

    //********************************************
    // ADCA Module Registers Configuration
    //********************************************
    AdcaRegs.ADCCTL2.bit.PRESCALE    = 6;       //set ADCCLK divider to /4�� ADCCLK=SYSCLK/4=200MHz/4=50MHz;
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;       //Set pulse positions to late
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ    = 1;       //power up the ADC
    DELAY_US(1000);                             //delay for 1ms to allow ADC time to power up

    AdcaRegs.ADCSOC0CTL.bit.CHSEL     = 2;       //SOC0 will convert pin A2
    AdcaRegs.ADCSOC0CTL.bit.ACQPS     = 14;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
    //AdcaRegs.ADCSOC0CTL.bit.ACQPS     = 63;    //sample window is 63+1 SYSCLK cycles��64*5ns=320ns(16bit mode MIN)
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0;       //0,end of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E   = 1;       //1,enable ADCINT1 ;0, disable ADCINT1;��ʹ��λ��ʹ��ADC EOC�ж���Ϊ�����㷨�жϳ���ʱ��λΪ1��
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;       //make sure INT1 flag is cleared

    #if ADC_SAMPLE_ALL
        // ADCA3-ub1
        AdcaRegs.ADCSOC1CTL.bit.CHSEL     = 3;       //SOC0 will convert pin A3
        AdcaRegs.ADCSOC1CTL.bit.ACQPS     = 14;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
        AdcaRegs.ADCSOC1CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA
    #endif
    //-----------------------------------------------------------

    //********************************************
    // ADCB Module Registers Configuration
    //********************************************
    AdcbRegs.ADCCTL2.bit.PRESCALE    = 6;       //set ADCCLK divider to /4�� ADCCLK=SYSCLK/4=200MHz/4=50MHz;
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;       //Set pulse positions to late
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ    = 1;       //power up the ADC
    DELAY_US(1000);                             //delay for 1ms to allow ADC time to power up

    AdcbRegs.ADCSOC0CTL.bit.CHSEL     = 2;       //SOC0 will convert pin B2
    AdcbRegs.ADCSOC0CTL.bit.ACQPS     = 14;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
    //AdcbRegs.ADCSOC0CTL.bit.ACQPS     = 63;      //sample window is 63+1 SYSCLK cycles��64*5ns=320ns(16bit mode MIN)
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA
    // ��Ҫ��λINT1Eʹ��ADCAINT1�жϣ�����AdcbRegs.ADCINTFLG.bit.ADCINT1��־λ������λ��
//    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 0;       //0,end of SOC0 will set INT1 flag
//    AdcbRegs.ADCINTSEL1N2.bit.INT1E   = 1;       //1,enable ADCINT1 ;0, disable ADCINT1;��ʹ��λ��ʹ��ADC EOC�ж���Ϊ�����㷨�жϳ���ʱ��λΪ1��
//    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;       //make sure INT1 flag is cleared

    #if ADC_SAMPLE_ALL
        // ADCB3-ua1
        AdcbRegs.ADCSOC1CTL.bit.CHSEL     = 3;       //SOC0 will convert pin B3
        AdcbRegs.ADCSOC1CTL.bit.ACQPS     = 14;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
        AdcbRegs.ADCSOC1CTL.bit.TRIGSEL   = 5;       //trigger on ePWM1 SOCA
    #endif
    //-----------------------------------------------------------

    EDIS;
}
#endif

// TODO ADCAINT1 Interrupt Configuration
void ADCA1_Interrupt_Set(PINT ADCA1_ISR)
{
    EALLOW;

    //���������ж�ADCAINT1��ش��������溯�������ã�����ʹ�����輶��ADC�жϺ�,PWM_ISRģʽ����ʹ�ñ�־λAdcaRegs.ADCINTFLG.bit.ADCINT1���жϲ����Ƿ����;
    // ʵ���Ѿ���֤ʹ�ñ�־λAdcaRegs.ADCINTFLG.bit.ADCINT1���жϲ����Ƿ���ɲ�׼ȷ��
//    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0;       //0,end of SOC0 will set INT1 flag
//    AdcaRegs.ADCINTSEL1N2.bit.INT1E   = 1;       //1,enable ADCINT1 ;0, disable ADCINT1;��ʹ��λ��ʹ��ADC EOC�ж���Ϊ�����㷨�жϳ���ʱ��λΪ1��
//    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;       //make sure INT1 flag is cleared

    // Enable PIE group 1 interrupt 1 for ADCA1_ISR
    // ADCA1 match INT1.1, and have the 1th priority in PIE group1.
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;

    // Enable CPU INT1 for ADCA1_INT
    IER |= M_INT1;
    PieVectTable.ADCA1_INT = ADCA1_ISR;
    EDIS;
}

// TODO ADC_C~D Module Configuration
void ADC_Setup2(void)
{
    EALLOW;

    //********************************************
    // ADCC Module Registers Configuration
    //********************************************
    AdccRegs.ADCCTL2.bit.PRESCALE    = 6;       //set ADCCLK divider to /4�� ADCCLK=SYSCLK/4=200MHz/4=50MHz;
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;       //Set pulse positions to late
    AdccRegs.ADCCTL1.bit.ADCPWDNZ    = 1;       //power up the ADC
    DELAY_US(1000);                             //delay for 1ms to allow ADC time to power up

    AdccRegs.ADCSOC0CTL.bit.CHSEL     = 2;       //SOC0 will convert pin C2
    AdccRegs.ADCSOC0CTL.bit.ACQPS     = 14;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
    //AdccRegs.ADCSOC0CTL.bit.ACQPS     = 63;    //sample window is 63+1 SYSCLK cycles��64*5ns=320ns(16bit mode MIN)
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL   = 0x05;    //trigger on ePWM1 SOCA
//    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0;       //0,end of SOC0 will set INT1 flag
//    AdccRegs.ADCINTSEL1N2.bit.INT1E   = 1;       //1,enable ADCINT1 ;0, disable ADCINT1;��ʹ��λ��ʹ��ADC EOC�ж���Ϊ�����㷨�жϳ���ʱ��λΪ1��
//    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;       //make sure INT1 flag is cleared

    #if ADC_SAMPLE_ALL
        // ADCC3-udc
        AdccRegs.ADCSOC1CTL.bit.CHSEL     = 3;       //SOC0 will convert pin C3
        AdccRegs.ADCSOC1CTL.bit.ACQPS     = 14;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
        AdccRegs.ADCSOC1CTL.bit.TRIGSEL   = 0x05;       //trigger on ePWM1 SOCA
    #endif

    //-----------------------------------------------------------

    //********************************************
    // ADCD Module Registers Configuration
    //********************************************
    AdcdRegs.ADCCTL2.bit.PRESCALE    = 6;       //set ADCCLK divider to /4�� ADCCLK=SYSCLK/4=200MHz/4=50MHz;
    AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;       //Set pulse positions to late
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ    = 1;       //power up the ADC
    DELAY_US(1000);                             //delay for 1ms to allow ADC time to power up

    AdcdRegs.ADCSOC0CTL.bit.CHSEL     = 0;       //SOC0 will convert pin D0
    AdcdRegs.ADCSOC0CTL.bit.ACQPS     = 14;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
    //AdcdRegs.ADCSOC0CTL.bit.ACQPS     = 63;      //sample window is 63+1 SYSCLK cycles��64*5ns=320ns(16bit mode MIN)
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL   = 0x05;       //trigger on ePWM1 SOCA
     // ��Ҫ��λINT1Eʹ��ADCAINT1�жϣ�����AdcdRegs.ADCINTFLG.bit.ADCINT1��־λ������λ��
//    AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = 0;       //0,end of SOC0 will set INT1 flag
//    AdcdRegs.ADCINTSEL1N2.bit.INT1E   = 1;       //1,enable ADCINT1 ;0, disable ADCINT1;��ʹ��λ��ʹ��ADC EOC�ж���Ϊ�����㷨�жϳ���ʱ��λΪ1��
//    AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;       //make sure INT1 flag is cleared
    #if ADC_SAMPLE_ALL
        // ADCIN14-idc
        AdcdRegs.ADCSOC1CTL.bit.CHSEL     = 14;       //SOC0 will convert pin ADCIN14
        AdcdRegs.ADCSOC1CTL.bit.ACQPS     = 14;      //sample window is 14+1 SYSCLK cycles��15*5ns=75ns(12bit mode MIN)
        AdcdRegs.ADCSOC1CTL.bit.TRIGSEL   = 0x05;       //trigger on ePWM1 SOCA
    #endif
    //-----------------------------------------------------------

    EDIS;
}

// TODO ADCCINT1 Interrupt Configuration
void ADCC1_Interrupt_Set(PINT ADCC1_ISR)
{
    EALLOW;

    //���������ж�ADCCINT1��ش��������溯�������ã�����ʹ�����輶��ADC�жϺ�,PWM_ISRģʽ����ʹ�ñ�־λAdccRegs.ADCINTFLG.bit.ADCINT1���жϲ����Ƿ����;
//    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0;       //0,end of SOC0 will set INT1 flag
//    AdccRegs.ADCINTSEL1N2.bit.INT1E   = 1;       //1,enable ADCINT1 ;0, disable ADCINT1;��ʹ��λ��ʹ��ADC EOC�ж���Ϊ�����㷨�жϳ���ʱ��λΪ1��
//    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;       //make sure INT1 flag is cleared

    // Enable PIE group 1 interrupt 3 for ADCC1_ISR
    // ADCC1 match INT1.3, and have the 3th priority in PIE group1.
    PieCtrlRegs.PIEIER1.bit.INTx3 = 1;

    // Enable CPU INT1 for ADCC1_INT
    IER |= M_INT1;
    PieVectTable.ADCC1_INT = ADCC1_ISR;
    EDIS;
}

/*
Analog to Digital Conversion channels
    ia1/ib1/ic1 (motor1)  --->  ADC B2/A2/D0
    ua1/ub1/uc1 (motor1)  --->  ADC B3/A3/C2
    udc/idc (motor1)      --->  ADC C3/IN14
*/
//----------------------------------------------------------------------
// TODO ADC zero offset acquisition and handle
// �ϵ���ƫ�Լ�⣬��������N�β��Բ������������Sum��N�κ�������ƫ������ƽ��ֵSum/N;
void ADC_Zero_Offset(ADC *p)
{
    if(p->Zero_Offset_Count<=128)
    {
        p->Result.Ia_Sum  += (AdcbResultRegs.ADCRESULT0);
        p->Result.Ib_Sum  += (AdcaResultRegs.ADCRESULT0);
        p->Result.Ic_Sum  += (AdcdResultRegs.ADCRESULT0);
        p->Result.Ua_Sum  += (AdcbResultRegs.ADCRESULT1);
        p->Result.Ub_Sum  += (AdcaResultRegs.ADCRESULT1);
        p->Result.Uc_Sum  += (AdccResultRegs.ADCRESULT0);
        p->Result.Idc_Sum += (AdcdResultRegs.ADCRESULT1);
        p->Result.Udc_Sum += (AdccResultRegs.ADCRESULT1);

        p->Zero_Offset_Count++;
        if(p->Zero_Offset_Count==128)
        {
            p->Result.Ia_Zero_Offset  = p->Result.Ia_Sum>>7;
            p->Result.Ib_Zero_Offset  = p->Result.Ib_Sum>>7;
            p->Result.Ic_Zero_Offset  = p->Result.Ic_Sum>>7;
            p->Result.Ua_Zero_Offset  = p->Result.Ua_Sum>>7;
            p->Result.Ub_Zero_Offset  = p->Result.Ub_Sum>>7;
            p->Result.Uc_Zero_Offset  = p->Result.Uc_Sum>>7;
            p->Result.Idc_Zero_Offset = p->Result.Idc_Sum>>7;
            p->Result.Udc_Zero_Offset = p->Result.Udc_Sum>>7;

            p->Result.Ia_Sum  = 0;
            p->Result.Ib_Sum  = 0;
            p->Result.Ic_Sum  = 0;
            p->Result.Ua_Sum  = 0;
            p->Result.Ub_Sum  = 0;
            p->Result.Uc_Sum  = 0;
            p->Result.Idc_Sum = 0;
            p->Result.Udc_Sum = 0;
            p->Zero_Offset_Count = 0;

            p->Flag.bit.Zero_Offset_End = 1;    //ADC���ƫ�ò���������־λ
        }
    }
}

// ADC��������Ĵ�����ȡ��28379D��ADC�������������������16λ����Ĵ����ĵ�12λ
// SOC0Ϊ��ǰ����ת��ֵ������SOCxΪ��һ����ADC����ֵ�������ȡ��Ia/Ib/Ic��Ϊ��ǰ����ֵ��
void ADC_Result_Read(ADC *p)
{
    #if ADC_OVER_SAMPLE
        Uint16 R_Temp1=0, R_Temp2=0;

        R_Temp1  = (AdcbResultRegs.ADCRESULT0+AdcbResultRegs.ADCRESULT1);
        R_Temp2  = (AdcbResultRegs.ADCRESULT2+AdcbResultRegs.ADCRESULT3);
        p->Result.Ia  = (R_Temp1+R_Temp2)>>2;
        R_Temp1  = (AdcaResultRegs.ADCRESULT0+AdcaResultRegs.ADCRESULT1);
        R_Temp2  = (AdcaResultRegs.ADCRESULT2+AdcaResultRegs.ADCRESULT3);
        p->Result.Ib  = (R_Temp1+R_Temp2)>>2;
    #else
        p->Result.Ia  = (AdcbResultRegs.ADCRESULT0);
        p->Result.Ib  = (AdcaResultRegs.ADCRESULT0);
    #endif

    p->Result.Ic  = (AdcdResultRegs.ADCRESULT0);
    p->Result.Ua  = (AdcbResultRegs.ADCRESULT1);
    p->Result.Ub  = (AdcaResultRegs.ADCRESULT1);
    p->Result.Uc  = (AdccResultRegs.ADCRESULT0);
    p->Result.Idc = (AdcdResultRegs.ADCRESULT1);
    p->Result.Udc = (AdccResultRegs.ADCRESULT1);
}

// TODO ADC�����������������������ȥ��ƫ���������ٳ���ADC����ת��ϵ���任Ϊʵ��ģ������ʵֵ��
void ADC_Result_Handle(ADC *p)
{
    volatile int16 i,Temp1;
    volatile float Udc_Temp,Idc_Temp,Utemp;

    Temp1 = p->Result.Ia-p->Result.Ia_Zero_Offset;
    p->Result.Current_A= Temp1*Current_Phase_Ratio;

    Temp1 = p->Result.Ib-p->Result.Ib_Zero_Offset;
    p->Result.Current_B= Temp1*Current_Phase_Ratio;

    Temp1 = p->Result.Ic-p->Result.Ic_Zero_Offset;
    p->Result.Current_C= Temp1*Current_Phase_Ratio;

    Temp1 = p->Result.Idc-p->Result.Idc_Zero_Offset;
    Idc_Temp= Temp1*Current_DC_Ratio;

    Temp1 = p->Result.Udc-p->Result.Udc_Zero_Offset;
    Udc_Temp = Temp1*Voltage_DC_Ratio;

    #if 0               //ABC����˵�ѹ�������㣬��ʱ����ʱע��
        Temp1 = p->Result.Ua-p->Result.Ua_Zero_Offset;
        p->Result.Voltage_A= Temp1*Voltage_Phase_Ratio;

        Temp1 = p->Result.Ub-p->Result.Ub_Zero_Offset;
        p->Result.Voltage_B= Temp1*Voltage_Phase_Ratio;

        Temp1 = p->Result.Uc-p->Result.Uc_Zero_Offset;
        p->Result.Voltage_C= Temp1*Voltage_Phase_Ratio;
    #endif
    //----------------------------------------------------------------------

    if(fabsf(p->Result.Current_A)>4.8)          //���A�������������������3�β��������������������λ��λ
    {
        p->Fault_Current_A_Count++;
        if(p->Fault_Current_A_Count>=3)
        {
            DISABLE_PWM1;
            p->Flag.bit.OverCurrent_A_Flag = 1;
            p->Flag.bit.Over_Fault_Flag = 1;
        }
    }
    else
    {
        p->Fault_Current_A_Count=0;
    }

    if(fabsf(p->Result.Current_B)>4.8)      //���B�������������������3�β��������������������λ��λ
    {
        p->Fault_Current_B_Count++;
        if(p->Fault_Current_B_Count>=3)
        {
            DISABLE_PWM1;
            p->Flag.bit.OverCurrent_B_Flag = 1;
            p->Flag.bit.Over_Fault_Flag = 1;
        }
    }
    else
    {
        p->Fault_Current_B_Count=0;
    }

    #if 0
    if(fabsf(Udc_Temp)>25)              //���ĸ�ߵ�ѹ��ѹ����������5�β���������ѹ���������λ��λ
    {
        p->Fault_Voltage_DC_Count++;
        if(p->Fault_Voltage_DC_Count>=3)
        {
            DISABLE_PWM1;
            p->Flag.bit.OverVoltage_DC_Flag = 1;
            p->Flag.bit.Over_Fault_Flag = 1;
        }
    }
    else
    {
        p->Fault_Voltage_DC_Count=0;
    }

    if(fabsf(Idc_Temp)>3)               //���ĸ�ߵ�����������������5�β��������������������λ��λ
    {
        p->Fault_Current_DC_Count++;
        if(p->Fault_Current_DC_Count>=3)
        {
            DISABLE_PWM1;
            p->Flag.bit.OverCurrent_DC_Flag = 1;
            p->Flag.bit.Over_Fault_Flag = 1;
        }
    }
    else
    {
        p->Fault_Current_DC_Count=0;
    }

    //Idcĸ�ߵ��������쳣ֵ�޳������ǰ�����β���ֵ��100us��ƫ��3A,����Ϊ���쳣ֵ�����˲���ֵʹ���ϴ�ֵ
    if(fabsf(Idc_Temp-p->Result.Current_DC_Last)>3)
    {
        Idc_Temp = p->Result.Current_DC_Last;
    }
    #endif

    //AB����������쳣ֵ�޳������ǰ�����β���ֵ��100us��ƫ��3A,����Ϊ���쳣ֵ
//    if(fabsf(p->Result.Current_A-p->Result.Current_A_Last)>1.5)
//    {
//        p->Result.Current_A = p->Result.Current_A_Last;
//    }
//    if(fabsf(p->Result.Current_B-p->Result.Current_B_Last)>1.5)
//    {
//        p->Result.Current_B = p->Result.Current_B_Last;
//    }

    //ֱ��ĸ�ߵ����˲�,��Ҫ����ֱ��ĸ�ߵ���������
    p->Result.Current_DC = p->Filter0_Idc*Idc_Temp + p->Filter1_Idc*p->Result.Current_DC;

    //ֱ��ĸ�ߵ�ѹ�˲�����Ҫ�������ź͵���������޷�
    Utemp = p->Filter0_Udc*Udc_Temp + p->Filter1_Udc*p->Result.Voltage_DC;
    p->Result.Voltage_DC = _IQsat(Utemp,24,0);      //������24V
    p->Result.Voltage_DC = 24;                      //���ʵ�����ѹ������׼ȷ��ֱ�Ӹ��̶�ֵ24V��

    //�����ݴ����ˢ��
    p->Result.Current_A_Last  = p->Result.Current_A;
    p->Result.Current_B_Last  = p->Result.Current_B;
    p->Result.Current_DC_Last = p->Result.Current_DC;
    p->Result.Voltage_DC_Last = p->Result.Voltage_DC;
}
//----------------------------------------------------------------------

