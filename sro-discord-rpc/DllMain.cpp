#include "DllMain.h"
#include "AppManager.h"

EXTERN_DLL_EXPORT BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
			AppManager::Initialize();
		break;
	}
	return TRUE;
}