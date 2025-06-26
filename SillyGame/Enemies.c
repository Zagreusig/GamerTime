#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Definitions.h"
#include "Entity.h"
#include "Enemies.h"
#include "DifficultyConfig.h"
#include "Stages.h"

// FIXME: TypeClass() functions should reference current stage ID and MasterIDList()
//		  and return the information from corresponding function. They should not build
//		  the struct variables themselves.

// FIXME: Add Enemy() functions for each type with a favored stage and possible stage for 
//		  dungeon crawler mode.

Enemy* BossOne(Difficulty* config) {
	Enemy* b = (Enemy*)malloc(sizeof(Enemy));
	if (b == NULL) {
		return NULL;
	}

	strcpy(b->name, "BOSS ONE");
	b->MAX_HP = 50;
	b->MAX_DMG = 30;
	b->RARITY = 5;
	b->prefStage = 0;
	b->secStage = 1;

	return b;
	free(b);
}

Enemy* BossTwo(Difficulty* config) {
	Enemy* b = (Enemy*)malloc(sizeof(Enemy));
	if (b == NULL) {
		return NULL;
	}

	strcpy(b->name, "BOSS TWO");
	b->MAX_HP = 100;
	b->MAX_DMG = 100;
	b->RARITY = 2;

	return b;
	free(b);
}

Enemy* BossThree(Difficulty* config) {
	Enemy* b = (Enemy*)malloc(sizeof(Enemy));
	if (b == NULL) {
		return NULL;
	}

	strcpy(b->name, "BOSS THREE");
	b->MAX_HP = 500;
	b->MAX_DMG = 1000;
	b->RARITY = 100;

	return b;
	free(b);
}



Enemy* BossClass(int StageID) {
	int numBosses;
	int chosen;
	EYTable* bosses = (EYTable*)malloc(sizeof(EYTable));
	Enemy* out = (Enemy*)malloc(sizeof(Enemy));
	if (out == NULL) {
		return NULL;
	}

	numBosses = CountBosses(StageID);




}

Enemy* EliteClass(int id) {
	Enemy* elite = (Enemy*)malloc(sizeof(Enemy));
	if (elite == NULL) {
		printf("Malloc err, EliteClass()\n");
		exit(1);
	}
	
	switch (id)
	{
	default:
		return NULL;
		break;
	case 0:
		strcpy(elite->name, "Elite enemy 1");
		elite->MAX_DMG = 15;
		elite->MAX_HP = 25;
		elite->RARITY = 3;
	case 1:
		strcpy(elite->name, "Elite enemy 2");
		elite->MAX_DMG = 50;
		elite->MAX_HP = 50;
		elite->RARITY = 9;
	case 2:
		strcpy(elite->name, "Elite enemy 3");
		elite->MAX_DMG = 500;
		elite->MAX_HP = 250;
		elite->RARITY = 4;
	}
	return elite;
	free(elite);
}

Enemy* CommonClass(int id) {
	Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));
	if (enemy == NULL) {
		printf("Malloc err, CommonClass()\n");
		exit(1);
	}
	switch (id)
	{
	default:
		return NULL;
		break;
	case 0:
		strcpy(enemy->name, "Common enemy 1");
		enemy->MAX_DMG = 4;
		enemy->MAX_HP = 10;
		enemy->RARITY = 1;
	case 1:
		strcpy(enemy->name, "Common enemy 2");
		enemy->MAX_DMG = 3;
		enemy->MAX_HP = 9;
		enemy->RARITY = 2;
	case 2:
		strcpy(enemy->name, "Common enemy 3");
		enemy->MAX_DMG = 4;
		enemy->MAX_HP = 13;
		enemy->RARITY = 0;
	}
	return enemy;
	free(enemy);
}

Enemy* SecretSecret() {
	Enemy* secrets = (Enemy*)malloc(sizeof(Enemy));
	if (secrets == NULL) {
		printf("Malloc err, SecretSecret()\n");
		exit(1);
	}
	strcpy(secrets->name, "Secret secrets");
	secrets->MAX_HP = 1000;
	secrets->MAX_DMG = 9999;
	secrets->RARITY = 100;
	return secrets;
	free(secrets);
}

Enemy* GetEnemyMulti(Difficulty* modifier) {
	Enemy* it[5] = { (Enemy*)malloc(5 * sizeof(Enemy)) };
	Enemy* s = (Enemy*)malloc(sizeof(Enemy));
	if (it == NULL) {
		printf("Malloc err, GetEnemyMulti()\n");
		exit(1);
	}

	double chance = (double)(rand()) * modifier->evilfactor;
	int type;
	int j = 0;
	int has_elite_flag = 0;
	// FIXME: Create funct for harder difficulty = increased number of elites.
	int enemyAmount = GetAmount(modifier);
	if (enemyAmount == 1) {
		enemyAmount++;
	}
	int enemyID;
	goto build_loop;


	build_loop:
	for (int i = 0; i < enemyAmount; i++) {
		enemyID = rand() % 4;
		type = GetType(modifier, chance);
		if (type == 1 && has_elite_flag == 1) {
			type = 0;
		}
		goto enemy_determine;
	}
	
	enemy_determine:
	switch (type)
	{
	default:
		return NULL;
		break;
	case 0:
		it[j] = CommonClass(enemyID);
		j++;
		goto build_loop;
	case 1:
		it[j] = EliteClass(enemyID);
		has_elite_flag = 1;
		j++;
		goto build_loop;
	case 999:
		s = SecretSecret();
		return s;
		break;
	}

	free(s);
	free(it);
}

Enemy* GetEnemySingle(Difficulty* modifier) {
	Enemy* it = (Enemy*)malloc(sizeof(Enemy));

	double chance = (double)(rand() % 10) * modifier->evilfactor;
	int type = GetType(modifier);
	int id = rand() % 4;

	switch (type)
	{
	default:
		return NULL;
		break;
	case 0:
		it = CommonClass(id);
	case 1:
		it = EliteClass(id);
	case 999:
		it = SecretSecret();
	}
	return it;
	free(it);
}

double GetEnemyChance(Difficulty* modifier) {
	double chance;
	chance = (double)(rand() % 10) * modifier->evilfactor;
	return chance;
}

double GetFunnyChance(Difficulty* modifier) {
	double chance;
	chance = (double)(rand() % 99999) * modifier->sillyfactor;
	return chance;
}

double GetDblRNG(double chance, int outof, int plus) {
	return (double)(rand() * chance) + plus;
}

int GetIntRNG(double chance, int outof, int plus) {
	return (rand() % outof) * (int)chance + plus;
}

int GetType(Difficulty* modifier) {
	int type;
	double chance = GetEnemyChance(modifier);
	double funnyChance = GetFunnyChance(modifier);
	
	if (funnyChance == 1924) {
		return 999;
	}
	else {
		type = GetIntRNG(chance, 4, 0);
		return type;
	}
	// ez: lower chance of elite enemy
	// norm: normal chance
	// hrd: slightly higher chance
	// ultra: very high chance
}

int GetAmount(Difficulty* config) { return 3; }