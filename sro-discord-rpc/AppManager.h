#pragma once
#include <memory>
#include <Windows.h>
#include "Lib/discord-game-sdk/discord.h"
#include <ctime>

// Discord client wrapper
struct DiscordClient
{
	struct IDiscordCore* core;
	struct IDiscordUserManager* users;
	struct IDiscordAchievementManager* achievements;
	struct IDiscordActivityManager* activities;
	struct IDiscordRelationshipManager* relationships;
	struct IDiscordApplicationManager* application;
	struct IDiscordLobbyManager* lobbies;
	DiscordUser currentUser;
};
// Game status known
enum GAME_STATE : char
{
	LOADING = 0,
	FINISH = 1,
	SERVER_SELECTION = 2,
	CHARACTER_SELECTION = 3,
	IN_GAME = 4
};

// Application manager handling general info
class AppManager
{
private:
	static time_t m_InGameTimestamp;
	static GAME_STATE m_GameState;
	static void InitHooks();
	static DWORD WINAPI InitDiscord();
	static void UpdateDiscord();
public:
	// Initialize application
	static void Initialize();
	// Updates the game state and discord rich presence associated
	static void UpdateGameState();
	// Updates the game state and discord rich presence associated
	static void UpdateGameState(GAME_STATE);
	// Discord driver
	static DiscordClient DiscordClient;
};