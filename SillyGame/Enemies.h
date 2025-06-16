#pragma once
#ifndef ENEMIES_H
#define ENEMIES_H

typedef struct Enemy_struct {
	char* name[50];
	
	int MAX_HP;
	int MAX_DMG;
	int RARITY;
	
	int currhp;
	int dmg;


}Enemy;

#endif