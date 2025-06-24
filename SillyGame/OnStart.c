#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "StructDefinitions.h"
#include "Items.h"
#include "Enemies.h"
#include "Entity.h"
#include "Inventory.h"
#include "OnStart.h"

//void StartUp(ITable* items, EYTable* enemies, ENTTable* entities, BTable* bosses) {
//	bool item = false, enemy = false, boss = false, npc = false;
//	int err = 0;
//	
//	item = ItemTablesFinished(item, items);
//	enemy = EnemyTablesFinished(item, enemies);
//	npc = NPCTablesFinished(item, entities);
//	boss = BossTableFinished(item, bosses);
//
//	if (!item) {
//		err = 1;
//	}
//	else {
//		printf("Item tables successfully initialized.\n");
//	}
//
//	if (!enemy) {
//		err = 2;
//	}
//	else {
//		printf("Enemy tables successfully initialized.\n");
//	}
//
//	if (!boss) {
//		err = 3;
//	}
//	else {
//		printf("Boss tables successfully initialized.\n");
//	}
//
//	if (!npc) {
//		err = 4;
//	}
//	else {
//		printf("Npc tables successfullly initalized.\n");
//	}
//
//	if (err = 0) {
//		printf("All initializations successful.\n");
//	}
//
//
//}

/*bool ItemTablesFinished(bool out, ITable* table) {
	if (table == NULL) {
		return false;
	}
	else {
		return true;
	}
}

bool EnemyTablesFinished(bool out, EYTable* table) {
	if (table == NULL) {
		return false;
	}
	else {
		return true;
	}
}

bool BossTableFinished(bool out, BTable* table) {
	if (table == NULL) {
		return false;
	}
	else {
		return true;
	}
}

bool NPCTablesFinished(bool out, ENTTable* table) {
	if (table == NULL) {
		return false;
	}
	else {
		return true;
	}
}*/

ITable* InitLootTables(ITable* table, int stageID) {
	ITable* build = (ITable*)malloc(sizeof(ITable));
	Item* item = (Item*)malloc(sizeof(Item));
	for (int i = 0; i < MAX_ITEMS; i++) {
		item = ItemIDsMaster(i);
		if (item == NULL) {
			goto end_func;
		}
		build->item[i] = *item;
	}
	end_func:
	table = build;
	free(build);
	free(item);
	return table;
}

EYTable* InitEnemyTable(EYTable* table) { // Stage ID ?
	EYTable* build = (EYTable*)malloc(sizeof(EYTable));
	Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));
	int j = 0;
	int r = 0;
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (i < 3) {
			enemy = CommonClass(i);
		}
		if (i >= 3 && i < 6) {
			enemy = EliteClass(j);
			j++;
		}
		if (i >= 6) {
			enemy = SecretSecret();
			r = 1;
		}
		build->enemy[i] = *enemy;
		if (r == 1) {
			goto end_function;
		}
	}

end_function:
	return build;
	free(build);
	free(enemy);
}

BTable* InitBossTable(BTable* table) {
	BTable* build = (BTable*)malloc(sizeof(BTable));
	Enemy* boss = (Enemy*)malloc(sizeof(Enemy));

	for (int i = 0; i < MAX_BOSSES; i++) {
		boss = BossClass(i);
		if (boss == NULL) {
			goto end_func;
		}
		build->boss[i] = *boss;
	}


end_func:
	table = build;
	free(boss);
	free(build);
	return table;
}

ENTTable* InitEntityTable(ENTTable* table) {
	ENTTable* build = (ENTTable*)malloc(sizeof(ENTTable));
	Entity* e = (Entity*)malloc(sizeof(Entity));

	e->damage = 1;
	e->health = 1;
	e->id = 1;
	e->interact = 'c';
	e->persuation = 1;
	e->type = 1;

	build->entity[0] = *e;
	build->entity[1] = *e;
	build->entity[2] = *e;
	build->entity[3] = *e;
	build->entity[4] = *e;

	return build;
	free(e);
	free(build);
}