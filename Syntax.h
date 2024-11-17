#pragma once
#include "stdafx.h"
using namespace std;
extern int iListPos;
extern vector<Token> TokenList;
extern SemanticFunc FuncList[200];
class CSyntax
{
public:
	CSyntax(void);
	int m_parseTbl[120][120];
	vector<string> m_szProductList;
	stack<int> m_ParseStack;
	void EnStack(string szProduct);
	bool DeStack(int &iTop);
	void ClearStack();
	void SetTbl(string strSyntaxFileName);
	~CSyntax(void);
	bool SyntaxParse();
		void Init();
};

