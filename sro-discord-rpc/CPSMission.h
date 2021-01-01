#pragma once
#include "CMsgStreamBuffer.h"
class CPSMission
{
public:
	int OnPacketRecv(class MsgStreamBuffer* MsgBuffer);
	int OnNetMsg(class MsgStreamBuffer* MsgBuffer);
	int OnCharacterSelection(class MsgStreamBuffer* MsgBuffer);
};