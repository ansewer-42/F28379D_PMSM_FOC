/*
 * SVPWM_Gen.h
 *
 *  Created on: 2019Äê3ÔÂ23ÈÕ
 *      Author: YY
 */

#ifndef SOURCE_SVPWM_GEN_H_
#define SOURCE_SVPWM_GEN_H_

#include "F28x_Project.h"
#ifndef MATH_TYPE
#define MATH_TYPE   1
#endif
#include "IQmathLib.h"

typedef struct 	{
					_iq  Ualpha; 			// Input: reference alfa-axis phase voltage
					_iq  Ubeta;           	// Input: reference beta-axis phase voltage
					_iq  UdcBus;

					_iq  Ta;				// Output: reference phase-a switching function
					_iq  Tb;				// Output: reference phase-b switching function
					_iq  Tc;				// Output: reference phase-c switching function
				} SVPWM;

typedef SVPWM *SVPWM_handle;
#define SVPWM_DEFAULTS	{ 0,0,0, 0,0,0 }


extern void SVPWM_GEN(SVPWM *v);

#endif /* SOURCE_SVPWM_GEN_H_ */
