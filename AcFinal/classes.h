#pragma once
#include <Windows.h>
#include <string>
#include "vector.h"


enum current_weapon {
	knife = 0,
	pistol,
	carbine,
	shotgun,
	smg,
	sniper,
	assaultRifle,
	padding,
	grenade,
	duelPistol
};

class Player {
private:
	Player();
public:
	static Player* GetLocal();
	static Player* GetEnt(int index);
	vec3 HeadPos();
	int* Health();
	int* Armor();
	v3ptr pPos();
	vec3 Pos();
	char* Name();
	v2ptr ViewAngles();
	int* GetTeam();
	int* CurrentWeapon();
};


class Weapon {
private:
	Weapon();
public:
	static Weapon* Get();

	int* ReserveAmmo(int curWep);
	int* Ammo(int curWep);
	int* WeaponSpeed(int curWep);
	int* RoundsShots(int curWep);
};


int maxPlayers();
bool isTeamGamemode();