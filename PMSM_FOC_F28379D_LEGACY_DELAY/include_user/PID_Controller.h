/*
 * PID_Controller.h
 *
 *  Created on: 2019Äê3ÔÂ23ÈÕ
 *      Author: YY
 */

#ifndef SOURCE_PID_CONTROLLER_H_
#define SOURCE_PID_CONTROLLER_H_

#ifndef MATH_TYPE
#define MATH_TYPE   1
#endif
#include "math.h"
#include "IQmathLib.h"

//*************************************************************************************************
typedef struct {  float  Ref;   			// Input: Reference input
				  float  Fdb;   			// Input: Feedback input
				  float  Out;   			// Output: PID output
		 	 	} PID_TERMINALS;
//-----------------------------------------------------------------------------
typedef struct {
				  float  Kp;				// Parameter: Proportional gain
				  float  Ki;			    // Parameter: Integral gain
				  float  Kc;		     	// Parameter: Integral correction gain
				  float  Kd; 		        // Parameter: Derivative gain

				  float  OutMax;		    // Parameter: Maximum output
				  float  OutMin;	    	// Parameter: Minimum output
				  float  UiMax;				// Parameter: Maximum output
				  float  UiMin;				// Parameter: Minimum output
				  float  UpMax;				// Parameter: Maximum output
				  float  UpMin;				// Parameter: Minimum output

				  float  Err_Tolerance;		// Parameter: Error tolerance that add the integral action;
				} PID_PARAMETERS;
//-----------------------------------------------------------------------------------------------
typedef struct {
				  float  Err;				// Variable: Error
				  float  Up;				// Variable: Proportional output
				  float  Ui;				// Variable: Integral output
				  float  Ud;				// Variable: Derivative output
				  float  OutPreSat; 		// Variable: Pre-saturated output
				  float  SatErr;			// Variable: Saturated difference
				  float  Up1;		   	    // History: Previous proportional output
				} PID_DATA;
//-----------------------------------------------------------------------------------------------

typedef struct {  PID_TERMINALS	 term;
				  PID_PARAMETERS param;
				  PID_DATA		 data;
				} PID_CONTROLLER;

typedef PID_CONTROLLER *PID_CONTROLLER_handle;

#define PID_CONTROLLER_DEFAULTS { {0,0,0} , {0,0,0,0, 0,0,0,0,0,0, 0} , {0,0,0,0,0,0,0} }
//-----------------------------------------------------------------------------------------------
extern void PID_POSITIONAL(PID_CONTROLLER_handle);
extern void PID_POSITIONAL2(PID_CONTROLLER_handle);
extern void PID_INCREMENTAL(PID_CONTROLLER_handle);
extern void PID_INTEGRAL_SEPARATED(PID_CONTROLLER_handle);


#endif /* SOURCE_PID_CONTROLLER_H_ */
