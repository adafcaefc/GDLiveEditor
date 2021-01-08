#include "includes.h"
#include <iostream>

extern int Main();
void showMainTrainer();
DWORD WINAPI showMainTrainerThread(LPVOID lpParam);

uintptr_t base = getBase();
uintptr_t libcocosbase = getBase("libcocos2d.dll");
uintptr_t createWithSprite = *(uintptr_t*)(base + 0x282284); // USED AS A POINTER
uintptr_t operatorPlus = *(uintptr_t*)(base + 0x282278);
uintptr_t sharedDirector = *(uintptr_t*)(base + 0x282270);
uintptr_t getString = base + 0xF840;
uintptr_t createMenu = base + 0x18EE0;
uintptr_t showMain = (uintptr_t)showMainTrainer;
uintptr_t retShowTrainerButton = base + 0x190BD5;
uintptr_t varShowTrainerButton = base + 0x2CD6D4;
char showTrainerButtonSpriteChar[] = "GJ_editBtn_001.png";
uintptr_t showTrainerButtonSprite = (uintptr_t)showTrainerButtonSpriteChar;

BOOL isTrainerOpen = false;
DWORD WINAPI showMainTrainerThread(LPVOID lpParam) {
	if (!isTrainerOpen) {
		isTrainerOpen = true;
		Main();
		isTrainerOpen = false;
	}
	return TRUE;
}

void showMainTrainer() {
	CreateThread(0, 0x1000, (LPTHREAD_START_ROUTINE)showMainTrainerThread, 0, 0, NULL);
	__asm ret 0x4
}

__declspec(naked) void showTrainerButton() {
	__asm {
		push showTrainerButtonSprite
		call dword ptr[createWithSprite]
		mov esi, eax
		mov dword ptr ss : [esp] , 0x3F800000
		mov ecx, esi
		mov edx, [esi]
		call dword ptr[edx + 0x50]
		push showMain
		push ebx
		mov ecx, esi
		call createMenu
		mov esi, [esp + 0x20]
		add esp, 0x08
		mov ecx, esi
		mov edx, [esi]
		push eax
		call dword ptr[edx + 0x000000E0]
		push varShowTrainerButton
		jmp[retShowTrainerButton]
	}
}

DWORD WINAPI mainMod(LPVOID lpParam) {
	placeJump(base + 0x190BD0, reinterpret_cast<uintptr_t>(showTrainerButton), 0x5);
	pipeMain("\\\\.\\pipe\\GDPipe");
	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0x1000, &mainMod, 0, 0, NULL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
