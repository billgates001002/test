#pragma once
#include "stdafx.h"
#include "Structure.h"
using namespace std;



class CSymbolTbl
{
public:
	map<int, TypeInfo> TypeInfoTbl;//������Ϣ��
	map<int, ConstInfo> ConstInfoTbl;//������Ϣ��
	vector<ProcInfo> ProcInfoTbl;//������Ϣ��
	map<int,LabelInfo> LabelInfoTbl;//���ű�
	stack<int> ProcStack;
	vector<UseFile> UseFileTbl;
	vector<EnumInfo> EnumInfoTbl;
	map<int,VarInfo> VarInfoTbl;
	int m_iListPos;


	

public:

	CSymbolTbl();
	~CSymbolTbl();
	int RecConstTbl(const string strValue, int iType);
	void AddConst(ConstInfo Tmp);
	void AddType(TypeInfo tmpInfo);
	int SearchUseFileTbl(string strFileName);
	int SearchProcInfoTbl(string strName, bool bIsForward=true);
	int SearchEnumInfoTbl(int  iProcIndex, string strName);
	int SearchConstTbl(int iProcIndex, string strConst);
	int SearchVarInfoTbl(int iProcIndex, string strConst);
	int SearchTypeInfoTbl(int iProcIndex, string strConst,bool bState=true);
	int SearchLabelInfoTbl(int iProcIndex, string strLabel);
	void AddLabel(LabelInfo liTmp);
	void AddVar(VarInfo vtmp);
	bool PtrCheck(int &iPos);

};

