#pragma once
#include "CString.h"

// Reflects a player entity in game
class CICPlayer {
public:
	unsigned char pad01[124];
	short m_Region;
	unsigned char pad02[122];
	// Unique ID identifing this object
	uint32_t m_EntityUniqueId;
	unsigned char pad03[1876];
	n_wstring m_Charname;
	unsigned char m_Level;
	unsigned char pad04[7];
	long long int m_ExpCurrent;
	unsigned char pad05[6304];
};

// The current player selected in game
#define g_CICPlayer (*((CICPlayer**)0x00EEF5EC))