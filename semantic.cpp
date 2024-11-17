#include "semantic.h"
#include "stdafx.h"
#include "Type.h"
#define PID SymbolTbl.ProcStack.top()
#define TOKENNAME TokenList.at(iListPos - 1).strContent
stack<int> iIdListFlag;
stack<int> iTypeFlag;
bool bForwardFlag;
int iEnumSize;
stack<string> szVarFieldFlag;
stack<string> szVarFieldConst;
enum LabelType  {ENUMTYPE, RECORDFIXTYPE, RECORDVARTYPE, FUNCPARATYPE, PARAGPARATYPE, VARITYPE, FILETYPE};

stack<int> iTypePos;//类型位置


//数据结构初始化 可以用模板实现
void semantic000()
{
	


}
//程序头 -> program 标识符 001 ( 标识符列表 ) ;
//填充ProcInfo结构
/*
struct ProcInfo
{
enum RANK { MAIN, SUB } m_eRank;
enum ETYPE {PROCEDURE, FUNCTION,FUNCPOINTER }m_eType;
string m_strProcName;
vector<ParaInfo> m_vParaTbl;
int m_iReturnType;
int m_iReturnVar;
enum CallType{StdCall, FastCall} m_eCallType;
vector<IRCode> m_Codes;
set<int> m_TmpMemShare;
int m_ValSize;
int m_TmpLink;
bool m_bUsed;
bool m_bTmpUsed;
ProcInfo();
};

*/
bool semantic001() 
{
	ProcInfo piTmp;
	piTmp.m_eRank = ProcInfo::MAIN;
	piTmp.m_strProcName = TokenList.at(iListPos - 1).strContent;
	piTmp.m_eType = ProcInfo::PROCEDURE;
	piTmp.m_vParaTbl.clear();
	SymbolTbl.ProcInfoTbl.push_back(piTmp);
	SymbolTbl.ProcStack.push(SymbolTbl.ProcInfoTbl.size() - 1);

	iIdListFlag.push(0);
	iTypeFlag.push(0);
	return true;
}

bool semantic002()
{

	
	
	return true;
}


//标号声明部分 -> label 002 标识符 003 标号声明列表 ;
//标号声明列表 -> , 标识符 003 标号声明列表
bool semantic003() 
{
	LabelInfo liTmp;
	liTmp.strName = TOKENNAME;
	liTmp.iProcIndex = PID;
	liTmp.m_bDef = false;
	liTmp.m_bUse = false;

	if (SymbolTbl.SearchLabelInfoTbl(PID, TOKENNAME) == -1 &&
		SymbolTbl.ProcInfoTbl.at(PID).m_strProcName.compare(TOKENNAME) != 0)
	{
		SymbolTbl.AddLabel(liTmp);
		return true;
	}
	else
	{
		EmitError("标号已定义", TokenList.at(iListPos - 1));
		return false;
	}

}
//常量定义 -> 标识符 102 = 表达式 004  用于表达式为常量的情况
bool semantic004() 
{
	string strLabel = TokenList.at(iListPos - 3).strContent;
	if (SymbolTbl.SearchLabelInfoTbl(PID, strLabel) == -1
		&& SymbolTbl.SearchConstTbl(PID, strLabel) == -1)
	{
		ConstInfo ciTmp;
		ciTmp = SymbolTbl.ConstInfoTbl[atoi(TokenList.at(iListPos - 1).strContent.c_str())];
		ciTmp.strName = strLabel;
		ciTmp.iProcIndex = PID;
		SymbolTbl.AddConst(ciTmp);
		return true;

	}
	else
	{
		EmitError("标号已定义", TokenList.at(iListPos - 1));
		return false;
	}

	return true;
}
bool semantic005() {return true;}
//类型定义 -> 标识符 006 = 类型
bool semantic006() 
{
	TypeInfo tiTmp;
	tiTmp.strName = TOKENNAME;
	tiTmp.m_szContent = TOKENNAME;
	tiTmp.iProcIndex = SymbolTbl.ProcStack.top();

	//判断重定义


	if (SymbolTbl.SearchTypeInfoTbl(SymbolTbl.ProcStack.top(), tiTmp.m_szContent) == -1
		&& SymbolTbl.SearchConstTbl(SymbolTbl.ProcStack.top(), tiTmp.m_szContent) == -1
		&& SymbolTbl.SearchLabelInfoTbl(SymbolTbl.ProcStack.top(), tiTmp.m_szContent) == -1
		&& SymbolTbl.SearchEnumInfoTbl(SymbolTbl.ProcStack.top(), tiTmp.m_szContent) == -1
		&& SymbolTbl.ProcInfoTbl.at(SymbolTbl.ProcStack.top()).m_strProcName.compare(tiTmp.m_szContent) != 0)
	{
		SymbolTbl.AddType(tiTmp);
		iTypePos.push(SymbolTbl.TypeInfoTbl.size() - 1);
		return true;
	}
	else
	{
		EmitError(tiTmp.strName.append("标识符已经存在"), TokenList.at(iListPos - 1));
		return false;
	}
	
	return true;
}

/*
struct TypeInfo:ObjectInfo
{
StoreType m_eDataType;
int m_iLink;
vector<FieldInfo> m_FieldInfo;
vector<ArrayInfo> m_ArrayInfo;
string  m_szContent;
StoreType m_eBaseType;
int m_iState;
int m_iSize;
};

有序类型        		→ char 007
058							→ boolean 007
059							→ 枚举类型
060							→ 整数类型
061 枚举类型        		→ ( 011 标识符列表 ) 014
062 整数类型        		→ integer 007
063							→ byte 007
064							→ shortint 007
065							→ smallint 007
066                   		→ word 007
067               			→ longword 007
068               			→ cardinal 007
069 无序类型              	→ real 007
→ single 007



*/
bool semantic007() 
{

	if (iTypeFlag.top() == 7)//函数返回类型
	{
		SymbolTbl.ProcInfoTbl.back().m_iReturnType = SymbolTbl.TypeInfoTbl.size();
		TypeInfo tiTmp;
		tiTmp.m_iState = 1;
		tiTmp.strName = "_noname";
		tiTmp.strName.append(GetSerialId());
		tiTmp.iProcIndex = PID;
		tiTmp.m_eDataType = TokenToType(TokenList.at(iListPos - 1).iID);
		tiTmp.m_eBaseType = tiTmp.m_eDataType;
		SymbolTbl.AddType(tiTmp);
		return true;
	}

	//StoreType m_eDataType;
	if (!iTypePos.empty() && iTypePos.top() != -1 &&
		SymbolTbl.TypeInfoTbl[iTypePos.top()].m_eDataType == StoreType::T_NONE)
	{
		StoreType tmpstoretype = TokenToType(TokenList.at(iListPos-1).iID);
		SymbolTbl.TypeInfoTbl[iTypePos.top()].m_eDataType = tmpstoretype;
		SymbolTbl.TypeInfoTbl[iTypePos.top()].m_eBaseType = tmpstoretype;
		SymbolTbl.TypeInfoTbl[iTypePos.top()].m_iState = 1; //类型描述终结。状态置1
	}

	return true;
}
/*
指针类型        		→^ 简单类型
简单类型        		→ 标识符 008
*/
bool semantic008()
{
	SymbolTbl.TypeInfoTbl.at(iTypePos.top()).m_eDataType = StoreType::T_POINTER;
	SymbolTbl.TypeInfoTbl.at(iTypePos.top()).m_szContent = TOKENNAME;
	int i;
	i = SymbolTbl.SearchTypeInfoTbl(SymbolTbl.ProcStack.top(), TOKENNAME, false);
	if (i != -1)
	{
		if (SymbolTbl.TypeInfoTbl.at(i).m_iState != 1)
		{
			EmitError("指针指向的类型声明不完整", TokenList.at(iListPos - 1));
			return false;
		}
	}
	return true;
}
/*
指针类型        		→^ 简单类型
简单类型        		→ 标识符 008
*/
bool semantic009() 
{
	SymbolTbl.TypeInfoTbl[iTypePos.top()].m_eDataType = StoreType::T_POINTER;
	SymbolTbl.TypeInfoTbl[iTypePos.top()].m_iLink = SymbolTbl.TypeInfoTbl.size();
	TypeInfo tmp;
	tmp.m_iState = 1;
	tmp.strName = "_noname";
	tmp.strName.append(GetSerialId());
	tmp.iProcIndex = PID;
	tmp.m_eDataType = TokenToType(TokenList.at(iListPos - 1).iID);
	tmp.m_eBaseType = tmp.m_eDataType;
	SymbolTbl.AddType(tmp);


	return true;
}

/*
类型            		→ 015 基本类型 010
						→ 015 函数类型 010
						→ 015 构造类型 010
						→ 015 指针类型 010
						→ 015 字符串类型 010
						→ 015 标识符  023 010
*/
bool semantic010() 
{
	if (!iTypePos.empty() && iTypePos.top() != -1)
	{
		SymbolTbl.TypeInfoTbl[iTypePos.top()].m_iState = 1;
		iTypePos.pop();
	}


	return true;
}

//枚举类型        		→ ( 011 标识符列表 ) 014
bool semantic011()
{
	//SymbolTbl.TypeInfoTbl[SymbolTbl.TypeInfoTbl.size()-1].m_eDataType=
	iEnumSize = SymbolTbl.EnumInfoTbl.size();

	if (iTypeFlag.top() == 2)
	{
		SymbolTbl.TypeInfoTbl[iTypePos.top()].m_iLink = SymbolTbl.TypeInfoTbl.size();
		SymbolTbl.TypeInfoTbl[iTypePos.top()].m_iState = 1;
		TypeInfo tmp;
		tmp.m_iState = 1;
		tmp.strName = "_noname";
		tmp.strName.append(GetSerialId());
		tmp.iProcIndex = PID;
		tmp.m_eDataType = StoreType::T_ENUM;
		tmp.m_iLink = SymbolTbl.EnumInfoTbl.size();
		iIdListFlag.push(1);
		SymbolTbl.AddType(tmp);
		return true;
	}

	SymbolTbl.TypeInfoTbl[iTypePos.top()].m_eDataType = StoreType::T_ENUM;
	SymbolTbl.TypeInfoTbl[iTypePos.top()].m_iLink = SymbolTbl.EnumInfoTbl.size();
	iIdListFlag.push(1);
	return true;
}

bool semantic012() 
{
	string strName = TokenList.at(iListPos - 1).strContent;
	int iProcIndex = SymbolTbl.ProcStack.top();
	//枚举类型
	if (iIdListFlag.top() == 1)
	{
		//搜索函数，类型，变量，标号名
		EnumInfo eiTmp;
		eiTmp.strName = TOKENNAME;
		eiTmp.iProcIndex= SymbolTbl.ProcStack.top();
		if (SymbolTbl.SearchConstTbl(iProcIndex, strName)==-1 &&
			SymbolTbl.SearchLabelInfoTbl(iProcIndex, strName)==-1 &&
			SymbolTbl.SearchTypeInfoTbl(iProcIndex, strName,false)==-1 &&
			SymbolTbl.SearchEnumInfoTbl(iProcIndex, strName)==-1
			&& SymbolTbl.ProcInfoTbl.at(iProcIndex).m_strProcName.compare(strName)!=0)
		{

			if (SymbolTbl.EnumInfoTbl.size() - iEnumSize <= 256)
			{
				SymbolTbl.EnumInfoTbl.push_back(eiTmp);
				return true;
			}
			else
			{
				EmitError("枚举类型的标号数不得大于256", TokenList.at(iListPos - 1));
				return false;
			}

		}
		else
		{
			EmitError("标号名已经存在，或该标号已经定义", TokenList.at(iListPos - 1));
			return false;
		}
	}
	//记录类型
	if (iIdListFlag.top() == 2)
	{
		FieldInfo fiTmp;
		fiTmp.strName = TOKENNAME;
		fiTmp.m_iState = 0;
		fiTmp.iProcIndex = PID;
		for (int i = SymbolTbl.TypeInfoTbl.at(iTypePos.top()).m_FieldInfo.size() - 1; i >= 0; i--)
		{
			string strTmp = SymbolTbl.TypeInfoTbl.at(iTypePos.top()).m_FieldInfo.at(i).strName;
			if (strTmp.compare(TOKENNAME) == 0)
			{
				EmitError(TOKENNAME.append("域名已经存在"), TokenList.at(iListPos - 1));
				return false;
			}
		}
		SymbolTbl.TypeInfoTbl.at(iTypePos.top()).m_FieldInfo.push_back(fiTmp);
	}
	if (iIdListFlag.top() == 3)
	{
		FieldInfo fiTmp;
		fiTmp.iProcIndex = PID;
		fiTmp.strName = TOKENNAME;
		fiTmp.m_iState = 0;
	    fiTmp.m_szVarFieldConst = szVarFieldConst.top();
		fiTmp.m_szVarFieldFlag = szVarFieldFlag.top();

		for (int i = SymbolTbl.TypeInfoTbl[iTypePos.top()].m_FieldInfo.size() - 1; i >= 0; i--)
		{
			FieldInfo *pTmp = &SymbolTbl.TypeInfoTbl[iTypePos.top()].m_FieldInfo.at(i);
			if (pTmp->strName.compare(fiTmp.strName) == 0)
			{
				EmitError(TOKENNAME.append("域名已经存在"), TokenList.at(iListPos - 1));
				return false;
			}
		}
		SymbolTbl.TypeInfoTbl[iTypePos.top()].m_FieldInfo.push_back(fiTmp);
	}
	//过程函数形式参数列表的标识符
	if (iIdListFlag.top() == 4 || iIdListFlag.top()== 5)
	{
		ParaInfo piTmp;
		piTmp.strParaName = TOKENNAME;
		piTmp.iParaType = iIdListFlag.top() == 4 ? ParaInfo::VAL : ParaInfo::VAR;
		piTmp.iParaType = -1;
		//检查重名
		for (int i = SymbolTbl.ProcInfoTbl.back().m_vParaTbl.size() - 1; i >= 0; i--)
		{
			if (SymbolTbl.ProcInfoTbl.back().m_vParaTbl.at(i).strParaName.compare(piTmp.strParaName) == 0)
			{
				EmitError(piTmp.strParaName.append("标识符名已经存在"), TokenList.at(iListPos - 1));
				return false;
			}
		}
		SymbolTbl.ProcInfoTbl.back().m_vParaTbl.push_back(piTmp);
		return true;
	}

	//文件标识符
	if (iIdListFlag.top() == 7)
	{
		UseFile ufTmp;
		ufTmp.m_szFileName = strName;
		ufTmp.m_bFlag = false;
		if (!SymbolTbl.SearchUseFileTbl(strName))
		{
			ufTmp.m_bFlag = true;
			SymbolTbl.UseFileTbl.push_back(ufTmp);
		}

	}

	//函数类型


	return true;
}

bool semantic013()
{
	iIdListFlag.push(6);
	return true;
}

bool semantic014() 
{
	EnumInfo eiTmp;
	int i = iEnumSize;
	int j = 0;
	while (i < SymbolTbl.EnumInfoTbl.size())
	{
		ConstInfo ciTmp;
		ciTmp.strName = SymbolTbl.EnumInfoTbl.at(i).strName;
		ciTmp.iProcIndex = PID;
		ciTmp.llval = j++;
		ciTmp.eEnumIdx = SymbolTbl.TypeInfoTbl.size() - 1;
		ciTmp.eConstType = ConstType::ENUM;
		ciTmp.eStoreType = StoreType::T_ENUM;
		SymbolTbl.AddConst(ciTmp);
		i++;
	}
	eiTmp.strName = "-1";
	SymbolTbl.EnumInfoTbl.push_back(eiTmp);
	iIdListFlag.pop();

	return true;
}
/*
类型            		→ 015 基本类型 010
						→ 015 函数类型 010
						→ 015 构造类型 010
						→ 015 指针类型 010
						→ 015 字符串类型 010
						→ 015 标识符  023 010
*/
bool semantic015() 
{
	if (iTypeFlag.top() == 2 ||
		iTypeFlag.top() == 5 ||
		iTypeFlag.top() == 6 ||
		iTypeFlag.top() == 7
		)
	{
		iTypePos.push(SymbolTbl.TypeInfoTbl.size() - 1);
		return true;
	}


	if (iTypeFlag.top() == 3 || iTypeFlag.top() == 4) //数组类型
	{
		TypeInfo tiTmp;
		tiTmp.strName = "_noname";
		tiTmp.strName.append(GetSerialId());
		tiTmp.iProcIndex = PID;
		tiTmp.m_iState = 0;
		SymbolTbl.TypeInfoTbl[iTypePos.top()].m_iLink = SymbolTbl.TypeInfoTbl.size();
		SymbolTbl.AddType(tiTmp);
		iTypePos.push(SymbolTbl.TypeInfoTbl.size() - 1);
		return true;

	}
	if (iTypeFlag.top() == 1)//记录类型
	{
		TypeInfo tiTmp;
		tiTmp.strName = "_noname";
		tiTmp.strName.append(GetSerialId());
		tiTmp.iProcIndex = SymbolTbl.ProcStack.top();
		tiTmp.m_iState = 0;
		SymbolTbl.AddType(tiTmp);
		int i = SymbolTbl.TypeInfoTbl[iTypePos.top()].m_FieldInfo.size() - 1;
		int iTmpTypePos = iTypePos.top();
		iTypePos.push(SymbolTbl.TypeInfoTbl.size() - 1);
		while (i >= 0)
		{
			if (SymbolTbl.TypeInfoTbl[iTmpTypePos].m_FieldInfo.at(i).m_iState == 0)
			{
				SymbolTbl.TypeInfoTbl[iTmpTypePos].m_FieldInfo.at(i).m_iLink = iTypePos.top();
				SymbolTbl.TypeInfoTbl[iTmpTypePos].m_FieldInfo.at(i).m_iState = 1;
			}
			i--;
		}
	}

	return true;
}
//数组类型		-> array 016 [ 数组界限 数组界限列表 ] of  类型 025
bool semantic016() 
{
	SymbolTbl.TypeInfoTbl[iTypePos.top()].m_eDataType = StoreType::T_ARRAY;
	iTypeFlag.push(3);
	return true;
}

/*
数组界限列表		->  , 数组界限 数组界限列表
-> ε
数组界限			-> 整数常量 017 .. 整数常量 018
*/
bool semantic017() 
{
	ArrayInfo aiTmp;
	int i = atoi(TOKENNAME.c_str());
	aiTmp.iStart = atoi(SymbolTbl.ConstInfoTbl[i].strVal.c_str());
	SymbolTbl.TypeInfoTbl[iTypePos.top()].m_ArrayInfo.push_back(aiTmp);
	return true;
}
/*
数组界限列表		->  , 数组界限 数组界限列表
-> ε
数组界限			-> 整数常量 017 .. 整数常量 018
*/
bool semantic018() 
{
	int i = atoi(TOKENNAME.c_str());
	int iTmp = atoi(SymbolTbl.ConstInfoTbl[i].strVal.c_str());
	ArrayInfo* tmp = &(SymbolTbl.TypeInfoTbl[iTypePos.top()].m_ArrayInfo.at(SymbolTbl.TypeInfoTbl[iTypePos.top()].m_ArrayInfo.size() - 1));
	if (tmp->iStart <= iTmp)
	{
		tmp->iEnd = iTmp;
		return true;
	}
	else
	{
		return true;
		EmitError("数组定义上限小于下限", TokenList.at(iListPos - 1));
		return false;
	}


	return true;
}
// 记录类型			-> record 019 字段列表 end 020
bool semantic019() 
{
	SymbolTbl.TypeInfoTbl[iTypePos.top()].m_eDataType = StoreType::T_RECORD;
	iTypeFlag.push(1);
	iIdListFlag.push(2);
	return true;
}

bool semantic020()
{
	iTypeFlag.pop();
	iIdListFlag.pop();
	return true;
}
// 函数类型              	→ procedure 021 形参列表 039 044    
/*

enum RANK { MAIN, SUB } m_eRank;
enum ETYPE {PROCEDURE, FUNCTION,FUNCPOINTER }m_eType;
string m_strProcName;
vector<ParaInfo> m_vParaTbl;
int m_iReturnType;
int m_iReturnVar;
enum Flag{Forward, Extern, None} m_eFlag;
int m_iExternStr;
int m_iProcDefToken;
enum CallType{StdCall, FastCall} m_eCallType;
vector<IRCode> m_Codes;
set<int> m_TmpMemShare;
int m_ValSize;
int m_TmpLink;
bool m_bUsed;
bool m_bTmpUsed;
*/
bool semantic021() 
{
	SymbolTbl.TypeInfoTbl[iTypePos.top()].m_eDataType = StoreType::T_PROC;
	SymbolTbl.TypeInfoTbl[iTypePos.top()].m_iLink = SymbolTbl.ProcInfoTbl.size();
	ProcInfo tmp;
	tmp.m_eRank = ProcInfo::SUB;
	tmp.m_eType = ProcInfo::FUNCPOINTER;
	tmp.strName = "_noname";
	SymbolTbl.ProcInfoTbl.push_back(tmp);
	iTypeFlag.push(5);
	return true;
}
/*
函数类型       → function  022 形参列表:038 类型 039
*/
bool semantic022() 
{
	SymbolTbl.TypeInfoTbl[iTypePos.top()].m_eDataType = StoreType::T_FUNC;
	SymbolTbl.TypeInfoTbl[iTypePos.top()].m_iLink = SymbolTbl.ProcInfoTbl.size();
	ProcInfo tmp;
	tmp.m_eRank = ProcInfo::SUB;
	tmp.m_eType = ProcInfo::FUNCPOINTER;
	tmp.strName = "_noname";
	SymbolTbl.ProcInfoTbl.push_back(tmp);
	iTypeFlag.push(6);
	return true;
}
//类型				-> 015 标识符 023 010
bool semantic023() 
{
	string strTmp = TOKENNAME;
	int i = SymbolTbl.SearchTypeInfoTbl(SymbolTbl.ProcStack.top(), strTmp);

	if (i == -1)
	{
		i = SymbolTbl.SearchTypeInfoTbl(0, strTmp);
	}
	if (i != -1)
	{
		if (iTypeFlag.top() == 2)
		{
			int j = i;
			string strTmp1;
			while (SymbolTbl.TypeInfoTbl[j].m_eDataType == StoreType::T_USER)
			{
				if (SymbolTbl.TypeInfoTbl[j].m_iLink == -1)
				{
					EmitError("系统分析出错", TokenList.at(iListPos - 1));
					return false;
				}
				strTmp1 = SymbolTbl.TypeInfoTbl[SymbolTbl.TypeInfoTbl[j].m_iLink].strName;
				j = SymbolTbl.SearchTypeInfoTbl(SymbolTbl.ProcStack.top(), strTmp1);
			}
			if (SymbolTbl.TypeInfoTbl[j].m_eBaseType == StoreType::T_BOOLEAN
				|| SymbolTbl.TypeInfoTbl[j].m_eBaseType == StoreType::T_BYTE
				|| SymbolTbl.TypeInfoTbl[j].m_eBaseType == StoreType::T_CHAR
				|| SymbolTbl.TypeInfoTbl[j].m_eBaseType == StoreType::T_SHORTINT
				|| SymbolTbl.TypeInfoTbl[j].m_eBaseType == StoreType::T_SMALLINT
				|| SymbolTbl.TypeInfoTbl[j].m_eBaseType == StoreType::T_ENUM
				)
			{
				SymbolTbl.TypeInfoTbl[iTypePos.top()].m_iLink = i;
				return true;
			}
			else
			{
				EmitError("集合类型的基类型不正确", TokenList.at(iListPos - 1));
				return false;
			}
		}
		if (iTypeFlag.top() == 5 || iTypeFlag.top() == 6)
		{
			for (int j = SymbolTbl.ProcInfoTbl.back().m_vParaTbl.size() - 1; j >= 0; j--)
			{
				if (SymbolTbl.ProcInfoTbl.back().m_vParaTbl.at(j).iParaType == -1)
				{
					SymbolTbl.ProcInfoTbl.back().m_vParaTbl.at(j).iParaType = i;
				}
			}
			return true;
		}
		if (iTypeFlag.top() == 7)
		{
			SymbolTbl.ProcInfoTbl.back().m_iReturnType = i;
			return true;
		}
		SymbolTbl.TypeInfoTbl[iTypePos.top()].m_iLink = i;
		SymbolTbl.TypeInfoTbl[iTypePos.top()].m_eDataType = StoreType::T_USER;
		return true;
	}
	else
	{
		EmitError(TOKENNAME.append("类型未定义或定义不完整"), TokenList.at(iListPos - 1));
		return false;
	}

	return true;
}
//集合类型        		→set 024 of 类型 025
//iTypeFlag=2
bool semantic024()
{
	SymbolTbl.TypeInfoTbl[iTypePos.top()].m_eDataType = StoreType::T_SET;
	iTypeFlag.push(2);
	return true;
}
//集合类型        		→set 024 of 类型 025
bool semantic025() 
{
	iTypeFlag.pop();
	return true;
}
//废弃
bool semantic026() {return true;}
//文件类型			-> file of 027 类型 028
bool semantic027() 
{
	SymbolTbl.TypeInfoTbl[iTypePos.top()].m_eDataType = StoreType::T_FILE;
	iTypeFlag.push(4);
	return true;
}

//文件类型			-> file of 027 类型 028
bool semantic028() 
{
	iTypeFlag.pop();
	return true;

}
/*
字段列表			-> 字段固定部分 字段列表
-> 字段可变部分 字段列表
-> ε
字段固定部分		-> 标识符列表  : 类型 ;
字段可变部分		-> case  标识符 029 : 类型  of 033 字段可变部分１ 034 end 030 ;
*/
bool semantic029() 
{
	FieldInfo fiTmp;
	fiTmp.strName = TOKENNAME;
	fiTmp.m_iState = 0;
	fiTmp.iProcIndex = PID;
	for (int i = SymbolTbl.TypeInfoTbl.at(iTypePos.top()).m_FieldInfo.size() - 1; i >= 0; i--)
	{
		string strTmp = SymbolTbl.TypeInfoTbl.at(iTypePos.top()).m_FieldInfo.at(i).strName;
		if (strTmp.compare(fiTmp.strName) == 0)
		{
			EmitError(TOKENNAME.append("域名已经存在"), TokenList.at(iListPos - 1));
			return false;
		}
	}
	szVarFieldFlag.push(fiTmp.strName);
	SymbolTbl.TypeInfoTbl.at(iTypePos.top()).m_FieldInfo.push_back(fiTmp);
	return true;
}
/*
字段列表			-> 字段固定部分 字段列表
-> 字段可变部分 字段列表
-> ε
字段固定部分		-> 标识符列表  : 类型 ;
字段可变部分		-> case  标识符 029 : 类型  of 033 字段可变部分１ 034 end 030 ;
*/
bool semantic030() 
{
	szVarFieldFlag.pop();
	return true;
}
// 字段可变部分１	-> 常量 031 : ( 字段列表 ) 032 ; 字段可变部分１
bool semantic031() 
{
	int i = atoi(TOKENNAME.c_str());
	szVarFieldConst.push(SymbolTbl.ConstInfoTbl[i].strVal);
	return true;
}
// 字段可变部分１	-> 常量 031 : ( 字段列表 ) 032 ; 字段可变部分１
bool semantic032() 
{
	szVarFieldConst.pop();
	return true;
}
bool semantic033() 
{
	iIdListFlag.push(3);
	return true;
}
// 字段可变部分		-> case  标识符 029 : 类型  of 033 字段可变部分１ 034 end 030 ;
bool semantic034() 
{
	iIdListFlag.pop();
	return true;
}
/*
过程头          		→ procedure 标识符 041 形参列表 039
函数头          		→ function  标识符 042 形参列表：038 类型 039 039 067
形参列表        		→ ( 参数说明 参数部分 )
						→ ε
参数部分        		→ ; 参数说明 参数部分
						→ ε
参数说明        		→ 035 标识符列表 036 : 类型
*/
bool semantic035() 
{
	iIdListFlag.push(4);


	return true;
}
bool semantic036() 
{
	iIdListFlag.pop();
	return true;
}
/*
形参列表			-> ( 参数说明 参数部分 )
-> ε
参数部分			-> ; 参数说明 参数部分
-> ε
参数说明			-> var 037 标识符列表 036 : 类型
*/
bool semantic037() 
{
	iIdListFlag.push(5);
	return true;
}
// 函数类型			-> function 022 形参列表 : 038  类型 039 039 044
bool semantic038() 
{
	iTypeFlag.push(7);
	return true;
}
// 函数类型			-> function 022 形参列表 : 038  类型 039 039 044
bool semantic039() 
{
	iTypeFlag.pop();
	return true;
}

// 变量定义			-> 013 标识符列表 040 : 类型
bool semantic040() 
{
	TypeInfo tmp;
	tmp.strName = "_noname";
	tmp.strName.append(GetSerialId());
	tmp.iProcIndex = PID;
	tmp.m_iLink = -1;
	SymbolTbl.AddType(tmp);
	iTypePos.push(SymbolTbl.TypeInfoTbl.size() - 1);
	iIdListFlag.pop();
	return true;
}
// 过程头			-> procedure 标识符 041 形参列表 039
bool semantic041()
{
	ProcInfo tmp;
	tmp.m_eRank = ProcInfo::SUB;
	tmp.m_eType = ProcInfo::PROCEDURE;
	tmp.m_strProcName = TOKENNAME;
	if (SymbolTbl.SearchTypeInfoTbl(0, TOKENNAME, false) != -1
		&& SymbolTbl.SearchConstTbl(0, TOKENNAME) != -1
		&& SymbolTbl.SearchLabelInfoTbl(0, TOKENNAME) != -1
		&& SymbolTbl.SearchEnumInfoTbl(0, TOKENNAME) != -1)
	{
		EmitError(TOKENNAME.append("标识符已经存在"), TokenList.at(iListPos - 1));
		return false;
	}
	int i = SymbolTbl.SearchProcInfoTbl(TOKENNAME);

	if (i == -1 || (i != -1 && SymbolTbl.ProcInfoTbl.at(i).m_eFlag) == ProcInfo::Forward)
	{
		if (i == -1 && SymbolTbl.SearchVarInfoTbl(0, tmp.strName) != -1)
		{
			EmitError(TOKENNAME.append("标识符已经存在"), TokenList.at(iListPos - 1));
			return false;
		}
		TypeInfo tiTmp;
		tiTmp.m_iLink = SymbolTbl.ProcInfoTbl.size();
		tiTmp.m_eDataType = StoreType::T_PROC;
		tiTmp.strName = "_noname";
		tiTmp.strName.append(GetSerialId());
		SymbolTbl.AddType(tiTmp);
		iTypePos.push(SymbolTbl.TypeInfoTbl.size() - 1);
		SymbolTbl.ProcInfoTbl.push_back(tmp);
		VarInfo varTmp;
		varTmp.strName = TOKENNAME;
		varTmp.iProcIndex = 0;
		varTmp.m_eRank = VarInfo::VAR;
		varTmp.m_iTypeLink = SymbolTbl.TypeInfoTbl.size() - 1;
		SymbolTbl.AddVar(varTmp);

		iTypeFlag.push(5);

		if (i != -1 && SymbolTbl.ProcInfoTbl.at(i).m_eFlag == ProcInfo::Forward)
		{
			bForwardFlag = true;
		}
		return true;	

	}
	else
	{
		EmitError(TOKENNAME.append("标识符已经存在"), TokenList.at(iListPos - 1));
		return false;
	}

	return true;
}
bool semantic042() 
{
	ProcInfo tmp;
	tmp.m_eRank = ProcInfo::SUB;
	tmp.m_eType = ProcInfo::FUNCTION;
	tmp.m_strProcName = TOKENNAME;
	if (SymbolTbl.SearchTypeInfoTbl(0, TOKENNAME, false) != -1
		&& SymbolTbl.SearchConstTbl(0, TOKENNAME) != -1
		&& SymbolTbl.SearchLabelInfoTbl(0, TOKENNAME) != -1
		&& SymbolTbl.SearchEnumInfoTbl(0, TOKENNAME) != -1)
	{
		EmitError(TOKENNAME.append("标识符已经存在"), TokenList.at(iListPos - 1));
		return false;
	}
	int i = SymbolTbl.SearchProcInfoTbl(TOKENNAME);

	if (i == -1 || (i != -1 && SymbolTbl.ProcInfoTbl.at(i).m_eFlag) == ProcInfo::Forward)
	{
		if (i == -1 && SymbolTbl.SearchVarInfoTbl(0, tmp.strName) != -1)
		{
			EmitError(TOKENNAME.append("标识符已经存在"), TokenList.at(iListPos - 1));
			return false;
		}
		TypeInfo tiTmp;
		tiTmp.m_iLink = SymbolTbl.ProcInfoTbl.size();
		tiTmp.m_eDataType = StoreType::T_FUNC;
		tiTmp.strName = "_noname";
		tiTmp.strName.append(GetSerialId());
		SymbolTbl.AddType(tiTmp);
		iTypePos.push(SymbolTbl.TypeInfoTbl.size() - 1);
		SymbolTbl.ProcInfoTbl.push_back(tmp);
		VarInfo varTmp;
		varTmp.strName = TOKENNAME;
		varTmp.iProcIndex = 0;
		varTmp.m_eRank = VarInfo::VAR;
		varTmp.m_iTypeLink = SymbolTbl.TypeInfoTbl.size() - 1;
		SymbolTbl.AddVar(varTmp);

		iTypeFlag.push(6);

		if (i != -1 && SymbolTbl.ProcInfoTbl.at(i).m_eFlag == ProcInfo::Forward)
		{
			bForwardFlag = true;
		}
		return true;

	}
	else
	{
		EmitError(TOKENNAME.append("标识符已经存在"), TokenList.at(iListPos - 1));
		return false;
	}

	return true;

}
//过程函数声明部分１		-> 043 044 程序块 047
bool semantic043() 
{
	if (bForwardFlag)
	{

	}
	return true;
}
//过程函数声明部分１		-> 来源指向 044 047
bool semantic044()
{
	int j = 0;
	for (int i = 0; i < SymbolTbl.ProcInfoTbl.back().m_vParaTbl.size(); i++)
	{
		VarInfo viTmp;
		viTmp.m_iTypeLink = SymbolTbl.ProcInfoTbl.back().m_vParaTbl.at(i).m_iParaVar;
		//StoreType paraType=SymbolTbl.TypeInfoTbl[CType::GetRealType(viTmp.m_iTypeLink)]

	}
	return true;
}
bool semantic045() {return true;}
bool semantic046() {return true;}
/*
过程函数声明部分１			-> 043 044 程序块 047
-> 来源指向 044 047
*/
bool semantic047() 
{
	SymbolTbl.ProcStack.pop();
	return true;
}
// 程序块					-> 说明部分 048 语句部分
bool semantic048() 
{

	return true;
}
bool semantic049() {return true;}
bool semantic050() {return true;}
bool semantic051() {return true;}
bool semantic052() {return true;}
bool semantic053() {return true;}
bool semantic054() {return true;}
bool semantic055() {return true;}
bool semantic056() {return true;}
bool semantic057() {return true;}
bool semantic058() {return true;}
bool semantic059() {return true;}
bool semantic060() {return true;}
bool semantic061() {return true;}
bool semantic062() {return true;}
bool semantic063() {return true;}
bool semantic064() {return true;}
bool semantic065() {return true;}
bool semantic066() {return true;}
//函数头	-> function 标识符 042 形参列表 : 038 类型 039 039 067
bool semantic067() 
{
	if (SymbolTbl.ProcInfoTbl.back().m_iReturnType == -1)
	{
		EmitError("函数返回类型只能是基本类型或用户自定义类型", TokenList.at(iListPos - 1));
		return false;
	}
	else
	{
		VarInfo viTmp;
		viTmp.strName = "RESULT";
		viTmp.iProcIndex = PID;
		viTmp.m_iTypeLink = SymbolTbl.ProcInfoTbl.back().m_iReturnType;
		viTmp.m_eRank = VarInfo::Rank::RET;
		SymbolTbl.AddVar(viTmp);
	}
	return true;
}
bool semantic068() {return true;}
bool semantic069() {return true;}
bool semantic070() {return true;}
bool semantic071() {return true;}
bool semantic072() {return true;}
bool semantic073() {return true;}
bool semantic074() {return true;}
bool semantic075() {return true;}
bool semantic076() {return true;}
bool semantic077() {return true;}
bool semantic078() {return true;}
bool semantic079() {return true;}
bool semantic080() {return true;}
bool semantic081() {return true;}
bool semantic082() {return true;}
bool semantic083() {return true;}
bool semantic084() {return true;}
bool semantic085() {return true;}
bool semantic086() {return true;}
bool semantic087() {return true;}
bool semantic088() {return true;}
bool semantic089() {return true;}
bool semantic090() {return true;}
bool semantic091() {return true;}
bool semantic092() {return true;}
//包含文件说明	-> uses 093 标识符列表 094 ;
bool semantic093() 
{
	iIdListFlag.push(7);
	return true;
}
//包含文件说明	-> uses 093 标识符列表 094 ;
bool semantic094() 
{
	iIdListFlag.pop();
	return true;
}
bool semantic095() {return true;}
bool semantic096() {return true;}
bool semantic097() {return true;}
bool semantic098() {return true;}
bool semantic099() {return true;}
bool semantic100() {return true;}
bool semantic101() {return true;}
bool semantic102() {return true;}
