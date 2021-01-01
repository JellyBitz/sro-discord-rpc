#pragma once
#include "CMsgStreamBuffer.h"
class EventHandler
{
private:
	static bool m_IsRegionChanged;
public:
	int OnPacketRecv(class MsgStreamBuffer* MsgBuffer);
	int OnNetMsg(class MsgStreamBuffer* MsgBuffer);
	int OnCharacterSelection(class MsgStreamBuffer* MsgBuffer);
};