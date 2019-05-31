// Errorhandle.cpp - Dialog (printing a message) 
//                   to handle my errors

#include "StdAfx.h"
#include "ErrorDialog.h"


//**************** s/p My_error **********
// Error message printing

void My_error(CString text)
{
	CErrorDialog dlg;
	dlg.m_Text = text;
	dlg.DoModal();
	exit(1);
}

