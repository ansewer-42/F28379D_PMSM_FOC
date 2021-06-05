/*
 * Ramp_Function.h
 *
 *  Created on: 2019Äê3ÔÂ24ÈÕ
 *      Author: YY
 */

#ifndef SOURCE_RAMP_FUNCTION_H_
#define SOURCE_RAMP_FUNCTION_H_

#include "F28x_Project.h"
#ifndef MATH_TYPE
#define MATH_TYPE   1
#endif
#include "IQmathLib.h"

typedef struct {
					 _iq    TargetValue; 	// Input: Target input (pu)
					 Uint16 RampDelayMax;	// Parameter: Maximum delay rate (Q0) - independently with global Q
					 _iq    RampLowLimit;	// Parameter: Minimum limit (pu)
					 _iq    RampHighLimit;	// Parameter: Maximum limit (pu)
					 Uint16 RampDelayCount; // Variable: Incremental delay (Q0) - independently with global Q
					 _iq    SetpointValue;	// Output: Target output (pu)
					 _iq    Gain;	// Output: Target output (pu)
					 Uint16 EqualFlag;		// Output: Flag output (Q0) - independently with global Q
					 void (*calc)();	  	// Pointer to calculation function
				 } RMPCNTL;

typedef RMPCNTL *RMPCNTL_handle;
#define RMPCNTL_DEFAULTS {  0, 1, _IQ(-1), _IQ(1), 0, 0, 0, 0,    	\
							(void (*)(long)) RAMP_FUNC 				}


extern void RAMP_FUNC(RMPCNTL *v);

#endif /* SOURCE_RAMP_FUNCTION_H_ */
