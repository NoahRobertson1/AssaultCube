#include "hooks.h"

void Patch(BYTE* dst, BYTE* src, unsigned int size) {
	DWORD oProc;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oProc);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oProc, &oProc);
}

bool Hook(BYTE* src, BYTE* dst, int len) {
	if (len < 5) return false;
	DWORD oProc;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oProc);
	memset(src, 0x90, len);
	uintptr_t relAddy = (uintptr_t)(dst - src - 5);
	*src = 0xE9;
	*(uintptr_t*)(src + 1) = (uintptr_t)relAddy;
	VirtualProtect(src, len, oProc, &oProc);
}

BYTE* TrampHook(BYTE* src, BYTE* dst, unsigned int len) {
	if (len < 5) return 0;
	BYTE* gateway = (BYTE*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(gateway, src, len);
	uintptr_t jumpAddy = (uintptr_t)(src - gateway - 5);
	*(gateway + len) = (char)0xE9;
	*(uintptr_t*)(gateway + len + 1) = jumpAddy;
	if (Hook(src, dst, len)) {
		return gateway;
	}
	else return nullptr;
}