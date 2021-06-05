/*
 * EPWM_Init.c
 *
 *  Created on: 2020��5��29��
 *      Author: YY
 */


#include "EPWM_Init.h"

// TODO EPWM1~3 Configuration
// �ؼ����ڶ��ڵ�������˵����CNT=PRDʱ�����ж�,ռ�ձȼ���ģʽΪ��Ҫ��CNT=PRDʱ���أ�������Ҫͬ��������Ϊ˫�������ã�
//����ΪCAU_CLEAR,CAD_SET,EPwm1Regs.ETSEL.bit.INTSEL = 2;
//��CNT=PRDʱ�򴥷�EPWM1�жϺ�SOCA�źţ���CMPCTL���ó���PRDʱCMPAӳ��Ĵ���װ��,ETSEL�Ĵ�����CNT=PRDʱ����PWM�ж�
//����ģʽ����ʯ��Ŀ���ò�һ��������CNT=PRDʱ����ռ�ձ�ˢ�£�ֻ��ͨ������CAU_CLEAR+CAD_SETʵ����ͬЧ����������Ϊ10k;
void EPWM_Setup1(PWM *v1)	//��Ӧmotor1��EPWM1~3ģ���ʼ������
{
     float temp1=0;

	 // Initialize PWM module, ������EPWMģ���ʱ��Ҫ��ѭһ�²��裺
	 // 1��ʹ�ܸ���ģ���ʱ��,����InitSysCtrl()���������;
	 // 2����TBCLKSYNCλ���㣬ֹͣ����ePWMģ���ʱ�ӣ�
	 // 3����ePWMģ���������
	 // 4����TBCLKSYNCλ��λ����������ePWMģ���ʱ��
	 EALLOW;
	 CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;

	 /* Init Timer-Base Control Register for EPWM1-EPWM3*/
	 // ��������������FREE_RUN_FLAG��������һ�μ�����ֹͣ���Ǽ���ѭ�����ڼ���;
	 // Ĭ��Ϊӳ��ģʽPRDLD_SHADOW�������Ҫ�ȴ�һ���ز����ڸ����ز�Ƶ�ʣ�������TBPRD�Ĵ���;
	 // ��������������ģʽ TIMER_CNT_UPDN;
	 // ��������ʱ�ӷ�Ƶϵ��HSPCLKDIV_PRESCALE_X_1*HSPCLKDIV_PRESCALE_X_1;
	 // ��Ĭ�ϲ���ƵTBCLK=EPWMCLK/(HSPCLKDIV * CLKDIV)=SYSCLKOUT/2/(1*1)=100MHz;
	 // ��λ����λPHSDIR_CNT_UPͬ���¼�������������ļ������������ϻ������£���������ģʽ��Ч��,��ʼ��Ϊ������������
	 /* TBCTL�� PHSEN��SYNCOSELλ�� ABC�����У�A��Ϊ��ģ�飬��Ҫ�������� */
	 EPwm1Regs.TBCTL.all = PWM_INIT_STATE;
	 EPwm2Regs.TBCTL.all = PWM_INIT_STATE;
	 EPwm3Regs.TBCTL.all = PWM_INIT_STATE;

     // EPWMģ����ƵĬ�Ͼ���PERCLKDIVSEL.EPWMCLKDIVλ����Ƶ��ΪCPU��Ƶ��1/2��Ҳ��ΪEPWMCLK=200/2=100MHz,
     // 16λTBCTR�İ����ڼ���ֵPeriodMax=TBPRD,up-down��������ģʽ
	 // Ҫע�ⲻ��floatǿ������ת�����ܻ����С����������ȥ������PWM_FREQUENCY=20�������PeriodMax = 2000,��ȷΪ2500,0.5��ȡ����ȥ;
	 temp1 = ((float)(SYSTEM_FREQUENCY/2)/(PWM_FREQUENCY))/2*1000;  //PRD=200/2*1e6/2/10000=5000;
	 v1->PeriodMax = temp1;

	 /*-----------------------------------------------------------------------------
 	   ��PWMģ���Դ���ͬ���źŵ�ģʽ������ePWM2/ePWM3�Ĵ����ź�Ϊǰ����ͬ���ź����,ePWM1��ͬ�������ź�����Ϊ�ⲿGPIO����(������)��
 	 �������ò���ePWM1���ⲿ����ͬ���ź���Ϊͬ����׼��������ePWM1��ͬ���ź������Ϊ�󼶸�ģ���ͬ����׼��
 	 ePWM1��ͬ���ź��������ΪCTR=0,Ϊ��ͬ���Ŵ��������룬����������EPWMģ�齫TBPHS��ֵװ�ص�CTR��
 	   ��ePWM1ģ��PHSENλdisbale�������ⲿͬ���źŴ�������ePWM1�����ͬ���ź�Ϊ����ePWMģ���ͬ���źŻ�׼��
	 -----------------------------------------------------------------------------*/
	 /* Allow each timer to be sync'ed*/     //������λͬ������ʹ��λ
	 EPwm1Regs.TBCTL.bit.PHSEN = 0;			//EPWM1��ʹ����λͬ��װ�ع��ܣ���EPWM1�����ⲿͬ���źſ��ƣ�
	 EPwm2Regs.TBCTL.bit.PHSEN = 1;			//ʹ����λͬ��װ�ع���
	 EPwm3Regs.TBCTL.bit.PHSEN = 1;			//ʹ����λͬ��װ�ع���

	 //TI��FCL�����е�PWM_1ch_UpDwnCnt_CNF������PHSEN,SYNCOSEL,TBPHS���ò�һ�������Ǻ����ֲ���ȥ�ˣ�����뱾�ļ�������ͬ!
	 EPwm1Regs.TBCTL.bit.SYNCOSEL = 1;       //���ͬ���źŴ����ź�ΪTB_CTR_ZERO
	 EPwm2Regs.TBCTL.bit.SYNCOSEL = 0;       //���ͬ���źŴ����ź�ΪEPWMxSYNCI��2�������ź�Ϊ1������ź�
	 EPwm3Regs.TBCTL.bit.SYNCOSEL = 0;       //���ͬ���źŴ����ź�ΪEPWMxSYNCI��3�������ź�Ϊ2������ź�

	 /* Init Timer-Base Phase Register for EPWM1-EPWM3*/
	 //������λͬ��װ����λ,���������ֲᣬTBCLK��=SYSCLKOUT,��λֵװ�ع�����1��TBCLK����ʱ�����������λװ��ֵΪ1
	 EPwm1Regs.TBPHS.bit.TBPHS = 0;
	 EPwm2Regs.TBPHS.bit.TBPHS = 1;
	 EPwm3Regs.TBPHS.bit.TBPHS = 1;

	 /* Init Timer-Base Period Register for EPWM1-EPWM3*/  //����PWM����,�������¼�����ʽfpwm=fsysclkout/2/PeriodMax
	 EPwm1Regs.TBPRD = v1->PeriodMax;
	 EPwm2Regs.TBPRD = v1->PeriodMax;
	 EPwm3Regs.TBPRD = v1->PeriodMax;

	 /* Init Timer-Counter Register for EPWM1-EPWM3*/
	 EPwm1Regs.TBCTR = 0;
	 EPwm2Regs.TBCTR = 0;
	 EPwm3Regs.TBCTR = 0;

	 /*-----------------------------------------------------------------------------
	 ͨ����LOADAMODE_PRD/LOADBMODE_PRD  ��CTR=PRDʱװ��CMPA��ֵ
	SHDWAMODE_SHADOW/SHDWBMODE_SHADOW ʹ��CMPA��ӳ��Ĵ��������壩
	----------------------------------------------------------------------------*/
	 /* Init Compare Control Register for EPWM1-EPWM3*/
	 // �����ӳ�ģʽ��CMPA�Ĵ����Ķ�д�������õ�CMPAӳ��Ĵ���
	 #if (CUR_CONTROL_MODE == DELAY_MODE)
		EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW; //ӳ��ģʽ��CMPA�Ĵ����Ķ�д����������CMPAӳ��Ĵ�����
		EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
		EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	 #endif
	 // ���ٵ�����ģʽ��CMPA�Ĵ����Ķ�д�����������õ�CMPA��ǰ�Ĵ���
	 #if (CUR_CONTROL_MODE == FCL_MODE)
		EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;  //����ģʽ��CMPA�Ĵ����Ķ�д����ֱ��������CMPA��ǰ�Ĵ�����
		EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;
		EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;
	 #endif
	 //----------------------------------------------------------------------------

	 //������ģʽ������TBCTR=TBPRDʱװ��CMPAӳ��Ĵ���ֵ����ǰCMPA�Ĵ���
	 #if (SAMPLING_METHOD == SINGLE_SAMPLING)
		EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
		EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
		EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
	 #endif
	 //˫����ģʽ����TBCTR=0��TBCTR=TBPRDʱ��װ��CMPAӳ��Ĵ���ֵ����ǰCMPA�Ĵ���
	 #if (SAMPLING_METHOD == DOUBLE_SAMPLING)
		EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
		EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
		EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
	 #endif
	 //----------------------------------------------------------------------------

	 /*-----------------------------------------------------------------------------
	 AQģ���޶�CTR=CMPAʱPWMA������ζ�����set or clear��**** �ؼ�����ģ��
	 CAU_SET:������ʱCTR=CMPA�øߣ�CAD_ClEAR:������ʱCTR=CMPA�õ͡���һ���ӦLOADAMODE=ZERO;
	 CAU_CLEAR:������ʱCTR=CMPA�õͣ�CAD_SET:������ʱCTR=CMPA�øߡ���һ���ӦLOADAMODE=PRD��
	 -----------------------------------------------------------------------------*/
	 /* Init Action Qualifier Output A Register for EPWM1-EPWM3*/ //����ռ�ձȲ�����β���(CTR=CMPAʱ���ø��õͻ��Ƿ�ת)
	 EPwm1Regs.AQCTLA.all = AQCTLA_INIT_STATE;
	 EPwm2Regs.AQCTLA.all = AQCTLA_INIT_STATE;
	 EPwm3Regs.AQCTLA.all = AQCTLA_INIT_STATE;

	 /*���ö����޶�ģ����������ģʽ�Ĵ���AQCSFRC��ӳ��Ĵ���װ�ص�ʱ��Ϊ����װ��immediate mode*/
	 EPwm1Regs.AQSFRC.bit.RLDCSF = 3;
	 EPwm2Regs.AQSFRC.bit.RLDCSF = 3;
	 EPwm3Regs.AQSFRC.bit.RLDCSF = 3;
	 //----------------------------------------------------------------------------

	 /* Init Dead-Band Generator Control Register for EPWM1-EPWM3*/
	 /* ������Ҫ��PWM���óɻ���ģʽ */
	 //����Ϊ����ģʽ����֮ǰAQģ���EPWMB�������·��DB�����EPWMB�ź�ֱ��������AQģ���EPWMA�źż��������࣬��AQ��EPWMB����޹أ�
	 EPwm1Regs.DBCTL.all = DBCTL_INIT_STATE;
	 EPwm2Regs.DBCTL.all = DBCTL_INIT_STATE;
	 EPwm3Regs.DBCTL.all = DBCTL_INIT_STATE;

	 /* ���û���ģʽ������ʱ�� */  //����ʱ������ΪDBFED*TBCLK=100/100MHz=1us
	 EPwm1Regs.DBFED.bit.DBFED = 100;
	 EPwm1Regs.DBRED.bit.DBRED = 100;
	 EPwm2Regs.DBFED.bit.DBFED = 100;
	 EPwm2Regs.DBRED.bit.DBRED = 100;
	 EPwm3Regs.DBFED.bit.DBFED = 100;
	 EPwm3Regs.DBRED.bit.DBRED = 100;;
	 //----------------------------------------------------------------------------

	 /* Init PWM Chopper Control Register for EPWM1-EPWM3*/  //������ɾ���ö�����
	 /* ���� PWM-chopper����һ��ն�����ܣ������ڵ�ƵPWM��Ϊ��ƵPWM�������Ƶ��ѹ�� */
	 EPwm1Regs.PCCTL.all = PCCTL_INIT_STATE;
	 EPwm2Regs.PCCTL.all = PCCTL_INIT_STATE;
	 EPwm3Regs.PCCTL.all = PCCTL_INIT_STATE;
	 //----------------------------------------------------------------------------

	 /* DC��ģ��Ĵ������ã����EPWM XBAR��CMPSSģ��ʵ��Ӳ�������������ܣ���ѡ����*/
	 // ����CMPSS�Ƚ��������ź������ϴ󣬻�����ʹ��event filter�˲����ܣ���GPIO�Ĳ������޶���࣬������ʱ�����ã�
	 /* DCACTL/DCBCTL���ƼĴ���Ĭ��ѡ��DCAEVT1/2,DCBEVT1/2��Ϊ����, ���˲�����ź�DCEVTFILT��·*/
	 #if EPWM_XBAR_DC
	 // ��ΪDCAEVT1,DCBEVT1�ſ�����OST����TZ��DCAEVT2,DCBEVT2��������CSC����TZ�����Ҫ��1��TRIP��������������¼�force�ź�;
     // ��Device_Init.c�ļ���������EPWM XBAR���ܣ���������������ADC���Ÿ���ΪCMPSS�������ţ�������������������������ۺϵ�TRIP4(motor1)��TRIP5(motor2)����;
     // motor1��ӦTRIP4���룬��ӦEPWM1~3�Ĵ�������;
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
     /***����ط���ʹ�� Enable TZ1  TZ2 as a one-shot trip source for this ePWM module*/
     /* ���ⲿTZ������ʱ��TZCTL��������������� */
     // ����INPUT XBAR���ܽ�IPM Error1�������ӵ��ڲ�TZ1�����ϣ�IPM Error2�������ӵ��ڲ�TZ2�����ϣ�
     // IPM Error�͵�ƽ��Ч��TZ�ź�Ҳ�ǵ͵�ƽ��������ʹ��IPM_Error��������δ�ӣ�ʹ��GPIO��������Ĭ��Ϊ�ߵ�ƽ�����ᴥ��TZ��
     // ͨ��TZCLR[OST]λ���TZFLG[OST]��־λ���Ѿ��궨��ΪEnable_PWMx_TZ
     // ���⣬���������ѡʹ��EPWM XBAR��DC��ģ����Ӳ����������������
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
	// ��ʼ��Ϊ0.5��ռ�ձȣ������������ܶ�·�������ܶ�·����ת�ز�����
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


void EPWMDAC_Setup(PWMDAC *v)    //��GPIO���Ÿ��ù��ܵĳ�ʼ����ʹ��PWM������ܣ����򲻻����PWM
{
	//��PWM_DAC��ģ�����Ƶ������ΪPWM�ز�Ƶ����ȣ���10kHz
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

    /* Init Compare Control Register for EPWM7/8*/  //����ط�֮ǰ����ΪLOADMODE_ZERO���񲻶ԣ�
    EPwm7Regs.CMPCTL.all = PWMDAC_CMPCTL_INIT_STATE;
    EPwm8Regs.CMPCTL.all = PWMDAC_CMPCTL_INIT_STATE;

    /* Init Action Qualifier Output A Register for EPWM7/8*/
    EPwm7Regs.AQCTLA.all = PWMDAC_AQCTLA_INIT_STATE;
    EPwm7Regs.AQCTLB.all = PWMDAC_AQCTLB_INIT_STATE;
    EPwm8Regs.AQCTLA.all = PWMDAC_AQCTLA_INIT_STATE;
    EPwm8Regs.AQCTLB.all = PWMDAC_AQCTLB_INIT_STATE;

    /* Init Dead-Band Generator Control Register for EPWM7/8*/
    // ֻ��Ҫ����OUT_MODE = 0x00,����IN_MODE��POLSELֱ����·��AQ��EPWMA/B�ֱ���DB�����RPWMA/B����;
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

//PwmDacCh1/2/3/4 ���ڵ���ռ�ձ��������ǰ���۲������ʵ��ֵ����ֵ���м������ͨ�����Ի�ֵ���ۻ���
//duty1,duty2,duty3,duty4�Ǳ��ۻ������䣨0,1���ڵ�ռ�ձ�,�ֱ��Ӧ4A(SCLK)/5A(RCLK)/6A(DIO)/6B
//Variables Watch by PWMDAC
void EPWMDAC_VAR_WATCH(PWMDAC *v)
{
	/* Update Timer-Base period Registers*/
	EPwm7Regs.TBPRD = v->PeriodMax;
	EPwm8Regs.TBPRD = v->PeriodMax;

	//CMPA�ļĴ���Ϊ32λ����16λΪʵ���õ���CMPAֵ����16λΪHRPWM��CMPAֵ,CMPBֵ��Ϊ16λ
	//�������ԣ�����ת��Ϊ�޷�������Ϊȥ������β����ֵ����������С��-1����������һ����-1~0��Χ�ڣ�ת����Ϊ0���ް���
	EPwm7Regs.CMPA.bit.CMPA = (Uint16)(v->PwmDacCh3*v->PeriodMax);
	EPwm7Regs.CMPB.bit.CMPB = (Uint16)(v->PwmDacCh4*v->PeriodMax);
	EPwm8Regs.CMPA.bit.CMPA = (Uint16)(v->PwmDacCh1*v->PeriodMax);
	EPwm8Regs.CMPB.bit.CMPB = (Uint16)(v->PwmDacCh2*v->PeriodMax);
}


