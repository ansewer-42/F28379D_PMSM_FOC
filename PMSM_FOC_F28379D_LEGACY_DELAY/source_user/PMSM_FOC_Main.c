/*
 * PMSM_FOC_Main.c
 * ----------------------------------------------------------------------------------
 * Description
 * This file is used for the definition of main function, motor control ISR and state machine;
 * ----------------------------------------------------------------------------------
 *  Created on: 2020年6月18日
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
        ADCINB2——CPMIN3P, ADCINA2——CPMIN1P, ADCIND0——CPMIN7P;
        ADCIN14——CPMIN4P;

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
		//--------——Position Connector: Bottom Board----------------------------
		GPIO32:IN  ---> Hall-U1, Position hall signal input (motor1)
		GPIO67:IN  ---> Hall-V1, Position hall signal input (motor1)
		GPIO111:IN ---> Hall-W1, Position hall signal input (motor1)
		//--------——Position Connector: Transfer Board (for back up)------------
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
// LPI的意思是Low Priority Interrupt 低优先级中断，类似于C28x的普通中断，可以被嵌套；
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
	* 程序要烧写到Flash中只需要:
	* 1) 进行_FLASH预定义#define即可
	* ————在工程文件的Properties/CCS Build/C2000 Compiler/Predefined Symbols 或者 在工程文件中#define _FLASH即可；
	* ————InitFlash()在System_Init()中运行，memcpy()在System_Init()/InitSysCtrl()中运行;
	* 2) 将CMD文件替换为2837xD_FLASH_lnk_cpu1.cmd
	*/
	System_Init();			 	    //SYSTEM Control系统控制模块初始化
	GPIO_Init();			 	    //GPIO模块初始化
    DAC_Setup();                    //DAC模块初始化
    CMPSS_Setup();                  //CMPSS模块初始化
    CMPSS_PHASE_CUR_PRO(&cmpss1);   //CMPSS内部DAC给定值初始化
	ADC_Setup1();		 	        //ADC_A~B模块初始化
	ADC_Setup2();                   //ADC_C~D模块初始化
	EQEP_Setup1(&qep1);		 	    //EQPEP1模块初始化
    EPWM_Setup1(&pwm1);             //EPWM1~3模块初始化，对应motor1
    EPWMDAC_Setup(&pwmdac1);        //PWMDAC模块初始化

	// 中断相关配置
    #if (CONTROL_ISR_MODE == ADC_ISR)
        ADCA1_Interrupt_Set(&Motor_Control_ISR1);
    #else  //(CONTROL_ISR_MODE == PWM_ISR)
        EPWM1_Interrupt_Set(&Motor_Control_ISR1);       //EPWM1中断相关寄存器设置，常规延时模式在EPWM1中断服务程序中运行双电机控制程序；
    #endif
    EnableInterrupts();
    ERTM;

    DISABLE_PWM1;                   //PWM输出使能脚拉高
    DISABLE_PWM_PINS_M1;            //PWM动作限定输出拉低
    Global_Variables_Clear();           //全局变量清零
    Global_Variables_Init();            //全局变量初始化
    Controller_Params_Preset();         //PID,SMC等控制器参数初始化

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

        // 开始记录数据前先给定Iq_Ref/Id_Ref的基础偏置值；
        // 单次存储只需要将模式选为Mode_Single_enm则自动按下述值进行阶跃给定并且自动保存数组操作
        // 连续模式，默认为Mode_Continuous_enm, 只需要将FLAG1.bit.Data_Record_ON 置1即可完成单次存储操作;
        if(Data_Record.Dlog_Mode == Mode_Single_enm)
        {

            // 注意这里每次都会刷新，2kHz下最大允许500*500us=250ms，超过上述LED延时时间，会重新刷新给定旧值，Id/Iq_Dlog_old
            Data_Record.Id_Dlog_old  = User_Terminal.in.Id_Ref;
            Data_Record.Iq_Dlog_old  = User_Terminal.in.Iq_Ref;
            Data_Record.Ud_Dlog_old  = User_Terminal.in.Ud_Disturbance;
            Data_Record.Uq_Dlog_old  = User_Terminal.in.Uq_Disturbance;

            User_Terminal.in.Id_Ref  = Data_Record.Id_Dlog;
            User_Terminal.in.Iq_Ref  = Data_Record.Iq_Dlog;
            User_Terminal.in.Ud_Disturbance = Data_Record.Ud_Dlog;
            User_Terminal.in.Uq_Disturbance = Data_Record.Uq_Dlog;
            FLAG1.bit.User_Input_ON  = 1;   // 实际User_Input这个标志位一直没用，可以去除；
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
    GpioDataRegs.GPDSET.bit.GPIO123 = 1;  // GPIO10用于示波器显示检测中断周期及程序运行时长

    #if DUAL_MOTOR_SYNC    //每次完成数据存储save memory,都要进行一次Data_Record.FLAG标志位清0操作；
    if((Data_Record.FLAG==0) && (GpioDataRegs.GPDDAT.bit.GPIO124 == 1))   //接收motor2的同步信号确认是否开始存储数据
    {
        FLAG1.bit.Data_Record_ON = 1;
        GpioDataRegs.GPDSET.bit.GPIO122 = 1;  // GPIO10用于示波器显示检测中断周期及程序运行时长
        Data_Record.FLAG = 1;                 // 堵塞标志位，否则FPIO124会一直触发判断将Data_Record标志位置位为1;
    }                                         //  每次motor2负载突加之间都要讲Data_Record.FLAG标志位清0；
    #endif

    #if (CONTROL_ISR_MODE == PWM_ISR)
    {
        asm(" RPT #72 || NOP");    //理论上75ns+280ns=355ns可完成一个通道的转换,实际观测示波器发现最小RPT #25可以,20不可以;
    //        DELAY_US(1);
    //        while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);  //PWM_ISR模式用该标志位判断不准确，需要加一段延时，或者先处理别的函数，已经用示波器观测SUM验证；
    }
    #endif

    //--------------------------------------------------------------------------------------------------
    //用于过压过流故障信号发生后的系统重启,检测IGBT信号是否置位，且手动重启信号是否给出
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

        EALLOW;         //TZFLG标志位清除
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

    if(FLAG1.bit.VAR_CLEAR==1)          //全局变量手动清零
    {
        Global_Variables_Clear();
        FLAG1.bit.VAR_CLEAR = 0;
    }

    //--------------------------------------------------------------------------------------------------
    if((IPM_ERROR1 == 0)||(adc1.Flag.bit.Over_Fault_Flag==1)||(EPwm1Regs.TZFLG.bit.OST==1))  //过流过压故障信号发生处理
    {
        DISABLE_PWM1;
        FLAG1.bit.GAME_ON = 0;
        FLAG1.bit.GAME_RESTART=0;
        FLAG1.bit.FAULT_ON = 1;
        User_Terminal.in.GAME_MODE = Mode_Stop_enm;
    }

    if(EQep1Regs.QEPSTS.bit.FIMF==0)    //先进行转速与角度位置计算，再进行电流采样节省计算时间；
    {
        Hall_Angle_Cal(&qep1);          //霍尔传感器角度测量
    }
    else
    {
        EQEP_Angle_Cal(&qep1);          //电角度计算
        EQEP_Speed_Cal(&qep1);          //实时转速计算
    }

    if(!adc1.Flag.bit.Zero_Offset_End)
    {
        ADC_Zero_Offset(&adc1);         //ADC通道零点偏置采样，在GAME_ON=0（封管）时不断进行中性点采样
    }                                   //要改为仅在上电初始化进行一次采样则需删除GAME_ON=0时的else中的Zero_Offset_End=0；
    else
    {
        ADC_Result_Read(&adc1);         //ADC采样结果读取及处理
        ADC_Result_Handle(&adc1);
    }

    //------------------------------------------------------------------------------------------
    /* 根据采得到的母线电压值实时计算DQ轴电流环的积分及输出限幅值 */
    Process_Data.Uref_Max  = _IQmpy(adc1.Result.Voltage_DC,_IQ(0.55));     //SVPWM调制最大电压圆半径，三相逆变器最高相电压
    Voltage_OutMax_Update();       //电流调节器输出电压限幅值根据母线采样电压值更新

    //------------------------------------------------------------------------------------------
    if(FLAG1.bit.GAME_OVER == 1)            //直接手动停机标志位模式
    {
        User_Terminal.in.GAME_MODE = Mode_Stop_enm;
    }

    if(User_Terminal.in.GAME_MODE == Mode_Stop_enm)         //停机模式封PWM，且变量清零；
    {
        FLAG1.bit.GAME_ON = 0;
        Global_Variables_Clear();
    }
    else
    {
        FLAG1.bit.GAME_ON = 1;
    }

    //------------------------------------------------------------------------------------------
    if(FLAG1.bit.CUR_CNT_Kp_ON == 1)        //使用Kp_Ratio变量在线修改调节器比例参数
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
        dcv_r_id.Kr = 3;        //改进离散域控制器有源阻尼倍数
        dcv_r_iq.Kr = 3;
    }
    //------------------------------------------------------------------------------------------
    if(adc1.Flag.bit.Zero_Offset_End && FLAG1.bit.GAME_ON && (!adc1.Flag.bit.Over_Fault_Flag))
    {
        //--------------------------------------------------------------------------------------------
        /* 使能PWM输出和输出缓冲芯片的OE引脚 */
        ENABLE_PWM1;
        ENABLE_PWM_PINS_M1;

        //--------------------------------------------------------------------------------------------
        if(User_Terminal.in.GAME_MODE == Mode_Openloop_enm)     //开环模式
        {
            if(User_Terminal.in.Angle_Enable == 0)  //位置开环,使用自给定转速和位置；
            {
                //开环模式下的角度自给定步进增加，此时调整角度步长即调整电机频率（转速）
                Process_Data.Theta_Elec_OpenL=Process_Data.Theta_Elec_OpenL+User_Terminal.in.Theta_Step;
                if(Process_Data.Theta_Elec_OpenL>_IQ(1))
                {
                    Process_Data.Theta_Elec_OpenL=Process_Data.Theta_Elec_OpenL-_IQ(1);      //用于theta_change为正的情况
                }
                else if(Process_Data.Theta_Elec_OpenL<_IQ(0))
                {
                    Process_Data.Theta_Elec_OpenL=Process_Data.Theta_Elec_OpenL+_IQ(1);      //用于theta_change为负的情况
                }

                //开环运行模式下电流Park变换和电压Ipark变换的电角度赋值，使用自给定步进角度；
                park1.Angle  = Process_Data.Theta_Elec_OpenL;
                ipark1.Angle = Process_Data.Theta_Elec_OpenL;
            }
            else                    //位置闭环，使用传感器的位置信息；
            {
                park1.Angle  = qep1.Theta_Elec;
                ipark1.Angle = qep1.Theta_Elec;
            }

            if(User_Terminal.in.IF_Enable == 1)     //IF模式使能，电流闭环，否则开环默认为VF模式，电流开环
            {
                Process_Data.IdRef = User_Terminal.in.IdRef_OpenL;
                Process_Data.IqRef = User_Terminal.in.IqRef_OpenL;
            }
        }
        else
        {
            //正常运行模式下电流Park变换和电压Ipark变换的电角度赋值，使用传感器角度；
            park1.Angle  = qep1.Theta_Elec;
            ipark1.Angle = qep1.Theta_Elec;
        }


        //--------------------------------------------------------------------------------------------
        /* Clark变换：A/B->Alpha/Beta */
        // clarke1.As = -adc1.Result.Current_B-adc1.Result.Current_C;
        clarke1.As = adc1.Result.Current_A;
        clarke1.Bs = adc1.Result.Current_B;
        CLARKE_TRANS(&clarke1);

        /* Park变换：Alpha/Beta->D/Q */
        //park1.Angle = qep1.Theta_Elec;  //添加I/F流频比控制后，角度赋值在上面模块
        park1.Alpha = clarke1.Alpha;
        park1.Beta  = clarke1.Beta;
        park1.Sine   = _IQsinPU(park1.Angle);
        park1.Cosine = _IQcosPU(park1.Angle);
        PARK_TRANS(&park1);

        //--------------------------------------------------------------------------------------------
        if(User_Terminal.in.GAME_MODE == Mode_Speed_enm )   //转速模式，给定转速方向Speed_DIR和转速给定Speed_Ref
        {
            /*转速给定Speed_Ref的斜坡跟踪*/
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

            /*转速PI调节器*/
            if(qep1.Speed_CNT_ON == 1)  //每10个开关周期进行一次转速计算与转速调节器作用
            {
                pid_spd.term.Ref = Process_Data.SpeedRef;
                pid_spd.term.Fdb = qep1.Speed_PU;
                PID_POSITIONAL(&pid_spd);
            }


            Process_Data.IdRef = 0;
            Process_Data.IqRef = pid_spd.term.Out;
        }

        //--------------------------------------------------------------------------------------------
        if(User_Terminal.in.GAME_MODE == Mode_Torque_enm ) //转矩模式，给定转速方向Speed_DIR和电流给定Is_Ref
        {
            if(FLAG1.bit.Is_Angle_ON == 1)
            {
                /*根据转矩角分配电流幅值，对于隐极式电机，转矩角为90度，Is_Angle = _IQ(0.25)*/
                /*电流给定Is_Ref的斜坡跟踪*/
//              ramp2.TargetValue = User_Terminal.in.Is_Ref;
//              RAMP_FUNC(&ramp2);
//              Process_Data.IsRef = ramp2.SetpointValue;
                Process_Data.IsRef = User_Terminal.in.Is_Ref;            //PI参数整定和算法测使用阶跃给定
                Process_Data.IdRef = _IQmpy(Process_Data.IsRef,_IQcosPU(Process_Data.Is_Angle));
                Process_Data.IqRef = _IQmpy(Process_Data.IsRef,_IQsinPU(Process_Data.Is_Angle));
            }
            else
            {
//              ramp2.TargetValue = User_Terminal.in.Iq_Ref;
//              RAMP_FUNC(&ramp2);
//              Process_Data.IqRef = ramp2.SetpointValue;
                Process_Data.IsRef = User_Terminal.in.Is_Ref;            //PI参数整定和算法测使用阶跃给定
                if(FLAG1.bit.User_Input_ON == 1)      //使能用户给定，故意加一段作用延时，用于存储稳态数据
                {
                    //单次数据记录有用户给定延时作用处理
                    if(Data_Record.Dlog_Mode == Mode_Single_enm && FLAG1.bit.Data_Record_ON == 1)
                    {
                        if(Data_Record.Counter >= DATA_N2)   //延时用户给定作用
                        {
                            Process_Data.IdRef = User_Terminal.in.Id_Ref;
                            Process_Data.IqRef = User_Terminal.in.Iq_Ref;
                            Process_Data.Ud_Disturbance = User_Terminal.in.Ud_Disturbance;
                            Process_Data.Uq_Disturbance = User_Terminal.in.Uq_Disturbance;
                        }
                    }
                    else   //非单次数据记录模式直接使能用户给定
                    {
                        Process_Data.IdRef = User_Terminal.in.Id_Ref;
                        Process_Data.IqRef = User_Terminal.in.Iq_Ref;
                        Process_Data.Ud_Disturbance = User_Terminal.in.Ud_Disturbance;
                        Process_Data.Uq_Disturbance = User_Terminal.in.Uq_Disturbance;
                    }
                }
            }

            /*反向旋转提供负转矩，IqRef<0*/
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
        //电流预测算法，注意要在三相电流ABC-->Alpha/Beta变换后添加；
        Current_Predictive(&(Process_Data.Id_Predict),&(Process_Data.Iq_Predict));
        if(FLAG1.bit.CUR_PREDICTIVE_ON == 1)   /*是否在电压解耦前馈项中添加电流预测*/
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
        if(FLAG1.bit.DISCRETE_CUR_ON == 1)      //传统离散域电流调节器
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
        else if(FLAG1.bit.DCV_R_CUR_ON == 1)    //改进离散域电流调节器
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
            /*D轴电流PI调节器*/
            pid_id.term.Ref = Process_Data.IdRef;
            pid_id.term.Fdb = Process_Data.Id_Cal;
            PID_POSITIONAL(&pid_id);

            /*Q轴电流PI调节器*/
            pid_iq.term.Ref = Process_Data.IqRef;
            pid_iq.term.Fdb = Process_Data.Iq_Cal;
            PID_POSITIONAL(&pid_iq);

            //开环调试
//                 pid_id.term.Out = -1.675;
//                 pid_iq.term.Out = 17.5;

            Process_Data.Ud_ACR_Out = pid_id.term.Out;
            Process_Data.Uq_ACR_Out = pid_iq.term.Out;
        }

        //------------------------------------------------------------------------------------------------------------------
        if(FLAG1.bit.VOL_FDB_DECOUPLE == 1)      /*电压前馈解耦*/
        {
             Voltage_Feedforward_Decouple(&(Process_Data.Ud_Decouple),&(Process_Data.Uq_Decouple));
             Process_Data.Ud_PreSat = Process_Data.Ud_ACR_Out+Process_Data.Ud_Decouple;
             Process_Data.Uq_PreSat = Process_Data.Uq_ACR_Out+Process_Data.Uq_Decouple+_IQmpy(Flux_Mag_IQ,qep1.Speed_We);
        }
        else if(FLAG1.bit.COMPL_VECT_DECOUPLE == 1)     /*复矢量解耦*/
        {
             Complex_Vector_Decouple(&(Process_Data.Ud_Decouple),&(Process_Data.Uq_Decouple));
             Process_Data.Ud_PreSat = Process_Data.Ud_ACR_Out+Process_Data.Ud_Decouple;
             Process_Data.Uq_PreSat = Process_Data.Uq_ACR_Out+Process_Data.Uq_Decouple+_IQmpy(Flux_Mag_IQ,qep1.Speed_We);
        }
        else if(FLAG1.bit.VI_CV_DECOUPLE == 1)  /*改进复矢量解耦-虚拟阻抗模型校正*/
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

        Process_Data.Ud_PreSat  = Process_Data.Ud_PreSat + Process_Data.Ud_Disturbance;     //D轴外部电压扰动给定
        Process_Data.Uq_PreSat  = Process_Data.Uq_PreSat + Process_Data.Uq_Disturbance;     //Q轴外部电压扰动给定
        ipark1.Ds = _IQsat(Process_Data.Ud_PreSat, pid_id.param.OutMax, pid_id.param.OutMin);
        ipark1.Qs = _IQsat(Process_Data.Uq_PreSat, pid_iq.param.OutMax, pid_iq.param.OutMin);

        //--------------------------------------------------------------------------------------------
        if((User_Terminal.in.GAME_MODE==Mode_Openloop_enm)&&(User_Terminal.in.IF_Enable==0))//Ud/Uq自给定开环运行
        {
            //VF模式，电流开环，Uq=3V,Theta_Step=_IQ(0.001),300rpm可运行
            ipark1.Ds = User_Terminal.in.UdRef_OpenL;
            ipark1.Qs = User_Terminal.in.UqRef_OpenL;
        }   //否则IF模式，电流闭环，由PI调节器输出给定的Ud/Uq电压信号；

        //--------------------------------------------------------------------------------------------
        /*电压过调制处理：电压圆轨迹规划*/
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
          /*电压过调制处理：按正余弦比例分配Ud,Uq*/
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
        if(FLAG1.bit.ANGLE_COMPEN_ON == 1)  //低速和极对数较少的情况下，角度补偿影响较小可以不添加
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
        //暂存当前拍计算输出电压作为下一拍的电流预测输入；
        Process_Data.Ud_Beat0 = ipark1.Ds;
        Process_Data.Uq_Beat0 = ipark1.Qs;
        //暂存当前拍的电流预测结果，用于下一拍计算；
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

    // 进程变量输出示波器观测，分别使用PWM+滤波模拟DAC，及使用实际DAC芯片输出方案；
    // PwmDacCh1/2/3/4 分别依次对应 EPWM8A/8B/7A/7B, 输入占空比范围在0~1之间；
    // dutya/dutyb分别依次对应DAC_A/B,输入占空比范围在0~1之间
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
    EPWMDAC_VAR_WATCH(&pwmdac1);  // 给定为占空比输出
    DAC_VAR_WATCH(&dac1);         // 给定为占空比输出

    //--------------------------------------------------------------------------------------------------
    if(Data_Record.Dlog_Mode == Mode_Continuous_enm)  //连续循环刷新模式
    {
       // 默认连续模式Mode_Continuous_enm 且用户给定使能User_Input_ON=1，Data_Record_ON=0；
       // 一般验证实验使用连续记录模式，将Data_Record_ON与User_Input_ON标志位皆置位为1即可；
       //连续记录模式下只需将Data_Record_ON一直置位为1，则一直保持变量数据循环刷新记录;
        // 最好不要在数组还在刷新的时候存储数据，容易出现数组存储数据断续的现象，因此改成连续记录模式Mode_Continuous单次记录500数据则停止；
        if(FLAG1.bit.Data_Record_ON == 1)   //开启变量结果暂存
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
                        GpioDataRegs.GPDCLEAR.bit.GPIO122 = 1;  // GPIO10用于示波器显示检测中断周期及程序运行时长

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
                    GpioDataRegs.GPDCLEAR.bit.GPIO122 = 1;  // GPIO10用于示波器显示检测中断周期及程序运行时长

                    Data_Record.Counter = 0;
                    FLAG1.bit.Data_Record_ON = 0;
                }
            #endif
        }
    }
    else if(Data_Record.Dlog_Mode == Mode_Single_enm)  //单次刷新模式
    {
       //单次刷新模式用于保存动态过程变量，主要是写论文绘图数据使用该模式；
       // 变量标志位FLAG1.bit.Data_Record_ON用于使能数组记录数据;
       // 变量标志位FLAG1.bit.User_Input_ON用于使能动态给定User_Terminal.in.Id_Ref/Iq_Ref;
       // 用上述两个标志位进行数据记录使能与动态给定使能解耦控制；
       // 只有两个标志位同时置位为1才能开启数组数据记录，存储数目根据开关频率与算法而定；
       // ——————————————————————————————————————————————————————————————————————————————————————
       // 1) 初始化Data_Record.Dlog_Mode = Mode_Single_enm，选择为单次刷新模式；
       // 2) 初始化默认设置Data_Record_ON=0，User_Input_ON=1；使能电流给定Id_Ref/Iq_Ref，给一个基本值Iq_Ref=1A；
       // 3) 阶跃动态给定之前，按顺序依次设置User_Input_ON=0，Data_Record_ON=1；
       // 4) 用户赋值User_Terminal.in.Id_Ref/Iq_Ref给定；
       // 5) 置位User_Input_ON=1使能用户给定同时使能数据记录，在程序中设置一段作用延时，存储几个给定前的稳态数值；
       // ——————————————————————————————————————————————————————————————————————————————————————
        if(FLAG1.bit.Data_Record_ON == 1)   //开启变量结果暂存
        {
            if(FLAG1.bit.User_Input_ON == 1)   //开启变量结果暂存
            {
                // 分组验证转速环和电流环性能
                #if 0    //验证电流环的动态性能
                    Data_Record.DlogCh1[Data_Record.Counter] = Process_Data.IqRef;
                    Data_Record.DlogCh2[Data_Record.Counter] = park1.Qs;
                    Data_Record.DlogCh3[Data_Record.Counter] = Process_Data.IdRef;
                    Data_Record.DlogCh4[Data_Record.Counter] = park1.Ds;
                #else   //验证电流环动态性能对转速环的影响；
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
                    //User_Terminal.in.GAME_MODE = Mode_Stop_enm;  一种选择是本此实验动态记录完成后立即停机；

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

//    GpioDataRegs.GPDCLEAR.bit.GPIO122 = 1;  // GPIO10用于示波器显示检测中断周期及程序运行时长
    GpioDataRegs.GPDCLEAR.bit.GPIO123 = 1;  // GPIO10用于示波器显示检测中断周期及程序运行时长
}

// -------------------------------------------------------------------------------
