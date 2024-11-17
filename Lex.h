#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include <map>
#include "Global.h"
#define _AFXDLL
#include <afx.h>

using namespace std;

extern vector<Token> TokenList;

class CLex
{
private:
	static int m_iPos;
	string strSourceFile;
	string strLexFile;
	static int lexTbl[50][130];
	string strLexBuffer;
	static map<string,int> mk;
	static vector<Token> vt;


public:
	CLex();
	~CLex();
	static void GenToken();
	void GetString(string strFileName);
	static void GetKeyWord(const char* strFileName);

	static void setLexTbl();
	static bool SearchKeyWd(string strkw, int& iKey);



};

