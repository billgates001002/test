#pragma once
#include "stdafx.h"
void EmitError(string strmsg, int iRow);
void EmitError(string strmsg, string  strContent, int line);
void EmitError(string szErr, Token token);