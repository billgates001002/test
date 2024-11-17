#include "stdafx.h"

CSymbolTbl SymbolTbl;

int iListPos;
vector<Token> TokenList;				//Êä³öµ¥´ÊÁ÷


SemanticFunc FuncList[200] = { NULL,semantic001,semantic002,semantic003,semantic004,semantic005,semantic006,semantic007,semantic008,semantic009,semantic010,
semantic011,semantic012,semantic013,semantic014,semantic015,semantic016,semantic017,semantic018,semantic019,semantic020,
semantic021,semantic022,semantic023,semantic024,semantic025,semantic026,semantic027,semantic028,semantic029,semantic030,
semantic031,semantic032,semantic033,semantic034,semantic035,semantic036,semantic037,semantic038,semantic039,semantic040,
semantic041,semantic042,semantic043,semantic044,semantic045,semantic046,semantic047,semantic048,semantic049,semantic050,
semantic051,semantic052,semantic053,semantic054,semantic055,semantic056,semantic057,semantic058,semantic059,semantic060,
semantic061,semantic062,semantic063,semantic064,semantic065,semantic066,semantic067,semantic068,semantic069,semantic070,
semantic071,semantic072,semantic073,semantic074,semantic075,semantic076,semantic077,semantic078,semantic079,semantic080,
semantic081,semantic082,semantic083,semantic084,semantic085,semantic086,semantic087,semantic088,semantic089,semantic090,
semantic091,semantic092,semantic093,semantic094,semantic095,semantic096,semantic097,semantic098,semantic099,semantic100,
semantic101,semantic102 };