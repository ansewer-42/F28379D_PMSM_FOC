/*
 * Coordinate_Trans.c
 *
 *  Created on: 2019Äê3ÔÂ23ÈÕ
 *      Author: YY
 */
#include "Coordinate_Trans.h"

//*****************************************************************************************
void CLARKE_TRANS(CLARKE *v)
{
	v->Alpha = v->As;
	v->Beta = _IQmpy((v->As +_IQmpy2(v->Bs)),_IQ(ONEbySQRT3));
}

//-----------------------------------------------------------------------------------------
void PARK_TRANS(PARK *v)
{
	v->Ds = _IQmpy(v->Alpha,v->Cosine) + _IQmpy(v->Beta,v->Sine);
    v->Qs = _IQmpy(v->Beta,v->Cosine) - _IQmpy(v->Alpha,v->Sine);
}

//-----------------------------------------------------------------------------------------
void IPARK_TRANS(IPARK *v)
{
	v->Alpha = _IQmpy(v->Ds,v->Cosine) - _IQmpy(v->Qs,v->Sine);
	v->Beta  = _IQmpy(v->Qs,v->Cosine) + _IQmpy(v->Ds,v->Sine);
}

//-----------------------------------------------------------------------------------------
