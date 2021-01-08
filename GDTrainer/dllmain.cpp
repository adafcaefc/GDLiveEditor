#include "includes.h"

DWORD WINAPI mainThread(LPVOID lpParam) {
	placeJump(Button::getStartShowTrainerButton(), reinterpret_cast<uintptr_t>(Button::showTrainerButton), 5);
	pipeMain("\\\\.\\pipe\\GDPipe");
	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpParam), 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0x1000, &mainThread, hModule, 0, NULL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
