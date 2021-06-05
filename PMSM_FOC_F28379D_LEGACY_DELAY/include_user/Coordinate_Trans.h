/*
 * Coordinate_Trans.h
 *
 *  Created on: 2019Äê3ÔÂ23ÈÕ
 *      Author: YY
 */

#ifndef COORDINATE_TRANS_H_
#define COORDINATE_TRANS_H_

#ifndef MATH_TYPE
#define MATH_TYPE   1
#endif
#include "IQmathLib.h"

#define  ONEbySQRT3   0.57735026918963    /* 1/sqrt(3) */


//**************************************************************
typedef struct {  _iq  As;  		// Input: phase-a stator variable
				  _iq  Bs;			// Input: phase-b stator variable
				  _iq  Cs;			// Input: phase-c stator variable
				  _iq  Alpha;		// Output: stationary d-axis stator variable
				  _iq  Beta;		// Output: stationary q-axis stator variable
		 	 	} CLARKE;

#define CLARK_DEFAULTS { 0,0,0,0,0 }
//----------------------------------------------------------------------------------------
typedef struct {  _iq  Alpha;  		// Input: stationary d-axis stator variable
				  _iq  Beta;	 	// Input: stationary q-axis stator variable
				  _iq  Angle;		// Input: rotating angle (pu)
				  _iq  Ds;			// Output: rotating d-axis stator variable
				  _iq  Qs;			// Output: rotating q-axis stator variable
				  _iq  Sine;
				  _iq  Cosine;
			   } PARK;

#define PARK_DEFAULTS { 0,0,0,0,0,0,0 }
//----------------------------------------------------------------------------------------
typedef struct {  _iq  Alpha;  		// Output: stationary d-axis stator variable
				  _iq  Beta;		// Output: stationary q-axis stator variable
				  _iq  Angle;		// Input: rotating angle (pu)
				  _iq  Ds;			// Input: rotating d-axis stator variable
				  _iq  Qs;			// Input: rotating q-axis stator variable
				  _iq  Sine;		// Input: Sine term
				  _iq  Cosine;		// Input: Cosine term
		 	   } IPARK;

#define IPARK_DEFAULTS { 0,0,0,0,0,0,0 }
//---------------------------------------------------------------------------------------------

extern void CLARKE_TRANS(CLARKE *v);
extern void PARK_TRANS(PARK *v);
extern void IPARK_TRANS(IPARK *v);

#endif /* COORDINATE_TRANS_H_ */
