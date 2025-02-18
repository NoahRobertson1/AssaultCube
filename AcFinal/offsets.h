#pragma once

struct playerOff {
	int headx = 0x4;
	int heady = 0x8;
	int headz = 0xc;

	int posx = 0x34;
	int posy = 0x38;
	int posz = 0x3c;

	int anglesx = 0x40;
	int anglesy = 0x44;

	int health = 0xf8;
	int armor = 0xfc;
	int name = 0x225;
	int team = 0x32C;
}playerOff;

struct weaponOff {
	int extraStart = 110;
	int ammoStart = 0x138;
	int speedStart = 0x160;
	int shotsStart = 0x188;
}weaponOff;