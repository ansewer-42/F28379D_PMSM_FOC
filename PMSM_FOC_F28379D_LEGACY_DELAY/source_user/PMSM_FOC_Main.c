/*
 * PMSM_FOC_Main.c
 * ----------------------------------------------------------------------------------
 * Description
 * This file is used for the definition of main function, motor control ISR and state machine;
 * ----------------------------------------------------------------------------------
 *  Created on: 2020定6埖18晩
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
        ADCINB2！！CPMIN3P, ADCINA2！！CPMIN1P, ADCIND0！！CPMIN7P;
        ADCIN14！！CPMIN4P;

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
		//--------！！Position Connector: Bottom Board----------------------------
		GPIO32:IN  ---> Hall-U1, Position hall signal input (motor1)
		GPIO67:IN  ---> Hall-V1, Position hall signal input (motor1)
		GPIO111:IN ---> Hall-W1, Position hall signal input (motor1)
		//--------！！Position Connector: Transfer Board (for back up)------------
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
// LPI議吭房頁Low Priority Interrupt 詰單枠雫嶄僅??窃貌噐C28x議噸宥嶄僅??辛參瓜廼耗??
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
	* 殻會勣付亟欺Flash嶄峪俶勣:
	* 1) 序佩_FLASH圓協吶#define軸辛
	* ！！！！壓垢殻猟周議Properties/CCS Build/C2000 Compiler/Predefined Symbols 賜宀 壓垢殻猟周嶄#define _FLASH軸辛??
	* ！！！！InitFlash()壓System_Init()嶄塰佩??memcpy()壓System_Init()/InitSysCtrl()嶄塰佩;
	* 2) 繍CMD猟周紋算葎2837xD_FLASH_lnk_cpu1.cmd
	*/
	System_Init();			 	    //SYSTEM Control狼由陣崙庁翠兜兵晒
	GPIO_Init();			 	    //GPIO庁翠兜兵晒
    DAC_Setup();                    //DAC庁翠兜兵晒
    CMPSS_Setup();                  //CMPSS庁翠兜兵晒
    CMPSS_PHASE_CUR_PRO(&cmpss1);   //CMPSS坪何DAC公協峙兜兵晒
	ADC_Setup1();		 	        //ADC_A~B庁翠兜兵晒
	ADC_Setup2();                   //ADC_C~D庁翠兜兵晒
	EQEP_Setup1(&qep1);		 	    //EQPEP1庁翠兜兵晒
    EPWM_Setup1(&pwm1);             //EPWM1~3庁翠兜兵晒??斤哘motor1
    EPWMDAC_Setup(&pwmdac1);        //PWMDAC庁翠兜兵晒

	// 嶄僅?犢愿籌?
    #if (CONTROL_ISR_MODE == ADC_ISR)
        ADCA1_Interrupt_Set(&Motor_Control_ISR1);
    #else  //(CONTROL_ISR_MODE == PWM_ISR)
        EPWM1_Interrupt_Set(&Motor_Control_ISR1);       //EPWM1嶄僅?犢惻調翔?譜崔??械号决扮庁塀壓EPWM1嶄僅捲暦殻會嶄塰佩褒窮字陣崙殻會??
    #endif
    EnableInterrupts();
    ERTM;

    DISABLE_PWM1;                   //PWM補竃聞嬬重性互
    DISABLE_PWM_PINS_M1;            //PWM強恬?浙?補竃性詰
    Global_Variables_Clear();           //畠蕉延楚賠巣
    Global_Variables_Init();            //畠蕉延楚兜兵晒
    Controller_Params_Preset();         //PID,SMC吉陣崙匂歌方兜兵晒

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

        // 蝕兵芝村方象念枠公協Iq_Ref/Id_Ref議児粥陶崔峙??
        // 汽肝贋刈峪俶勣繍庁塀僉葎Mode_Single_enm夸徭強梓和峰峙序佩竣埔公協旺拝徭強隠贋方怏荷恬
        // 銭偬庁塀??潮範葎Mode_Continuous_enm, 峪俶勣繍FLAG1.bit.Data_Record_ON 崔1軸辛頼撹汽肝贋刈荷恬;
        if(Data_Record.Dlog_Mode == Mode_Single_enm)
        {

            // 廣吭宸戦耽肝脅氏泡仟??2kHz和恷寄塋俯500*500us=250ms??階狛貧峰LED决扮扮寂??氏嶷仟泡仟公協症峙??Id/Iq_Dlog_old
            Data_Record.Id_Dlog_old  = User_Terminal.in.Id_Ref;
            Data_Record.Iq_Dlog_old  = User_Terminal.in.Iq_Ref;
            Data_Record.Ud_Dlog_old  = User_Terminal.in.Ud_Disturbance;
            Data_Record.Uq_Dlog_old  = User_Terminal.in.Uq_Disturbance;

            User_Terminal.in.Id_Ref  = Data_Record.Id_Dlog;
            User_Terminal.in.Iq_Ref  = Data_Record.Iq_Dlog;
            User_Terminal.in.Ud_Disturbance = Data_Record.Ud_Dlog;
            User_Terminal.in.Uq_Disturbance = Data_Record.Uq_Dlog;
            FLAG1.bit.User_Input_ON  = 1;   // 糞縞User_Input宸倖炎崗了匯岷短喘??辛參肇茅??
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
    GpioDataRegs.GPDSET.bit.GPIO123 = 1;  // GPIO10喘噐幣襖匂?塋昭豌瞞亢?巓豚式殻會塰佩扮海

    #if DUAL_MOTOR_SYNC    //耽肝頼撹方象贋刈save memory,脅勣序佩匯肝Data_Record.FLAG炎崗了賠0荷恬??
    if((Data_Record.FLAG==0) && (GpioDataRegs.GPDDAT.bit.GPIO124 == 1))   //俊辺motor2議揖化佚催鳩範頁倦蝕兵贋刈方象
    {
        FLAG1.bit.Data_Record_ON = 1;
        GpioDataRegs.GPDSET.bit.GPIO122 = 1;  // GPIO10喘噐幣襖匂?塋昭豌瞞亢?巓豚式殻會塰佩扮海
        Data_Record.FLAG = 1;                 // 饗毘炎崗了??倦夸FPIO124氏匯岷乾窟登僅繍Data_Record炎崗了崔了葎1;
    }                                         //  耽肝motor2減墮融紗岻寂脅勣讐Data_Record.FLAG炎崗了賠0??
    #endif

    #if (CONTROL_ISR_MODE == PWM_ISR)
    {
        asm(" RPT #72 || NOP");    //尖胎貧75ns+280ns=355ns辛頼撹匯倖宥祇議廬算,糞縞鉱霞幣襖匂窟?嶐醫?RPT #25辛參,20音辛參;
    //        DELAY_US(1);
    //        while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);  //PWM_ISR庁塀喘乎炎崗了登僅音彈鳩??俶勣紗匯粁决扮??賜宀枠侃尖艶議痕方??厮将喘幣襖匂鉱霞SUM刮屬??
    }
    #endif

    //--------------------------------------------------------------------------------------------------
    //喘噐狛儿狛送絞嬾佚催窟伏朔議狼由嶷尼,殊霞IGBT佚催頁倦崔了??拝返強嶷尼佚催頁倦公竃
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

        EALLOW;         //TZFLG炎崗了賠茅
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

    if(FLAG1.bit.VAR_CLEAR==1)          //畠蕉延楚返強賠巣
    {
        Global_Variables_Clear();
        FLAG1.bit.VAR_CLEAR = 0;
    }

    //--------------------------------------------------------------------------------------------------
    if((IPM_ERROR1 == 0)||(adc1.Flag.bit.Over_Fault_Flag==1)||(EPwm1Regs.TZFLG.bit.OST==1))  //狛送狛儿絞嬾佚催窟伏侃尖
    {
        DISABLE_PWM1;
        FLAG1.bit.GAME_ON = 0;
        FLAG1.bit.GAME_RESTART=0;
        FLAG1.bit.FAULT_ON = 1;
        User_Terminal.in.GAME_MODE = Mode_Stop_enm;
    }

    if(EQep1Regs.QEPSTS.bit.FIMF==0)    //枠序佩廬堀嚥叔業了崔柴麻??壅序佩窮送寡劔准福柴麻扮寂??
    {
        Hall_Angle_Cal(&qep1);          //飼櫛勧湖匂叔業霞楚
    }
    else
    {
        EQEP_Angle_Cal(&qep1);          //窮叔業柴麻
        EQEP_Speed_Cal(&qep1);          //糞扮廬堀柴麻
    }

    if(!adc1.Flag.bit.Zero_Offset_End)
    {
        ADC_Zero_Offset(&adc1);         //ADC宥祇巣泣陶崔寡劔??壓GAME_ON=0?┠盥棕?扮音僅序佩嶄來泣寡劔
    }                                   //勣個葎叙壓貧窮兜兵晒序佩匯肝寡劔夸俶評茅GAME_ON=0扮議else嶄議Zero_Offset_End=0??
    else
    {
        ADC_Result_Read(&adc1);         //ADC寡劔潤惚響函式侃尖
        ADC_Result_Handle(&adc1);
    }

    //------------------------------------------------------------------------------------------
    /* 功象寡誼欺議銚?澣舊講喫喫閏橡?DQ已窮送桟議持蛍式補竃?涎?峙 */
    Process_Data.Uref_Max  = _IQmpy(adc1.Result.Voltage_DC,_IQ(0.55));     //SVPWM距崙恷寄窮儿垈磯抄??眉?狡羆籠?恷互?犁舊?
    Voltage_OutMax_Update();       //窮送距准匂補竃窮儿?涎?峙功象銚?濂頻?窮儿峙厚仟

    //------------------------------------------------------------------------------------------
    if(FLAG1.bit.GAME_OVER == 1)            //岷俊返強唯字炎崗了庁塀
    {
        User_Terminal.in.GAME_MODE = Mode_Stop_enm;
    }

    if(User_Terminal.in.GAME_MODE == Mode_Stop_enm)         //唯字庁塀撃PWM??拝延楚賠巣??
    {
        FLAG1.bit.GAME_ON = 0;
        Global_Variables_Clear();
    }
    else
    {
        FLAG1.bit.GAME_ON = 1;
    }

    //------------------------------------------------------------------------------------------
    if(FLAG1.bit.CUR_CNT_Kp_ON == 1)        //聞喘Kp_Ratio延楚壓?瀰涕諜?准匂曳箭歌方
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
        dcv_r_id.Kr = 3;        //個序宣柊囃陣崙匂嗤坿怦鶴蔚方
        dcv_r_iq.Kr = 3;
    }
    //------------------------------------------------------------------------------------------
    if(adc1.Flag.bit.Zero_Offset_End && FLAG1.bit.GAME_ON && (!adc1.Flag.bit.Over_Fault_Flag))
    {
        //--------------------------------------------------------------------------------------------
        /* 聞嬬PWM補竃才補竃産喝仂頭議OE哈重 */
        ENABLE_PWM1;
        ENABLE_PWM_PINS_M1;

        //--------------------------------------------------------------------------------------------
        if(User_Terminal.in.GAME_MODE == Mode_Openloop_enm)     //蝕桟庁塀
        {
            if(User_Terminal.in.Angle_Enable == 0)  //了崔蝕桟,聞喘徭公協廬堀才了崔??
            {
                //蝕桟庁塀和議叔業徭公協化序奐紗??緩扮距屁叔業化海軸距屁窮字撞楕??廬堀??
                Process_Data.Theta_Elec_OpenL=Process_Data.Theta_Elec_OpenL+User_Terminal.in.Theta_Step;
                if(Process_Data.Theta_Elec_OpenL>_IQ(1))
                {
                    Process_Data.Theta_Elec_OpenL=Process_Data.Theta_Elec_OpenL-_IQ(1);      //喘噐theta_change葎屎議秤趨
                }
                else if(Process_Data.Theta_Elec_OpenL<_IQ(0))
                {
                    Process_Data.Theta_Elec_OpenL=Process_Data.Theta_Elec_OpenL+_IQ(1);      //喘噐theta_change葎減議秤趨
                }

                //蝕桟塰佩庁塀和窮送Park延算才窮儿Ipark延算議窮叔業験峙??聞喘徭公協化序叔業??
                park1.Angle  = Process_Data.Theta_Elec_OpenL;
                ipark1.Angle = Process_Data.Theta_Elec_OpenL;
            }
            else                    //了崔液桟??聞喘勧湖匂議了崔佚連??
            {
                park1.Angle  = qep1.Theta_Elec;
                ipark1.Angle = qep1.Theta_Elec;
            }

            if(User_Terminal.in.IF_Enable == 1)     //IF庁塀聞嬬??窮送液桟??倦夸蝕桟潮範葎VF庁塀??窮送蝕桟
            {
                Process_Data.IdRef = User_Terminal.in.IdRef_OpenL;
                Process_Data.IqRef = User_Terminal.in.IqRef_OpenL;
            }
        }
        else
        {
            //屎械塰佩庁塀和窮送Park延算才窮儿Ipark延算議窮叔業験峙??聞喘勧湖匂叔業??
            park1.Angle  = qep1.Theta_Elec;
            ipark1.Angle = qep1.Theta_Elec;
        }


        //--------------------------------------------------------------------------------------------
        /* Clark延算??A/B->Alpha/Beta */
        // clarke1.As = -adc1.Result.Current_B-adc1.Result.Current_C;
        clarke1.As = adc1.Result.Current_A;
        clarke1.Bs = adc1.Result.Current_B;
        CLARKE_TRANS(&clarke1);

        /* Park延算??Alpha/Beta->D/Q */
        //park1.Angle = qep1.Theta_Elec;  //耶紗I/F送撞曳陣崙朔??叔業験峙壓貧中庁翠
        park1.Alpha = clarke1.Alpha;
        park1.Beta  = clarke1.Beta;
        park1.Sine   = _IQsinPU(park1.Angle);
        park1.Cosine = _IQcosPU(park1.Angle);
        PARK_TRANS(&park1);

        //--------------------------------------------------------------------------------------------
        if(User_Terminal.in.GAME_MODE == Mode_Speed_enm )   //廬堀庁塀??公協廬堀圭??Speed_DIR才廬堀公協Speed_Ref
        {
            /*廬堀公協Speed_Ref議弍涜効忸*/
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

            /*廬堀PI距准匂*/
            if(qep1.Speed_CNT_ON == 1)  //耽10倖蝕購巓豚序佩匯肝廬堀柴麻嚥廬堀距准匂恬喘
            {
                pid_spd.term.Ref = Process_Data.SpeedRef;
                pid_spd.term.Fdb = qep1.Speed_PU;
                PID_POSITIONAL(&pid_spd);
            }


            Process_Data.IdRef = 0;
            Process_Data.IqRef = pid_spd.term.Out;
        }

        //--------------------------------------------------------------------------------------------
        if(User_Terminal.in.GAME_MODE == Mode_Torque_enm ) //廬裳庁塀??公協廬堀圭??Speed_DIR才窮送公協Is_Ref
        {
            if(FLAG1.bit.Is_Angle_ON == 1)
            {
                /*功象廬裳叔蛍塘窮送嫌峙??斤噐咨自塀窮字??廬裳叔葎90業??Is_Angle = _IQ(0.25)*/
                /*窮送公協Is_Ref議弍涜効忸*/
//              ramp2.TargetValue = User_Terminal.in.Is_Ref;
//              RAMP_FUNC(&ramp2);
//              Process_Data.IsRef = ramp2.SetpointValue;
                Process_Data.IsRef = User_Terminal.in.Is_Ref;            //PI歌方屁協才麻隈霞聞喘竣埔公協
                Process_Data.IdRef = _IQmpy(Process_Data.IsRef,_IQcosPU(Process_Data.Is_Angle));
                Process_Data.IqRef = _IQmpy(Process_Data.IsRef,_IQsinPU(Process_Data.Is_Angle));
            }
            else
            {
//              ramp2.TargetValue = User_Terminal.in.Iq_Ref;
//              RAMP_FUNC(&ramp2);
//              Process_Data.IqRef = ramp2.SetpointValue;
                Process_Data.IsRef = User_Terminal.in.Is_Ref;            //PI歌方屁協才麻隈霞聞喘竣埔公協
                if(FLAG1.bit.User_Input_ON == 1)      //聞嬬喘薩公協??絞吭紗匯粁恬喘决扮??喘噐贋刈糧蓑方象
                {
                    //汽肝方象芝村嗤喘薩公協决扮恬喘侃尖
                    if(Data_Record.Dlog_Mode == Mode_Single_enm && FLAG1.bit.Data_Record_ON == 1)
                    {
                        if(Data_Record.Counter >= DATA_N2)   //决扮喘薩公協恬喘
                        {
                            Process_Data.IdRef = User_Terminal.in.Id_Ref;
                            Process_Data.IqRef = User_Terminal.in.Iq_Ref;
                            Process_Data.Ud_Disturbance = User_Terminal.in.Ud_Disturbance;
                            Process_Data.Uq_Disturbance = User_Terminal.in.Uq_Disturbance;
                        }
                    }
                    else   //掲汽肝方象芝村庁塀岷俊聞嬬喘薩公協
                    {
                        Process_Data.IdRef = User_Terminal.in.Id_Ref;
                        Process_Data.IqRef = User_Terminal.in.Iq_Ref;
                        Process_Data.Ud_Disturbance = User_Terminal.in.Ud_Disturbance;
                        Process_Data.Uq_Disturbance = User_Terminal.in.Uq_Disturbance;
                    }
                }
            }

            /*郡?鯰?廬戻工減廬裳??IqRef<0*/
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
        //窮送圓霞麻隈??廣吭勣壓眉?犁臍?ABC-->Alpha/Beta延算朔耶紗??
        Current_Predictive(&(Process_Data.Id_Predict),&(Process_Data.Iq_Predict));
        if(FLAG1.bit.CUR_PREDICTIVE_ON == 1)   /*頁倦壓窮儿盾骼念澄?釀侈躰啜臍?圓霞*/
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
        if(FLAG1.bit.DISCRETE_CUR_ON == 1)      //勧由宣柊囃窮送距准匂
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
        else if(FLAG1.bit.DCV_R_CUR_ON == 1)    //個序宣柊囃窮送距准匂
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
            /*D已窮送PI距准匂*/
            pid_id.term.Ref = Process_Data.IdRef;
            pid_id.term.Fdb = Process_Data.Id_Cal;
            PID_POSITIONAL(&pid_id);

            /*Q已窮送PI距准匂*/
            pid_iq.term.Ref = Process_Data.IqRef;
            pid_iq.term.Fdb = Process_Data.Iq_Cal;
            PID_POSITIONAL(&pid_iq);

            //蝕桟距編
//                 pid_id.term.Out = -1.675;
//                 pid_iq.term.Out = 17.5;

            Process_Data.Ud_ACR_Out = pid_id.term.Out;
            Process_Data.Uq_ACR_Out = pid_iq.term.Out;
        }

        //------------------------------------------------------------------------------------------------------------------
        if(FLAG1.bit.VOL_FDB_DECOUPLE == 1)      /*窮儿念澄盾骼*/
        {
             Voltage_Feedforward_Decouple(&(Process_Data.Ud_Decouple),&(Process_Data.Uq_Decouple));
             Process_Data.Ud_PreSat = Process_Data.Ud_ACR_Out+Process_Data.Ud_Decouple;
             Process_Data.Uq_PreSat = Process_Data.Uq_ACR_Out+Process_Data.Uq_Decouple+_IQmpy(Flux_Mag_IQ,qep1.Speed_We);
        }
        else if(FLAG1.bit.COMPL_VECT_DECOUPLE == 1)     /*鹸文楚盾骼*/
        {
             Complex_Vector_Decouple(&(Process_Data.Ud_Decouple),&(Process_Data.Uq_Decouple));
             Process_Data.Ud_PreSat = Process_Data.Ud_ACR_Out+Process_Data.Ud_Decouple;
             Process_Data.Uq_PreSat = Process_Data.Uq_ACR_Out+Process_Data.Uq_Decouple+_IQmpy(Flux_Mag_IQ,qep1.Speed_We);
        }
        else if(FLAG1.bit.VI_CV_DECOUPLE == 1)  /*個序鹸文楚盾骼-倡亭怦森庁侏丕屎*/
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

        Process_Data.Ud_PreSat  = Process_Data.Ud_PreSat + Process_Data.Ud_Disturbance;     //D已翌何窮儿氾強公協
        Process_Data.Uq_PreSat  = Process_Data.Uq_PreSat + Process_Data.Uq_Disturbance;     //Q已翌何窮儿氾強公協
        ipark1.Ds = _IQsat(Process_Data.Ud_PreSat, pid_id.param.OutMax, pid_id.param.OutMin);
        ipark1.Qs = _IQsat(Process_Data.Uq_PreSat, pid_iq.param.OutMax, pid_iq.param.OutMin);

        //--------------------------------------------------------------------------------------------
        if((User_Terminal.in.GAME_MODE==Mode_Openloop_enm)&&(User_Terminal.in.IF_Enable==0))//Ud/Uq徭公協蝕桟塰佩
        {
            //VF庁塀??窮送蝕桟??Uq=3V,Theta_Step=_IQ(0.001),300rpm辛塰佩
            ipark1.Ds = User_Terminal.in.UdRef_OpenL;
            ipark1.Qs = User_Terminal.in.UqRef_OpenL;
        }   //倦夸IF庁塀??窮送液桟??喇PI距准匂補竃公協議Ud/Uq窮儿佚催??

        //--------------------------------------------------------------------------------------------
        /*窮儿狛距崙侃尖?叉舊行温貅９羯?*/
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
          /*窮儿狛距崙侃尖?紺鑑?噫碗曳箭蛍塘Ud,Uq*/
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
        if(FLAG1.bit.ANGLE_COMPEN_ON == 1)  //詰堀才自斤方熟富議秤趨和??叔業温灰唹?貊椀／敏垈嗣躰?
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
        //壙贋輝念田柴麻補竃窮儿恬葎和匯田議窮送圓霞補秘??
        Process_Data.Ud_Beat0 = ipark1.Ds;
        Process_Data.Uq_Beat0 = ipark1.Qs;
        //壙贋輝念田議窮送圓霞潤惚??喘噐和匯田柴麻??
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

    // 序殻延楚補竃幣襖匂鉱霞??蛍艶聞喘PWM+陀襖庁亭DAC??式聞喘糞縞DAC仂頭補竃圭宛??
    // PwmDacCh1/2/3/4 蛍艶卆肝斤哘 EPWM8A/8B/7A/7B, 補秘媼腎曳袈律壓0~1岻寂??
    // dutya/dutyb蛍艶卆肝斤哘DAC_A/B,補秘媼腎曳袈律壓0~1岻寂
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
    EPWMDAC_VAR_WATCH(&pwmdac1);  // 公協葎媼腎曳補竃
    DAC_VAR_WATCH(&dac1);         // 公協葎媼腎曳補竃

    //--------------------------------------------------------------------------------------------------
    if(Data_Record.Dlog_Mode == Mode_Continuous_enm)  //銭偬儉桟泡仟庁塀
    {
       // 潮範銭偬庁塀Mode_Continuous_enm 拝喘薩公協聞嬬User_Input_ON=1??Data_Record_ON=0??
       // 匯違刮屬糞刮聞喘銭偬芝村庁塀??繍Data_Record_ON嚥User_Input_ON炎崗了峻崔了葎1軸辛??
       //銭偬芝村庁塀和峪俶繍Data_Record_ON匯岷崔了葎1??夸匯岷隠隔延楚方象儉桟泡仟芝村;
        // 恷挫音勣壓方怏珊壓泡仟議扮昨贋刈方象??否叟竃?嵎?怏贋刈方象僅偬議?嶇鵤?咀緩個撹銭偬芝村庁塀Mode_Continuous汽肝芝村500方象夸唯峭??
        if(FLAG1.bit.Data_Record_ON == 1)   //蝕尼延楚潤惚壙贋
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
                        GpioDataRegs.GPDCLEAR.bit.GPIO122 = 1;  // GPIO10喘噐幣襖匂?塋昭豌瞞亢?巓豚式殻會塰佩扮海

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
                    GpioDataRegs.GPDCLEAR.bit.GPIO122 = 1;  // GPIO10喘噐幣襖匂?塋昭豌瞞亢?巓豚式殻會塰佩扮海

                    Data_Record.Counter = 0;
                    FLAG1.bit.Data_Record_ON = 0;
                }
            #endif
        }
    }
    else if(Data_Record.Dlog_Mode == Mode_Single_enm)  //汽肝泡仟庁塀
    {
       //汽肝泡仟庁塀喘噐隠贋強蓑狛殻延楚??麼勣頁亟胎猟紙夕方象聞喘乎庁塀??
       // 延楚炎崗了FLAG1.bit.Data_Record_ON喘噐聞嬬方怏芝村方象;
       // 延楚炎崗了FLAG1.bit.User_Input_ON喘噐聞嬬強蓑公協User_Terminal.in.Id_Ref/Iq_Ref;
       // 喘貧峰曾倖炎崗了序佩方象芝村聞嬬嚥強蓑公協聞嬬盾骼陣崙??
       // 峪嗤曾倖炎崗了揖扮崔了葎1嘉嬬蝕尼方怏方象芝村??贋刈方朕功象蝕購撞楕嚥麻隈遇協??
       // ！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
       // 1) 兜兵晒Data_Record.Dlog_Mode = Mode_Single_enm??僉夲葎汽肝泡仟庁塀??
       // 2) 兜兵晒潮範譜崔Data_Record_ON=0??User_Input_ON=1?司皇楜臍?公協Id_Ref/Iq_Ref??公匯倖児云峙Iq_Ref=1A??
       // 3) 竣埔強蓑公協岻念??梓乏會卆肝譜崔User_Input_ON=0??Data_Record_ON=1??
       // 4) 喘薩験峙User_Terminal.in.Id_Ref/Iq_Ref公協??
       // 5) 崔了User_Input_ON=1聞嬬喘薩公協揖扮聞嬬方象芝村??壓殻會嶄譜崔匯粁恬喘决扮??贋刈叱倖公協念議糧蓑方峙??
       // ！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
        if(FLAG1.bit.Data_Record_ON == 1)   //蝕尼延楚潤惚壙贋
        {
            if(FLAG1.bit.User_Input_ON == 1)   //蝕尼延楚潤惚壙贋
            {
                // 蛍怏刮屬廬堀桟才窮送桟來嬬
                #if 0    //刮屬窮送桟議強蓑來嬬
                    Data_Record.DlogCh1[Data_Record.Counter] = Process_Data.IqRef;
                    Data_Record.DlogCh2[Data_Record.Counter] = park1.Qs;
                    Data_Record.DlogCh3[Data_Record.Counter] = Process_Data.IdRef;
                    Data_Record.DlogCh4[Data_Record.Counter] = park1.Ds;
                #else   //刮屬窮送桟強蓑來嬬斤廬堀桟議唹?譯?
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
                    //User_Terminal.in.GAME_MODE = Mode_Stop_enm;  匯嶽僉夲頁云緩糞刮強蓑芝村頼撹朔羨軸唯字??

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

//    GpioDataRegs.GPDCLEAR.bit.GPIO122 = 1;  // GPIO10喘噐幣襖匂?塋昭豌瞞亢?巓豚式殻會塰佩扮海
    GpioDataRegs.GPDCLEAR.bit.GPIO123 = 1;  // GPIO10喘噐幣襖匂?塋昭豌瞞亢?巓豚式殻會塰佩扮海
}

// -------------------------------------------------------------------------------
