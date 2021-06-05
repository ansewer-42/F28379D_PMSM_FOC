/*
 * PMSM_FOC_Main.c
 * ----------------------------------------------------------------------------------
 * Description
 * This file is used for the definition of main function, motor control ISR and state machine;
 * ----------------------------------------------------------------------------------
 *  Created on: 2020��6��18��
 *      Author: YY
 * ----------------------------------------------------------------------------------
 */


/************************************************************************************/
//	Peripheral connections and functions:
/************************************************************************************/
/*
 * Single-core control, CPU1 control the FOC process of motor1 and motor2;
 *
    EPWMs
		EPWM1, EPWM2, EPWM3 ---> Inverter PWMs for phases A1, B1, C1 (motor1)
		EPWM7, EPWM8        ---> PWM_DAC output, General purpose display;

    Analog to Digital Conversion channels
        ia1/ib1/ic1 (motor1)  --->  ADC B2/A2/D0
        ua1/ub1/uc1 (motor1)  --->  ADC B3/A3/C2
        udc/idc (motor1)      --->  ADC C3/IN14

    CMPSS
        ADCINB2����CPMIN3P, ADCINA2����CPMIN1P, ADCIND0����CPMIN7P;
        ADCIN14����CPMIN4P;

	QEPs
		EQEP1A	--->	encoder_A1 (motor1)
		EQEP1B	--->	encoder_B1 (motor1)
		EQEP1Z	--->	encoder_Z1 (motor1)

	Digital to Analog Conversion channels
		DAC-A  ---> General purpose display
		DAC-B  ---> General purpose display

	GPIO Input/Output function
		GPIO29:OUT ---> PWM enable signal output (motor1)
		GPIO22:IN  ---> IPM error signal input (motor1)
		//--------����Position Connector: Bottom Board----------------------------
		GPIO32:IN  ---> Hall-U1, Position hall signal input (motor1)
		GPIO67:IN  ---> Hall-V1, Position hall signal input (motor1)
		GPIO111:IN ---> Hall-W1, Position hall signal input (motor1)
		//--------����Position Connector: Transfer Board (for back up)------------
		GPIO94:IN  ---> Hall-U1, Position hall signal input (motor1)
		GPIO97:IN  ---> Hall-V1, Position hall signal input (motor1)
		GPIO95:IN  ---> Hall-W1, Position hall signal input (motor1)
        //----------Switch S1~S4 Status Input-----------------------------------
        GPIO131:IN ---> S1,Read 1 while button down, Read 0 while button up;
        GPIO130:IN ---> S2,Read 1 while button down, Read 0 while button up;
        GPIO27 :IN ---> S3,Read 1 while button down, Read 0 while button up;
        GPIO26 :IN ---> S4,Read 1 while button down, Read 0 while button up;
*/
//------------------------------------------------------------------------------------


/************************************************************************************/
//	Include header files used in the main function
/************************************************************************************/
#include "PMSM_FOC_Main.h"

//------------------------------------------------------------------------------------


/************************************************************************************/
//	Code Section Mapping to ramfunc of the MotorControlISR function
/************************************************************************************/
#ifdef _FLASH
#ifndef __cplusplus
    #ifdef __TI_COMPILER_VERSION__
        #if __TI_COMPILER_VERSION__ >= 15009000
            #pragma CODE_SECTION(MotorControlISR,".TI.ramfunc");
        #else
            #pragma CODE_SECTION(MotorControlISR,"ramfuncs");
        #endif
    #endif
#endif
#endif
//------------------------------------------------------------------------------------


/************************************************************************************/
// Prototype statements for Local Functions
/************************************************************************************/
// LPI����˼��Low Priority Interrupt �����ȼ��жϣ�������C28x����ͨ�жϣ����Ա�Ƕ�ף�
#pragma INTERRUPT (Motor_Control_ISR1, LPI)
interrupt void Motor_Control_ISR1(void);
interrupt void EPWM1_TripZone_ISR(void);
//------------------------------------------------------------------------------------

float i;

// ****************************************************************************
//TODO  main function entry
// ***********************************************************************
void main(void)
{
    /*
	* ����Ҫ��д��Flash��ֻ��Ҫ:
	* 1) ����_FLASHԤ����#define����
	* ���������ڹ����ļ���Properties/CCS Build/C2000 Compiler/Predefined Symbols ���� �ڹ����ļ���#define _FLASH���ɣ�
	* ��������InitFlash()��System_Init()�����У�memcpy()��System_Init()/InitSysCtrl()������;
	* 2) ��CMD�ļ��滻Ϊ2837xD_FLASH_lnk_cpu1.cmd
	*/
	System_Init();			 	    //SYSTEM Controlϵͳ����ģ���ʼ��
	GPIO_Init();			 	    //GPIOģ���ʼ��
    DAC_Setup();                    //DACģ���ʼ��
    CMPSS_Setup();                  //CMPSSģ���ʼ��
    CMPSS_PHASE_CUR_PRO(&cmpss1);   //CMPSS�ڲ�DAC����ֵ��ʼ��
	ADC_Setup1();		 	        //ADC_A~Bģ���ʼ��
	ADC_Setup2();                   //ADC_C~Dģ���ʼ��
	EQEP_Setup1(&qep1);		 	    //EQPEP1ģ���ʼ��
    EPWM_Setup1(&pwm1);             //EPWM1~3ģ���ʼ������Ӧmotor1
    EPWMDAC_Setup(&pwmdac1);        //PWMDACģ���ʼ��

	// �ж��������
    #if (CONTROL_ISR_MODE == ADC_ISR)
        ADCA1_Interrupt_Set(&Motor_Control_ISR1);
    #else  //(CONTROL_ISR_MODE == PWM_ISR)
        EPWM1_Interrupt_Set(&Motor_Control_ISR1);       //EPWM1�ж���ؼĴ������ã�������ʱģʽ��EPWM1�жϷ������������˫������Ƴ���
    #endif
    EnableInterrupts();
    ERTM;

    DISABLE_PWM1;                   //PWM���ʹ�ܽ�����
    DISABLE_PWM_PINS_M1;            //PWM�����޶��������
    Global_Variables_Clear();           //ȫ�ֱ�������
    Global_Variables_Init();            //ȫ�ֱ�����ʼ��
    Controller_Params_Preset();         //PID,SMC�ȿ�����������ʼ��

	for(;;)  //infinite loop
	{
        /******************************************************/
        /***********************Code For Debug*****************/
        /******************************************************/
        for(i=0;i<5;i++)
        {
            RED_LED_ON;
            BLUE_LED_OFF;
            //    asm(" RPT #2000000 || NOP");
            //    DELAY_US(1000000);        // 1s
            DELAY_US(100000);
            RED_LED_OFF;
            BLUE_LED_ON;
            DELAY_US(100000);
        }

        CMPSS_PHASE_CUR_PRO(&cmpss1);

        // ��ʼ��¼����ǰ�ȸ���Iq_Ref/Id_Ref�Ļ���ƫ��ֵ��
        // ���δ洢ֻ��Ҫ��ģʽѡΪMode_Single_enm���Զ�������ֵ���н�Ծ���������Զ������������
        // ����ģʽ��Ĭ��ΪMode_Continuous_enm, ֻ��Ҫ��FLAG1.bit.Data_Record_ON ��1������ɵ��δ洢����;
        if(Data_Record.Dlog_Mode == Mode_Single_enm)
        {

            // ע������ÿ�ζ���ˢ�£�2kHz���������500*500us=250ms����������LED��ʱʱ�䣬������ˢ�¸�����ֵ��Id/Iq_Dlog_old
            Data_Record.Id_Dlog_old  = User_Terminal.in.Id_Ref;
            Data_Record.Iq_Dlog_old  = User_Terminal.in.Iq_Ref;
            Data_Record.Ud_Dlog_old  = User_Terminal.in.Ud_Disturbance;
            Data_Record.Uq_Dlog_old  = User_Terminal.in.Uq_Disturbance;

            User_Terminal.in.Id_Ref  = Data_Record.Id_Dlog;
            User_Terminal.in.Iq_Ref  = Data_Record.Iq_Dlog;
            User_Terminal.in.Ud_Disturbance = Data_Record.Ud_Dlog;
            User_Terminal.in.Uq_Disturbance = Data_Record.Uq_Dlog;
            FLAG1.bit.User_Input_ON  = 1;   // ʵ��User_Input�����־λһֱû�ã�����ȥ����
            FLAG1.bit.Data_Record_ON = 1;
        }


        //---------------------------------------------------------
	}

}


// ****************************************************************************
// ****************************************************************************
//TODO Motor Control ISR2 //Delay Mode,dual motor contorl;FCL Mode, motor2 contorl;
// ****************************************************************************
// ****************************************************************************
interrupt void Motor_Control_ISR1(void)
{
    GpioDataRegs.GPDSET.bit.GPIO123 = 1;  // GPIO10����ʾ������ʾ����ж����ڼ���������ʱ��

    #if DUAL_MOTOR_SYNC    //ÿ��������ݴ洢save memory,��Ҫ����һ��Data_Record.FLAG��־λ��0������
    if((Data_Record.FLAG==0) && (GpioDataRegs.GPDDAT.bit.GPIO124 == 1))   //����motor2��ͬ���ź�ȷ���Ƿ�ʼ�洢����
    {
        FLAG1.bit.Data_Record_ON = 1;
        GpioDataRegs.GPDSET.bit.GPIO122 = 1;  // GPIO10����ʾ������ʾ����ж����ڼ���������ʱ��
        Data_Record.FLAG = 1;                 // ������־λ������FPIO124��һֱ�����жϽ�Data_Record��־λ��λΪ1;
    }                                         //  ÿ��motor2����ͻ��֮�䶼Ҫ��Data_Record.FLAG��־λ��0��
    #endif

    #if (CONTROL_ISR_MODE == PWM_ISR)
    {
        asm(" RPT #72 || NOP");    //������75ns+280ns=355ns�����һ��ͨ����ת��,ʵ�ʹ۲�ʾ����������СRPT #25����,20������;
    //        DELAY_US(1);
    //        while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);  //PWM_ISRģʽ�øñ�־λ�жϲ�׼ȷ����Ҫ��һ����ʱ�������ȴ����ĺ������Ѿ���ʾ�����۲�SUM��֤��
    }
    #endif

    //--------------------------------------------------------------------------------------------------
    //���ڹ�ѹ���������źŷ������ϵͳ����,���IGBT�ź��Ƿ���λ�����ֶ������ź��Ƿ����
    if((IPM_ERROR1==1) && (FLAG1.bit.GAME_RESTART==1))
    {
        ENABLE_PWM1;
        FLAG1.bit.GAME_RESTART = 0;
        adc1.Flag.all = 0;
        adc1.Fault_Current_A_Count = 0;
        adc1.Fault_Current_B_Count = 0;
        adc1.Fault_Current_DC_Count = 0;
        adc1.Fault_Voltage_DC_Count = 0;
        FLAG1.bit.FAULT_ON = 0;

        EALLOW;         //TZFLG��־λ���
        EPwm1Regs.TZCLR.bit.OST = 1;
        EPwm1Regs.TZCLR.bit.DCAEVT1 = 1;
        EPwm1Regs.TZCLR.bit.DCBEVT1 = 1;
        EPwm2Regs.TZCLR.bit.OST = 1;
        EPwm2Regs.TZCLR.bit.DCAEVT1 = 1;
        EPwm2Regs.TZCLR.bit.DCBEVT1 = 1;
        EPwm3Regs.TZCLR.bit.OST = 1;
        EPwm3Regs.TZCLR.bit.DCAEVT1 = 1;
        EPwm3Regs.TZCLR.bit.DCBEVT1 = 1;
        EDIS;

        Global_Variables_Clear();
    }

    if(FLAG1.bit.VAR_CLEAR==1)          //ȫ�ֱ����ֶ�����
    {
        Global_Variables_Clear();
        FLAG1.bit.VAR_CLEAR = 0;
    }

    //--------------------------------------------------------------------------------------------------
    if((IPM_ERROR1 == 0)||(adc1.Flag.bit.Over_Fault_Flag==1)||(EPwm1Regs.TZFLG.bit.OST==1))  //������ѹ�����źŷ�������
    {
        DISABLE_PWM1;
        FLAG1.bit.GAME_ON = 0;
        FLAG1.bit.GAME_RESTART=0;
        FLAG1.bit.FAULT_ON = 1;
        User_Terminal.in.GAME_MODE = Mode_Stop_enm;
    }

    if(EQep1Regs.QEPSTS.bit.FIMF==0)    //�Ƚ���ת����Ƕ�λ�ü��㣬�ٽ��е���������ʡ����ʱ�䣻
    {
        Hall_Angle_Cal(&qep1);          //�����������ǶȲ���
    }
    else
    {
        EQEP_Angle_Cal(&qep1);          //��Ƕȼ���
        EQEP_Speed_Cal(&qep1);          //ʵʱת�ټ���
    }

    if(!adc1.Flag.bit.Zero_Offset_End)
    {
        ADC_Zero_Offset(&adc1);         //ADCͨ�����ƫ�ò�������GAME_ON=0����ܣ�ʱ���Ͻ������Ե����
    }                                   //Ҫ��Ϊ�����ϵ��ʼ������һ�β�������ɾ��GAME_ON=0ʱ��else�е�Zero_Offset_End=0��
    else
    {
        ADC_Result_Read(&adc1);         //ADC���������ȡ������
        ADC_Result_Handle(&adc1);
    }

    //------------------------------------------------------------------------------------------
    /* ���ݲɵõ���ĸ�ߵ�ѹֵʵʱ����DQ��������Ļ��ּ�����޷�ֵ */
    Process_Data.Uref_Max  = _IQmpy(adc1.Result.Voltage_DC,_IQ(0.55));     //SVPWM��������ѹԲ�뾶�����������������ѹ
    Voltage_OutMax_Update();       //���������������ѹ�޷�ֵ����ĸ�߲�����ѹֵ����

    //------------------------------------------------------------------------------------------
    if(FLAG1.bit.GAME_OVER == 1)            //ֱ���ֶ�ͣ����־λģʽ
    {
        User_Terminal.in.GAME_MODE = Mode_Stop_enm;
    }

    if(User_Terminal.in.GAME_MODE == Mode_Stop_enm)         //ͣ��ģʽ��PWM���ұ������㣻
    {
        FLAG1.bit.GAME_ON = 0;
        Global_Variables_Clear();
    }
    else
    {
        FLAG1.bit.GAME_ON = 1;
    }

    //------------------------------------------------------------------------------------------
    if(FLAG1.bit.CUR_CNT_Kp_ON == 1)        //ʹ��Kp_Ratio���������޸ĵ�������������
    {
        //------------------------------------------------------------------------------------------------------------------
        pid_id.param.Kp=2*pi*PWM_FREQUENCY*1000/Process_Data.Kp_Ratio*Ld_IQ;     //Kp=a*Ld, a=2*pi*Ld*fs*(1/18~1/12)=1.61~2.41(5kHz)
        pid_id.param.Ki=Rs_IQ/Ld_IQ*T_Sample;          //Ki=R/L*Ts=0.34/0.00092*T_Sample=0.074(5kHz)
        pid_iq.param.Kp=pid_id.param.Kp;               //Kp=a*Ld, a=2*pi*Ld*fs*(1/18~1/12)=1.61~2.41(5kHz)
        pid_iq.param.Ki=pid_id.param.Ki;               //Ki=R/L*Ts=0.34/0.00092*T_Sample=0.074(5kHz)

        dcv_id.Kp   = pid_id.param.Kp;
        dcv_iq.Kp   = dcv_id.Kp;
        dcv_r_id.Kp = dcv_id.Kp;
        dcv_r_iq.Kp = dcv_id.Kp;
        dcv_r_id.Kr = 3;        //�Ľ���ɢ���������Դ���ᱶ��
        dcv_r_iq.Kr = 3;
    }
    //------------------------------------------------------------------------------------------
    if(adc1.Flag.bit.Zero_Offset_End && FLAG1.bit.GAME_ON && (!adc1.Flag.bit.Over_Fault_Flag))
    {
        //--------------------------------------------------------------------------------------------
        /* ʹ��PWM������������оƬ��OE���� */
        ENABLE_PWM1;
        ENABLE_PWM_PINS_M1;

        //--------------------------------------------------------------------------------------------
        if(User_Terminal.in.GAME_MODE == Mode_Openloop_enm)     //����ģʽ
        {
            if(User_Terminal.in.Angle_Enable == 0)  //λ�ÿ���,ʹ���Ը���ת�ٺ�λ�ã�
            {
                //����ģʽ�µĽǶ��Ը����������ӣ���ʱ�����ǶȲ������������Ƶ�ʣ�ת�٣�
                Process_Data.Theta_Elec_OpenL=Process_Data.Theta_Elec_OpenL+User_Terminal.in.Theta_Step;
                if(Process_Data.Theta_Elec_OpenL>_IQ(1))
                {
                    Process_Data.Theta_Elec_OpenL=Process_Data.Theta_Elec_OpenL-_IQ(1);      //����theta_changeΪ�������
                }
                else if(Process_Data.Theta_Elec_OpenL<_IQ(0))
                {
                    Process_Data.Theta_Elec_OpenL=Process_Data.Theta_Elec_OpenL+_IQ(1);      //����theta_changeΪ�������
                }

                //��������ģʽ�µ���Park�任�͵�ѹIpark�任�ĵ�Ƕȸ�ֵ��ʹ���Ը��������Ƕȣ�
                park1.Angle  = Process_Data.Theta_Elec_OpenL;
                ipark1.Angle = Process_Data.Theta_Elec_OpenL;
            }
            else                    //λ�ñջ���ʹ�ô�������λ����Ϣ��
            {
                park1.Angle  = qep1.Theta_Elec;
                ipark1.Angle = qep1.Theta_Elec;
            }

            if(User_Terminal.in.IF_Enable == 1)     //IFģʽʹ�ܣ������ջ������򿪻�Ĭ��ΪVFģʽ����������
            {
                Process_Data.IdRef = User_Terminal.in.IdRef_OpenL;
                Process_Data.IqRef = User_Terminal.in.IqRef_OpenL;
            }
        }
        else
        {
            //��������ģʽ�µ���Park�任�͵�ѹIpark�任�ĵ�Ƕȸ�ֵ��ʹ�ô������Ƕȣ�
            park1.Angle  = qep1.Theta_Elec;
            ipark1.Angle = qep1.Theta_Elec;
        }


        //--------------------------------------------------------------------------------------------
        /* Clark�任��A/B->Alpha/Beta */
        // clarke1.As = -adc1.Result.Current_B-adc1.Result.Current_C;
        clarke1.As = adc1.Result.Current_A;
        clarke1.Bs = adc1.Result.Current_B;
        CLARKE_TRANS(&clarke1);

        /* Park�任��Alpha/Beta->D/Q */
        //park1.Angle = qep1.Theta_Elec;  //���I/F��Ƶ�ȿ��ƺ󣬽Ƕȸ�ֵ������ģ��
        park1.Alpha = clarke1.Alpha;
        park1.Beta  = clarke1.Beta;
        park1.Sine   = _IQsinPU(park1.Angle);
        park1.Cosine = _IQcosPU(park1.Angle);
        PARK_TRANS(&park1);

        //--------------------------------------------------------------------------------------------
        if(User_Terminal.in.GAME_MODE == Mode_Speed_enm )   //ת��ģʽ������ת�ٷ���Speed_DIR��ת�ٸ���Speed_Ref
        {
            /*ת�ٸ���Speed_Ref��б�¸���*/
//          ramp1.TargetValue = User_Terminal.in.Speed_Ref;
//          RAMP_FUNC(&ramp1);
//          Process_Data.SpeedRef = ramp1.SetpointValue;
            Process_Data.SpeedRef = User_Terminal.in.Speed_Ref;
            if(Process_Data.SpeedRef < 0)
            {
                User_Terminal.in.Speed_DIR = 1;
            }
            else
            {
                User_Terminal.in.Speed_DIR = 0;
            }

            /*ת��PI������*/
            if(qep1.Speed_CNT_ON == 1)  //ÿ10���������ڽ���һ��ת�ټ�����ת�ٵ���������
            {
                pid_spd.term.Ref = Process_Data.SpeedRef;
                pid_spd.term.Fdb = qep1.Speed_PU;
                PID_POSITIONAL(&pid_spd);
            }


            Process_Data.IdRef = 0;
            Process_Data.IqRef = pid_spd.term.Out;
        }

        //--------------------------------------------------------------------------------------------
        if(User_Terminal.in.GAME_MODE == Mode_Torque_enm ) //ת��ģʽ������ת�ٷ���Speed_DIR�͵�������Is_Ref
        {
            if(FLAG1.bit.Is_Angle_ON == 1)
            {
                /*����ת�ؽǷ��������ֵ����������ʽ�����ת�ؽ�Ϊ90�ȣ�Is_Angle = _IQ(0.25)*/
                /*��������Is_Ref��б�¸���*/
//              ramp2.TargetValue = User_Terminal.in.Is_Ref;
//              RAMP_FUNC(&ramp2);
//              Process_Data.IsRef = ramp2.SetpointValue;
                Process_Data.IsRef = User_Terminal.in.Is_Ref;            //PI�����������㷨��ʹ�ý�Ծ����
                Process_Data.IdRef = _IQmpy(Process_Data.IsRef,_IQcosPU(Process_Data.Is_Angle));
                Process_Data.IqRef = _IQmpy(Process_Data.IsRef,_IQsinPU(Process_Data.Is_Angle));
            }
            else
            {
//              ramp2.TargetValue = User_Terminal.in.Iq_Ref;
//              RAMP_FUNC(&ramp2);
//              Process_Data.IqRef = ramp2.SetpointValue;
                Process_Data.IsRef = User_Terminal.in.Is_Ref;            //PI�����������㷨��ʹ�ý�Ծ����
                if(FLAG1.bit.User_Input_ON == 1)      //ʹ���û������������һ��������ʱ�����ڴ洢��̬����
                {
                    //�������ݼ�¼���û�������ʱ���ô���
                    if(Data_Record.Dlog_Mode == Mode_Single_enm && FLAG1.bit.Data_Record_ON == 1)
                    {
                        if(Data_Record.Counter >= DATA_N2)   //��ʱ�û���������
                        {
                            Process_Data.IdRef = User_Terminal.in.Id_Ref;
                            Process_Data.IqRef = User_Terminal.in.Iq_Ref;
                            Process_Data.Ud_Disturbance = User_Terminal.in.Ud_Disturbance;
                            Process_Data.Uq_Disturbance = User_Terminal.in.Uq_Disturbance;
                        }
                    }
                    else   //�ǵ������ݼ�¼ģʽֱ��ʹ���û�����
                    {
                        Process_Data.IdRef = User_Terminal.in.Id_Ref;
                        Process_Data.IqRef = User_Terminal.in.Iq_Ref;
                        Process_Data.Ud_Disturbance = User_Terminal.in.Ud_Disturbance;
                        Process_Data.Uq_Disturbance = User_Terminal.in.Uq_Disturbance;
                    }
                }
            }

            /*������ת�ṩ��ת�أ�IqRef<0*/
            if(User_Terminal.in.Speed_DIR == 1)
            {
                Process_Data.IqRef = -Process_Data.IqRef;
            }
            else
            {
                Process_Data.IqRef = Process_Data.IqRef+0;
            }
        }

        //--------------------------------------------------------------------------------------------
        //����Ԥ���㷨��ע��Ҫ���������ABC-->Alpha/Beta�任����ӣ�
        Current_Predictive(&(Process_Data.Id_Predict),&(Process_Data.Iq_Predict));
        if(FLAG1.bit.CUR_PREDICTIVE_ON == 1)   /*�Ƿ��ڵ�ѹ����ǰ��������ӵ���Ԥ��*/
        {
             Process_Data.Id_Cal = Process_Data.Id_Predict;
             Process_Data.Iq_Cal = Process_Data.Iq_Predict;
        }
        else
        {
             Process_Data.Id_Cal = park1.Ds;
             Process_Data.Iq_Cal = park1.Qs;
        }

        //--------------------------------------------------------------------------------------------
        if(FLAG1.bit.DISCRETE_CUR_ON == 1)      //��ͳ��ɢ�����������
        {
//            FLAG1.bit.ANGLE_COMPEN_ON = 1;
            Process_Data.Angle_Compen_Ratio = _IQ(2.0);
            Process_Data.Current_Predict_Beats = 0;

            dcv_id.Ref = Process_Data.IdRef;
            dcv_id.Fdb = park1.Ds;
            dcv_iq.Ref = Process_Data.IqRef;
            dcv_iq.Fdb = park1.Qs;
            DiscreteTime_CV_Controller( &dcv_id, &dcv_iq);

            Process_Data.Ud_ACR_Out = dcv_id.Out;
            Process_Data.Uq_ACR_Out = dcv_iq.Out;
        }
        else if(FLAG1.bit.DCV_R_CUR_ON == 1)    //�Ľ���ɢ�����������
        {

//            FLAG1.bit.ANGLE_COMPEN_ON = 1;
            Process_Data.Angle_Compen_Ratio = _IQ(2.0);
            Process_Data.Current_Predict_Beats = 1;

            dcv_r_id.Ref = Process_Data.IdRef;
            dcv_r_id.Fdb = park1.Ds;
            dcv_r_iq.Ref = Process_Data.IqRef;
            dcv_r_iq.Fdb = park1.Qs;
            Improved_DCV_Controller( &dcv_r_id, &dcv_r_iq);

            Process_Data.Ud_ACR_Out = dcv_r_id.Out;
            Process_Data.Uq_ACR_Out = dcv_r_iq.Out;
        }
        else
        {
            /*D�����PI������*/
            pid_id.term.Ref = Process_Data.IdRef;
            pid_id.term.Fdb = Process_Data.Id_Cal;
            PID_POSITIONAL(&pid_id);

            /*Q�����PI������*/
            pid_iq.term.Ref = Process_Data.IqRef;
            pid_iq.term.Fdb = Process_Data.Iq_Cal;
            PID_POSITIONAL(&pid_iq);

            //��������
//                 pid_id.term.Out = -1.675;
//                 pid_iq.term.Out = 17.5;

            Process_Data.Ud_ACR_Out = pid_id.term.Out;
            Process_Data.Uq_ACR_Out = pid_iq.term.Out;
        }

        //------------------------------------------------------------------------------------------------------------------
        if(FLAG1.bit.VOL_FDB_DECOUPLE == 1)      /*��ѹǰ������*/
        {
             Voltage_Feedforward_Decouple(&(Process_Data.Ud_Decouple),&(Process_Data.Uq_Decouple));
             Process_Data.Ud_PreSat = Process_Data.Ud_ACR_Out+Process_Data.Ud_Decouple;
             Process_Data.Uq_PreSat = Process_Data.Uq_ACR_Out+Process_Data.Uq_Decouple+_IQmpy(Flux_Mag_IQ,qep1.Speed_We);
        }
        else if(FLAG1.bit.COMPL_VECT_DECOUPLE == 1)     /*��ʸ������*/
        {
             Complex_Vector_Decouple(&(Process_Data.Ud_Decouple),&(Process_Data.Uq_Decouple));
             Process_Data.Ud_PreSat = Process_Data.Ud_ACR_Out+Process_Data.Ud_Decouple;
             Process_Data.Uq_PreSat = Process_Data.Uq_ACR_Out+Process_Data.Uq_Decouple+_IQmpy(Flux_Mag_IQ,qep1.Speed_We);
        }
        else if(FLAG1.bit.VI_CV_DECOUPLE == 1)  /*�Ľ���ʸ������-�����迹ģ��У��*/
        {
             Virtual_Impedance_CV_Decouple(&(Process_Data.Ud_Decouple),&(Process_Data.Uq_Decouple));
             Process_Data.Ud_PreSat = Process_Data.Ud_ACR_Out+Process_Data.Ud_Decouple;
             Process_Data.Uq_PreSat = Process_Data.Uq_ACR_Out+Process_Data.Uq_Decouple+_IQmpy(Flux_Mag_IQ,qep1.Speed_We);
        }
        else
        {
             Process_Data.Ud_Decouple = 0;
             Process_Data.Uq_Decouple = 0;
             Process_Data.Ud_PreSat = Process_Data.Ud_ACR_Out+ Process_Data.Ud_Decouple;
             Process_Data.Uq_PreSat = Process_Data.Uq_ACR_Out+ Process_Data.Uq_Decouple+_IQmpy(Flux_Mag_IQ,qep1.Speed_We);
        }

        Process_Data.Ud_PreSat  = Process_Data.Ud_PreSat + Process_Data.Ud_Disturbance;     //D���ⲿ��ѹ�Ŷ�����
        Process_Data.Uq_PreSat  = Process_Data.Uq_PreSat + Process_Data.Uq_Disturbance;     //Q���ⲿ��ѹ�Ŷ�����
        ipark1.Ds = _IQsat(Process_Data.Ud_PreSat, pid_id.param.OutMax, pid_id.param.OutMin);
        ipark1.Qs = _IQsat(Process_Data.Uq_PreSat, pid_iq.param.OutMax, pid_iq.param.OutMin);

        //--------------------------------------------------------------------------------------------
        if((User_Terminal.in.GAME_MODE==Mode_Openloop_enm)&&(User_Terminal.in.IF_Enable==0))//Ud/Uq�Ը�����������
        {
            //VFģʽ������������Uq=3V,Theta_Step=_IQ(0.001),300rpm������
            ipark1.Ds = User_Terminal.in.UdRef_OpenL;
            ipark1.Qs = User_Terminal.in.UqRef_OpenL;
        }   //����IFģʽ�������ջ�����PI���������������Ud/Uq��ѹ�źţ�

        //--------------------------------------------------------------------------------------------
        /*��ѹ�����ƴ�����ѹԲ�켣�滮*/
         #if 1
         Process_Data.Uref_Temp = _IQmpy(Process_Data.Uref_Max,Process_Data.Uref_Max)-_IQmpy(ipark1.Ds,ipark1.Ds);
         Process_Data.Uref_Temp = _IQsat(Process_Data.Uref_Temp,3000,0);
         Process_Data.Uref_Temp = _IQsqrt(Process_Data.Uref_Temp);
         if(User_Terminal.in.Speed_DIR == 1)
         {
             if(_IQabs(ipark1.Qs) > Process_Data.Uref_Temp)
             {
                 ipark1.Qs = -Process_Data.Uref_Temp;
             }
         }
         else
         {
             if(_IQabs(ipark1.Qs) > Process_Data.Uref_Temp)
             {
                 ipark1.Qs = Process_Data.Uref_Temp;
             }
         }
         #endif

        #if 0
          /*��ѹ�����ƴ����������ұ�������Ud,Uq*/
          Process_Data.Uref_Temp = _IQmpy(ipark1.Ds,ipark1.Ds)+_IQmpy(ipark1.Qs,ipark1.Qs);
          Process_Data.Uref_Temp = _IQsqrt(Process_Data.Uref_Temp);
          if(Process_Data.Uref_Temp>Process_Data.Uref_Max)
          {
              if(User_Terminal.in.Speed_DIR == 1)
              {
                  ipark1.Qs = -_IQmpy(Process_Data.Uref_Max,_IQdiv(ipark1.Qs,Process_Data.Uref_Temp));
              }
              else
              {
                  ipark1.Qs = _IQmpy(Process_Data.Uref_Max,_IQdiv(ipark1.Qs,Process_Data.Uref_Temp));
              }
              ipark1.Ds = -_IQmpy(Process_Data.Uref_Max,_IQdiv(ipark1.Ds,Process_Data.Uref_Temp));
          }
        #endif

          //--------------------------------------------------------------------------------------------
        if(FLAG1.bit.ANGLE_COMPEN_ON == 1)  //���ٺͼ��������ٵ�����£��ǶȲ���Ӱ���С���Բ����
        {
          Process_Data.Theta_Compen = _IQmpy(_IQmpy(qep1.Speed_We ,T_Sample),Process_Data.Angle_Compen_Ratio);
          ipark1.Angle = ipark1.Angle+Process_Data.Theta_Compen;

          if(ipark1.Angle>_IQ(1))
          {
              ipark1.Angle = ipark1.Angle-_IQ(1);
          }
          else if(ipark1.Angle<_IQ(-1))
          {
              ipark1.Angle = ipark1.Angle+_IQ(1);
          }
        }

        //--------------------------------------------------------------------------------------------
        //�ݴ浱ǰ�ļ��������ѹ��Ϊ��һ�ĵĵ���Ԥ�����룻
        Process_Data.Ud_Beat0 = ipark1.Ds;
        Process_Data.Uq_Beat0 = ipark1.Qs;
        //�ݴ浱ǰ�ĵĵ���Ԥ������������һ�ļ��㣻
        Process_Data.Id_Predict_1 = Process_Data.Id_Predict;
        Process_Data.Iq_Predict_1 = Process_Data.Iq_Predict;

        ipark1.Sine   = _IQsinPU(ipark1.Angle);
        ipark1.Cosine = _IQcosPU(ipark1.Angle);
        IPARK_TRANS(&ipark1);

        svpwm1.Ualpha = ipark1.Alpha;
        svpwm1.Ubeta = ipark1.Beta;
        svpwm1.UdcBus= adc1.Result.Voltage_DC;
        SVPWM_GEN(&svpwm1);

        EPwm1Regs.CMPA.bit.CMPA = _IQmpy(svpwm1.Ta, pwm1.PeriodMax); // PWM 1A - PhaseA
        EPwm2Regs.CMPA.bit.CMPA = _IQmpy(svpwm1.Tb, pwm1.PeriodMax); // PWM 2A - PhaseB
        EPwm3Regs.CMPA.bit.CMPA = _IQmpy(svpwm1.Tc, pwm1.PeriodMax); // PWM 3A - PhaseC

        //Use for Debug
    //      EPwm1Regs.CMPA.bit.CMPA = 1500;    // PWM 1A - PhaseA
    //      EPwm2Regs.CMPA.bit.CMPA = 3000;    // PWM 2A - PhaseB
    //      EPwm3Regs.CMPA.bit.CMPA = 4500;    // PWM 3A - PhaseC
        //___________________________________________________________
    }
    else
    {
        //adc1.Flag.bit.Zero_Offset_End=0;
        DISABLE_PWM1;
        DISABLE_PWM_PINS_M1;
    }

    // ���̱������ʾ�����۲⣬�ֱ�ʹ��PWM+�˲�ģ��DAC����ʹ��ʵ��DACоƬ���������
    // PwmDacCh1/2/3/4 �ֱ����ζ�Ӧ EPWM8A/8B/7A/7B, ����ռ�ձȷ�Χ��0~1֮�䣻
    // dutya/dutyb�ֱ����ζ�ӦDAC_A/B,����ռ�ձȷ�Χ��0~1֮��
    if(Process_Data.Show_Mode == Show_Iq_enm)
    {
        pwmdac1.PwmDacCh1 = _IQmpy(Process_Data.IqRef, 2*INV_BASE_CURRENT);
        pwmdac1.PwmDacCh2 = _IQmpy(park1.Qs, 2*INV_BASE_CURRENT);
        pwmdac1.PwmDacCh3 = _IQmpy(Process_Data.IdRef, 2*INV_BASE_CURRENT);
        pwmdac1.PwmDacCh4 = _IQmpy(park1.Ds, 2*INV_BASE_CURRENT);

        dac1.dutya = _IQmpy(Process_Data.IqRef,INV_BASE_CURRENT*2);
//        dac1.dutya = _IQmpy(park1.Ds,INV_BASE_CURRENT*2);
        dac1.dutyb = _IQmpy(park1.Qs,INV_BASE_CURRENT*2);
    }
    if(Process_Data.Show_Mode == Show_Id_enm)
    {
        pwmdac1.PwmDacCh1 = _IQmpy(Process_Data.IqRef, 2*INV_BASE_CURRENT);
        pwmdac1.PwmDacCh2 = _IQmpy(park1.Qs, 2*INV_BASE_CURRENT);
        pwmdac1.PwmDacCh3 = _IQmpy(Process_Data.IdRef, 2*INV_BASE_CURRENT);
        pwmdac1.PwmDacCh4 = _IQmpy(park1.Ds, 2*INV_BASE_CURRENT);

//        dac1.dutya = _IQmpy(Process_Data.IqRef,INV_BASE_CURRENT*2);
        dac1.dutya = _IQmpy(park1.Ds,INV_BASE_CURRENT*2);
        dac1.dutyb = _IQmpy(park1.Qs,INV_BASE_CURRENT*2);
    }
    else if(Process_Data.Show_Mode == Show_Speed_enm)
    {
        pwmdac1.PwmDacCh1 = _IQmpy(Process_Data.SpeedRef, INV_BASE_SPEED);
        pwmdac1.PwmDacCh2 = _IQmpy(qep1.Speed_PU, INV_BASE_SPEED);
//        pwmdac1.PwmDacCh3 = _IQmpy(Process_Data.IqRef, 2*INV_BASE_CURRENT);
        pwmdac1.PwmDacCh4 = _IQmpy(park1.Qs, 2*INV_BASE_CURRENT);
        pwmdac1.PwmDacCh3 = qep1.Theta_Elec;

        dac1.dutya = _IQmpy(Process_Data.SpeedRef,INV_BASE_SPEED);
        dac1.dutyb = _IQmpy(qep1.Speed_PU,INV_BASE_SPEED);
    }
    EPWMDAC_VAR_WATCH(&pwmdac1);  // ����Ϊռ�ձ����
    DAC_VAR_WATCH(&dac1);         // ����Ϊռ�ձ����

    //--------------------------------------------------------------------------------------------------
    if(Data_Record.Dlog_Mode == Mode_Continuous_enm)  //����ѭ��ˢ��ģʽ
    {
       // Ĭ������ģʽMode_Continuous_enm ���û�����ʹ��User_Input_ON=1��Data_Record_ON=0��
       // һ����֤ʵ��ʹ��������¼ģʽ����Data_Record_ON��User_Input_ON��־λ����λΪ1���ɣ�
       //������¼ģʽ��ֻ�轫Data_Record_ONһֱ��λΪ1����һֱ���ֱ�������ѭ��ˢ�¼�¼;
        // ��ò�Ҫ�����黹��ˢ�µ�ʱ��洢���ݣ����׳�������洢���ݶ�����������˸ĳ�������¼ģʽMode_Continuous���μ�¼500������ֹͣ��
        if(FLAG1.bit.Data_Record_ON == 1)   //������������ݴ�
        {
            #if DUAL_MOTOR_SYNC
                if(qep1.Speed_CNT_ON == 1)
                {
                    Data_Record.DlogCh1[Data_Record.Counter] = Process_Data.IqRef;
                    Data_Record.DlogCh2[Data_Record.Counter] = park1.Qs;
                    Data_Record.DlogCh3[Data_Record.Counter] = Process_Data.SpeedRef;
                    Data_Record.DlogCh4[Data_Record.Counter] = qep1.Speed_PU;

                    Data_Record.Counter++;
                    if(Data_Record.Counter >= DATA_MAX)
                    {
                        GpioDataRegs.GPDCLEAR.bit.GPIO122 = 1;  // GPIO10����ʾ������ʾ����ж����ڼ���������ʱ��

                        Data_Record.Counter = 0;
                        FLAG1.bit.Data_Record_ON = 0;
                    }
                }
            #else
                Data_Record.DlogCh1[Data_Record.Counter] = qep1.Speed_We;
                Data_Record.DlogCh2[Data_Record.Counter] = park1.Ds;
                Data_Record.DlogCh3[Data_Record.Counter] = qep1.Theta_Elec;
                Data_Record.DlogCh4[Data_Record.Counter] = park1.Qs;

                Data_Record.Counter++;
                if(Data_Record.Counter >= DATA_MAX)
                {
                    GpioDataRegs.GPDCLEAR.bit.GPIO122 = 1;  // GPIO10����ʾ������ʾ����ж����ڼ���������ʱ��

                    Data_Record.Counter = 0;
                    FLAG1.bit.Data_Record_ON = 0;
                }
            #endif
        }
    }
    else if(Data_Record.Dlog_Mode == Mode_Single_enm)  //����ˢ��ģʽ
    {
       //����ˢ��ģʽ���ڱ��涯̬���̱�������Ҫ��д���Ļ�ͼ����ʹ�ø�ģʽ��
       // ������־λFLAG1.bit.Data_Record_ON����ʹ�������¼����;
       // ������־λFLAG1.bit.User_Input_ON����ʹ�ܶ�̬����User_Terminal.in.Id_Ref/Iq_Ref;
       // ������������־λ�������ݼ�¼ʹ���붯̬����ʹ�ܽ�����ƣ�
       // ֻ��������־λͬʱ��λΪ1���ܿ����������ݼ�¼���洢��Ŀ���ݿ���Ƶ�����㷨������
       // ����������������������������������������������������������������������������������������������������������������������������������������������������������������������������
       // 1) ��ʼ��Data_Record.Dlog_Mode = Mode_Single_enm��ѡ��Ϊ����ˢ��ģʽ��
       // 2) ��ʼ��Ĭ������Data_Record_ON=0��User_Input_ON=1��ʹ�ܵ�������Id_Ref/Iq_Ref����һ������ֵIq_Ref=1A��
       // 3) ��Ծ��̬����֮ǰ����˳����������User_Input_ON=0��Data_Record_ON=1��
       // 4) �û���ֵUser_Terminal.in.Id_Ref/Iq_Ref������
       // 5) ��λUser_Input_ON=1ʹ���û�����ͬʱʹ�����ݼ�¼���ڳ���������һ��������ʱ���洢��������ǰ����̬��ֵ��
       // ����������������������������������������������������������������������������������������������������������������������������������������������������������������������������
        if(FLAG1.bit.Data_Record_ON == 1)   //������������ݴ�
        {
            if(FLAG1.bit.User_Input_ON == 1)   //������������ݴ�
            {
                // ������֤ת�ٻ��͵���������
                #if 0    //��֤�������Ķ�̬����
                    Data_Record.DlogCh1[Data_Record.Counter] = Process_Data.IqRef;
                    Data_Record.DlogCh2[Data_Record.Counter] = park1.Qs;
                    Data_Record.DlogCh3[Data_Record.Counter] = Process_Data.IdRef;
                    Data_Record.DlogCh4[Data_Record.Counter] = park1.Ds;
                #else   //��֤��������̬���ܶ�ת�ٻ���Ӱ�죻
                    Data_Record.DlogCh1[Data_Record.Counter] = Process_Data.IqRef;
                    Data_Record.DlogCh2[Data_Record.Counter] = park1.Qs;
                    Data_Record.DlogCh3[Data_Record.Counter] = Process_Data.SpeedRef;
                    Data_Record.DlogCh4[Data_Record.Counter] = qep1.Speed_PU;
                #endif

                Data_Record.Counter++;

                if(Data_Record.Counter >= DATA_MAX)
                {
                    Data_Record.Counter = 0;
                    FLAG1.bit.Data_Record_ON = 0;
                    Data_Record.Dlog_Mode = Mode_Continuous_enm;
                    //User_Terminal.in.GAME_MODE = Mode_Stop_enm;  һ��ѡ���Ǳ���ʵ�鶯̬��¼��ɺ�����ͣ����

                    User_Terminal.in.Id_Ref = Data_Record.Id_Dlog_old;
                    User_Terminal.in.Iq_Ref = Data_Record.Iq_Dlog_old;
                    User_Terminal.in.Ud_Disturbance = Data_Record.Ud_Dlog_old;
                    User_Terminal.in.Uq_Disturbance = Data_Record.Uq_Dlog_old;
                    Data_Record.FLAG = 0;
                }
            }
        }
    }

    //--------------------------------------------------------
    #if (CONTROL_ISR_MODE == ADC_ISR)
    {
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;       //make sure INT1 flag is cleared
        PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    }
    #else  //(CONTROL_ISR_MODE == PWM_ISR)
    {
        // Clear INT flag for this timer
        EPwm1Regs.ETCLR.bit.INT  = 1;
        EPwm1Regs.ETCLR.bit.SOCA = 1;
        // Acknowledge this interrupt to receive more interrupts from group 3
        PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    }
    #endif

//    GpioDataRegs.GPDCLEAR.bit.GPIO122 = 1;  // GPIO10����ʾ������ʾ����ж����ڼ���������ʱ��
    GpioDataRegs.GPDCLEAR.bit.GPIO123 = 1;  // GPIO10����ʾ������ʾ����ж����ڼ���������ʱ��
}

// -------------------------------------------------------------------------------
