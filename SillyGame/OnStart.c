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

void OnStart(GameState* game) {
	GameState_Init(game);

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

	// FIXME: change so that there is a registry for all entity classes
	// eg: Elite / Boss

	// === Initialize Player ===
	Player* player = &game->player;
	strncpy(player->name, "Player", sizeof(Player));
	player->hp = 100;
	player->dmg = 10;
	player->persuasion = 5;
	InventoryInit(&player->inventory);
	player->status_amount = 0;
	memset(&player->equipment, 0, sizeof(Equipment));

	//FIXME: intitialize difficulty to NORMAL.
}