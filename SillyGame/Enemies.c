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

Enemy* EliteClass(int id) {
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

Enemy* GetEnemy(Difficulty* modifier) {
	Enemy it;

	int type;
	double likelihood = (double)(rand()) / modifier->evilfactor;
	int enemyAmount = GetAmount(modifier);
	if (enemyAmount == 1) {
		enemyAmount++;
	}
}

int GetAmount(Difficulty* modifier) {
	return (int)(rand(modifier->evilfactor) % 4) + 1;
}