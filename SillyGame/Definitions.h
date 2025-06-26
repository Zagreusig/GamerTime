#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MAX_BOSSES 5
#define MAX_ENEMIES 10
#define MAX_NPCS 5
#define MAX_ITEMS 5
#define SECONDARY_STAGE_CHANCE 0.5

typedef struct Enemy_struct {
	char name[50];
	int id;

	int MAX_HP;
	int MAX_DMG;
	int RARITY;

	int currhp;
	int dmg;

	// When on the stage marked by "prefStage", the enemy is more likely to be
	// chosen. There is a reduced chance to appear on the secondary stage (50% as likely).
	int prefStage;
	int secStage;

}Enemy;

typedef struct Entity_struct {
	int type;
	/*
	* If Enemy:
	* 0 = normal, 1 = elite
	*
	* If NPC:
	* 0 = Helpful, 1 = Harmful, 2 = Jester
	*
	* If Shopkeeper:
	* 0 = Stingy, 1 = Neutral, 2 = Generous
	*/

	int health;
	int damage;
	int id;
	int persuation;

	char interact;

}Entity;

typedef struct Entity_Table_Struct {
	Entity entity[5];
}ENTTable;

typedef struct Item_struct {
	char name[50];
	char description[500];
	int id;
	int hp;
	int dmg;
}Item;


typedef struct Item_Table_Struct {
	Item item[5];
	int stageID;
}ITable;


// 0: tbd
// 1: tbd
// 2: tbd


typedef struct Enemy_Table_Struct {
	Enemy enemy[10];
	int stageID;
}EYTable;

typedef struct Boss_Table_Struct {
	Enemy boss[5];
	int stageID;
}BTable;

typedef struct Stage_struct {
	int id;
	ITable Items;
	EYTable Enemies;
	BTable Bosses;
	ENTTable Npcs;

}Stage;

typedef struct Difficulty_struct {
	int code;

	double evilfactor;
	// Controls the bonus for all negative things.
	double sillyfactor;
	// Controls the fake items.
	double playerfactor;
	// Controls player things (health related, dmg related, etc.)
	double likeability;
	// RNG bonus for player chance interactions.
}Difficulty;


#endif