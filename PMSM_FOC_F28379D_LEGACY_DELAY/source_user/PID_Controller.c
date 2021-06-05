/*
 * PID_Controller.c
 *
 *  Created on: 2019��3��23��
 *      Author: YY
 */

#include "PID_Controller.h"

//***********************************************************************************************
/*λ��ʽPID�㷨��ʵ��Ki*=_IQmpy(v->param.Kp,v->param.Ki); */
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

/*����λ��ʽPID������Kp��Ki���뿪������ϵ�������ϵ���޹�ϵ*/
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
/*����ʽPI�㷨��ֻ��Ҫ��������޷�*/
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

/*���ַ���ʽPI�㷨����һ�������������Ͷ��������ã���һ�������͡����Ƴ�����Ч��*/
void PID_INTEGRAL_SEPARATED(PID_CONTROLLER *v)
{
	/* Compute and Saturate the input error */
	v->data.Err = v->term.Ref - v->term.Fdb;

	/* Compute and Saturate the proportional output */
	v->data.Up = _IQmpy(v->param.Kp,v->data.Err);
	v->data.Up = _IQsat(v->data.Up, v->param.UpMax, v->param.UpMin);

	/* Compute and Saturate the integral output */
	//���ַ���ʽPI�㷨�������Err����ֵ���������֮����Ͷ��������ã�����Ͷ���������;
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


