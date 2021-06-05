/*
 * Global_Functions_Variables_Call.c
 *
 *  Created on: 2020��3��30��
 *      Author: YY
 */

#include "Global_Functions_Variables_Call.h"

/************************************************************************************/
// Global variables used in this system
/************************************************************************************/
PWM pwm1 = PWM_DEFAULTS;
DAC dac1 = DAC_DEFAULTS;
ADC adc1 = ADC_DEFAULTS;
QEP qep1 = QEP_DEFAULTS;
CMPSS  cmpss1  = CMPSS_DEFAULTS;
PWMDAC pwmdac1 = PWMDAC_DEFAULTS;

RMPCNTL ramp1 =  RMPCNTL_DEFAULTS;
RMPCNTL ramp2 =  RMPCNTL_DEFAULTS;
RMPCNTL ramp3 =  RMPCNTL_DEFAULTS;
RMPCNTL ramp4 =  RMPCNTL_DEFAULTS;

PARK   park1  = PARK_DEFAULTS;
IPARK  ipark1 = IPARK_DEFAULTS;
CLARKE clarke1 = CLARK_DEFAULTS;
SVPWM  svpwm1 = SVPWM_DEFAULTS;

PID_CONTROLLER pid_id = PID_CONTROLLER_DEFAULTS;
PID_CONTROLLER pid_iq = PID_CONTROLLER_DEFAULTS;
PID_CONTROLLER pid_spd = PID_CONTROLLER_DEFAULTS;

DCV_CONTROLLER dcv_id  = DCV_CONTROLLER_DEFAULTS;
DCV_CONTROLLER dcv_iq  = DCV_CONTROLLER_DEFAULTS;

DCV_CONTROLLER dcv_r_id  = DCV_CONTROLLER_DEFAULTS;
DCV_CONTROLLER dcv_r_iq  = DCV_CONTROLLER_DEFAULTS;

FLAG_VAR FLAG1 = FLAG_DEFAULTS;
PROCESS_DATA Process_Data = PROCESS_DATA_DEFAULTS;
USER_TERMINALS User_Terminal = USER_TERMINALS_DEFAULTS;

/*���ڹ��̱����ݴ�*/
DATA_RECORD Data_Record = DATA_RECORD_DEFAULTS;

#if MOTOR_PARAM_VAR
/*����ʵ����̵��������еȲ�������궨��ͬ��*/
_iq Ld_IQ   = 0.00092;
_iq Lq_IQ   = 0.00092;
_iq Rs_IQ   = 0.34;
_iq Flux_Mag_IQ = 0.01428;
_iq Inertia_IQ  = 0.0004808;

/*����ʵ����������迹������ƣ���궨��ͬ��*/
_iq Lad_IQ = 0.0005;
_iq Laq_IQ = 0.0005;
_iq Ra_IQ  =  0.34;
#endif
//-------------------------------------------------------------------------------------

/************************************************************************************/
// Global Functions Defined in this file
/************************************************************************************/
void Global_Variables_Init(void)
{
    //USER_TERMINAL������ʼ��
    User_Terminal.in.Is_Ref = 0;
    User_Terminal.in.Id_Ref = 0;
    User_Terminal.in.Iq_Ref = 0;
    User_Terminal.in.Speed_Ref = 0;
    User_Terminal.in.Speed_DIR = 0;
    User_Terminal.in.GAME_MODE = Mode_Stop_enm;
    User_Terminal.in.Theta_Step  = 0;       //10kHz��0.0001��Ӧ1Hz,5kHz��0.0002��Ӧ1Hz��n=60f/p=30rpm
    User_Terminal.in.UdRef_OpenL = 0;       //VFģʽ��10kHz�£�Uq=3V=_IQ(0.1),Theta_Step=_IQ(0.001),300rpm������
    User_Terminal.in.UqRef_OpenL = 0;       //VF���Ծ��飺�ȼӴ�Uq,��������Theta_Step
    User_Terminal.in.IdRef_OpenL = 0;       //fe=fs*Theta_Step,
    User_Terminal.in.IqRef_OpenL = 0;
    User_Terminal.in.IF_Enable = 0;
    User_Terminal.in.Angle_Enable = 0;
    User_Terminal.in.Ud_Disturbance = 0;
    User_Terminal.in.Uq_Disturbance = 0;

    //Flag������ʼ��
    FLAG1.bit.GAME_ON      = 0;
    FLAG1.bit.GAME_RESTART = 0;
    FLAG1.bit.VAR_CLEAR    = 0;
    FLAG1.bit.Data_Record_ON = 0;
    FLAG1.bit.User_Input_ON = 1;

    // ע�ⲻͬ����ģʽ��־λ�������ڴ˳�ʼ��������ű��е�count��־λ��1�����������޷����㣻
    FLAG1.bit.ANGLE_COMPEN_ON = 1;
    FLAG1.bit.VOL_FDB_DECOUPLE = 0;
    FLAG1.bit.COMPL_VECT_DECOUPLE =1;
    FLAG1.bit.VI_CV_DECOUPLE       = 0;
    FLAG1.bit.CUR_PREDICTIVE_ON = 0;
    FLAG1.bit.DISCRETE_CUR_ON = 0;
    FLAG1.bit.DCV_R_CUR_ON = 0;
    FLAG1.bit.CUR_PREDICT_COMPEN = 0;
    FLAG1.bit.CUR_CNT_Kp_ON  = 1;

    //QEP������ʼ��
    qep1.Device_Index = 1;
    qep1.Encoder_N = 4000;
    qep1.Speed_Cal_CNT = 0;
    qep1.Speed_PU = 0;
    qep1.Filter0_Speed = _IQ(0.1);
    qep1.Filter1_Speed = _IQ(0.9);

    //ADC������ʼ��
    adc1.Flag.all = 0;
    adc1.Filter0_Idc = _IQ(0.1);
    adc1.Filter1_Idc = _IQ(0.9);
    adc1.Filter0_Udc = _IQ(0.1);
    adc1.Filter1_Udc = _IQ(0.9);
    adc1.Result.Current_A_Last = 0;
    adc1.Result.Current_B_Last = 0;
    adc1.Result.Current_C_Last = 0;
    adc1.Result.Current_DC_Last = 0;
    adc1.Result.Voltage_A_Last = 0;
    adc1.Result.Voltage_B_Last = 0;
    adc1.Result.Voltage_C_Last = 0;
    adc1.Result.Voltage_C_Last = 0;
    adc1.Result.Voltage_DC_Last = 0;
    adc1.Fault_Current_A_Count = 0;
    adc1.Fault_Current_B_Count = 0;
    adc1.Fault_Current_DC_Count = 0;
    adc1.Fault_Voltage_DC_Count = 0;

    //���̱�����ʼ��
    Process_Data.Theta_Elec_OpenL = 0;
    Process_Data.Is_Angle = _IQ(0.25);
    Process_Data.Angle_Compen_Ratio = _IQ(2.0);
    Process_Data.Current_Predict_Beats = 1.5;
    Process_Data.Kp_Ratio = 12;
    Process_Data.Current_Predict_Mode = 0;
    Process_Data.Ki_Predict = _IQmpy(2*pi*PWM_FREQUENCY*1000/Process_Data.Kp_Ratio*Rs_IQ,T_Sample);

    //RAMP������ʼ��
    ramp1.Gain=_IQ(0.00001)*BASE_SPEED;
    ramp2.Gain=_IQ(0.00001)*BASE_CURRENT;
    ramp3.Gain=_IQ(0.000001)*BASE_CURRENT;
    ramp4.Gain=_IQ(0.000001)*BASE_CURRENT;

    //Data_Record ������ʼ��
    Data_Record.Counter = 0;
    Data_Record.Dlog_State =T_Sample;
    Data_Record.Ud_Dlog = 0;
    Data_Record.Uq_Dlog = 0;
    Data_Record.Id_Dlog = 0;
    Data_Record.Iq_Dlog = 0;
    Data_Record.Ud_Dlog_old = 0;
    Data_Record.Uq_Dlog_old = 0;
    Data_Record.Id_Dlog_old = 0;
    Data_Record.Iq_Dlog_old = 0;
    Data_Record.Dlog_Mode = Mode_Continuous_enm;
}


void Global_Variables_Clear(void)
{
    //-----------------------------------------------------------------------------------
    pid_id.data.Up=0;
    pid_id.data.Ui=0;
    pid_id.data.Ud=0;
    pid_id.term.Out=0;
    pid_id.data.OutPreSat=0;
    pid_id.data.SatErr=0;

    pid_iq.data.Up=0;
    pid_iq.data.Ui=0;
    pid_iq.data.Ud=0;
    pid_iq.term.Out=0;
    pid_iq.data.OutPreSat=0;
    pid_iq.data.SatErr=0;

    pid_spd.data.Up=0;
    pid_spd.data.Ui=0;
    pid_spd.data.Ud=0;
    pid_spd.term.Out=0;
    pid_spd.data.OutPreSat=0;
    pid_spd.data.SatErr=0;

    //------------------------------------------------------------------------------------------------------------------
    dcv_id.Out = 0;
    dcv_id.Err = 0;
    dcv_id.Err_Beat_1 = 0;

    dcv_iq.Out = 0;
    dcv_iq.Err = 0;
    dcv_iq.Err_Beat_1 = 0;

    //------------------------------------------------------------------------------------------------------------------
    dcv_r_id.Out = 0;
    dcv_r_id.Err = 0;
    dcv_r_id.Err_Beat_1 = 0;
    dcv_r_id.Uc = 0;
    dcv_r_iq.Ur = 0;

    dcv_r_iq.Out = 0;
    dcv_r_iq.Err = 0;
    dcv_r_iq.Err_Beat_1 = 0;
    dcv_r_iq.Uc = 0;
    dcv_r_id.Ur = 0;

    //------------------------------------------------------------------------------------------------------------------
    User_Terminal.in.Is_Ref = 0;
    User_Terminal.in.Id_Ref = 0;
    User_Terminal.in.Iq_Ref = 0;
    User_Terminal.in.Speed_Ref = 0;
    User_Terminal.in.Speed_DIR = 0;
    User_Terminal.in.GAME_MODE = Mode_Stop_enm;
    User_Terminal.in.Theta_Step = 0;
    User_Terminal.in.UdRef_OpenL = 0;
    User_Terminal.in.UqRef_OpenL = 0;
    User_Terminal.in.IdRef_OpenL = 0;
    User_Terminal.in.IqRef_OpenL = 0;
    User_Terminal.in.IF_Enable = 0;
    User_Terminal.in.Angle_Enable = 0;
    User_Terminal.in.Ud_Disturbance = 0;
    User_Terminal.in.Uq_Disturbance = 0;

    //Data_Record ������ʼ��
    FLAG1.bit.Data_Record_ON = 0;
    Data_Record.Counter = 0;
    Data_Record.FLAG    = 0;
    Data_Record.Ud_Dlog = 0;
    Data_Record.Uq_Dlog = 0;
    Data_Record.Id_Dlog = 0;
    Data_Record.Iq_Dlog = 0;
    Data_Record.Dlog_Mode = Mode_Continuous_enm;

//    qep1.Speed_Cal_CNT = 0;
//    qep1.Speed_PU = 0;
//    qep1.Speed_We = 0;
//    qep1.Speed_PU = 0;

    adc1.Result.Current_A_Last = 0;
    adc1.Result.Current_B_Last = 0;
    adc1.Result.Current_C_Last = 0;
    adc1.Result.Current_DC_Last = 0;
    adc1.Result.Voltage_A_Last = 0;
    adc1.Result.Voltage_B_Last = 0;
    adc1.Result.Voltage_C_Last = 0;
    adc1.Result.Voltage_C_Last = 0;
    adc1.Result.Voltage_DC_Last = 0;

    park1.Ds = 0;
    park1.Qs = 0;

    ramp1.SetpointValue = 0;
    ramp2.SetpointValue = 0;
    ramp3.SetpointValue = 0;
    ramp4.SetpointValue = 0;

    Process_Data.Ud_Decouple = 0;
    Process_Data.Uq_Decouple = 0;
    Process_Data.Ud_CV_Decouple = 0;
    Process_Data.Uq_CV_Decouple = 0;
    Process_Data.Ud_VI_Decouple = 0;
    Process_Data.Uq_VI_Decouple = 0;
    Process_Data.Ud_PreSat = 0;
    Process_Data.Uq_PreSat = 0;
    Process_Data.Theta_Elec_OpenL = 0;
    Process_Data.Ud_Disturbance = 0;
    Process_Data.Uq_Disturbance = 0;
    Process_Data.Counter = 0;
    Process_Data.Ud_Compen = 0;
    Process_Data.Uq_Compen = 0;

    EPwm1Regs.CMPA.bit.CMPA = 0.5*pwm1.PeriodMax;    // PWM 1A - PhaseA
    EPwm2Regs.CMPA.bit.CMPA = 0.5*pwm1.PeriodMax;    // PWM 2A - PhaseB
    EPwm3Regs.CMPA.bit.CMPA = 0.5*pwm1.PeriodMax;    // PWM 3A - PhaseC
    //-------------------------------------------------------------------------------------------
}

void Controller_Params_Preset(void)
{
    pid_id.param.Kp=_IQ(2.4);       //Kp=a*Ld, a=2*pi*Ld*fs*(1/18~1/12)=1.61~2.41(5kHz)
    pid_id.param.Ki=_IQ(0.074);     //Ki=R/L*Ts=0.34/0.00092*T_Sample=0.074(5kHz)
    pid_id.param.Kc=_IQ(0.4);
    pid_id.param.Kd=_IQ(0.0);
    pid_id.param.OutMax=_IQmpy(adc1.Result.Voltage_DC,0.55);        //Id��������������޷�����ĸ�ߵ�ѹʵʱ�����仯
    pid_id.param.OutMin=-pid_id.param.OutMax;
    pid_id.param.UpMax=_IQ( 0.2)*BASE_CURRENT;
    pid_id.param.UpMin=_IQ(-0.2)*BASE_CURRENT;
    pid_id.param.UiMax=pid_id.param.OutMax;     //Id���������������޷�����ĸ�ߵ�ѹʵʱ�����仯��������޷����
    pid_id.param.UiMin=pid_id.param.OutMin;

    pid_iq.param.Kp=_IQ(2.4);
    pid_iq.param.Ki=_IQ(0.074);
    pid_iq.param.Kc=_IQ(0.0);
    pid_iq.param.Kd=_IQ(0.0);
    pid_iq.param.OutMax=_IQmpy(adc1.Result.Voltage_DC,0.55);      //Iq��������������޷�����ĸ�ߵ�ѹʵʱ�����仯
    pid_iq.param.OutMin=-pid_iq.param.OutMax;
    pid_iq.param.UpMax=_IQ( 0.2)*BASE_CURRENT;
    pid_iq.param.UpMin=_IQ(-0.2)*BASE_CURRENT;
    pid_iq.param.UiMax=pid_iq.param.OutMax;     //Iq���������������޷�����ĸ�ߵ�ѹʵʱ�����仯��������޷����
    pid_iq.param.UiMin=pid_iq.param.OutMin;

    pid_spd.param.Kp=_IQ(0.01);           //Ĭ��Ϊ10kHzʱ������5kHzʱKp=0.005,Ki=0.01;
    pid_spd.param.Ki=_IQ(0.025);          //2kHzʱ��Kp=0.0025,Ki=0.005;
    pid_spd.param.Kc=_IQ(0.4);
    pid_spd.param.Kd=_IQ(0.0);
    pid_spd.param.OutMax=_IQ( 0.35)*BASE_CURRENT;       //ת�ٻ�PI����������޷�,��ΪIq�������ֵ
    pid_spd.param.OutMin=_IQ(-0.35)*BASE_CURRENT;
    pid_spd.param.UpMax=_IQ( 1)*BASE_SPEED;
    pid_spd.param.UpMin=_IQ(-1)*BASE_SPEED;
    pid_spd.param.UiMax=pid_spd.param.OutMax;   //ת�ٻ�PI����������޷�������޷����
    pid_spd.param.UiMin=pid_spd.param.OutMin;

    //--------------------------------------------------------------------------------------------------------------------------
    dcv_id.Kp = 0.5;
    dcv_iq.Kp = 0.5;
    dcv_r_id.Kp = 0.5;
    dcv_r_iq.Kp = 0.5;
    dcv_r_id.Kr = 3;        //�Ľ���ɢ���������Դ���ᱶ��
    dcv_r_iq.Kr = 3;
    //--------------------------------------------------------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------------------------------------------------------------------------
// ÿ���ڸ���ֱ��ĸ�ߵ�ѹUdc��ADC����ֵ������������ѹ
//*******************************************************************************************************************
void Voltage_OutMax_Update(void)
{
    //PI�������������ֺ�����޷�ֵ����
    pid_id.param.OutMax = _IQmpy(adc1.Result.Voltage_DC,0.55);   //����PI����������ת�ػ������޷�
    pid_id.param.OutMin = -pid_id.param.OutMax;
    pid_id.param.UiMax  = pid_id.param.OutMax;
    pid_id.param.UiMin  = pid_id.param.OutMin;
    pid_iq.param.OutMax = _IQmpy(adc1.Result.Voltage_DC,0.55);   //����PI����������ת�ػ������޷�
    pid_iq.param.OutMin = -pid_iq.param.OutMax;
    pid_iq.param.UiMax  = pid_iq.param.OutMax;
    pid_iq.param.UiMin  = pid_iq.param.OutMin;

    //��ɢ��ʸ��DCV��������������޷�ֵ����
    dcv_id.OutMax = _IQmpy(adc1.Result.Voltage_DC,0.55);
    dcv_id.OutMin = -dcv_id.OutMax;
    dcv_iq.OutMax = _IQmpy(adc1.Result.Voltage_DC,0.55);
    dcv_iq.OutMin = -dcv_iq.OutMax;

    //��ɢ��ʸ��DCV��������������޷�ֵ����
    dcv_r_id.OutMax = _IQmpy(adc1.Result.Voltage_DC,0.55);
    dcv_r_id.OutMin = -dcv_r_id.OutMax;
    dcv_r_iq.OutMax = _IQmpy(adc1.Result.Voltage_DC,0.55);
    dcv_r_iq.OutMin = -dcv_r_iq.OutMax;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// ��ѹ���̽�����һ�ĵ���Ԥ��,
//*******************************************************************************************************************
void Current_Predictive(_iq *Id_Pre,_iq *Iq_Pre)
{
    _iq Id_Temp1,Id_Temp2,Id_Temp3,Id_Temp4,Ud_Temp1;
    _iq Iq_Temp1,Iq_Temp2,Iq_Temp3,Iq_Temp4,Uq_Temp1;
    _iq Ts_Predict =  _IQmpy(T_Sample,Process_Data.Current_Predict_Beats);
    _iq Rs_Ts__Ldq = _IQmpy(_IQdiv(Rs_IQ,Ld_IQ),Ts_Predict);
    _iq cos_weTs,sin_weTs;

    if(FLAG1.bit.CUR_PREDICT_COMPEN == 1)
    {
        //����Ԥ�����dq�Ჹ����ѹ���㣬�������ַ�
        Process_Data.Ud_Compen += _IQmpy(Process_Data.Ki_Predict, park1.Ds-Process_Data.Id_Predict_1);
        Process_Data.Uq_Compen += _IQmpy(Process_Data.Ki_Predict, park1.Qs-Process_Data.Iq_Predict_1);
        Process_Data.Ud_Compen = _IQsat(Process_Data.Ud_Compen, pid_id.param.OutMax, pid_id.param.OutMin);
        Process_Data.Uq_Compen = _IQsat(Process_Data.Uq_Compen, pid_iq.param.OutMax, pid_iq.param.OutMin);
    }
    else
    {
        Process_Data.Ud_Compen = 0;
        Process_Data.Uq_Compen = 0;
    }

    //��ͬ����Ԥ���㷨ѡ��0-һ��ǰ��ŷ������1-����ŷ����ɢ��2-�����ݼ����Ż�
    if(Process_Data.Current_Predict_Mode == 0)
    {
        //----------һ��ǰ��ŷ����ɢ����ѹ����-------------------------------------------------------------------------------
        Ud_Temp1 = _IQmpy(_IQmpy(qep1.Speed_We,park1.Qs),Lq_IQ)+Process_Data.Ud_Beat0+Process_Data.Ud_Compen;
        Id_Temp1  = _IQdiv(_IQmpy(Ud_Temp1,Ts_Predict),Ld_IQ);
        Id_Temp2  = 1-_IQmpy(_IQdiv(Rs_IQ,Ld_IQ),Ts_Predict);
        Id_Temp2  = _IQmpy(park1.Ds,Id_Temp2);
        *Id_Pre = Id_Temp1+Id_Temp2;

         Uq_Temp1 = Process_Data.Uq_Beat0-_IQmpy(_IQmpy(qep1.Speed_We,park1.Ds),Ld_IQ);
         Uq_Temp1 = Uq_Temp1 - _IQmpy(qep1.Speed_We,Flux_Mag_IQ)+Process_Data.Uq_Compen;
         Iq_Temp1  = _IQdiv(_IQmpy(Uq_Temp1,Ts_Predict),Lq_IQ);
         Iq_Temp2  = 1-_IQmpy(_IQdiv(Rs_IQ,Lq_IQ),Ts_Predict);
         Iq_Temp2  = _IQmpy(park1.Qs,Iq_Temp2);
         *Iq_Pre = Iq_Temp1+Iq_Temp2;
         //__________________________________________________________
    }
    else if(Process_Data.Current_Predict_Mode == 1)
    {
         //----------����ŷ����ɢ����ѹ����-------------------------------------------------------------------------------
        Ud_Temp1 = _IQmpy(_IQmpy(qep1.Speed_We,park1.Qs),Lq_IQ)+Process_Data.Ud_Beat0+Process_Data.Ud_Compen;
        Id_Temp1  = _IQdiv(_IQmpy(Ud_Temp1,Ts_Predict),Ld_IQ);
        Id_Temp2  = 1-_IQmpy(_IQdiv(Rs_IQ,Ld_IQ),Ts_Predict);
        Id_Temp2  = _IQmpy(park1.Ds,Id_Temp2);
        Id_Temp3 = Id_Temp1+Id_Temp2;
        Id_Temp4 = _IQmpy(_IQmpy(_IQdiv(Rs_IQ,Ld_IQ),Ts_Predict),Id_Temp3-park1.Ds);
        *Id_Pre = Id_Temp3 - 0.5*Id_Temp4;

         Uq_Temp1 = Process_Data.Uq_Beat0-_IQmpy(_IQmpy(qep1.Speed_We,park1.Ds),Ld_IQ);
         Uq_Temp1 = Uq_Temp1 - _IQmpy(qep1.Speed_We,Flux_Mag_IQ)+Process_Data.Uq_Compen;
         Iq_Temp1  = _IQdiv(_IQmpy(Uq_Temp1,Ts_Predict),Lq_IQ);
         Iq_Temp2  = 1-_IQmpy(_IQdiv(Rs_IQ,Lq_IQ),Ts_Predict);
         Iq_Temp2  = _IQmpy(park1.Qs,Iq_Temp2);
         Iq_Temp3 = Iq_Temp1+Iq_Temp2;
         Iq_Temp4 = -_IQmpy(_IQmpy(_IQdiv(Rs_IQ,Lq_IQ),Ts_Predict),Iq_Temp3-park1.Qs);
         *Iq_Pre = Iq_Temp3 - 0.5*Iq_Temp4;
        //__________________________________________________________
    }
    else if(Process_Data.Current_Predict_Mode == 2)
    {
        //----------�����ݼ���e^(A*Ts)�Ż���-------------------------------------------------------------------------------
        //-----------ֻ�Ż�id(k)/iq(k)ϵ������e^(A*Ts)------------------------------------------
       cos_weTs = _IQcos(_IQmpy(qep1.Speed_We,Ts_Predict));
       sin_weTs  =  _IQsin(_IQmpy(qep1.Speed_We,Ts_Predict));

//        cos_weTs = 1;
//        sin_weTs = _IQmpy(qep1.Speed_We,Ts_Predict) ;

       Id_Temp1 = _IQmpy(cos_weTs,park1.Ds)+_IQmpy(sin_weTs,park1.Qs);
       Id_Temp2 = _IQmpy(Id_Temp1,1-Rs_Ts__Ldq);
       Id_Temp3  = _IQdiv(_IQmpy(Process_Data.Ud_Beat0+Process_Data.Ud_Compen,Ts_Predict),Ld_IQ);
       *Id_Pre = Id_Temp2+Id_Temp3;

       Iq_Temp1 = -_IQmpy(sin_weTs,park1.Ds)+_IQmpy(cos_weTs,park1.Qs);
       Iq_Temp2 = _IQmpy(Iq_Temp1,1-Rs_Ts__Ldq);
       Uq_Temp1 = Process_Data.Uq_Beat0+Process_Data.Uq_Compen-_IQmpy(qep1.Speed_We,Flux_Mag_IQ);
       Iq_Temp3  = _IQdiv(_IQmpy(Uq_Temp1,Ts_Predict),Lq_IQ);
       *Iq_Pre = Iq_Temp2+Iq_Temp3;
        //__________________________________________________________
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------
// ������������Ʋ���
//*******************************************************************************************************************************
void Voltage_Feedforward_Decouple(_iq *Ud,_iq *Uq)    /*��ѹǰ���������*/
{
    //ע��DQ���ѹǰ����ķ����෴
    *Ud = -_IQmpy(_IQmpy(Lq_IQ,Process_Data.Iq_Cal),qep1.Speed_We);
    *Uq =  _IQmpy(_IQmpy(Ld_IQ,Process_Data.Id_Cal),qep1.Speed_We);
}

void Complex_Vector_Decouple(_iq *Ud,_iq *Uq)        /*��ʸ���������*/
{
    _iq  Ud_Temp_Decouple=0, Uq_Temp_Decouple=0;

    //ע��DQ���ѹǰ����ķ����෴
    Ud_Temp_Decouple = _IQmpy(_IQmpy(pid_iq.param.Kp, qep1.Speed_We),pid_iq.data.Err);
    Uq_Temp_Decouple = _IQmpy(_IQmpy(pid_id.param.Kp, qep1.Speed_We),pid_id.data.Err);
    Ud_Temp_Decouple = -_IQmpy(Ud_Temp_Decouple ,T_Sample);
    Uq_Temp_Decouple =  _IQmpy(Uq_Temp_Decouple ,T_Sample);

    *Ud = *Ud+Ud_Temp_Decouple;
    *Uq = *Uq+Uq_Temp_Decouple;
//     Process_Data.Ud_Decouple  =  _IQsat(Process_Data.Ud_Decouple, pid_id.param.OutMax, pid_id.param.OutMin);
//     Process_Data.Uq_Decouple  =  _IQsat(Process_Data.Uq_Decouple, pid_iq.param.OutMax, pid_iq.param.OutMin);
}

void Virtual_Impedance_CV_Decouple(_iq *Ud,_iq *Uq)   /*���������迹��ѹǰ��+��ʸ���������*/
{
    _iq  Ud_Temp_CV=0, Uq_Temp_CV=0;
    _iq  Ud_Temp_VI=0, Uq_Temp_VI=0;

    //��ʸ������ǰ����ѹ����
    //ע��DQ���ѹǰ����ķ����෴
    Ud_Temp_CV = _IQmpy(_IQmpy(pid_iq.param.Kp, qep1.Speed_We),pid_iq.data.Err);
    Uq_Temp_CV = _IQmpy(_IQmpy(pid_id.param.Kp, qep1.Speed_We),pid_id.data.Err);
    Ud_Temp_CV = -_IQmpy(_IQmpy(Ud_Temp_CV ,1+Process_Data.VI_CV_c2), T_Sample);
    Uq_Temp_CV =  _IQmpy(_IQmpy(Uq_Temp_CV ,1+Process_Data.VI_CV_c2), T_Sample);
    Process_Data.Ud_CV_Decouple =  Process_Data.Ud_CV_Decouple + Ud_Temp_CV;
    Process_Data.Uq_CV_Decouple =  Process_Data.Uq_CV_Decouple + Uq_Temp_CV;

    //ע�����жϳ��������������ѹʩ����һ�����ţ�ע��˴���������
    //�����迹��Чģ��ǰ����ѹ����
    Ud_Temp_VI = _IQmpy(Ra_IQ, Process_Data.Id_Cal) - _IQmpy(_IQmpy(Laq_IQ, qep1.Speed_We),Process_Data.Iq_Cal);
    Uq_Temp_VI = _IQmpy(Ra_IQ, Process_Data.Iq_Cal) + _IQmpy(_IQmpy(Lad_IQ, qep1.Speed_We),Process_Data.Id_Cal);
    Process_Data.Ud_VI_Decouple = -Ud_Temp_VI;
    Process_Data.Uq_VI_Decouple = -Uq_Temp_VI;

    //ע�����жϳ��������������ѹʩ����һ�����ţ�
    //20191219 ���жϳ����еĽ�����봦�ĸ����Ѿ��ı�Ϊ���ţ�
    *Ud = Process_Data.Ud_CV_Decouple + Process_Data.Ud_VI_Decouple;
    *Uq = Process_Data.Uq_CV_Decouple + Process_Data.Uq_VI_Decouple;
}

//---------------------------------------------------------------------------------------------------------
