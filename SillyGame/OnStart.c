#define _CRT_SECURE_NO_WARNINGS

#include "GameState.h"
#include "json_loader.h"
#include "Items.h"
#include "Entity.h"
#include "Player.h"
#include "Difficulty.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External global registries
extern ItemRegistry g_item_registry;
extern EntityRegistry g_entity_registry;
extern DConfig g_difficulty;
extern GameState g_save_state;

void OnStart() {

	// === Load Item Registry ===
	memset(&g_item_registry, 0, sizeof(ItemRegistry));
	int item_count = LoadItemJSON("data/items.json", g_item_registry.items, MAX_ITEM_REGISTRY_SIZE);
	g_item_registry.numitems = item_count;
	printf("Loaded %d items into registry.\n", item_count);

	// === Load Entity Registry ===
	memset(&g_entity_registry, 0, sizeof(EntityRegistry));
	int entity_count = LoadEntitiesJSON("data/entities.json", g_entity_registry.entities, MAX_ENTITY_REGISTRY_SIZE);
	g_entity_registry.numEnt = entity_count;
	printf("Loaded %d entities into registry.\n", entity_count);
	
	// === GameState initialization ===
	memset(&g_save_state, 0, sizeof(GameState));
	printf("Initialized GameState.\n");

	// FIXME: There will be specific (global) registries that are for each class.
	// FIXME: Check for save file.
}

void NewGame() {
	Player* player = (Player*)malloc(sizeof(Player));
	Level* level = (Level*)malloc(sizeof(Level));
	if (!player || !level) { fprintf(stderr, "Malloc err, NewGame()\n"); exit(1); }

	Player_Init(player, "Default Player");
	Level_Init(level, 1, "Default Level");
	
	g_save_state.player = player;
	g_save_state.current_level = level;

	/*FILE* save = fopen("/data/savestate.json", "r");
	char c;

	printf("Start new game? (y / n)\n");

	(void)scanf(" %c", &c);

	player->id = -1;
	player->name = "IF YOU SEE THIS ERROR HAPPEN";
	player->hp = 0;
	player->dmg = 0;
	player->persuasion = 0;
	player->stealth = 0;

	if (c == 'y' || c == 'Y' ^ !save) {

		char name[24] = { '\0' };

		printf("What is your name?\n");
		(void)scanf(" %s", &name);
		Player_Init(player, name);

		g_save_state.player = &player;

		SetDifficulty(&g_difficulty, DIFFICULTY_CHALLENGE);

		for (int i = 0; i < g_entity_registry.numEnt; i++) {
			ScaleEntityStats(&g_entity_registry.entities[i], g_difficulty);
		}

		Level_Init(&g_save_state.current_level, 0, "Default");
	}
	else {
		GameState_Load_State(&g_save_state, "/data/savestate.json");
	}*/

}