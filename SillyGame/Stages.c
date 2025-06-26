#include <stdio.h>
#include <stdlib.h>
#include "Stages.h"
#include "Items.h"
#include "Enemies.h"
#include "Entity.h"
#include "OnStart.h"
#include "Definitions.h"

// I'm going to change this entire bit. Instead it's going call the Table functions here.
// They will compile the information for each bassed on hard coded infornation.
// Eg: Instead of having Elite/Common/Boss functions used to store a master list, they will each
//	   be coded into a function per enemy with unique IDs and stage IDs. That way I can dynamically
//     add more enemies to be randomly dispersed into other stages, rather than hard code each enemy into each stage.

Stage* InitStage(Stage* level, int ID) {
	level->id = ID;
	level->Enemies = *InitEnemyTable(&level->Enemies);
	level->Bosses = *InitBossTable(&level->Bosses);
	level->Items = *InitLootTables(&level->Items, ID);
	level->Npcs = *InitEntityTable(&level->Npcs);
	return level;
}

Stage* StageInfoReference(int ID) {
	Stage* inDev = (Stage*)calloc(1, sizeof(Stage));
	
	if (inDev == NULL) {
		return NULL;
	}

	switch (ID)
	{
	default:
		return NULL;
		break;
	case 0:
		inDev->id = 0;
		inDev->Enemies = *InitEnemyTable(&inDev->Enemies);
		inDev->Bosses = *InitBossTable(&inDev->Bosses);
		inDev->Items = *InitLootTables(&inDev->Items, ID);
		inDev->Npcs = *InitEntityTable(&inDev->Npcs);
		goto end_func;
	case 1:

		inDev->id = 1;
		inDev->Enemies = *InitEnemyTable(&inDev->Enemies);
		inDev->Bosses = *InitBossTable(&inDev->Bosses);
		inDev->Items = *InitLootTables(&inDev->Items, ID);
		inDev->Npcs = *InitEntityTable(&inDev->Npcs);
	case 2:

		inDev->id = 2;
		inDev->Enemies = *InitEnemyTable(&inDev->Enemies);
		inDev->Bosses = *InitBossTable(&inDev->Bosses);
		inDev->Items = *InitLootTables(&inDev->Items, ID);
		inDev->Npcs = *InitEntityTable(&inDev->Npcs);
	}

end_func:
	return inDev;
	free(inDev);
}

int CountBosses(int StageID, char keyattribute, int key) {

}