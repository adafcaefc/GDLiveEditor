#include "includes.h"

BOOL placeJump(const uintptr_t address, const uintptr_t destination, const unsigned int& length) {

	DWORD oldProtect, newProtect;
	uintptr_t relativeAddress;

	if (!VirtualProtect(reinterpret_cast<LPVOID>(address), length, PAGE_EXECUTE_READWRITE, &oldProtect))
		return FALSE;

	relativeAddress = destination - address - 5;
	*reinterpret_cast<uint8_t*>(address) = 0xE9;
	*reinterpret_cast<uintptr_t*>(address + 1) = relativeAddress;

	for (unsigned int i = 5; i < length; i++)
		*reinterpret_cast<uint8_t*>(address) = 0x90;

	return VirtualProtect(reinterpret_cast<LPVOID>(address), length, oldProtect, &newProtect);

}

HANDLE __fastcall getHandle(std::string const& name) { return GetModuleHandleA(name.size() ? name.c_str() : nullptr); }

std::uintptr_t __fastcall getBase(std::string const& name) { return reinterpret_cast<std::uintptr_t>(getHandle(name)); }

BOOL safePaste(const std::string& objectString, const int& sleepFor) {

	uintptr_t base = getBase();
	uintptr_t cocosBase = getBase("libcocos2d.dll");

	DWORD oldProtect, newProtect;

	if (!base || !cocosBase)
		return FALSE;

	uintptr_t gameManager = *reinterpret_cast<uintptr_t*>(base + 0x3222D0);

	if (!gameManager)
		return FALSE;

	uintptr_t levelEditorLayer = *reinterpret_cast<uintptr_t*>(gameManager + 0x168);

	if (!levelEditorLayer)
		return FALSE;

	uintptr_t pasteObjectInstance = *reinterpret_cast<uintptr_t*>(levelEditorLayer + 0x380);

	if (!pasteObjectInstance)
		return FALSE;

	if (!VirtualProtect(reinterpret_cast<LPVOID>(cocosBase + 0xC16A3), 8, PAGE_EXECUTE_READWRITE, &oldProtect))
		return FALSE;

	*reinterpret_cast<uint64_t*>(cocosBase + 0xC16A3) = 0x0E74000000026DE9;

	std::this_thread::sleep_for(std::chrono::milliseconds(sleepFor));

	reinterpret_cast<fPasteFunction>(base + 0x88240)(pasteObjectInstance, objectString);

	std::this_thread::sleep_for(std::chrono::milliseconds(sleepFor));

	*reinterpret_cast<uint64_t*>(cocosBase + 0xC16A3) = 0x0E74000000958638;

	return VirtualProtect(reinterpret_cast<LPVOID>(cocosBase + 0xC16A3), 8, oldProtect, &newProtect);

}

void pipeMain(std::string const& pipeName) {

	char buffer[1024];
	DWORD dwRead;
	HANDLE hPipe;

	hPipe = CreateNamedPipeA(
		pipeName.c_str(),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		8,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL
	);

	while (hPipe != INVALID_HANDLE_VALUE) {
		if (ConnectNamedPipe(hPipe, NULL) != FALSE) {
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE) {
				buffer[dwRead] = '\0';
				std::string objectString(buffer);
				safePaste(objectString);
			}
		}
		DisconnectNamedPipe(hPipe);
	}

}