#include "stdafx.h"
#include "Syntax.h"
#include <string>


CSyntax::CSyntax()
{
}


CSyntax::~CSyntax()
{
}




void CSyntax::SetTbl(string strSyntaxFileName)
{



}

void CSyntax::Init()
{
	int iRow = 1;
	int iCol = 1;
	string strFile = "";
	fstream fs("C:\\Users\\liu\\Desktop\\neopascal - 副本\\Sys\\parse.txt", ios_base::in);
	while (!fs.eof())
	{
		getline(fs, strFile); 
	}
	int iCnt = 0;
	for (int i = 0; i < 99; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			m_parseTbl[i][j] = atoi(strFile.substr(iCnt, 3).c_str());
			iCnt = iCnt + 3;
		}
	}
	fs.close();
	//fs.open()
	fs.open("C:\\Users\\liu\\Desktop\\neopascal - 副本\\Sys\\product.txt", ios_base::in);
	string strProduct = "";
	m_szProductList.push_back("");
	while (!fs.eof())
	{
		getline(fs, strProduct);
		m_szProductList.push_back(strProduct);
	}

}


void CSyntax::EnStack(string strProduct)
{
	if (strProduct.length() <= 0)
	{
		return;
	}
	for (int i = strProduct.length() - 3; i >= 0; i = i - 3)
	{
		m_ParseStack.push(atoi(strProduct.substr(i, 3).c_str()));
	}
}

bool CSyntax::DeStack(int &iTop)
{
	if (m_ParseStack.empty())
	{
		return false;
	}
	else
	{
		iTop = m_ParseStack.top();
		m_ParseStack.pop();
		return true;
	}
}

bool CSyntax::SyntaxParse()
{
	int iRow = 0;
	int iCol = 0;
	int iVal = 0;
	iListPos = 0;
	while (!m_ParseStack.empty())
	{
		m_ParseStack.pop();
	}
	m_ParseStack.push(100);

	while (!m_ParseStack.empty() && iListPos < TokenList.size())
	{
		if (DeStack(iVal))
		{
			if (iVal == 0)
			{
				continue;
			}
			if (iVal < 100)
			{
				if (iVal == TokenList.at(iListPos).iID)
				{
					iListPos++;
				}
				else
				{
					EmitError("语法错误", TokenList.at(iListPos - 1).strContent, TokenList.at(iListPos-1).iLine);
					break;
				}
			}
			if (iVal >= 100 && iVal < 300)
			{
				iCol = TokenList.at(iListPos).iID;
				iRow = iVal - 100;
				int iTmp = m_parseTbl[iRow][iCol];
				if (iTmp == -1)
				{
					EmitError("语法错误", TokenList.at(iListPos - 1).strContent, TokenList.at(iListPos - 1).iLine);
					break;
				}
				else
				{
					EnStack(m_szProductList[iTmp]);
				}
			}
			if (iVal >= 300)
			{
				cout << iVal << ','<<endl;
				if (!FuncList[iVal - 300]())
				{
					break;
				}
			}
		}

	}
	return (m_ParseStack.empty() && iListPos == TokenList.size());
}