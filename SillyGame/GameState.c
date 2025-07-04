#define _CRT_SECURE_NO_WARNINGS

/*
*	
*  Zagreus Silvey 06/2025
*  This file serves to create a "shadow" of the game to be serialized for saves.
*  Calls on the difficulty, item and entity registries to do so. 
*  Utilizing cJSON to store into a JSON file.
* 
*/

#include "cJSON.h"
#include "json_loader.h"
#include "Player.h"
#include "GameState.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern ItemRegistry g_item_registry;
extern EntityRegistry g_entity_registry;
extern DConfig g_difficulty;

GameState g_save_state = NULL;

void GameState_Save_state(GameState* state, const char* filepath) {
	cJSON* root = cJSON_CreateObject();
	cJSON* jplayer = cJSON_CreateObject();

	Player* player = &state->player;
	memset(player, 0, sizeof(Player));

	cJSON_AddStringToObject(jplayer, "name", player->name);
	cJSON_AddNumberToObject(jplayer, "hp", player->hp);
	cJSON_AddNumberToObject(jplayer, "dmg", player->dmg);
	cJSON_AddNumberToObject(jplayer, "persuasion", player->persuasion);

	// Status Effects
	cJSON* jstatuses = cJSON_CreateArray();
	for (int i = 0; i < player->status_amount; i++) {
		PlayerStatus* stat = &player->status_effects[i];
		cJSON* jstatus = cJSON_CreateObject();
		cJSON_AddStringToObject(jstatus, "name", stat->name);
		cJSON_AddNumberToObject(jstatus, "strength", stat->strength);
		cJSON_AddNumberToObject(jstatus, "duration", stat->duration);
		cJSON_AddNumberToObject(jstatus, "stacks", stat->stacks);
		cJSON_AddItemToArray(jstatuses, jstatus);
	}
	cJSON_AddItemToObject(jplayer, "status_effects", jstatuses);

	// Inventory
	cJSON* jinv = cJSON_CreateArray();
	for (int i = 0; i < player->inventory.totalItems; i++) {
		Item* item = player->inventory.items[i];
		cJSON* jitem = cJSON_CreateObject();
		cJSON_AddNumberToObject(jitem, "id", item->id);
		cJSON_AddItemToObject(jitem, "metadata", MD_ToJSON(&item->metadata));
		cJSON_AddItemToArray(jinv, jitem);
	}
	cJSON_AddItemToObject(jplayer, "inventory", jinv);

	//Equipment
	cJSON* jequip = cJSON_CreateObject();
	SERIALIZE_SLOT_ARRAY(armor, player->equipment.armor, MAX_ARMOR_SLOTS);
	SERIALIZE_SLOT_ARRAY(accessories, player->equipment.accessories, MAX_ACCESSORY_SLOTS);
	SERIALIZE_SLOT_ARRAY(weapons, player->equipment.weapons, MAX_WEAPON_SLOTS);

	cJSON_AddItemToObject(jplayer, "equipment", jequip);
	cJSON_AddItemToObject(root, "player", jplayer);

	cJSON* jdifficulty = cJSON_CreateObject();
	cJSON_AddNumberToObject(jdifficulty, "id", g_difficulty.diff)->valueint;
	cJSON_AddItemToObject(root, "difficulty", jdifficulty);


	// Things coming from the level struct.
	Level* level = &state->current_level;
	cJSON* jlevel = cJSON_CreateObject();
	if (!level || !jlevel) { fprintf(stderr, "[saving] err with level data.\n"); exit(1); }
	
	cJSON_AddNumberToObject(jlevel, "id", level->id)->valueint;
	cJSON_AddStringToObject(jlevel, "name", level->name);

	// Entities
	cJSON* jentities = cJSON_CreateArray();
	for (int i = 0; i < level->entity_amount; i++) {
		Entity* e = level->entities[i];
		if (!e) { continue; }

		cJSON* je = cJSON_CreateObject();
		cJSON_AddNumberToObject(je, "id", e->id);
		cJSON_AddItemToObject(je, "metadata", MD_ToJSON(&e->metadata));
		cJSON_AddItemToArray(jentities, je);
	}
	cJSON_AddItemToObject(jlevel, "entities", jentities);


	// Items
	cJSON* jitems = cJSON_CreateArray();
	for (int i = 0; i < level->item_amount; i++) {
		Item* t = level->items[i];
		if (!t) { continue; }

		cJSON* jt = cJSON_CreateObject();
		cJSON_AddNumberToObject(jt, "id", t->id)->valueint;
		cJSON_AddItemToObject(jt, "metdata", MD_ToJSON(&t->metadata));
		cJSON_AddItemToArray(jitems, jt);
	}
	cJSON_AddItemToObject(jlevel, "items", jitems);
	cJSON_AddItemToObject(root, "level", jlevel);

	char* json_string = cJSON_Print(root);
	FILE* file = fopen(filepath, "w");
	if (!file) {
		fprintf(stderr, "[saving] Error creating save file.\n");
		exit(1);
	}
	if (file) {
		fprintf(file, "%s\n", json_string);
		fclose(file);
	}

	cJSON_Delete(root);
	free(json_string);
}

int GameState_Load_State(GameState* state, const char* path) {
	FILE* file = fopen(path, "rb");
	if (!file) { return 0; }

	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	rewind(file);
	
	char* data = malloc(len + 1);
	fread(data, 1, len, file);
	data[len] = '\0';
	fclose(file);

	cJSON* root = cJSON_Parse(data);
	free(data);

	cJSON* jplayer = cJSON_GetObjectItem(root, "player");
	if (!jplayer) { cJSON_Delete(root); return 0; }
	Player* player = &state->player;
	memset(player, 0, sizeof(Player));

	strncpy(player->name, cJSON_GetObjectItem(jplayer, "name")->valuestring, sizeof(player->name));
	player->hp = cJSON_GetObjectItem(jplayer, "hp")->valueint;
	player->dmg = cJSON_GetObjectItem(jplayer, "dmg")->valueint;
	player->persuasion = cJSON_GetObjectItem(jplayer, "persuasion")->valueint;
	player->stealth = cJSON_GetObjectItem(jplayer, "stealth")->valueint;

	// Status effects
	cJSON* jstatuses = cJSON_GetObjectItem(jplayer, "status_effects");
	if (jstatuses) {
		int count = cJSON_GetArraySize(jstatuses);
		for (int i = 0; i < count && i < MAX_STATUS_EFFECTS; i++) {
			cJSON* jstatus = cJSON_GetArrayItem(jstatuses, i);
			PlayerStatus* effect = &player->status_effects[i];
			strncpy(effect->name, cJSON_GetObjectItem(jstatus, "name")->valuestring, sizeof(effect->name));
			effect->strength = cJSON_GetObjectItem(jstatus, "strength")->valuedouble;
			effect->duration = cJSON_GetObjectItem(jstatus, "duration")->valueint;
			effect->stacks = cJSON_GetObjectItem(jstatus, "stacks")->valueint;
		}
		player->status_amount = count;
	}

	// Inventory
	InventoryInit(&player->inventory);
	cJSON* jinv = cJSON_GetObjectItem(jplayer, "inventory");
	if (jinv) {
		int count = cJSON_GetArraySize(jinv);
		for (int i = 0; i < count && i < INVENTORY_SIZE; i++) {
			cJSON* jitem = cJSON_GetArrayItem(jinv, i);
			int id = cJSON_GetObjectItem(jitem, "id")->valueint;
			cJSON* jmeta = cJSON_GetObjectItem(jitem, "metadata");

			const Item* base = ItemRegistry_GetByID(id);
			if (!base) { continue; }

			Item* new_item = malloc(sizeof(Item));
			memcpy(new_item, base, sizeof(Item));
			JSON_ToMD(jmeta, &new_item->metadata);
			Inventory_AddItem(&player->inventory, new_item);
		}
	}

	// Equipment
	cJSON* jequip = cJSON_GetObjectItem(jplayer, "equipment");
	if (jequip) {

		DESERIALIZE_SLOT_ARRAY(armor, player->equipment.armor, MAX_ARMOR_SLOTS);
		DESERIALIZE_SLOT_ARRAY(accessories, player->equipment.accessories, MAX_ACCESSORY_SLOTS);
		DESERIALIZE_SLOT_ARRAY(weapons, player->equipment.weapons, MAX_WEAPON_SLOTS);
	}

	
	// Deserializing level.

	cJSON* jlevel = cJSON_GetObjectItem(root, "level");
	if (!jlevel) { cJSON_Delete(root); return 0; }
	Level* level = &state->current_level;
	memset(level, 0, sizeof(Level));

	level->id = cJSON_GetObjectItem(jlevel, "id");
	level->name = cJSON_GetObjectItem(jlevel, "name");

	// Entities
	cJSON* jentities = cJSON_GetObjectItem(jlevel, "entities");
	if (jentities) {
		int count = cJSON_GetArraySize(jentities);
		for (int i = 0; i < count && i < MAX_ENTITIES_PER_LEVEL; i++) {
			cJSON* je = cJSON_GetArrayItem(jentities, i);
			if (!cJSON_IsObject(je)) { continue; }

			int id = cJSON_GetObjectItem(je, "id")->valueint;
			cJSON* jmeta = cJSON_GetObjectItem(je, "metadata");

			Entity* base = EntityRegistry_CloneByID(id);
			if (!base) { continue; }

			JSON_ToMD(jmeta, &base->metadata);
			level->entities[level->entity_amount++] = base;
		}
	}

	cJSON* jdifficulty = cJSON_GetObjectItem(root, "difficulty");
	int id = cJSON_GetObjectItem(jdifficulty, "id");
	if (id >= 0) { SetDifficulty(&g_difficulty, id); }

	state->current_level = level;
	state->player = player;

	cJSON_Delete(root);
	return 1;
}