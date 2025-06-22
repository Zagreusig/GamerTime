#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "DifficultyConfig.h"
#include "Entity.h"
#include "Enemies.h"
#include "Items.h"

/*
* ENCOUNTER TYPES:
* 
* --- COMBAT ---
* Single target
* Multi target
* BOSS
* ~ Elite target possible as either single / multi, not boss ~
* 
* --- NPC ---
* Helping hand
* Vengful
* Jester encounter
* Shop
* - Tools / Weapon&Armor / Materials / Fortune Teller -
* 
* 
* 
*/

void RegularCombatEncounter(Difficulty* modifier) {
	// FIXME: Place in StartUp();
	srand(3);
	//


	Enemy* Enemies[5];
	int enemyTypes[5];
	double likelihood = (double)(rand()) / modifier->evilfactor;
	int enemyAmount = (int)(rand() * modifier->evilfactor) % 4 + 1;
	if (enemyAmount == 1) {
		enemyAmount++;
	}

	*Enemies = (Enemy*)malloc(enemyAmount * sizeof(Enemy));
	if (Enemies == NULL) {
		printf("Malloc err in CombatEncounter()\n");
		exit(1);
	}



}