// MyComplier.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int main()
{

	//�����﷨��

	CSyntax cs;
	cs.Init();


	CLex lex;

	CLex::setLexTbl();

	lex.GetKeyWord("C:\\Users\\liu\\Desktop\\neopascal - ����\\Sys\\KEYWORDS.txt");

	CLex::GenToken();
	
	cs.SyntaxParse();

	cout << iListPos << endl;

    return 0;
}

