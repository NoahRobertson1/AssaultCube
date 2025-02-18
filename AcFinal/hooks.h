#pragma once
#include <Windows.h>

void Patch(BYTE* dst, BYTE* src, unsigned int size);

bool Hook(BYTE* src, BYTE* dst, int len);

BYTE* TrampHook(BYTE* src, BYTE* dst, unsigned int len);
