#pragma once
#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>

typedef void(__thiscall* fPasteFunction)(uint32_t instance, std::string objectString);
BOOL safePaste(std::string objectString, int sleepFor = 20);
HANDLE getHandle(std::string const& name = "");
std::uintptr_t getBase(std::string const& name = "");
BOOL placeJump(uintptr_t address, uintptr_t destination, int length);
void pipeMain(std::string const& pipeName);