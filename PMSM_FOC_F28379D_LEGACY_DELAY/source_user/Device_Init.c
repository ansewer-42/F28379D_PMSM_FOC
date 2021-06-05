/*
 * Device_Init.c
 * ----------------------------------------------------------------------------------
 * Description
 * This file is used for the system and gpio register initialization;
 * ----------------------------------------------------------------------------------
 *  Created on: 2020��3��30��
 *      Author: YY
 * ----------------------------------------------------------------------------------
 */

#include "Device_Init.h"

//***********************************************************************************
// System function initialization, system control module, PIE control module, PIE vector tables;
//------------------------------------------------------------------------------------
void System_Init(void)
{
	// Initialize System Control:
	// WatchDog, memcpy&InitFlash(if _FLASH defined), ADC_cal,
	// PLL, enable Peripheral Clocks
	// This example function is found in the F2837xD_SysCtrl.c file.
	InitSysCtrl();

	// Only used if running from FLASH
	// Note that the variable FLASH is defined by the compiler
	#ifdef _FLASH
	// Call Flash Initialization to setup flash waitstates
	// This function must reside in RAM
		InitFlash();	// Call the flash wrapper init function
	#endif //(FLASH)

	// Clear all interrupts and initialize PIE vector table:
	// Disable CPU interrupts
	DINT;

	// Initialize the PIE control registers to their default state.
	// The default state is all PIE interrupts disabled and flags
	// are cleared.
	// This function is found in the F28M3Xx_PieCtrl.c file.
	InitPieCtrl();

	IER = 0x0000;
	IFR = 0x0000;

	// Disable CPU interrupts and clear all CPU interrupt flags:

	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR).
	// This will populate the entire table, even if the interrupt
	// is not used in this example.  This is useful for debug purposes.
	// The shell ISR routines are found in F28M3Xx_DefaultIsr.c.
	// This function is found in F28M3Xx_PieVect.c.
	InitPieVectTable();
}


//***********************************************************************************
// GPIO pins mux functions and direction initialization.
//------------------------------------------------------------------------------------
void GPIO_Init(void)
{
	EALLOW;

	// EPWM1/2/3_A/B�ź���� (motor1)
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B
    GpioCtrlRegs.GPAPUD.bit.GPIO0  = 0;   // 0,Enable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1  = 0;   // 0,Enable pull-up on GPIO1 (EPWM1B)
    GpioCtrlRegs.GPAPUD.bit.GPIO2  = 0;   // 0,Enable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO3  = 0;   // 0,Enable pull-up on GPIO3 (EPWM2B)
    GpioCtrlRegs.GPAPUD.bit.GPIO4  = 0;   // 0,Enable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5  = 0;   // 0,Enable pull-up on GPIO5 (EPWM3B)

    //PWMDAC�����۲����,EPWM7/8_A/B
    GpioCtrlRegs.GPEMUX2.bit.GPIO157 = 1;   // Configure GPIO157 as EPWM7A
    GpioCtrlRegs.GPEMUX2.bit.GPIO158 = 1;   // Configure GPIO158 as EPWM7B
    GpioCtrlRegs.GPEMUX2.bit.GPIO159 = 1;   // Configure GPIO159 as EPWM8A
    GpioCtrlRegs.GPFMUX1.bit.GPIO160 = 1;   // Configure GPIO160 as EPWM8B
    GpioCtrlRegs.GPEPUD.bit.GPIO157  = 0;   // 0,Enable pull-up on GPIO157 (EPWM7A)
    GpioCtrlRegs.GPEPUD.bit.GPIO158  = 0;   // 0,Enable pull-up on GPIO158 (EPWM7B)
    GpioCtrlRegs.GPEPUD.bit.GPIO159  = 0;   // 0,Enable pull-up on GPIO159 (EPWM8A)
    GpioCtrlRegs.GPFPUD.bit.GPIO160  = 0;   // 0,Enable pull-up on GPIO160 (EPWM8B)

	//�����ź�����IPM_Error1,�͵�ƽ������Ч (motor1), ͨ��INPUT XBAR����ΪINPUT1,��ΪTZ1���룻
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;	// Configure GPIO22 as GPIO22
	GpioCtrlRegs.GPADIR.bit.GPIO22  = 0;  	// 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
    GpioCtrlRegs.GPAPUD.bit.GPIO22  = 0;    // 0,Enable pull-up on GPIO22
    InputXbarRegs.INPUT1SELECT     = 22;    // Configure GPIO22 as INPUT1 of INPUT X-BAR

	//PWMʹ���ź����(motor1) GpyCESL1-4����ѡ��Ĵ���Ĭ�����õ�CPU1,��CPU1�Ը�GPIO���Ž��в���
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;	// Configure GPIO29 as GPIO29
	GpioCtrlRegs.GPADIR.bit.GPIO29  = 1;  	// 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
	GpioCtrlRegs.GPAPUD.bit.GPIO29  = 0;    // 0,Enable pull-up on GPIO29
	GpioDataRegs.GPASET.bit.GPIO29  = 1;	// �͵�ƽenableʹ����Ч
	GpioCtrlRegs.GPACSEL4.bit.GPIO29 = 0x0000;	//Core CPU1 Selected(default)

	// OUTPUT-XBARģ������,�������ĸ�OPXBARx����
	// GPIO14/OPXBAR3,GPIO15/OPXBAR4, GPIO16/OPXBAR7, GPIO24/OPXBAR1;
	GpioCtrlRegs.GPAGMUX1.bit.GPIO14 = 1;	// Configure GPIO14 Group MUX
	GpioCtrlRegs.GPAGMUX1.bit.GPIO15 = 1;	// Configure GPIO15 Group MUX
	GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 0;	// Configure GPIO16 Group MUX
	GpioCtrlRegs.GPAGMUX2.bit.GPIO24 = 0;	// Configure GPIO24 Group MUX
	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;	// Configure GPIO14 as OUTPUT-XBAR
	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;	// Configure GPIO15 as OUTPUT-XBAR
	GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 3;	// Configure GPIO16 as OUTPUT-XBAR
	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 1;	// Configure GPIO24 as OUTPUT-XBAR
	GpioCtrlRegs.GPADIR.bit.GPIO14  = 1;  	// 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
	GpioCtrlRegs.GPADIR.bit.GPIO15  = 1;  	// 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
	GpioCtrlRegs.GPADIR.bit.GPIO16  = 1;  	// 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
	GpioCtrlRegs.GPADIR.bit.GPIO24  = 1;  	// 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;     // 0,Enable pull-up on GPIO25
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;     // 0,Enable pull-up on GPIO25
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;     // 0,Enable pull-up on GPIO25
    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;     // 0,Enable pull-up on GPIO25

    // motor1: ia1/ib1/ic1 -->  CPMIN 3P/1P/7P
    // ����CMPSS�Ƚ����߼�����������DAC���ֵ��Ϊ�Ƚ�������ֵ,���±Ƚ������ֻҪ��һ���Ǹߵ�ƽ1��������Ϸ�������˿�ͨ�����߼��жϹ����Ƿ�����
    // CMPSSģ�������OUTPUT XBAR������ΪCTRIPOUTH/CTRIPOUTL,�����ePWM XBAR������ΪCTRIPH/CTRIPL;
#if 1
    // GPIO14/OPXBAR3: motor1������������źž����Ƚ������������л��߼�����,CMPSS1~3:CTRIPOUTH OR CTRIPOUTL;
    OutputXbarRegs.OUTPUT3MUX0TO15CFG.bit.MUX0 = 1;  //CMPSS1.CTRIPOUTH_OR_CTRIPOUTL
    OutputXbarRegs.OUTPUT3MUX0TO15CFG.bit.MUX4 = 1;  //CMPSS3.CTRIPOUTH_OR_CTRIPOUTL
    OutputXbarRegs.OUTPUT3MUX0TO15CFG.bit.MUX12 = 1; //CMPSS7.CTRIPOUTH_OR_CTRIPOUTL
    OutputXbarRegs.OUTPUT3MUXENABLE.bit.MUX0  = 1;   //Enable MUX0 to drive the OUTPUT;
    OutputXbarRegs.OUTPUT3MUXENABLE.bit.MUX4  = 1;   //Enable MUX4 to drive the OUTPUT;
    OutputXbarRegs.OUTPUT3MUXENABLE.bit.MUX12 = 1;   //Enable MUX12 to drive the OUTPUT;

    // GPIO15/OPXBAR4: motor2������������źž����Ƚ������������л��߼�����,CMPSS6~8:CTRIPOUTH OR CTRIPOUTL;
    // CMPSSINxP���ſ����������ϵ�ƽ������ģ����벻һ����0��ƽ����˶�Ӧ�����CTRIPOUTH_OR_CTRIPOUTL��һ����1��
    OutputXbarRegs.OUTPUT4MUX0TO15CFG.bit.MUX10 = 1; //CMPSS6.CTRIPOUTH_OR_CTRIPOUTL
    OutputXbarRegs.OUTPUT4MUX0TO15CFG.bit.MUX12 = 1; //CMPSS7.CTRIPOUTH_OR_CTRIPOUTL
    OutputXbarRegs.OUTPUT4MUX0TO15CFG.bit.MUX14 = 1; //CMPSS8.CTRIPOUTH_OR_CTRIPOUTL
    OutputXbarRegs.OUTPUT4MUXENABLE.bit.MUX10 = 1;   //Enable MUX10 to drive the OUTPUT;
    OutputXbarRegs.OUTPUT4MUXENABLE.bit.MUX12 = 1;   //Enable MUX12 to drive the OUTPUT;
    OutputXbarRegs.OUTPUT4MUXENABLE.bit.MUX14 = 1;   //Enable MUX14 to drive the OUTPUT;
#else   //�����ô��룬�ֱ�����۲����±Ƚ��������ֵ��
    OutputXbarRegs.OUTPUT3MUX0TO15CFG.bit.MUX0 = 0; //CTRIPOUTH
    OutputXbarRegs.OUTPUT3MUXENABLE.bit.MUX0 = 1;   //Enable MUX0 to drive the OUTPUT;
    OutputXbarRegs.OUTPUT4MUX0TO15CFG.bit.MUX1 = 0; //CTRIPOUTL
    OutputXbarRegs.OUTPUT4MUXENABLE.bit.MUX1 = 1;   //Enable MUX0 to drive the OUTPUT;
#endif

    // GPIO16/OPXBAR7: INUPUTXBAR1
    OutputXbarRegs.OUTPUT7MUX0TO15CFG.bit.MUX1 = 1;	//INUPUTXBAR1
    OutputXbarRegs.OUTPUT7MUXENABLE.bit.MUX1 = 1;   //Enable MUX0 to drive the OUTPUT;
    // GPIO24/OPXBAR1: ADCSOCAO
    OutputXbarRegs.OUTPUT1MUX0TO15CFG.bit.MUX13= 1; //ADCSOCAO
    OutputXbarRegs.OUTPUT1MUXENABLE.bit.MUX13 = 1;	//Enable MUX0 to drive the OUTPUT;


    // ePWM XBARģ������
    // ADC A2/B2/A4--->ia1/ib1/ic1 (motor1), ADC C2/D2/D0--->ia2/ib2/ic2 (motor2)
    // A2-CMP1P,B2-CMP3P,D0-CMP7P;
    //---------------------------------------------------------------------------
    #if EPWM_XBAR_DC
    // TRIP4: motor1������������źž����Ƚ������������л��߼�����,CMPSS1~3:CTRIPOUTH OR CTRIPOUTL;
    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX0  = 1; //CMPSS1.CTRIPOUTH_OR_CTRIPOUTL
    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX4  = 1; //CMPSS3.CTRIPOUTH_OR_CTRIPOUTL
    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX12 = 1; //CMPSS7.CTRIPOUTH_OR_CTRIPOUTL
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX0  = 1;	 //Enable MUX0 to drive the TRIP4;   //ADC�������գ�CMPSS�����ȷ���������ʱֻ�ų�A2���ŵ�������
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX4  = 1;   //Enable MUX4 to drive the TRIP4;
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX12 = 1;   //Enable MUX12 to drive the TRIP4;

    /****************Code For Debug***********************/
//    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX1 = 1; //INPUTXBAR1,GPIO22
//    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX1 = 1; //Enable MUX1 to drive the TRIP4;
    #endif
    //---------------------------------------------------------------------------

    //QEP1����ź�����(motor1)
    GpioCtrlRegs.GPADIR.bit.GPIO20  = 0;  	// 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
    GpioCtrlRegs.GPADIR.bit.GPIO21  = 0;  	// 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
    GpioCtrlRegs.GPDDIR.bit.GPIO99  = 0;  	// 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
    GpioCtrlRegs.GPAMUX2.bit.GPIO20  = 1;   // Configure GPIO20 as EQEP1A
    GpioCtrlRegs.GPAMUX2.bit.GPIO21  = 1;   // Configure GPIO21 as EQEP1B
    GpioCtrlRegs.GPDGMUX1.bit.GPIO99 = 1;   // Configure GPIO99 as EQEP1I (Don't forget configuring the GPxFMUXy register)
    GpioCtrlRegs.GPDMUX1.bit.GPIO99  = 1;   // Configure GPIO99 as EQEP1I
    GpioCtrlRegs.GPAPUD.bit.GPIO20   = 0;   // Enable pull-up on GPIO20 (EQEP1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO21   = 0;   // Enable pull-up on GPIO21 (EQEP1B)
    GpioCtrlRegs.GPDPUD.bit.GPIO99   = 0;   // Enable pull-up on GPIO23 (EQEP1I)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 0;   // Sync to SYSCLKOUT GPIO20 (EQEP1A)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 0;   // Sync to SYSCLKOUT GPIO21 (EQEP1B)
    GpioCtrlRegs.GPDQSEL1.bit.GPIO99 = 0;   // Sync to SYSCLKOUT GPIO99 (EQEP1I)

	//UVW�����ź����� (motor1)
    #if (POSITION_CONNECTOR == BOTTOM_BOARD)
    {
        GpioCtrlRegs.GPBMUX1.bit.GPIO32  = 0;   // Configure GPIO32  as GPIO32
        GpioCtrlRegs.GPCMUX1.bit.GPIO67  = 0;   // Configure GPIO67  as GPIO67
        GpioCtrlRegs.GPDMUX1.bit.GPIO111 = 0;   // Configure GPIO111 as GPIO111
        GpioCtrlRegs.GPBDIR.bit.GPIO32   = 0;   // 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
        GpioCtrlRegs.GPCDIR.bit.GPIO67   = 0;   // 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
        GpioCtrlRegs.GPDDIR.bit.GPIO111  = 0;   // 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
    }
    #else
    {
        GpioCtrlRegs.GPCMUX2.bit.GPIO95  = 0;   // Configure GPIO32 as GPIO24
        GpioCtrlRegs.GPDMUX1.bit.GPIO97  = 0;   // Configure GPIO67 as GPIO25
        GpioCtrlRegs.GPCMUX2.bit.GPIO94  = 0;   // Configure GPIO111 as GPIO26
        GpioCtrlRegs.GPCDIR.bit.GPIO95   = 0;   // 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
        GpioCtrlRegs.GPDDIR.bit.GPIO97   = 0;   // 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
        GpioCtrlRegs.GPCDIR.bit.GPIO94   = 0;   // 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
    }
    #endif

    // 4��������������״̬���룺S1~S4
    GpioCtrlRegs.GPEMUX1.bit.GPIO131  = 0;    // Configure GPIO131  as GPIO131
    GpioCtrlRegs.GPEMUX1.bit.GPIO130  = 0;    // Configure GPIO130  as GPIO130
    GpioCtrlRegs.GPAMUX2.bit.GPIO27   = 0;    // Configure GPIO27 as GPIO27
    GpioCtrlRegs.GPAMUX2.bit.GPIO26   = 0;    // Configure GPIO26 as GPIO26
    GpioCtrlRegs.GPEDIR.bit.GPIO131   = 0;    // 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
    GpioCtrlRegs.GPEDIR.bit.GPIO130   = 0;    // 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
    GpioCtrlRegs.GPADIR.bit.GPIO27    = 0;    // 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
    GpioCtrlRegs.GPADIR.bit.GPIO26    = 0;    // 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��

	//LED�����ź��������D9(RED)
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;	// Configure GPIO34 as GPIO34
	GpioCtrlRegs.GPBDIR.bit.GPIO34  = 1;  	// 1=OUTput, 0=INput
	GpioDataRegs.GPBCLEAR.bit.GPIO34  = 1;	// �͵�ƽ����LED
	//LED�����ź��������D10(BLUE)
	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;	// Configure GPIO31 as GPIO31
	GpioCtrlRegs.GPADIR.bit.GPIO31  = 1;  	// 1=OUTput, 0=INput
	GpioDataRegs.GPACLEAR.bit.GPIO31  = 1;	// �͵�ƽ����LED

	//Boot�����������GetMode��boot to flash,
	GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 0;	// Configure GPIO72 as Boot pins;
	GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 0;	// Configure GPIO84 as Boot pins;
	GpioCtrlRegs.GPCPUD.bit.GPIO72  = 0;	// Enable pull-up on GPIO72
	GpioCtrlRegs.GPCPUD.bit.GPIO84  = 0;	// Enable pull-up on GPIO84
	GpioCtrlRegs.GPCDIR.bit.GPIO72  = 1;  	// 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
	GpioCtrlRegs.GPCDIR.bit.GPIO84  = 1;  	// 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
	GpioDataRegs.GPCSET.bit.GPIO72  = 1;	// �͵�ƽ����LED
	GpioDataRegs.GPCSET.bit.GPIO84  = 1;	// �͵�ƽ����LED

	//GPIO������������ڹ۲��������ʱ��
    GpioCtrlRegs.GPDMUX2.bit.GPIO122  = 0;    // Configure GPIO122  as GPIO122
    GpioCtrlRegs.GPDDIR.bit.GPIO122   = 1;    // 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
    GpioCtrlRegs.GPDMUX2.bit.GPIO123  = 0;    // Configure GPIO123  as GPIO123
    GpioCtrlRegs.GPDDIR.bit.GPIO123   = 1;    // 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��

    // GPIO�������룬���ڽ���motor2�Ľ�Ծ�ź����ͬ���ź�,��������������޶����ܣ�
    GpioCtrlRegs.GPDMUX2.bit.GPIO124  = 0;    // Configure GPIO123  as GPIO123
    GpioCtrlRegs.GPDDIR.bit.GPIO124   = 0;    // 1=OUTput, 0=INput��Ĭ��Ϊ0���뷽��
//    GpioCtrlRegs.GPDQSEL2.bit.GPIO124 = 2;   //GPIO124,00,ͬ��;01,3�β�����;10,6�β�����;11,�첽ģʽ��
//    GpioCtrlRegs.GPDCTRL.bit.QUALPRD3 =0xFF;//���������޶�

	EDIS;
}
