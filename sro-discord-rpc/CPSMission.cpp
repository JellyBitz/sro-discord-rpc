#include "CPSMission.h"
#include <iostream>
#include <sstream>
#include "AppManager.h"
#include "Lib/discord-game-sdk/discord.h"
#include "CTextStringManager.h"
#include "ICPlayer.h"
#include "GlobalHelpers.h"

// Called on every packet sent to client after getting in game
int CPSMission::OnPacketRecv(MsgStreamBuffer* MsgBuffer)
{
	// Filtering
	switch (MsgBuffer->Opcode)
	{
	case 0x3020: // SERVER_ENVIROMENT_CELESTIAL_POSITION
	{
		// Called after every teleport
		AppManager::UpdateGameState(GAME_STATE::IN_GAME);
	}
	break;
	case 0x3054: // SERVER_ENTITY_LEVEL_UP
	{
		// Check if character level up
		auto uid = MsgBuffer->Read<uint32_t>();
		if (uid == g_CICPlayer->m_EntityUniqueId)
		{
			AppManager::UpdateGameState();
		}
	}
	break;
	}

	// Reset packet index, because client will use the same buffer
	MsgBuffer->TotalReadBytes = 0;
	return reinterpret_cast<int(__thiscall*)(CPSMission*, MsgStreamBuffer*)>(0x0084CAB0)(this, MsgBuffer);
}

// Called after loading on server selection
int CPSMission::OnNetMsg(MsgStreamBuffer* MsgBuffer)
{
	// SERVER_SHARD_LIST_RESPONSE = 0xA101 but coded
	if (MsgBuffer->Opcode == 0x0FF2)
	{
		AppManager::UpdateGameState(GAME_STATE::SERVER_SELECTION);
	}
	return reinterpret_cast<int(__thiscall*)(CPSMission*, MsgStreamBuffer*)>(0x0086BFC0)(this, MsgBuffer);
}

// Called on character selection
int CPSMission::OnCharacterSelection(MsgStreamBuffer* MsgBuffer)
{
	// Just to be sure...
	if (MsgBuffer->Opcode == 0xB007)
	{
		auto action = MsgBuffer->Read<byte>();
		// Update on character list display 
		if (action == 2)
		{
			AppManager::UpdateGameState(GAME_STATE::CHARACTER_SELECTION);
		}
	}

	// Reset packet index, because client will use the same buffer
	MsgBuffer->TotalReadBytes = 0;
	return reinterpret_cast<int(__thiscall*)(CPSMission*, MsgStreamBuffer*)>(0x0085FC60)(this, MsgBuffer);
}