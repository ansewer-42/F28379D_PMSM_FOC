/*
 * SVPWM_Gen.c
 *
 *  Created on: 2019年3月23日
 *      Author: YY
 */

#include "SVPWM_Gen.h"

void SVPWM_GEN(SVPWM *v)
{
	Uint32 N = 0;  // N is treated as Q0 - independently with global Q
	_iq Vref1,Vref2,Vref3;
	_iq Va,Vb,Vc;
	_iq t1,t2;
	_iq duty1,duty2,duty3;
	_iq temp2;

	Vref1=v->Ubeta;
	Vref2=_IQmpy((-v->Ubeta+_IQmpy(v->Ualpha,_IQ(1.732051))),_IQ(0.5));
	Vref3=_IQmpy((-v->Ubeta-_IQmpy(v->Ualpha,_IQ(1.732051))),_IQ(0.5));

	if(Vref1>0)
		N = 1;
	if(Vref2>0)
		N = N + 2;
	if(Vref3>0)
		N = N + 4;

	Va = _IQmpy(_IQ(1.7320508),v->Ubeta);						         // X = Va
	Vb = _IQmpy(_IQ(0.8660254),v->Ubeta) + _IQmpy(_IQ(1.5),v->Ualpha);   // Y = Vb
	Vc = _IQmpy(_IQ(0.8660254),v->Ubeta) - _IQmpy(_IQ(1.5),v->Ualpha);   // Z = Vc
	//	Va = _IQdiv(Va,v->UdcBus);
	//	Vb = _IQdiv(Vb,v->UdcBus);
	//	Vc = _IQdiv(Vc,v->UdcBus);
	temp2 = _IQdiv(_IQ(1),v->UdcBus);
	Va = _IQmpy(Va,temp2);
	Vb = _IQmpy(Vb,temp2);
	Vc = _IQmpy(Vc,temp2);

	if (N==0)  // Sector 0: this is special case for (Ualpha,Ubeta) = (0,0)
	{
		v->Ta = _IQ(0.5);
		v->Tb = _IQ(0.5);
		v->Tc = _IQ(0.5);
	}
	if (N==1)  // Sector 2: t1=Z and t2=Y (abc ---> Tb,Ta,Tc)
	{
		t1 = Vc;
		t2 = Vb;
	}
	else if (N==2)  // Sector 6: t1=Y and t2=-X (abc ---> Ta,Tc,Tb)
	{
		t1 = Vb;
		t2 = -Va;
	}
	else if (N==3)  // Sector 1: t1=-Z and t2=X (abc ---> Ta,Tb,Tc)
	{
		t1 = -Vc;
		t2 = Va;
	}
	else if (N==4)  // Sector 4: t1=-X and t2=Z (abc ---> Tc,Tb,Ta)
	{
		t1 = -Va;
		t2 = Vc;
	}
	else if (N==5)  // Sector 3: t1=X and t2=-Y (abc ---> Tb,Tc,Ta)
	{
		t1 = Va;
		t2 = -Vb;
	}
	else if (N==6)  // Sector 5: t1=-Y and t2=-Z (abc ---> Tc,Ta,Tb)
	{
		t1 = -Vb;
		t2 = -Vc;
	}

	if(t1+t2>_IQ(1))
	{
	//		t1= _IQdiv(t1,t1+t2);
	//		t2= _IQdiv(t2,t1+t2);
		temp2= _IQdiv(_IQ(1),t1+t2);
		t1= _IQmpy(t1,temp2);
		t1= _IQmpy(t1,temp2);
	}

	//各扇区的8段波形排列顺序为T0/4,t1/2,t2/2,T7/4,T7/4,t2/2,t1/2,T0/4
	//通过上述t1,t2根据扇区号赋不同值，区分不同扇区的t1,t2时间段为X,Y,Z哪个数据
	//sector1(N=3)	t1=T4=-Z,t2=T6= X,
	//sector2(N=1)	t1=T2= Z,t2=T6= Y,
	//sector3(N=5)  t1=T2= X,t2=T3=-Y,
	//sector4(N=4)  t1=T1=-X,t2=T3= Z,
	//sector5(N=6)	t1=T1=-Y,t2=T5=-Z,
	//sector6(N=2)	t1=T4= Y,t2=T5=-X,
	duty1 = _IQmpy(_IQ(0.5),(_IQ(1)-t1-t2));  //T7/4+T7/4
	duty2 = duty1+t2;				 		  //T7/4+T7/4+t2/2+t2/2
	duty3 = duty2+t1;						  //T7/4+T7/4+t2/2+t2/2+t1/2+t1/2

	if (N==1) 		 // Sector 2
	{
		v->Ta = duty2;
		v->Tb = duty3;
		v->Tc = duty1;
	}
	else if (N==2) 	// Sector 6
	{
		v->Ta = duty3;
		v->Tb = duty1;
		v->Tc = duty2;
	}
	else if (N==3)  // Sector 1
	{
		v->Ta = duty3;
		v->Tb = duty2;
		v->Tc = duty1;
	}
	else if (N==4) // Sector 4
	{
		v->Ta = duty1;
		v->Tb = duty2;
		v->Tc = duty3;
	}
	else if (N==5) // Sector 3
	{
		v->Ta = duty1;
		v->Tb = duty3;
		v->Tc = duty2;
	}
	else if (N==6) // Sector 5
	{
		v->Ta = duty2;
		v->Tb = duty1;
		v->Tc = duty3;
	}

	// Convert the unsigned GLOBAL_Q format (ranged (0,1)) -> signed GLOBAL_Q format (ranged (-1,1))
//	v->Ta = _IQmpy(_IQ(2.0),v->Ta-_IQ(0.5));
//	v->Tb = _IQmpy(_IQ(2.0),v->Tb-_IQ(0.5));
//	v->Tc = _IQmpy(_IQ(2.0),v->Tc-_IQ(0.5));
}
