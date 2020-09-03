#include <Windows.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

typedef void(__stdcall* fPasteFunction)(std::string testString);

void pipeMain() {
	char buffer[1024];
	DWORD dwRead;
	HANDLE hPipe;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\GDPipe"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		1,
		1024 * 15,
		1024 * 15,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);

	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				/* add terminating zero */
				buffer[dwRead] = '\0';

				/* do something with data in buffer */
				DWORD oldProtect, newProtect;

				DWORD base = (DWORD)GetModuleHandleA(0);
				DWORD libcocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");

				fPasteFunction pasteFunction = (fPasteFunction)(base + 0x88240);

				VirtualProtect((LPVOID)(libcocosbase + 0xC16A3), 8, PAGE_EXECUTE_READWRITE, &oldProtect);
				*((__int64*)(libcocosbase + 0xC16A3)) = 0x0E74000000026DE9;

				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				std::string objectString = buffer;


				pasteFunction(objectString);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

				*((__int64*)(libcocosbase + 0xC16A3)) = 0x0E74000000958638;

				VirtualProtect((LPVOID)(libcocosbase + 0xC16A3), 8, oldProtect, &newProtect);
			}
		}

		DisconnectNamedPipe(hPipe);
	}
}


void placeJump(BYTE* address, DWORD jumpTo, DWORD length)
{
	DWORD oldProtect, newProtect, relativeAddress;
	VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &oldProtect);
	relativeAddress = (DWORD)(jumpTo - (DWORD)address) - 5;
	*address = 0xE9;
	*((DWORD*)(address + 0x1)) = relativeAddress;
	for (DWORD x = 0x5; x < length; x++)
	{
		*(address + x) = 0x90;
	}
	VirtualProtect(address, length, oldProtect, &newProtect);
}

void writeByte(BYTE* address, BYTE content)
{
	DWORD oldProtect, newProtect;
	VirtualProtect(address, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
	*address = content;
	VirtualProtect(address, 1, oldProtect, &newProtect);
}

extern int Main();
BOOL isTrainerOpen = false;

DWORD WINAPI showMainTrainerThread(LPVOID lpParam)
{
	if (not isTrainerOpen) {
		isTrainerOpen = true;
		Main();
	}
	else {
		return 1;
	}
	isTrainerOpen = false;
	return 1;
}

void showMainTrainer()
{
	CreateThread(0, 0x1000, (LPTHREAD_START_ROUTINE)showMainTrainerThread, 0, 0, NULL);
	__asm ret 0x4

}

DWORD base = (DWORD)GetModuleHandleA(0);
DWORD libcocosbase = (DWORD)GetModuleHandleA("libcocos2d.dll");

DWORD createWithSprite = base + 0x282284; // USE AS A POINTER
DWORD createWithSprite_ = (DWORD)createWithSprite;
DWORD operatorPlus = base + 0x282278; // USE AS A POINTER
DWORD operatorPlus_ = (DWORD)operatorPlus;
DWORD sharedDirector = base + 0x282270; // USE AS A POINTER
DWORD sharedDirector_ = (DWORD)sharedDirector;

DWORD getString = base + 0xF840;
DWORD createMenu = base + 0x18EE0;
DWORD showMain = (DWORD)showMainTrainer;
DWORD retShowTrainerButton = base + 0x190BD5;
DWORD varShowTrainerButton = base + 0x2CD6D4;
char showTrainerButtonSpriteChar[] = "GJ_editBtn_001.png";
DWORD showTrainerButtonSprite = (DWORD)showTrainerButtonSpriteChar;

DWORD gameManager = base + 0x3222D0;

DWORD retPasteObjects = base + 0x88245;

__declspec(naked) void pasteObjects() {

	__asm {

		mov ecx, gameManager
		mov ecx, [ecx]
		add ecx, 0x168
		mov ecx, [ecx]
		test ecx, ecx
		je return_end
		add ecx, 0x380
		mov ecx, [ecx]
		test ecx, ecx
		je return_end
		push ebp
		mov ebp, esp
		push -0x01
		jmp[retPasteObjects]
		return_end:
		ret 0x0018
	}
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

__declspec(naked) void loadPointer() {

	__asm {

		pushad
		mov eax, createWithSprite_
		mov eax, [eax]
		mov[createWithSprite], eax
		mov eax, operatorPlus_
		mov eax, [eax]
		mov[operatorPlus], eax
		mov eax, sharedDirector_
		mov eax, [eax]
		mov[sharedDirector], eax
		popad
		ret

	}
}

DWORD WINAPI mainMod(LPVOID lpParam)
{
	loadPointer();
	placeJump((BYTE*)base + 0x190BD0, (DWORD)showTrainerButton, 0x5);
	placeJump((BYTE*)base + 0x88240, (DWORD)pasteObjects, 0x5);
	CreateThread(0, 0x1000, (LPTHREAD_START_ROUTINE)pipeMain, 0, 0, NULL);
	return 1;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0x1000, &mainMod, 0, 0, NULL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
