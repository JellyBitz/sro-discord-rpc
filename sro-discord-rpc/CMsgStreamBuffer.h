#pragma once
#include <Windows.h>
#include <iostream>

class MsgStreamBuffer
{
private:
	int unk_0000; //0x0000

public:
	std::size_t TotalReadBytes; //0x0004
	std::size_t TotalBytes; //0x0008
	bool IsOpen; //0x000C
	BYTE unk_000D; //0x000D
	BYTE unk_000E; //0x000E
	BYTE unk_000F; //0x000F
	int unk_0010; //0x0010
	int unk_0014; //0x0014
	WORD Opcode; //0x0018
	bool Sent; //0x001A
	char pad_001B; //0x001B
	int pad_001C; //0x001C

	MsgStreamBuffer(void);
	MsgStreamBuffer(const MsgStreamBuffer& SRC);
	MsgStreamBuffer(WORD Opcode);
	~MsgStreamBuffer(void);
	void Open();
	void Close();

	template<typename T>
	T Read()
	{
		T Data;
		reinterpret_cast<void(__thiscall*)(MsgStreamBuffer*, T*, int)>(0x004F7220)(this, &Data, sizeof(T));
		return Data;
	}

	template<typename T>
	T Read(std::size_t size)
	{
		T Data;
		reinterpret_cast<void(__thiscall*)(MsgStreamBuffer*, T*, int)>(0x004F7220)(this, &Data, size);
		return Data;
	}

	BYTE* ReadBytes(std::size_t size)
	{
		BYTE* Data = new BYTE[size + 1];
		reinterpret_cast<void(__thiscall*)(MsgStreamBuffer*, BYTE*, int)>(0x004F7220)(this, Data, size);
		return Data;
	}

	template<typename T>
	void Write(T Data)
	{
		reinterpret_cast<void(__thiscall*)(MsgStreamBuffer*, T*, int)>(0x00508FE0)(this, &Data, sizeof(Data));
	}

	template<typename T>
	void Write(T Data, std::size_t size)
	{
		reinterpret_cast<void(__thiscall*)(MsgStreamBuffer*, T*, int)>(0x00508FE0)(this, &Data, size);
	}

	std::string ReadStringA();
	std::wstring ReadStringW();
	void WriteStringA(const char* str);
	void WriteStringW(const wchar_t* wcs);
	static void SendMsg(MsgStreamBuffer* buffer);
	static bool sub_841780(WORD Opcode, int a2);
};