#include "CTextStringManager.h"
using namespace std;

// Converts a code name to a readable user string
const n_wstring* CTextStringManager::GetString(const wchar_t* CodeName)
{
	return reinterpret_cast<n_wstring*(__thiscall*)(CTextStringManager*, const wchar_t*)>(0x008C9C30)(this, CodeName);
}

// Converts a name id to a readable user string
const n_wstring CTextStringManager::GetString(const n_wstring NameStrID)
{
	return reinterpret_cast<const n_wstring &(__thiscall *)(CTextStringManager *, const n_wstring &)>(0x008c9bb0)(this, NameStrID);
}