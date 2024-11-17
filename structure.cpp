#include "stdafx.h"
#include "Structure.h"
FieldInfo::FieldInfo()
{
	m_iLink = -1;
	m_iSize = 0;
	m_szVarFieldConst = "";
	m_szVarFieldFlag = "";
	m_iState = 0;
	m_iOffset = 0;
}
VarInfo::VarInfo()
{

}
ArrayInfo::ArrayInfo()
{

}

TypeInfo::TypeInfo()
{
	m_iLink = -1;
	m_szContent = "";
	m_eDataType = StoreType::T_NONE;
	m_eBaseType = StoreType::T_NONE;
	m_iState = 0;
	m_iSize = 0;

}


