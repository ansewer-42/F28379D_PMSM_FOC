/*
 * EQEP_Init.c
 *
 *  Created on: 2020年6月6日
 *      Author: PMSM
 */

#include "EQEP_Init.h"

void EQEP_Setup1(QEP *p)
{
    p->Device_Index = 1;       //外设在芯片内的标号，QEP1位1，QEP2为2;
    p->Encoder_N  = 4000;
    p->Utimer_PRD = 2000000;   // 暂时不使能定时器Utimer模块，下列相关配置缺省；
    p->POSCNT_Offset = 761;    //转子起始点偏置值实际转子位置需要加上该偏置;
    p->Theta_Offset  = 0.38055555; //转子起始点偏置值对应的电角度偏置值;

    EQep1Regs.QPOSMAX = p->Encoder_N-1;         // 1000槽的编码器*4-1=3999
    EQep1Regs.QUPRD   = p->Utimer_PRD;          // Unit Timer for 100Hz at 200MHz SYSCLKOUT,2000000;
    EQep1Regs.QDECCTL.bit.QSRC      =00;        // Quadrature Count Mode (freq. measurement)
    EQep1Regs.QEPCTL.bit.FREE_SOFT  =2;         // Unit Timer不被看门狗中断影响
    EQep1Regs.QEPCTL.bit.PCRM       =00;        // QPOSCNT reset on the maximum position
    EQep1Regs.QEPCTL.bit.UTE        =0;         // Unit Timer Enable  定时器基准单元不使能，不适用基准定时器，使用CPU读来锁存分数个脉冲位置；
    EQep1Regs.QEPCTL.bit.QCLM       =0;         // 当CPU read的时候  数据保存到QPOSLAT, QCTMRLAT and QCPRDLAT
    EQep1Regs.QEPCTL.bit.QPEN       =1;         // QEP enable

    //对于CAP时钟位CCPS的应用，高速段CCPS越小越好，UPPS越大越好，即CAP时钟频率越高越好;
    //低速段CCPS保证定时器的周期大于一个脉冲的周期时长，CCPS相比高速段要增大，此时UPPS越小越好，
    EQep1Regs.QCAPCTL.bit.UPPS  =0;             // 1/1 for unit position
    EQep1Regs.QCAPCTL.bit.CCPS  =0;             // 1/1 for CAP clock  at 200MHz SYSCLKOUT,要测量的速度越低，则捕获时钟频率越低
    EQep1Regs.QCAPCTL.bit.CEN   =1;             // QEP Capture Enable
    EQep1Regs.QEPCTL.bit.SWI    =1;
}


void EQEP_Angle_Cal(QEP *p)
{
    Uint16 k;

    p->Rotation_DIR = EQep1Regs.QEPSTS.bit.QDF;
    p->POSCNT_Temp = EQep1Regs.QPOSCNT;
    p->Theta_Mech  = p->POSCNT_Temp*INV_ENCODER_N;
    p->Theta_Elec  = p->Theta_Mech*POLE_PAIRS+p->Theta_Offset;
    for(k=1;k<POLE_PAIRS+1;k++)
    {
        if( (p->Theta_Elec>k) && (p->Theta_Elec<=(k+1))  )
        {
            p->Theta_Elec = p->Theta_Elec-k;
        }
    }

    //下列暂且注释，不需要将（0-1）标幺值化为实际弧度值，因为_IQsinPU(A)=sin((A)*6.283185307)
//  //将电角度标幺化值（0~1）转换到弧度制范围（0~2*pi）,注释为通用宏定义公式，前面未优化表达式
//  p->Theta_Elec = pi_2*p->Theta_Elec;  //p->Theta_Elec = 2*pi*p->Theta_Elec;
}

void EQEP_Speed_Cal(QEP *p)
{
    float Temp1,Temp2;

    // 20200520核算过，这里用9表示1ms的逻辑是正确的，仔细推演，每两次Spped_Cal_CNT=N之间间隔时间为N+1个PWM中断周期
    // 20200520修改代码功能，无论开关周期如何变化，以1ms为周期进行转速计算，计划最低开关频率为1kHz
    // 判断条件fs=10kHz,N=9, fs=5kHz,N=PWM_ISR_FRQ*0.001-1，需要考虑单采样与双采样的区别
    if( p->Speed_Cal_CNT<(ISR_FREQUENCY-1) )        //1ms计算一次转速，改变转速计算周期时，转速计算参数也要改变，最低适用到ISR_FREQUENCY = 1;
//  if( p->Speed_Cal_CNT<9 )        //1ms计算一次转速，改变转速计算周期时，转速计算参数也要改变
    {
        p->Speed_Cal_CNT++;
        p->Speed_CNT_ON = 0;        //转速调节器标志位清零，本周期不进行转速计算与转速调节器计算；
    }
    else
    {
//      GpioDataRegs.GPASET.bit.GPIO6 = 1;  //输出GPIO用来观测转速计算周期
        p->Speed_Cal_CNT = 0;
        p->Speed_CNT_ON = 1;        //转速调节器标志位置位，本周期进行转速计算与转速调节器计算；


        // UPEVNT触发的QCTMR值锁存到周期寄存器QCPRD中，在CPU Read的时候当前QCPRD寄存器值锁存到QCPRDLAT寄存器中，捕获定时器值QCTMR锁存到QCTMRLAT寄存器中；
        // 一般情况下，此时锁存的值为上一个UPEVNT触发的值，相邻两个UPEVNT脉冲的QCPRD值可视为不变，用当前QCTMR的值除以上个QCLK周期的值，可以计算出分数个QCLK数量；
        Temp1 = EQep1Regs.QCTMRLAT;
        Temp2 = EQep1Regs.QCPRDLAT;
        p->POSCNT_New = p->POSCNT_Temp;
        if(Temp2==0)
        {
            p->POSCNT_Fractional = 0;
        }
        else
        {
            if(Temp1<Temp2)
            {
                p->POSCNT_Fractional = Temp1/Temp2;
            }
            else
            {
                p->POSCNT_Fractional = 1;    //减速过程可能出现PRD逐渐增大，但不会变化太大
            }
        }

        p->POSCNT_New_Real  = p->POSCNT_New + p->POSCNT_Fractional;
        p->POSCNT_Diff_New  = p->POSCNT_New_Real - p->POSCNT_Old_Real;
        if(p->POSCNT_Diff_New>500)          //3000rpm下1ms最大有200个脉冲，选择500为裕度
        {
            p->POSCNT_Diff_New -= p->Encoder_N;
        }
        else if(p->POSCNT_Diff_New<-3500)
        {
            p->POSCNT_Diff_New += p->Encoder_N;
        }

        //3000rpm=50rps=0.05rpms---->0.05*4000=200,3000/200=15 rpm/pulse;
        //we=Speed_PU*Base_Speed*2*pi/60*4,Speed_We(PU)=we/Base_Speed_WE=Speed_PU
        p->Speed_Real = p->POSCNT_Diff_New*15;
        p->Speed_PU   = p->Filter0_Speed*p->Speed_Real + p->Filter1_Speed*p->Speed_PU;
        p->Speed_We   = p->Speed_PU*0.20944;   //p->Speed_We = POLE_PAIRS*2_pi*p->Speed_PU/60;

        //----------------------------------------------------------------
        p->POSCNT_Old = p->POSCNT_New;
        p->POSCNT_Old_Real = p->POSCNT_New_Real;
        p->POSCNT_Diff_Old = p->POSCNT_Diff_New;

//      GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;     //输出GPIO用来观测转速计算周期
    }

}

void Hall_Angle_Cal(QEP *p)
{
    int16 Hall_Temp=0;

    Hall_Temp = HALL_U1*4+HALL_V1*2+HALL_W1;
    switch((long)Hall_Temp)
    {
        case 1:
                p->Theta_Elec = 0;          //001,对应转子位置为电角度330-30度
                break;
        case 2:
                p->Theta_Elec = 0.66667;    //010,对应转子位置为电角度210-270度
                break;
        case 3:
                p->Theta_Elec = 0.83333;    //011,对应转子位置为电角度270-330度
                break;
        case 4:
                p->Theta_Elec = 0.33333;    //100,对应转子位置为电角度90-150度
                break;
        case 5:
                p->Theta_Elec = 0.166;      //101,对应转子位置为电角度30-90度
                break;
        case 6:
                p->Theta_Elec = 0.5;        //110,对应转子位置为电角度150-210度
                break;
        default:
                break;
    }
}
