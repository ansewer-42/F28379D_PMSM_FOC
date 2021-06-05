/*
 * Ramp_Function.c
 *
 *  Created on: 2019Äê3ÔÂ24ÈÕ
 *      Author: YY
 */


#include "Ramp_Function.h"

void RAMP_FUNC(RMPCNTL *v)
{
    _iq tmp;

    tmp = v->TargetValue - v->SetpointValue;
	if (_IQabs(tmp) > v->Gain  ) 					//_IQ(0.0000305)
	{
		v->RampDelayCount += 1;
		if (v->RampDelayCount >= v->RampDelayMax)
		{
			if (v->TargetValue >= v->SetpointValue)
			{
				v->SetpointValue += v->Gain;        // 0.0000305 is resolution of Q15
//				if (v->SetpointValue > v->RampHighLimit)
//					v->SetpointValue = v->RampHighLimit;
				v->RampDelayCount = 0;
			}
			else
			{
				v->SetpointValue -= v->Gain;        // 0.0000305 is resolution of Q15
//				if (v->SetpointValue < v->RampLowLimit)
//					v->SetpointValue = v->RampLowLimit;
				v->RampDelayCount = 0;
			}
		}
	}
	else
	{	//v->EqualFlag = 0x7FFFFFFF;
		v->SetpointValue=v->TargetValue ;
	}
}
