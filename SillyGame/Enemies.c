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

}

Enemy* SecretSecret() {
	Enemy* secrets;
	secrets->MAX_HP = 1000;
	secrets->MAX_DMG = 9999;
	secrets->RARITY = 100;
	
}

Enemy* GetEnemyMulti(Difficulty* modifier) {
	Enemy it;

	double chance = (double)(rand()) * modifier->evilfactor;
	int type = GetType(modifier, chance);
	int enemyAmount = GetAmount(chance);
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

Enemy* GetEnemySingle(Difficulty* modifier) {
	Enemy it;

	double chance = (double)(rand() % 10) * modifier->evilfactor;
	int type = GetType(modifier);


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
	return (double)(rand(chance) % outof) + plus;
}

int GetIntRNG(double chance, int outof, int plus) {
	return (int)(rand(chance) % outof) + plus;
}

int GetType(Difficulty* modifier) {
	int type;
	double chance;
	double funnyChance;
	switch (modifier->code)
	{
	default:
		printf("GetType() err. \n");
		break;
	case 0:
		chance = GetEnemyChance(modifier);
		funnyChance = GetFunnyChance(modifier);
		type = GetIntRNG(chance, 4, 0);
		// lower chance of elite enemy
	case 1:
		chance = GetEnemyChance(modifier);
		funnyChance = GetFunnyChance(modifier);
		type = GetIntRNG(chance, 4, 0);
		// normal chance
	case 2:
		chance = GetEnemyChance(modifier);
		funnyChance = GetFunnyChance(modifier);
		type = GetIntRNG(chance, 4, 0);
		// slightly higher chance
	case 3:
		chance = GetEnemyChance(modifier);
		funnyChance = GetFunnyChance(modifier);
		type = GetIntRNG(chance, 4, 0);
	}	// very high chance

}