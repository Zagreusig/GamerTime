#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Entity.h"
#include "Enemies.h"
#include "DifficultyConfig.h"

Enemy* BossClass(int id) {
	Enemy* boss = (Enemy*)malloc(sizeof(Enemy));
	if (boss == NULL) {
		printf("Malloc err, BossClass()\n");
		exit(1);
	}
	switch (id)
	{
	default:
		printf("Boss func err\n");
		break;
	case 0:
		strcpy(boss->name, "THIS IS A BOSS!!!");
		boss->MAX_HP = 50;
		boss->MAX_DMG = 30;
		boss->RARITY = 5;
	case 1:
		strcpy(boss->name, "THIS IS A DIFFERENT BOSS!!!");
		boss->MAX_HP = 100;
		boss->MAX_DMG = 100;
		boss->RARITY = 2;
	case 2:
		strcpy(boss->name, "THIS IS A THIRD CASE WAOH!");
		boss->MAX_HP = 500;
		boss->MAX_DMG = 1000;
		boss->RARITY = 100;
	}
	return boss;
}

Enemy* EliteClass() {
	// FIXME: DELETE
	srand(3);
	int id = rand() % 3;

	Enemy* elite = (Enemy*)malloc(sizeof(Enemy));
	if (elite == NULL) {
		printf("Malloc err, EliteClass()\n");
		exit(1);
	}
	
	switch (id)
	{
	default:
		printf("Boss func err\n");
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
}

Enemy* CommonClass() {
	int id = rand() % 3;
	Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));
	if (enemy == NULL) {
		printf("Malloc err, CommonClass()\n");
		exit(1);
	}
	switch (id)
	{
	default:
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
}

Enemy* GetEnemyMulti(Difficulty* modifier) {
	Enemy* it = (Enemy*)malloc(sizeof(Enemy));
	if (it == NULL) {
		printf("Malloc err, GetEnemyMulti()\n");
		exit(1);
	}

	double chance = (double)(rand()) * modifier->evilfactor;
	int type = GetType(modifier, chance);
	int enemyAmount = GetAmount(modifier);
	if (enemyAmount == 1) {
		enemyAmount++;
	}

	switch (type)
	{
	default:
		break;
	case 0:
		CommonClass();
	case 1:
		EliteClass();
	case 3:
		SecretSecret();
	}
}

//Enemy* GetEnemySingle(Difficulty* modifier) {
//	Enemy it;
//
//	double chance = (double)(rand() % 10) * modifier->evilfactor;
//	int type = GetType(modifier);
//
//
//}

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