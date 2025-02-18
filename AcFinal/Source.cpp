#include <iostream>
#include <Windows.h>
#include "classes.h"
#include "hooks.h"
#include "draw.h"
#include "toggles.h"
#include <string>

typedef BOOL(__stdcall* twglSwapBuffers) (HDC hdc);

twglSwapBuffers owglSwapBuffers;


GL::font glFont;
const int FONT_HEIGHT = 15;
const int FONT_WIDTH = 9;

uintptr_t viewMatrix = 0x501AE8;
float Matrix[16];
int screenHeight;
int screenWidth;

void draw() {
	memcpy(&Matrix, (PBYTE*)(viewMatrix), sizeof(Matrix));
	HDC currentHDC = wglGetCurrentDC();

	if (!glFont.bBuilt || currentHDC != glFont.hdc) {
		glFont.build(FONT_HEIGHT);
	}

	GL::SetupOrtho();

	GLubyte Red[3] = { 255, 0, 0 };
	GLubyte Green[3] = { 0, 255, 0 };
	GLubyte Black[3] = { 0, 0, 0 };
	GLubyte White[3] = { 255, 255, 255 };
	GLubyte curCol[3];
	for (int i = 1; i < maxPlayers(); i++) {
		Player* curEnt = Player::GetEnt(i);
		if (*curEnt->Health() <= 0) continue;

		vec3 headPos3d = curEnt->HeadPos();
		vec2 headPos2d;
		vec3 playerPos3d = curEnt->Pos();
		headPos3d.z += 0.5f;
		vec2 playerPos2d;

		if (isTeamGamemode()) {
			if (*Player::GetLocal()->GetTeam() == *curEnt->GetTeam()) {
				curCol[0] = 0;
				curCol[1] = 0;
				curCol[2] = 255;
			}
			else
			{
				curCol[0] = 255;
				curCol[1] = 0;
				curCol[2] = 0;
			}
		}
		else {
			curCol[0] = 255;
			curCol[1] = 0;
			curCol[2] = 0;
		}

		if (GL::WorldToScreen(playerPos3d, playerPos2d, Matrix, screenWidth, screenHeight)) {
			if (GL::WorldToScreen(headPos3d, headPos2d, Matrix, screenWidth, screenHeight)) {
				

				// box (the sides are inverted because of the next line | it should be playerPos - headPos)
				int dist = headPos2d.y - playerPos2d.y;
				vec2 topL = headPos2d;
				topL.x -= dist / 4;
				vec2 topR = headPos2d;
				topR.x += dist / 4;
				vec2 botL = playerPos2d;
				botL.x -= dist / 4;
				vec2 botR = playerPos2d;
				botR.x += dist / 4;

				GL::DrawOutline(topL, topR, botL, botR, 1.f, curCol);

				//health bar
				int newY = dist;

				if (*curEnt->Health() < 100)
					newY *= (float)(*curEnt->Health() / 100.f);

				topR.x -= 5;
				vec2 healthtl = topR;
				healthtl.y = botR.y + newY;
				topR.x += 5;
				vec2 healthtr = topR;
				healthtr.y = botR.y + newY;
				botR.x -= 5;
				vec2 healthbl = botR;
				botR.x += 5;
				vec2 healthbr = botR;
				GL::DrawFilledRect(healthtl, healthtr, healthbl, healthbr, Green);

				// snaplines
				vec2 tl = playerPos2d;
				tl.x -= 2;
				vec2 tr = playerPos2d;
				tr.x += 2;
				vec2 bl;
				bl.x = (screenWidth / 2) - 2;
				bl.y = screenHeight;
				vec2 br;
				br.x = (screenWidth / 2) + 2;
				br.y = screenHeight;

				GL::DrawFilledRect(tl, tr, bl, br, curCol);
		
				// name
				std::string name = curEnt->Name();
				float textX = glFont.centerText(headPos2d.x, name.length() * FONT_WIDTH);
				glFont.print(textX, topL.y - FONT_HEIGHT, White, "%s", curEnt->Name());

				// sniper crosshair
				glFont.print(botL.x + 5, botL.y, White, "%d", (int)DistanceVec3(Player::GetLocal()->Pos(), curEnt->Pos()));		
			}
		}
	}
	if (*Player::GetLocal()->CurrentWeapon() == 5) {
		vec2 L1;
		L1.x = screenWidth / 2 - 5;
		L1.y = screenHeight / 2 + 1;
		vec2 L2;
		L2.x = screenWidth / 2 - 5;
		L2.y = screenHeight / 2 - 1;
		vec2 R1;
		R1.x = screenWidth / 2 + 5;
		R1.y = screenHeight / 2 + 1;
		vec2 R2;
		R2.x = screenWidth / 2 + 5;
		R2.y = screenHeight / 2 - 1;
		vec2 T1;
		T1.x = screenWidth / 2 + 1;
		T1.y = screenHeight / 2 - 5;
		vec2 T2;
		T2.x = screenWidth / 2 - 1;
		T2.y = screenHeight / 2 - 5;
		vec2 B1;
		B1.x = screenWidth / 2 + 1;
		B1.y = screenHeight / 2 + 5;
		vec2 B2;
		B2.x = screenWidth / 2 - 1;
		B2.y = screenHeight / 2 + 5;
		GL::DrawFilledRect(L2, R2, L1, R1, White);
		GL::DrawFilledRect(T2, T1, B2, B1, White);
	}
	GL::RestoreGL();
}

BOOL __stdcall hkwglSwapBuffers(HDC hdc) {
	
	if (globals::health) {
		Player* localPlayer;
		*localPlayer->GetLocal()->Health() = 100;
		*localPlayer->GetLocal()->Armor() = 100;
	}
	if (globals::ammo) {
		Weapon* weapon;
		Player* player = Player::GetLocal();
		*weapon->Get()->Ammo(*player->CurrentWeapon()) = 50;
		*weapon->Get()->WeaponSpeed(*player->CurrentWeapon()) = 0;
	}

	draw();

	return owglSwapBuffers(hdc);
}


DWORD WINAPI mainThread(HMODULE hModule) {
	
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	 
	AllocConsole();
	FILE* file;
	freopen_s(&file, "CONOUT$", "w", stdout);
	
	std::cout << "  ---General---\n";
	std::cout << "Health      | Num 0\n";
	std::cout << "Guns & Ammo | Num 1\n";

	owglSwapBuffers = (twglSwapBuffers)GetProcAddress(GetModuleHandle("opengl32.dll"), "wglSwapBuffers");
	owglSwapBuffers = (twglSwapBuffers)TrampHook((BYTE*)owglSwapBuffers, (BYTE*)hkwglSwapBuffers, 5);

	*Weapon::Get()->Ammo(grenade) = 1;

	//std::cout << "Exiting thread\n";
	//FreeLibraryAndExitThread(hModule, 0);
	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
	if (reason == DLL_PROCESS_ATTACH)
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)mainThread, hModule, 0, 0));
	return TRUE;
}