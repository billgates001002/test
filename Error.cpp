#pragma once
#include "stdafx.h"
void EmitError(string strmsg, int iRow)
{
	cout << strmsg.c_str() << iRow << endl;

}

void EmitError(string strmsg, string  strContent, int line)
{
	cout << strmsg.c_str() << strContent<<"at :"<<line << endl;

}

void EmitError(string szErr, Token token)
{
	cout << szErr <<":row:"<< token.iLine  << endl;

}