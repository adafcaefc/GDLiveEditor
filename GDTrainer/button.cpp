#include "includes.h"
extern int Main();

namespace Button {

	__declspec(noinline) void __stdcall showMainTrainer(LPVOID lpParam);

	uintptr_t createWithSprite = *reinterpret_cast<uintptr_t*>(getBase() + 0x282284);
	uintptr_t operatorPlus = *reinterpret_cast<uintptr_t*>(getBase() + 0x282278);
	uintptr_t sharedDirector = *reinterpret_cast<uintptr_t*>(getBase() + 0x282270);
	uintptr_t getString = getBase() + 0xF840;
	uintptr_t createMenu = getBase() + 0x18EE0;
	uintptr_t showMain = reinterpret_cast<uintptr_t>(showMainTrainer);
	uintptr_t startShowTrainerButton = getBase() + 0x190BD0;
	uintptr_t retShowTrainerButton = startShowTrainerButton + 5;
	uintptr_t varShowTrainerButton = getBase() + 0x2CD6D4;
	constexpr char showTrainerButtonSpriteChar[] = "GJ_editBtn_001.png";
	const uintptr_t showTrainerButtonSprite = reinterpret_cast<uintptr_t>(showTrainerButtonSpriteChar);

	uintptr_t __fastcall getStartShowTrainerButton() { return startShowTrainerButton; }

	bool isTrainerOpen = false;
	DWORD WINAPI showMainTrainerThread(LPVOID lpParam) {
		if (!isTrainerOpen) {
			isTrainerOpen ^= true;
			Main();
			isTrainerOpen ^= true;
		}
		return TRUE;
	}

	__declspec(noinline) void __stdcall showMainTrainer(LPVOID lpParam) { CreateThread(0, 0x1000, &showMainTrainerThread, 0, 0, NULL); }

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

}