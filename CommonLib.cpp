#include "CommonLib.h"
#include "stdafx.h"

static int SerialId;
string itos(int i)
{
	char cBuffer[10];
	int iPos, iSign;
	_itoa_s(i, cBuffer, 10);
	return cBuffer;
}

string GetSerialId()
{
	return itos(SerialId++);
}
string StrReplace(string str, string src, string des)
{
	string szTmp = str;
	int i = szTmp.find(src);

	while (i != string::npos)
	{
		szTmp.replace(i, src.length(), des);
		i = szTmp.find(src);
	}

	return szTmp;
}
