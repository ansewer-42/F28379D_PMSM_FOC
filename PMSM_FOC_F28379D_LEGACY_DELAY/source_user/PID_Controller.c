/*
 * PID_Controller.c
 *
 *  Created on: 2019年3月23日
 *      Author: YY
 */

#include "PID_Controller.h"

//***********************************************************************************************
/*位置式PID算法，实际Ki*=_IQmpy(v->param.Kp,v->param.Ki); */
void PID_POSITIONAL(PID_CONTROLLER *v)
{
	/* Compute and Saturate the input error */
	v->data.Err = v->term.Ref - v->term.Fdb;

	/* Compute and Saturate the proportional output */
	v->data.Up = _IQmpy(v->param.Kp,v->data.Err);
//	v->data.Up = _IQsat(v->data.Up, v->param.UpMax, v->param.UpMin);

	/* Compute and Saturate the integral output */
	v->data.Ui = v->data.Ui + _IQmpy(v->param.Ki,v->data.Up) + _IQmpy(v->param.Kc,v->data.SatErr);
	v->data.Ui = _IQsat(v->data.Ui, v->param.UiMax, v->param.UiMin);

	/* Compute the derivative output */
	v->data.Ud = _IQmpy(v->param.Kd,v->data.Up-v->data.Up1);

	/* Compute and Saturate the output */
	v->data.OutPreSat= v->data.Up + v->data.Ui + v->data.Ud;
	v->term.Out = _IQsat(v->data.OutPreSat, v->param.OutMax, v->param.OutMin);

	/* Compute the saturate difference */
	v->data.SatErr = v->term.Out - v->data.OutPreSat;

	/* Refresh the derivative data*/
	v->data.Up1 = v->data.Up;
}

/*下列位置式PID函数将Kp与Ki分离开，比例系数与积分系数无关系*/
void PID_POSITIONAL2(PID_CONTROLLER *v)
{
	/* Compute and Saturate the input error */
	v->data.Err = v->term.Ref - v->term.Fdb;

	/* Compute and Saturate the proportional output */
	v->data.Up = _IQmpy(v->param.Kp,v->data.Err);
//	v->data.Up = _IQsat(v->data.Up, v->param.UpMax, v->param.UpMin);

	/* Compute and Saturate the integral output */
	v->data.Ui = v->data.Ui + _IQmpy(v->param.Ki,v->data.Err) + _IQmpy(v->param.Kc,v->data.SatErr);
	v->data.Ui = _IQsat(v->data.Ui, v->param.UiMax, v->param.UiMin);

	/* Compute the derivative output */
	v->data.Ud = _IQmpy(v->param.Kd,v->data.Err-v->data.Up1);

	/* Compute and Saturate the output */
	v->data.OutPreSat= v->data.Up + v->data.Ui + v->data.Ud;
	v->term.Out = _IQsat(v->data.OutPreSat, v->param.OutMax, v->param.OutMin);

	/* Compute the saturate difference */
	v->data.SatErr = v->term.Out - v->data.OutPreSat;

	/* Refresh the derivative data*/
	v->data.Up1 = v->data.Err;
}
//-----------------------------------------------------------------------------------------
/*增量式PI算法，只需要设置输出限幅*/
void PID_INCREMENTAL(PID_CONTROLLER *v)
{
	float Delta_Err;

	/* Compute and Saturate the input error */
	v->data.Err = v->term.Ref - v->term.Fdb;

	/* Compute and Saturate the proportional output */
	Delta_Err = v->data.Err - v->data.Up1;

	/* Compute and Saturate the proportional output */
	v->data.Up = _IQmpy(v->param.Kp,Delta_Err);
	v->data.Up = _IQsat(v->data.Up, v->param.UpMax, v->param.UpMin);

	/* Compute and Saturate the integral output */
	v->data.Ui = _IQmpy(v->param.Ki,v->data.Err);

	/* Compute and Saturate the output */
	v->data.OutPreSat = v->term.Out + v->data.Up + v->data.Ui;
	v->term.Out = _IQsat(v->data.OutPreSat, v->param.OutMax, v->param.OutMin);

	/* Refresh the error of last beat*/
	v->data.Up1 = v->data.Err;
}

/*积分分离式PI算法，在一定误差容限内再投入积分作用，有一定抗饱和、抑制超调的效果*/
void PID_INTEGRAL_SEPARATED(PID_CONTROLLER *v)
{
	/* Compute and Saturate the input error */
	v->data.Err = v->term.Ref - v->term.Fdb;

	/* Compute and Saturate the proportional output */
	v->data.Up = _IQmpy(v->param.Kp,v->data.Err);
	v->data.Up = _IQsat(v->data.Up, v->param.UpMax, v->param.UpMin);

	/* Compute and Saturate the integral output */
	//积分分离式PI算法，当误差Err绝对值在误差容限之内再投入积分作用，否则不投入积分作用;
	if((v->data.Err > v->param.Err_Tolerance)||(v->data.Err < -v->param.Err_Tolerance))
	{
		v->data.Ui = v->data.Ui + _IQmpy(v->param.Kc,v->data.SatErr);
	}
	else
	{
		v->data.Ui = v->data.Ui + _IQmpy(v->param.Ki,v->data.Err) + _IQmpy(v->param.Kc,v->data.SatErr);
	}
	v->data.Ui = _IQsat(v->data.Ui, v->param.UiMax, v->param.UiMin);

	/* Compute the derivative output */
	v->data.Ud = _IQmpy(v->param.Kd,v->data.Err-v->data.Up1);

	/* Compute and Saturate the output */
	v->data.OutPreSat= v->data.Up + v->data.Ui + v->data.Ud;
	v->term.Out = _IQsat(v->data.OutPreSat, v->param.OutMax, v->param.OutMin);

	/* Compute the saturate difference */
	v->data.SatErr = v->term.Out - v->data.OutPreSat;

	/* Refresh the derivative data*/
	v->data.Up1 = v->data.Err;
}


