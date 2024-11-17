#include "stdafx.h"
#include "Type.h"
StoreType TokenToType(int iTokenKind)
{

	switch (iTokenKind)
	{
	case 49:return StoreType::T_CHAR;		break;
	case 44:return StoreType::T_BOOLEAN;	break;
	case 64:return StoreType::T_INTEGER;	break;
	case 45:return StoreType::T_BYTE;		break;
	case 79:return StoreType::T_SHORTINT;	break;
	case 82:return StoreType::T_SMALLINT;	break;
	case 92:return StoreType::T_WORD;		break;
	case 66:return StoreType::T_LONGWORD;	break;
	case 47:return StoreType::T_CARDINAL;	break;
	case 74:return StoreType::T_REAL;		break;
	case 81:return StoreType::T_SINGLE;		break;
	case 83:return StoreType::T_STRING;		break;
	}
}