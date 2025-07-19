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

	Player_Init(player, NULL);
	Level_Init(level, NULL, NULL);
	
	g_save_state.player = player;
	g_save_state.current_level = level;

}