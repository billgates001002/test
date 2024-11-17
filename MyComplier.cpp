// MyComplier.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int main()
{

	//载入语法表

	CSyntax cs;
	cs.Init();


	CLex lex;

	CLex::setLexTbl();

	lex.GetKeyWord("C:\\Users\\liu\\Desktop\\neopascal - 副本\\Sys\\KEYWORDS.txt");

	CLex::GenToken();
	
	cs.SyntaxParse();

	cout << iListPos << endl;

    return 0;
}

