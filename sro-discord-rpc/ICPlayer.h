#pragma once
#include "CString.h"

// Possible types for job mode
enum JOB_TYPE : char {
	TRADER = 0,
	THIEF = 1,
	HUNTER = 3,
	NONE = 4
};

// Reflects a player entity in game
class CICPlayer
{
public:
	/// Entity
	unsigned char pad_0x00[124];
	unsigned short m_Region;
	unsigned char pad_0x7E[122];
	uint32_t m_EntityUniqueId;
	unsigned char pad_0xFC[344];
	/// Character
	unsigned char pad_0x254[697];
	JOB_TYPE m_JobType;
	unsigned char pad_0x50E[638];
	/// User
	unsigned char pad_0x78C[160];
	/// Player
	unsigned char pad_0x82C[32];
	n_wstring m_Charname;
	unsigned char m_Level;
	unsigned char pad_0x869[6319];
public:
	// Returns the guild name or an empty string
	const n_wstring GetGuildName()
	{
		return reinterpret_cast<const n_wstring&(__thiscall*)(CICPlayer*)>(0x009DB100)(this);
	};
};

// The current player selected in game
#define g_CICPlayer (*((CICPlayer**)0x00EEF5EC))