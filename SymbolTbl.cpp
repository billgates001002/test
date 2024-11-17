#include "stdafx.h"
#include "SymbolTbl.h"


CSymbolTbl::CSymbolTbl()
{
}


CSymbolTbl::~CSymbolTbl()
{
}


int CSymbolTbl::RecConstTbl(const string strValue, int iType)
{
	m_iListPos = iListPos;
	ConstInfo ci;
	ci.strName = strValue;
	ci.strVal = strValue;
	ci.eStoreType = StoreType::T_NONE;
	iType = -iType;
	switch (iType)
	{
	case 2:
	{
		ci.eConstType = ConstType::STRING;
		ci.strVal = ci.strVal.substr(1, ci.strVal.length() - 2);
		ci.strVal= ci.strVal = StrReplace(ci.strVal, "''", "'");
	} break;
	case 3:
		ci.eConstType = ConstType::INTEGER; break;
	case 4:
		ci.eConstType = ConstType::REAL; break;
	case 5:
		
	case 6:
		ci.eConstType = ConstType::EREAL;
		break;
	case 7:
		ci.eConstType = ConstType::EBOOLEAN; break;
		
	}
	map<int, int> m = { {1,2,}, {3,4,}, {5,6} };

	//map<StoreType, pair<double, double>>  mType;
	
	vector<pair<StoreType, pair<double, double>>> mType;
	mType.push_back(pair<StoreType, pair<double, double>>(T_SHORTINT, { -128, 127 }));
	mType.push_back(pair<StoreType, pair<double, double>>(T_SMALLINT, { -32768, 32767 }));
	mType.push_back(pair<StoreType, pair<double, double>>(T_INTEGER, { -2147483648.0, 2147483647.0 }));
	mType.push_back(pair<StoreType, pair<double, double>>(T_BYTE, { 0, 255 }));
	mType.push_back(pair<StoreType, pair<double, double>>(T_WORD, { 0, 65535 }));
	mType.push_back(pair<StoreType, pair<double, double>>(T_LONGWORD, { 0, 4294967295 }));
	mType.push_back(pair<StoreType, pair<double, double>>(T_SINGLE, { 1.5e-45, 3.4e38 }));
	//mType.insert(pair<StoreType, pair<double, double>>(T_REAL, { 1.5e-45, 3.4e38 }));

	//设置常量存储类型
	switch (ci.eConstType)
	{
	case ConstType::INTEGER://T_SHORTINT T_SMALLINT T_INTEGER  T_BYTE  T_WORD  T_LONGWORD
	{
		int ival = atoi(ci.strVal.c_str());
		if (ival < 0)
		{
			for (auto it = mType.begin();it!=mType.end();it++)
			{
				if (ival >= it->second.first && -ival <= it->second.second)
				{
					ci.eStoreType = it->first;
					break;
				}

			}
			
			
			
		}
		else
		{
			for (auto it = mType.begin(); it != mType.end(); it++)
			{
				if (it->second.first < 0)
				{
					continue;
				}
				if (ival <= it->second.second)
				{
					ci.eStoreType = it->first;
					break;
				}

			}
		}

		ci.llval = ival;
		ci.fVal = ival;
		

	}; break;

	case ConstType::EBOOLEAN:
	{
		ci.eStoreType = T_BOOLEAN;
		if (ci.strVal == "TRUE")
		{
			ci.bVal = true;

		}
		else
		{
			ci.bVal = false;
		}

	}; break;
	case ConstType::STRING:
	{
		if (ci.strVal.length() == 1)
		{
			ci.eStoreType = T_CHAR;
		}
		else
		{
			ci.eStoreType = T_STRING;
		}
	}; break;
	case ConstType::EREAL:
	case ConstType::REAL:
	{
		double fVal = atof(ci.strName.c_str());
		//pair<double, double> p = mType[T_SINGLE];  1.5e-45, 3.4e38
		if (fVal > 1.5e-45 &&fVal <  3.4e38)
		{
			ci.eStoreType = T_SINGLE;
		}
		else
		{
			ci.eStoreType = T_REAL;
		}

	}; break;


	default:
		break;
	}
	SymbolTbl.AddConst(ci);
	//m_ConstInfoTbl
	return ConstInfoTbl.size() - 1;

}

void CSymbolTbl::AddConst(ConstInfo Tmp)
{
	SymbolTbl.ConstInfoTbl.insert(pair<int, ConstInfo>(SymbolTbl.ConstInfoTbl.size(), Tmp));
}

void CSymbolTbl::AddType(TypeInfo tiTmp)
{
	SymbolTbl.TypeInfoTbl.insert(pair<int, TypeInfo>(SymbolTbl.TypeInfoTbl.size(), tiTmp));
}

int  CSymbolTbl::SearchUseFileTbl(string strFileName)
{
	int i = UseFileTbl.size() - 1;
	while (i >= 0 && UseFileTbl.at(i).m_szFileName.compare(strFileName)!=0 )
	{
		i--;
	}
	return i;
}

int CSymbolTbl::SearchEnumInfoTbl(int  iProcIndex, string strName)
{
	
	int i = EnumInfoTbl.size() - 1;
	while (i >= 0 && EnumInfoTbl.at(i).strName.compare(strName) != 0)
	{
		i--;
	}
	return i;
}



int CSymbolTbl::SearchProcInfoTbl(string strName, bool bIsForward)
{
	int i = ProcInfoTbl.size() - 1;
	while (i >= 0 && (ProcInfoTbl.at(i).strName.compare(strName) != 0 ||(bIsForward?false:ProcInfoTbl.at(i).m_eFlag!=ProcInfo::Forward)))
	{
		i--;
	}
	return i;

}


int CSymbolTbl::SearchConstTbl(int iProcIndex, string strConst)
{
	int i = ConstInfoTbl.size() - 1;
	while (i >= 0 && (ConstInfoTbl[i].strName.compare(strConst) != 0 || ConstInfoTbl[i].iProcIndex == iProcIndex))
	{
		i--;
	}
	return i;
}
int CSymbolTbl::SearchVarInfoTbl(int iProcIndex, string strVarName)
{
	int i = VarInfoTbl.size() - 1;
	while (i >= 0 && (VarInfoTbl[i].strName.compare(strVarName) != 0||VarInfoTbl[i].iProcIndex==iProcIndex))
	{
		i--;
	}
	return i;

}
int CSymbolTbl::SearchTypeInfoTbl(int iProcIndex, string strTypeName, bool bState)
{
	int i = TypeInfoTbl.size() - 1;
	while (i >= 0 && (
		TypeInfoTbl[i].strName.compare(strTypeName) != 0 
		|| TypeInfoTbl[i].iProcIndex != iProcIndex
		|| (bState ? TypeInfoTbl[i].m_iState == 0 : false)))
	{
		i--;
	}
	return i;
}
int CSymbolTbl::SearchLabelInfoTbl(int iProcIndex, string strLabel)
{

	int i = LabelInfoTbl.size() - 1;
	while (i >= 0 && (LabelInfoTbl[i].strName.compare(strLabel) != 0 || LabelInfoTbl[i].iProcIndex == iProcIndex))
	{
		i--;
	}
	return i;
}

void CSymbolTbl::AddLabel(LabelInfo tmp)
{
	LabelInfoTbl.insert(pair<int, LabelInfo>(SymbolTbl.LabelInfoTbl.size(), tmp));
}

void CSymbolTbl::AddVar(VarInfo vtmp)
{
	SymbolTbl.VarInfoTbl.insert(pair<int, VarInfo>(SymbolTbl.VarInfoTbl.size(), vtmp));

}


bool CSymbolTbl::PtrCheck(int &iPos)
{
	int i = 0;
	for (i = 0; i < TypeInfoTbl.size(); i++)
	{
		if (TypeInfoTbl.at(i).m_eDataType == StoreType::T_POINTER
			&& !TypeInfoTbl.at(i).m_szContent.empty())
		{
			int j = SearchTypeInfoTbl(TypeInfoTbl.at(i).iProcIndex, TypeInfoTbl.at(i).m_szContent, false);
			if (j == -1)
			{
				j = SearchTypeInfoTbl(0, TypeInfoTbl.at(i).m_szContent, false);

			}
			if (j != -1)
			{
				TypeInfoTbl.at(i).m_szContent = "";
				TypeInfoTbl.at(i).m_iLink = j;
			}
			else
			{
				iPos = i;
				return false;
			}
		}

	}
	return true;
}