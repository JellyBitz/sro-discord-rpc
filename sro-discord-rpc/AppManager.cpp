#include "AppManager.h"
#include "hook.h"
#include <Windows.h>
#include <iostream> // cout stuffs
#include <sstream>
#include <csignal>
#include "GlobalHelpers.h"
#include "EventHandler.h"
#include "CTextStringManager.h"
#include "CString.h"
#include "ICPlayer.h"

/// Static stuffs
DiscordClient AppManager::DiscordClient;
GAME_STATE AppManager::m_GameState;
time_t AppManager::m_InGameTimestamp;

// Initialize core of the application
void AppManager::Initialize()
{
	// Attach console
	//AllocConsole();
	//freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

	AppManager::InitHooks();
	HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AppManager::InitDiscord, 0, 0, 0);
	// Set discord stuffs as background process (below normal)
	SetThreadPriority(hThread, -1);
}

// Set the respectives hooks
void AppManager::InitHooks()
{
	replaceAddr(0x00DD92BC + 0x18, addr_from_this(&EventHandler::OnNetMsg));
	replaceAddr(0x00DD811C + 0x18, addr_from_this(&EventHandler::OnCharacterSelection));
	replaceAddr(0x00DD440C, addr_from_this(&EventHandler::OnPacketRecv));
}

void OnUserUpdated(void* data)
{
	struct DiscordClient* app = (struct DiscordClient*)data;
	struct DiscordUser user;
	app->users->get_current_user(app->users, &user);
	app->currentUser = user;

	printf("Connected user: %s#%s\r\n", user.username, user.discriminator);

	// Update state after connection
	AppManager::UpdateGameState();
}

void SignalInterrupt(int code)
{
	AppManager::UpdateGameState(FINISH);
}

// Initialize the rich presence for the current game in a independant thread
DWORD WINAPI AppManager::InitDiscord()
{
	// Create and set instance
	memset(&DiscordClient, 0, sizeof(DiscordClient));

	// Create and set events
	struct IDiscordUserEvents users_events;
	memset(&users_events, 0, sizeof(users_events));
	users_events.on_current_user_update = OnUserUpdated;

	// Set params (options)
	struct DiscordCreateParams params;
	DiscordCreateParamsSetDefault(&params);
	params.client_id = 792263872274366504;
	params.flags = DiscordCreateFlags_NoRequireDiscord;
	params.event_data = &DiscordClient;
	params.user_events = &users_events;

	// Try to create app connection
	do
	{
		EDiscordResult result = DiscordCreate(DISCORD_VERSION, &params, &DiscordClient.core);
		if (m_GameState == FINISH)
			return 0;
		if (result == DiscordResult_Ok)
			break;
		// wait delay to try again
		Sleep(30000);
	} while (true);

	// Set managers
	DiscordClient.users = DiscordClient.core->get_user_manager(DiscordClient.core);
	DiscordClient.achievements = DiscordClient.core->get_achievement_manager(DiscordClient.core);
	DiscordClient.activities = DiscordClient.core->get_activity_manager(DiscordClient.core);
	DiscordClient.application = DiscordClient.core->get_application_manager(DiscordClient.core);
	DiscordClient.lobbies = DiscordClient.core->get_lobby_manager(DiscordClient.core);

	// Make sure to stop this thread
	signal(SIGINT, SignalInterrupt);

	// Update callbacks
	while (m_GameState != FINISH)
	{
		DiscordClient.core->run_callbacks(DiscordClient.core);
		Sleep(2500);
	}

	return 0;
}

// Updates the game state and discord rich presence associated
void AppManager::UpdateGameState(GAME_STATE State)
{
	// Catch first INGAME state
	if (m_GameState != IN_GAME && State == IN_GAME)
	{
		m_InGameTimestamp = std::time(0);
	}
	m_GameState = State;
	UpdateDiscord();
}

void UpdateActivityCallback(void* data, enum EDiscordResult result)
{
	if (result != DiscordResult_Ok)
	{
		printf("Discord Activity ( ErrCode %d )\r\n", result);
	}
}

// Updates the game state and discord rich presence associated
void AppManager::UpdateGameState()
{
	UpdateGameState(m_GameState);
}

// Updates discord rich presence
void AppManager::UpdateDiscord()
{
	// Make sure client is not exiting
	if (m_GameState != FINISH)
	{
		// Check if discord has been initialized
		if (DiscordClient.core)
		{
			struct DiscordActivity activity;
			memset(&activity, 0, sizeof(activity));
			// Game state
			switch (m_GameState)
			{
			case LOADING:
				sprintf(activity.state, "Loading");
				sprintf(activity.assets.large_image, "logo");
				sprintf(activity.assets.large_text, "SilkroadOnline.com");
				break;
			case SERVER_SELECTION:
				sprintf(activity.state, "Selecting Server");
				sprintf(activity.assets.large_image, "logo");
				sprintf(activity.assets.large_text, "SilkroadOnline.com");
				break;
			case CHARACTER_SELECTION:
				sprintf(activity.state, "Selecting Character");
				sprintf(activity.assets.large_image, "logo");
				sprintf(activity.assets.large_text, "SilkroadOnline.com");
				break;
			case IN_GAME:
			{
				// Show job mode activity
				switch (g_CICPlayer->m_JobType)
				{
				case TRADER:
					sprintf(activity.state, "Job Mode (Trader)");
					break;
				case THIEF:
					sprintf(activity.state, "Job Mode (Thief)");
					break;
				case HUNTER:
					sprintf(activity.state, "Job Mode (Hunter)");
					break;
				default:
					sprintf(activity.state, "In Game");
					break;
				}
				// Player details
				std::stringstream details;
				details << n_wstr_to_str(g_CICPlayer->m_Charname) << " Lv." << (int)g_CICPlayer->m_Level;
				// Check guild name
				n_wstring nwGuildName = g_CICPlayer->GetGuildName();
				std::string guildName = n_wstr_to_str(nwGuildName);
				if (guildName != "")
				{
					details << " [" << guildName << "]";
				}
				sprintf(activity.details, details.str().c_str());
				// Region Name if is not in job mode
				if (g_CICPlayer->m_JobType == NONE)
				{
					std::wstringstream ssRegion;
					ssRegion << g_CICPlayer->m_Region;
					const n_wstring* nwRegionName = g_CTextStringManager->GetString(ssRegion.str().c_str());
					std::string regionName(nwRegionName->begin(), nwRegionName->end());
					sprintf(activity.assets.large_text, regionName.c_str());
					// Region image
					if (regionName == "Jangan") {
						sprintf(activity.assets.large_image, "loading_zangan");
					}
					else if (regionName == "Western China Donwhang") {
						sprintf(activity.assets.large_image, "loading_dunwhang");
					}
					else if (regionName == "Hotan") {
						sprintf(activity.assets.large_image, "loading_hotan");
					}
					else if (regionName == "Samarkand") {
						sprintf(activity.assets.large_image, "loading_samarkand");
					}
					else if (regionName == "Constantinople") {
						sprintf(activity.assets.large_image, "loading_constantinople");
					}
					else if (regionName == "Alexandria") {
						sprintf(activity.assets.large_image, "loading_alex");
					}
					else if (regionName == "Baghdad") {
						sprintf(activity.assets.large_image, "loading_baghdad_2011");
					}
					else if (regionName == "Donwhang Stone Cave") {
						sprintf(activity.assets.large_image, "loading_dungeons_donwhang");
					}
					else if (str_starts_with(regionName, "Underground Level")) {
						sprintf(activity.assets.large_image, "loading_royalmausoleum");
					}
					else if (regionName == "Pharaoh tomb") {
						sprintf(activity.assets.large_image, "loading_pharaoh");
					}
					else if (regionName == "Togui Village") {
						sprintf(activity.assets.large_image, "loading_togui_2011");
					}
					else if (regionName == "Flame Mountain") {
						sprintf(activity.assets.large_image, "loading_flame_2011");
					}
					else if (str_ends_with(regionName, " of the Shipwreck")) {
						sprintf(activity.assets.large_image, "loading_wreck_2011");
					}
					else if (regionName == "Jupiter Temple") {
						sprintf(activity.assets.large_image, "loading_jupiter_field_2011");
					}
					else if (str_ends_with(regionName, "'s Room")) {
						sprintf(activity.assets.large_image, "loading_hall_2011");
					}
					else if (regionName == "Zealots Hideout") {
						sprintf(activity.assets.large_image, "loading_hideout_2011");
					}
					else if (regionName == "Petra (Dungeon)") {
						sprintf(activity.assets.large_image, "loading_petra_2011");
					}
					else if (regionName == "Devil's Garden (Dungeon)") {
						sprintf(activity.assets.large_image, "loading_garden_2011");
					}
					else {
						sprintf(activity.assets.large_image, "loading_default");
					}
				}
				else
				{
					sprintf(activity.assets.large_image, "loading_default");
				}
				// Game icon or Character icon
				sprintf(activity.assets.small_image, "logo");
				// Game website or Character race
				sprintf(activity.assets.small_text, "SilkroadOnline.com");
				// Set Timestamp (Elapsed time)
				activity.timestamps.start = m_InGameTimestamp;
			}
			break;
			}
			// Set activity
			DiscordClient.activities->update_activity(DiscordClient.activities, &activity, DiscordClient.application, UpdateActivityCallback);
		}
	}
}
