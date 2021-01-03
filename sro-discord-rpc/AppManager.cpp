#include "AppManager.h"
#include "hook.h"
#include <iostream> // cout stuffs
#include <sstream>
#include <thread>
#include <chrono>
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
	auto hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AppManager::InitDiscord, 0, 0, 0);
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
// Initialize the rich presence for the current game in a independant thread
DWORD WINAPI AppManager::InitDiscord()
{
	discord::ClientId CLIENT_ID(792263872274366504);
	// Try to create discord api instance
	discord::Core* core{};
	int delay = 30000;
	do {
		auto result = discord::Core::Create(CLIENT_ID, DiscordCreateFlags_NoRequireDiscord, &core);
		// Check failure
		if (!core) {
			std::cout << "Discord Create (Err " << static_cast<int>(result) << ")" << std::endl;
			// Try again
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		}
	} while (!core);
	// Success
	DiscordClient.Core.reset(core);

	// Log stuffs
	DiscordClient.Core->SetLogHook(
		discord::LogLevel::Debug, [](discord::LogLevel level, const char* message) {
		std::cout << "Log(" << static_cast<uint32_t>(level) << "): " << message << std::endl;
	});

	// Called when the discord client connects to this API
	DiscordClient.Core->UserManager().OnCurrentUserUpdate.Connect([]() {
		// Set current user
		auto result = DiscordClient.Core->UserManager().GetCurrentUser(&DiscordClient.CurrentUser);

		// Check error
		if (result != discord::Result::Ok)
			std::cout << "Discord Connection (Err " << static_cast<int>(result) << ")" << std::endl;

		std::cout << "Connected user: "
			<< DiscordClient.CurrentUser.GetUsername() << "#"
			<< DiscordClient.CurrentUser.GetDiscriminator() << std::endl;

		// Updates the rich presence
		AppManager::UpdateGameState();
	});

	// Stops this thread on exit
	signal(SIGINT, [](int) {
		m_GameState = GAME_STATE::FINISH;
	});

	// Lock thread checking discord callbacks
	delay = 5000;
	while (GAME_STATE::FINISH)
	{
		DiscordClient.Core->RunCallbacks();
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}
	return 0;
}

// Updates the game state and discord rich presence associated
void AppManager::UpdateGameState(GAME_STATE State)
{
	// Catch first INGAME state
	if (m_GameState != GAME_STATE::IN_GAME && State == GAME_STATE::IN_GAME)
	{
		m_InGameTimestamp = std::time(nullptr);
	}
	m_GameState = State;
	UpdateDiscord();
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
	if (m_GameState != GAME_STATE::FINISH)
	{
		// Check if discord has been initialized
		if (DiscordClient.Core)
		{
			discord::Activity activity{};
			// Game state
			switch (m_GameState)
			{
			case GAME_STATE::LOADING:
				activity.SetState("Loading");
				activity.GetAssets().SetLargeImage("logo");
				activity.GetAssets().SetLargeText("SilkroadLatino.com");
				break;
			case GAME_STATE::SERVER_SELECTION:
				activity.SetState("Selecting Server");
				activity.GetAssets().SetLargeImage("logo");
				activity.GetAssets().SetLargeText("SilkroadLatino.com");
				break;
			case GAME_STATE::CHARACTER_SELECTION:
				activity.SetState("Selecting Character");
				activity.GetAssets().SetLargeImage("logo");
				activity.GetAssets().SetLargeText("SilkroadLatino.com");
				break;
			case GAME_STATE::IN_GAME:
			{
				// Show job mode activity
				switch (g_CICPlayer->m_JobType)
				{
				case JOB_TYPE::TRADER:
					activity.SetState("Job Mode (Trader)");
					break;
				case JOB_TYPE::THIEF:
					activity.SetState("Job Mode (Thief)");
					break;
				case JOB_TYPE::HUNTER:
					activity.SetState("Job Mode (Hunter)");
					break;
				default:
					activity.SetState("In Game");
					break;
				}
				// Player details
				std::stringstream details;
				details << n_wstr_to_str(g_CICPlayer->m_Charname) << " Lv." << (int)g_CICPlayer->m_Level;
				// Check guild name
				auto nwGuildName = g_CICPlayer->GetGuildName();
				auto guildName = n_wstr_to_str(nwGuildName);
				if (guildName != "")
				{
					details << " [" << guildName << "]";
				}
				activity.SetDetails(ss_to_str(details).c_str());
				// Region Name
				std::stringstream ssRegion;
				ssRegion << g_CICPlayer->m_Region;
				auto nwRegionName = g_CTextStringManager->GetString(ss_to_n_wstr(ssRegion));
				auto regionName = n_wstr_to_str(nwRegionName);
				activity.GetAssets().SetLargeText(regionName.c_str());
				// Region image
				if (regionName == "Jangan") {
					activity.GetAssets().SetLargeImage("loading_zangan");
				}
				else if (regionName == "Donwhang") {
					activity.GetAssets().SetLargeImage("loading_dunwhang");
				}
				else if (regionName == "Hotan") {
					activity.GetAssets().SetLargeImage("loading_hotan");
				}
				else if (regionName == "Samarkand") {
					activity.GetAssets().SetLargeImage("loading_samarkand");
				}
				else if (regionName == "Constantinople") {
					activity.GetAssets().SetLargeImage("loading_constantinople");
				}
				else if (regionName == "Alexandria") {
					activity.GetAssets().SetLargeImage("loading_alex");
				}
				else if (regionName == "Baghdad") {
					activity.GetAssets().SetLargeImage("loading_baghdad_2011");
				}
				else if (regionName == "Donwhang Stone Cave") {
					activity.GetAssets().SetLargeImage("loading_dungeons_donwhang");
				}
				else if (str_starts_with(regionName,"Underground Level")) {
					activity.GetAssets().SetLargeImage("loading_royalmausoleum");
				}
				else if (regionName == "Pharaoh tomb") {
					activity.GetAssets().SetLargeImage("loading_pharaoh");
				}
				else if (regionName == "Togui Village") {
					activity.GetAssets().SetLargeImage("loading_togui_2011");
				}
				else if (regionName == "Flame Mountain") {
					activity.GetAssets().SetLargeImage("loading_flame_2011");
				}
				else if (str_ends_with(regionName, " of the Shipwreck")) {
					activity.GetAssets().SetLargeImage("loading_wreck_2011");
				}
				else if (regionName == "Jupiter Temple") {
					activity.GetAssets().SetLargeImage("loading_jupiter_field_2011");
				}
				else if (str_ends_with(regionName,"'s Room")) {
					activity.GetAssets().SetLargeImage("loading_hall_2011");
				}
				else if ( regionName == "Zealots Hideout") {
					activity.GetAssets().SetLargeImage("loading_hideout_2011");
				}
				else if (regionName == "Petra (Dungeon)") {
					activity.GetAssets().SetLargeImage("loading_petra_2011");
				}
				else if (regionName == "Devil's Garden (Dungeon)") {
					activity.GetAssets().SetLargeImage("loading_garden_2011");
				}
				else {
					activity.GetAssets().SetLargeImage("loading_default");
				}
				// Game icon or Character icon
				activity.GetAssets().SetSmallImage("logo");
				// Game website or Character race
				activity.GetAssets().SetSmallText("SilkroadLatino.com");
				// Set Timestamp (Elapsed time)
				activity.GetTimestamps().SetStart(m_InGameTimestamp);
			}
			break;
			}
			// Always will be playing mode
			activity.SetType(discord::ActivityType::Playing);
			// Set activity
			DiscordClient.Core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
				// Check error
				if (result != discord::Result::Ok)
					std::cout << "Discord Activity (Err " << static_cast<int>(result) << ")" << std::endl;
			});
		}
	}
}