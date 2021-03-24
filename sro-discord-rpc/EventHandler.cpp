#include "EventHandler.h"
#include <iostream>
#include <sstream>
#include "AppManager.h"
#include "Lib/discord-game-sdk/discord.h"
#include "ICPlayer.h"
#include "GlobalHelpers.h"

/// Static stuffs
bool EventHandler::m_IsRegionChanged;

// Called on every packet received after character selection
int EventHandler::OnPacketRecv(MsgStreamBuffer* MsgBuffer)
{
	// Filtering opcodes
	switch (MsgBuffer->Opcode)
	{
	case 0x3020: // SERVER_ENVIROMENT_CELESTIAL_POSITION
	{
		// Called after every teleport
		AppManager::UpdateGameState(IN_GAME);
	}
	break;
	case 0x3054: // SERVER_ENTITY_LEVEL_UP
	{
		// Check if my character level up
		if (MsgBuffer->Read<uint32_t>() == g_CICPlayer->m_EntityUniqueId)
		{
			AppManager::UpdateGameState();
		}
	}
	break;
	case 0xB021: // SERVER_ENTITY_MOVEMENT
	{
		// Check my character movements
		if (MsgBuffer->Read<uint32_t>() == g_CICPlayer->m_EntityUniqueId)
		{
			// Check if new zone has been triggered
			if (m_IsRegionChanged)
			{
				m_IsRegionChanged = false;
				AppManager::UpdateGameState();
			}
			// Read packet
			byte hasCoordinate = MsgBuffer->Read<byte>() == 1;
			if (hasCoordinate)
			{
				short newRegionID = MsgBuffer->Read<short>();
				// Check if the region will be changed in the next step
				if (g_CICPlayer->m_Region != newRegionID)
				{
					// We guess the next movement will be from a new region
					m_IsRegionChanged = true;
				}
			}
		}
	}
	break;
	case 0x34B4: // SERVER_GUILD_DATA_END
	{
		int result = reinterpret_cast<int(__thiscall*)(EventHandler*, MsgStreamBuffer*)>(0x0084CAB0)(this, MsgBuffer);
		// Guild data has been processed, update guild name
		AppManager::UpdateGameState();
		// return normally
		return result;
	}
	break;
	}

	// Reset packet index, because client will use the same buffer
	MsgBuffer->TotalReadBytes = 0;
	return reinterpret_cast<int(__thiscall*)(EventHandler*, MsgStreamBuffer*)>(0x0084CAB0)(this, MsgBuffer);
}

// Called on login screen from server selection
int EventHandler::OnNetMsg(MsgStreamBuffer* MsgBuffer)
{
	// SERVER_SHARD_LIST_RESPONSE = 0xA101 but coded
	if (MsgBuffer->Opcode == 0x0FF2)
	{
		AppManager::UpdateGameState(SERVER_SELECTION);
	}
	return reinterpret_cast<int(__thiscall*)(EventHandler*, MsgStreamBuffer*)>(0x0086BFC0)(this, MsgBuffer);
}

// Called on every action from character selection
int EventHandler::OnCharacterSelection(MsgStreamBuffer* MsgBuffer)
{
	// Just to be sure...
	if (MsgBuffer->Opcode == 0xB007)
	{
		byte action = MsgBuffer->Read<byte>();
		// Update on character list display 
		if (action == 2)
		{
			AppManager::UpdateGameState(CHARACTER_SELECTION);
		}
		// Reset packet index, because client will use the same buffer
		MsgBuffer->TotalReadBytes = 0;
	}
	return reinterpret_cast<int(__thiscall*)(EventHandler*, MsgStreamBuffer*)>(0x0085FC60)(this, MsgBuffer);
}