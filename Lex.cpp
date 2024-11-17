#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "Lex.h"
#include <afx.h>
#include <string>
#include <cctype>  // ���� toupper ����
#include <algorithm>  // ���� transform ����
#define LEXDBG 0
using namespace std;

CLex::CLex()
{

}


CLex::~CLex()
{
}

int CLex::lexTbl[50][130];

int CLex::m_iPos;



vector<Token> CLex::vt;


map<string, int> CLex::mk;
void CLex::setLexTbl()
{

	


	//std::ifstream mFile("C:\\Users\\liu\\Desktop\\neopascal - ����\\Sys\\lex.txt",CFile::modeRead );
	
	CFile mFile(_T("C:\\Users\\liu\\Desktop\\neopascal - ����\\Sys\\lex.txt"), CFile::modeRead);
	

	string strkw;
	

	int iFileLenth = mFile.GetLength();
	
	char* pBuf = new char[iFileLenth + 1];
	mFile.Read(pBuf, iFileLenth);
	



	for (int k = 0; k < iFileLenth; k++)
	{
		if (pBuf[k] != '\n' && pBuf[k]!='\r')
		{
			strkw.push_back(pBuf[k]);
		}
		
	}

	int iTmp = 0;

	for (int i = 0; i <= 36; i++)
	{
		for (int j = 0; j <= 128; j++)
		{
			lexTbl[i][j] = atoi(strkw.substr(iTmp, 3).c_str());
			iTmp = iTmp + 3;
		}
	}

#if LEXDBG
	for (int i = 0; i <= 36; i++)
	{
		for (int j = 0; j <= 128; j++)
		{
			cout << lexTbl[i][j] << "  ";
		}
		cout << endl;
	}
#endif

}


//

void CLex::GetKeyWord(const char* strFileName)
{
	CFile file;
	string strkw;
	int i = 0;
	int id = 40;
	char sRead[2];
	//CFile mFile(_T("C:\\Users\\liu\\Desktop\\neopascal - ����\\Sys\\KEYWORDS.txt"), CFile::modeRead);


	std::ifstream mFile("C:\\Users\\liu\\Desktop\\neopascal - ����\\Sys\\KEYWORDS.txt");  // ���ļ����ж�ȡ
	if (!mFile) {
		std::cerr << "�޷����ļ�" << std::endl;
		return;
	}

	
	while (std::getline(mFile, strkw)) {  // ���ж�ȡ
		std::cout << strkw << std::endl;  // ���ÿһ��
		mk.insert(pair<string, int>(strkw, id));
		id++;
	}

	mFile.close();  // �ر��ļ�

	/*
	int iFileLenth = mFile.GetLength();
	char* pBuf = new char[iFileLenth + 1];
	mFile.Read(pBuf, iFileLenth);
	while (i <= iFileLenth)
	{
		if (pBuf[i] != '\n' || pBuf[i] != '\r')
		{
			strkw.push_back(pBuf[i]);
		}


		i++;
		if (pBuf[i+1] == '\n' && pBuf[i] == '\r')
		{
			i++;
			mk.insert(pair<string,int>(strkw,id));
			id++;
			strkw.clear();
		}

	}
	delete[] pBuf;
	*/
	mk["."] = 7;
	mk["("] = 8;
	mk[")"] = 9;
	mk[";"] = 10;
	mk[","] = 11;
	mk[":"] = 12;
	mk[".."] = 13;
	mk["["] = 14;
	mk["]"] = 15;

	mk["="] = 16;
	mk[":="] = 17;
	mk["<"] = 18;
	mk["<>"] = 19;
	mk["<="] = 20;
	mk[">"] = 21;
	mk[">="] = 22;
	mk["+"] = 23;
	mk["-"] = 24;
	mk["*"] = 25;
	mk["/"] = 26;
	mk["^"] = 27;
	mk["@"] = 28;

#if LEXDBG
	for (const auto &w : mk)
		std::cout << w.first << ":" << w.second << endl;

#endif

}




string rtrim(string str)
{
	if (str.empty())
	{
		return str;
	}

	string szTmp = "";
	int i;
	for (i = str.length(); str[i] <= 32 && i >= 0; i--);

	for (int j = 0; j <= i; j++)
	{
		szTmp.append(1, str[j]);
	}

	return szTmp;
}
string ltrim(string str)
{
	if (str.empty())
	{
		return str;
	}

	string szTmp = "";
	int i;

	for (i = 0; str[i] <= 32&& i < str.length(); i++)
	{
		/*if (str[i] <= 32)
		{
			continue;
		}*/
		//#define TMPCOUT 1
		//#ifdef TMPCOUT
		//		cout << str[i] << endl;
		//#endif // TMPCOUT

		
	};

	for (; i<str.length(); i++)
	{
		szTmp.append(1, str[i]);
	}

	return szTmp;
}

string trim(string str)
{
	return rtrim(ltrim(str));
}

bool CLex::SearchKeyWd(string strkw, int& iKey)
{


	// ʹ�� std::transform ���ַ���ת��Ϊ��д
	std::transform(strkw.begin(), strkw.end(), strkw.begin(), ::toupper);

	map<string, int>::iterator it = mk.find(strkw);
	if (it != mk.end())
	{
		iKey = it->second;
		return true;
	}
	return false;

}
#define FILELEN 1278
void CLex::GenToken()
{
	CFile file;
	CFile mFile(_T("C:\\Users\\liu\\Desktop\\neopascal - ����\\Sys\\pas.txt"), CFile::modeRead);
	int iFileLenth = mFile.GetLength();
	char* pBuf = new char[iFileLenth + 1];
	mFile.Read(pBuf, iFileLenth);

	string strSource(pBuf);
	strSource.append(" ");

	m_iPos = 0;
	int iRow = 0;
	char cTmp;
	int itk;
	int  iTag=0;
	Token tk;
	string strTk;
	int ijk = strSource.length();
	while (m_iPos <=FILELEN)
	{

		cTmp = strSource[m_iPos];
		iTag = lexTbl[iTag][cTmp];
		if (iTag == -99)
		{
			EmitError("�ʷ�����", iRow);
			return;
		}
		if (strSource[m_iPos] == '\n')
		{
			iRow++;
		}
		if (iTag < 0)
		{
			string strTmp = trim(strTk);
			strTk = strTmp;
			cout << strTk << endl;

			//if (iTag == -13)//..
			//{
			//	m_iPos--;
			//}

			if (iTag == -42)//
			{
				m_iPos-=1;
				strTk.erase(strTk.end()-1);
				//strTk.erase(strTk.end());
				SymbolTbl.RecConstTbl(strTk, 3);

				tk.iID = 3;
				tk.strContent = strTk;
				tk.iLine = iRow;
				TokenList.push_back(tk);

				iTag = 0;

			}

			if (iTag == -1)//��ʶ��
			{
				int kw;
				//�ؼ���
				if (CLex::SearchKeyWd(strTk, kw))
				{
					tk.iID = kw;
					tk.iLine = iRow;
					tk.strContent = strTk;
					TokenList.push_back(tk);
				}
				else
				{
					tk.iID = 1;
					tk.strContent = strTk;
					tk.iLine = iRow;
					TokenList.push_back(tk);
				}
				
			}
			else//�Ǳ�ʶ��
			{
				if (strTk.compare("TRUE")==0 || strTk.compare("FALSE")==0)
				{
					SymbolTbl.RecConstTbl(strTk, 7);

				}
				

			}

			if (iTag <=-2 && iTag>=-6)
			{
				//���� ���� ʵ�� �ַ���

				SymbolTbl.RecConstTbl(strTk, iTag);
				tk.iID = -iTag;
				tk.strContent = strTk;
				tk.iLine = iRow;
				TokenList.push_back(tk);

			}

			//���

			if (iTag <= -7 && iTag >= -15)
			{
				//���� ���� ʵ�� �ַ���
				tk.iID = -iTag;
				tk.strContent = strTk;
				tk.iLine = iRow;
				TokenList.push_back(tk);


			}

			//�����
			if (iTag <= -16 && iTag >= -28)
			{
				//���� ���� ʵ�� �ַ���
				tk.iID = -iTag;
				tk.strContent = strTk;
				tk.iLine = iRow;
				TokenList.push_back(tk);

			}

			
			//tk.iLine = iRow;
			//tk.strContent = strTk;
			//TokenList.push_back(tk);
			m_iPos--;
			
			strTk.clear();
			iTag = 0;
			



		}
		else //��������
		{
			
			strTk.push_back(cTmp);

		
			//.strContent.push_back(cTmp);

		}		
		m_iPos++;
		
	}
}

