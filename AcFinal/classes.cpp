#include "classes.h"
#include "offsets.h"

Player* Player::GetLocal() {
	static uintptr_t Base = (uintptr_t)GetModuleHandle("ac_client.exe");
	static Player* player = (Player*)(Base + 0x0010F4F4);
	return player;
}

Player* Player::GetEnt(int index) {
	static uintptr_t Base = (uintptr_t)GetModuleHandle("ac_client.exe");
	static uintptr_t entityList = Base + 0x10f4f8;
	return (Player*)(*(uintptr_t*)entityList + index * 4);
}

vec3 Player::HeadPos() {
	vec3 temp;
	temp.x = *(float*)(*(uintptr_t*)this + playerOff.headx);
	temp.y = *(float*)(*(uintptr_t*)this + playerOff.heady);
	temp.z = *(float*)(*(uintptr_t*)this + playerOff.headz);
	return temp;
}

int* Player::Health() {
	return (int*)(*(uintptr_t*)this + playerOff.health);
}

int* Player::Armor() {
	return (int*)(*(uintptr_t*)this + playerOff.armor);
}

v3ptr Player::pPos() {
	v3ptr temp = { 0, 0, 0 };
	temp.x = (float*)(*(uintptr_t*)this + playerOff.posx);
	temp.y = (float*)(*(uintptr_t*)this + playerOff.posy);
	temp.z = (float*)(*(uintptr_t*)this + playerOff.posz);
	return temp;
}

vec3 Player::Pos() {
	vec3 temp;
	temp.x = *(float*)(*(uintptr_t*)this + playerOff.posx);
	temp.y = *(float*)(*(uintptr_t*)this + playerOff.posy);
	temp.z = *(float*)(*(uintptr_t*)this + playerOff.posz);
	return temp;
}

char* Player::Name() {
	/*byte* p = (byte*)(*(uintptr_t*)this + playerOff.name);
	char c[16];
	memcpy(c, p, sizeof(c));
	return  c;*/
	return (char*)(*(uintptr_t*)this + playerOff.name);
}

v2ptr Player::ViewAngles() {
	v2ptr temp = { 0, 0 };
	temp.x = (float*)(*(uintptr_t*)this + playerOff.anglesx);
	temp.y = (float*)(*(uintptr_t*)this + playerOff.anglesy);
	return temp;
}

int* Player::GetTeam() {
	return (int*)(*(uintptr_t*)this + playerOff.team);
}

int* Player::CurrentWeapon() {
	uintptr_t* curWep = (uintptr_t*)(*(uintptr_t*)this + 0x374);
	return (int*)(*(uintptr_t*)curWep + 0x4);
}

// Weapons

Weapon* Weapon::Get() {
	uintptr_t Base = (uintptr_t)GetModuleHandle("ac_client.exe");
	Weapon* wep = (Weapon*)(Base + 0x0011E20C);
	return wep;
}

int* Weapon::ReserveAmmo(int curWep) {
	return (int*)(*(uintptr_t*)this + (curWep * 0x4 + weaponOff.extraStart));
}

int* Weapon::Ammo(int curWep) {
	return (int*)(*(uintptr_t*)this + (curWep * 0x4 + weaponOff.ammoStart));
}

int* Weapon::WeaponSpeed(int curWep) {
	return (int*)(*(uintptr_t*)this + (curWep * 0x4 + weaponOff.speedStart));
}

int* Weapon::RoundsShots(int curWep) {
	return (int*)(*(uintptr_t*)this + (curWep * 0x4 + weaponOff.shotsStart));
}

// non class functions

int maxPlayers() {
	uintptr_t Base = (uintptr_t)GetModuleHandle("ac_client.exe");
	return (int)*(uintptr_t*)(Base + 0x10F500);
}

bool isTeamGamemode() {
	int* pgameMode = (int*)0x50f49c;
	int gameMode = *(int*)pgameMode;

	return (gameMode == 0 || gameMode == 4 || gameMode == 5 || gameMode == 7 || gameMode == 11
		|| gameMode == 13 || gameMode == 14 || gameMode == 16 || gameMode == 17 || gameMode == 20 || gameMode == 21);
}