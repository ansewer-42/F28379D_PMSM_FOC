/*
 * PMSM_FOC_Main.h
 * ----------------------------------------------------------------------------------
 * Description
 * This file is used for head files include;
 * ----------------------------------------------------------------------------------
 *  Created on: 2020Äê3ÔÂ28ÈÕ
 *      Author: YY
 * ----------------------------------------------------------------------------------
 */

#ifndef INCLUDE_USER_PMSM_FOC_MAIN_H_
#define INCLUDE_USER_PMSM_FOC_MAIN_H_


/*-------------------------------------------------------------------------------
Include project specific include files.
-------------------------------------------------------------------------------*/

//****************************************************************************
// Include files from device_support driver libraries.
//----------------------------------------------------------------------------
#include "F28x_Project.h"
#include "f2837xbmsk.h"

//****************************************************************************
// Include files from motor control and arithmetic support libraries.
//----------------------------------------------------------------------------
// define math type as float(1)
#define   MATH_TYPE      1
#include "IQmathLib.h"
#include "math.h"
#include "stdio.h"
#include "c28x_fpu_fastrts.h"

//****************************************************************************
// Include files user self-defined.
//----------------------------------------------------------------------------
#include "Macro_Predefine.h"

#include "Device_Init.h"
#include "EPWM_Init.h"
#include "DAC_Init.h"
#include "ADC_Init.h"
#include "EQEP_Init.h"

#include "SVPWM_Gen.h"
#include "PID_CONTROLLER.h"
#include "Coordinate_Trans.h"
#include "Ramp_Function.h"

#include "Global_Functions_Variables_Call.h"

#endif /* INCLUDE_USER_PMSM_FOC_MAIN_H_ */
