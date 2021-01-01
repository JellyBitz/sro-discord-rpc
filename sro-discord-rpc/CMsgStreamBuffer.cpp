#include "CMsgStreamBuffer.h"
#include "hook.h"

MsgStreamBuffer::MsgStreamBuffer(void)
{
	this->unk_0000 = NULL;
	this->TotalReadBytes = NULL;
	this->TotalBytes = NULL;
	this->IsOpen = NULL;
	this->unk_000D = NULL;
	this->unk_000E = NULL;
	this->unk_000F = NULL;
	this->unk_0010 = NULL;
	this->unk_0014 = NULL;
	this->Opcode = NULL;
	this->Sent = NULL;
	this->pad_001B = NULL;
	this->pad_001C = NULL;
}

MsgStreamBuffer::MsgStreamBuffer(const MsgStreamBuffer& SRC)
{
	this->unk_0000 = SRC.unk_0000;
	this->TotalReadBytes = SRC.TotalReadBytes;
	this->TotalBytes = SRC.TotalBytes;
	this->IsOpen = SRC.IsOpen;
	this->unk_000D = SRC.unk_000D;
	this->unk_000E = SRC.unk_000E;
	this->unk_000F = SRC.unk_000F;
	this->unk_0010 = SRC.unk_0010;
	this->unk_0014 = SRC.unk_0014;
	this->Opcode = SRC.Opcode;
	this->Sent = SRC.Sent;
	this->pad_001B = SRC.pad_001B;
	this->pad_001C = SRC.pad_001C;
}

MsgStreamBuffer::MsgStreamBuffer(WORD opcode)
{
	reinterpret_cast<void(__thiscall*)(MsgStreamBuffer*, WORD)>(0x0053CEA0)(this, opcode);
}

MsgStreamBuffer::~MsgStreamBuffer(void)
{

}

void MsgStreamBuffer::Open()
{
	if (!this->IsOpen)
	{
		int alter = this->unk_0010;
		this->IsOpen = true;
		this->unk_0014 = alter;
		this->TotalBytes = 0;
	}
}

void MsgStreamBuffer::Close()
{
	if (this->IsOpen)
	{
		int alter = this->unk_0010;
		this->IsOpen = false;
		this->unk_0014 = alter;
		this->TotalReadBytes = 0;
	}
}

std::string MsgStreamBuffer::ReadStringA()
{
	WORD str_len = this->Read<WORD>();
	std::string str(str_len, 0);
	reinterpret_cast<void(__thiscall*)(MsgStreamBuffer*, char*, int)>(0x004F7220)(this, &str[0], str_len);
	return str;
}

std::wstring MsgStreamBuffer::ReadStringW()
{
	WORD wcs_len = this->Read<WORD>();
	std::wstring wcs(wcs_len, 0);
	reinterpret_cast<void(__thiscall*)(MsgStreamBuffer*, wchar_t*, int)>(0x004F7220)(this, &wcs[0], wcs_len * 2);
	return wcs;
}

void MsgStreamBuffer::WriteStringA(const char* str)
{
	WORD len = strlen(str);
	reinterpret_cast<void(__thiscall*)(MsgStreamBuffer*, WORD*, int)>(0x00508FE0)(this, &len, sizeof(WORD));
	reinterpret_cast<void(__thiscall*)(MsgStreamBuffer*, const char*, int)>(0x00508FE0)(this, str, len);
}

void MsgStreamBuffer::WriteStringW(const wchar_t* wcs)
{
	WORD len = wcslen(wcs) * 2;
	reinterpret_cast<void(__thiscall*)(MsgStreamBuffer*, WORD*, int)>(0x00508FE0)(this, &len, sizeof(WORD));
	reinterpret_cast<void(__thiscall*)(MsgStreamBuffer*, const wchar_t*, int)>(0x00508FE0)(this, wcs, len);
}

void MsgStreamBuffer::SendMsg(MsgStreamBuffer* buffer)
{
	reinterpret_cast<void(__cdecl*)(MsgStreamBuffer*)>(0x008418D0)(buffer);
}

bool MsgStreamBuffer::sub_841780(WORD Opcode, int a2)
{
	return reinterpret_cast<bool(__cdecl*)(WORD, int)>(0x00841780)(Opcode, a2);
}