#pragma once
#include "stdafx.h"

typedef void(__thiscall* fPasteFunction)(uint32_t instance, std::string objectString);
BOOL safePaste(const std::string& objectString, const int& sleepFor = 20);
HANDLE __fastcall getHandle(std::string const& name = "");
std::uintptr_t __fastcall getBase(std::string const& name = "");
BOOL placeJump(const uintptr_t address, const uintptr_t destination, const unsigned int& length);
void pipeMain(std::string const& pipeName);
namespace Button { void showTrainerButton(); uintptr_t __fastcall getStartShowTrainerButton(); };