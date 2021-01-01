#pragma once
#include "CString.h"

class CTextStringManager
{
public:
	const n_wstring* GetString(const wchar_t* CodeName);
	const n_wstring GetString(const n_wstring NameStrID);
};

#define g_CTextStringManager ((CTextStringManager*)0x00EEDE90)

#define TSM_GETTEXTPTR(text) (g_CTextStringManager->GetString(L##text)->c_str())
#define TSM_GETTEXT(text) (g_CTextStringManager->GetString(L##text))