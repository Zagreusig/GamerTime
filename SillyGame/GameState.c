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
#include "NewGameState.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern ItemRegistry g_item_registry;
extern EntityRegistry g_entity_registry;
extern DConfig g_difficulty;

GameState g_save_state = { NULL };

void GameState_Save_state(GameState* state, const char* filepath) {
    if (!state || !state->player || !state->current_level) {
        fprintf(stderr, "[saving] Invalid game state (missing player or level).\n");
        exit(1);
    }

    bool success = false;
    cJSON* root = cJSON_CreateObject();
    if (!root) { fprintf(stderr, "[saving] Failed to create root JSON.\n"); exit(1); }

    // --- PLAYER ---
    Player* player = state->player;
    cJSON* jplayer = cJSON_CreateObject();
    if (!jplayer) { fprintf(stderr, "[saving] Failed to create jplayer.\n"); goto cleanup; }

    cJSON_AddStringToObject(jplayer, "name", player->name);
    cJSON_AddNumberToObject(jplayer, "hp", player->hp);
    cJSON_AddNumberToObject(jplayer, "dmg", player->dmg);
    cJSON_AddNumberToObject(jplayer, "persuasion", player->persuasion);

    // --- Status Effects ---
    cJSON* jstatuses = cJSON_CreateArray();
    if (!jstatuses) { fprintf(stderr, "[saving] Failed to create status array.\n"); goto cleanup; }

    for (int i = 0; i < player->status_amount; i++) {
        PlayerStatus* stat = &player->status_effects[i];
        cJSON* jstatus = cJSON_CreateObject();
        if (!jstatus) continue;
        cJSON_AddStringToObject(jstatus, "name", stat->name);
        cJSON_AddNumberToObject(jstatus, "strength", stat->strength);
        cJSON_AddNumberToObject(jstatus, "duration", stat->duration);
        cJSON_AddNumberToObject(jstatus, "stacks", stat->stacks);
        cJSON_AddItemToArray(jstatuses, jstatus);
    }
    cJSON_AddItemToObject(jplayer, "status_effects", jstatuses);

    // --- Inventory ---
    cJSON* jinv = cJSON_CreateArray();
    if (!jinv) { fprintf(stderr, "[saving] Failed to create inventory array.\n"); goto cleanup; }

    for (int i = 0; i < player->inventory.totalItems; i++) {
        Item* item = player->inventory.items[i];
        if (!item) continue;

        cJSON* jitem = cJSON_CreateObject();
        if (!jitem) continue;

        cJSON_AddNumberToObject(jitem, "id", item->id);
        cJSON* jmeta = MD_ToJSON(&item->metadata);
        if (!jmeta) continue;
        cJSON_AddItemToObject(jitem, "metadata", jmeta);
        cJSON_AddItemToArray(jinv, jitem);
    }
    cJSON_AddItemToObject(jplayer, "inventory", jinv);

    // --- Equipment ---
    cJSON* jequip = cJSON_CreateObject();
    if (!jequip) { printf("[saving] Failed to create equipment object.\n"); goto cleanup; }

    SERIALIZE_SLOT_ARRAY(armor, player->equipment.armor, MAX_ARMOR_SLOTS);
    SERIALIZE_SLOT_ARRAY(accessories, player->equipment.accessories, MAX_ACCESSORY_SLOTS);
    SERIALIZE_SLOT_ARRAY(weapons, player->equipment.weapons, MAX_WEAPON_SLOTS);
    cJSON_AddItemToObject(jplayer, "equipment", jequip);

    // --- Difficulty ---
    cJSON* jdifficulty = cJSON_CreateObject();
    if (!jdifficulty) { printf("[saving] Failed to create difficulty object.\n"); goto cleanup; }

    cJSON_AddNumberToObject(jdifficulty, "id", g_difficulty.diff);

    // --- Level ---
    Level* level = state->current_level;
    cJSON* jlevel = cJSON_CreateObject();
    if (!jlevel) { printf("[saving] Failed to create level object.\n"); goto cleanup; }

    cJSON_AddNumberToObject(jlevel, "id", level->id);
    cJSON_AddStringToObject(jlevel, "name", level->name);

    // Entities
    cJSON* jentities = cJSON_CreateArray();
    if (!jentities) { printf("[saving] Failed to create entity array.\n"); goto cleanup; }

    for (int i = 0; i < level->entity_amount; i++) {
        Entity* e = level->entities[i];
        if (!e) continue;

        cJSON* je = cJSON_CreateObject();
        if (!je) continue;

        cJSON_AddNumberToObject(je, "id", e->id);
        cJSON* jmeta = MD_ToJSON(&e->metadata);
        if (!jmeta) continue;
        cJSON_AddItemToObject(je, "metadata", jmeta);
        cJSON_AddItemToArray(jentities, je);
    }
    cJSON_AddItemToObject(jlevel, "entities", jentities);

    // Items
    cJSON* jitems = cJSON_CreateArray();
    if (!jitems) { printf("[saving] Failed to create level items array.\n"); goto cleanup; }

    for (int i = 0; i < level->item_amount; i++) {
        Item* t = level->items[i];
        if (!t) continue;

        cJSON* jt = cJSON_CreateObject();
        if (!jt) continue;

        cJSON_AddNumberToObject(jt, "id", t->id);
        cJSON* jmeta = MD_ToJSON(&t->metadata);
        if (!jmeta) continue;
        cJSON_AddItemToObject(jt, "metadata", jmeta);
        cJSON_AddItemToArray(jitems, jt);
    }
    cJSON_AddItemToObject(jlevel, "items", jitems);

    // Add sections to root
    cJSON_AddItemToObject(root, "player", jplayer);
    cJSON_AddItemToObject(root, "difficulty", jdifficulty);
    cJSON_AddItemToObject(root, "level", jlevel);

    // Write to file
    char* json_string = cJSON_Print(root);
    if (!json_string) { printf("[saving] Failed to print JSON.\n"); goto cleanup; }

    FILE* file = fopen(filepath, "w+");
    if (!file) {
        printf("[saving] Could not open file for writing: %s\n", filepath);
        goto cleanup_json;
    }

    fprintf(file, "%s\n", json_string);
    fclose(file);
    success = true;

cleanup_json:
    free(json_string);
cleanup:
    cJSON_Delete(root);
    if (!success) {
        printf("[saving] Save failed.\n");
        exit(1);
    }
}

int GameState_Load_State(GameState* state, const char* filepath) {
	FILE* file = fopen(filepath, "rb");
	if (!file) { return 0; }

	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	rewind(file);

	char* data = malloc(len + 1);
	if (!data) { fclose(file); return 0; }

	fread(data, 1, len, file);
	data[len] = '\0';
	fclose(file);

	cJSON* root = cJSON_Parse(data);
	free(data);
	if (!root) return 0;

	cJSON* jplayer = cJSON_GetObjectItem(root, "player");
	if (!cJSON_IsObject(jplayer)) { cJSON_Delete(root); return 0; }

	Player* player = state->player;
	if (!player) { player = calloc(1, sizeof(Player)); state->player = player; }
	else { memset(player, 0, sizeof(Player)); }

	cJSON* val = NULL;

	val = cJSON_GetObjectItem(jplayer, "name");
	if (cJSON_IsString(val)) strncpy(player->name, val->valuestring, sizeof(player->name));

	val = cJSON_GetObjectItem(jplayer, "hp");
	if (cJSON_IsNumber(val)) player->hp = val->valueint;

	val = cJSON_GetObjectItem(jplayer, "dmg");
	if (cJSON_IsNumber(val)) player->dmg = val->valueint;

	val = cJSON_GetObjectItem(jplayer, "persuasion");
	if (cJSON_IsNumber(val)) player->persuasion = val->valueint;

	val = cJSON_GetObjectItem(jplayer, "stealth");
	if (cJSON_IsNumber(val)) player->stealth = val->valueint;

	// --- Status Effects ---
	cJSON* jstatuses = cJSON_GetObjectItem(jplayer, "status_effects");
	if (cJSON_IsArray(jstatuses)) {
		int count = cJSON_GetArraySize(jstatuses);
		for (int i = 0; i < count && i < MAX_STATUS_EFFECTS; i++) {
			cJSON* jstatus = cJSON_GetArrayItem(jstatuses, i);
			if (!cJSON_IsObject(jstatus)) continue;

			PlayerStatus* effect = &player->status_effects[i];

			val = cJSON_GetObjectItem(jstatus, "name");
			if (cJSON_IsString(val)) strncpy(effect->name, val->valuestring, sizeof(effect->name));

			val = cJSON_GetObjectItem(jstatus, "strength");
			if (cJSON_IsNumber(val)) effect->strength = val->valuedouble;

			val = cJSON_GetObjectItem(jstatus, "duration");
			if (cJSON_IsNumber(val)) effect->duration = val->valueint;

			val = cJSON_GetObjectItem(jstatus, "stacks");
			if (cJSON_IsNumber(val)) effect->stacks = val->valueint;
		}
		player->status_amount = count;
	}

	// --- Inventory ---
	InventoryInit(&player->inventory);
	cJSON* jinv = cJSON_GetObjectItem(jplayer, "inventory");
	if (cJSON_IsArray(jinv)) {
		int count = cJSON_GetArraySize(jinv);
		for (int i = 0; i < count && i < INVENTORY_SIZE; i++) {
			cJSON* jitem = cJSON_GetArrayItem(jinv, i);
			if (!cJSON_IsObject(jitem)) continue;

			val = cJSON_GetObjectItem(jitem, "id");
			if (!cJSON_IsNumber(val)) continue;
			int id = val->valueint;

			const Item* base = ItemRegistry_GetByID(id);
			if (!base) continue;

			Item* new_item = malloc(sizeof(Item));
			if (!new_item) continue;

			memcpy(new_item, base, sizeof(Item));

			cJSON* jmeta = cJSON_GetObjectItem(jitem, "metadata");
			if (cJSON_IsObject(jmeta)) {
				JSON_ToMD(jmeta, &new_item->metadata);
			}

			Inventory_AddItem(&player->inventory, new_item);
		}
	}

	// --- Equipment ---
	cJSON* jequip = cJSON_GetObjectItem(jplayer, "equipment");
	if (cJSON_IsObject(jequip)) {
		DESERIALIZE_SLOT_ARRAY(armor, player->equipment.armor, MAX_ARMOR_SLOTS);
		DESERIALIZE_SLOT_ARRAY(accessories, player->equipment.accessories, MAX_ACCESSORY_SLOTS);
		DESERIALIZE_SLOT_ARRAY(weapons, player->equipment.weapons, MAX_WEAPON_SLOTS);
	}

	// --- Level ---
	cJSON* jlevel = cJSON_GetObjectItem(root, "level");
	if (!cJSON_IsObject(jlevel)) { cJSON_Delete(root); return 0; }

	Level* level = state->current_level;
	if (!level) {
		level = calloc(1, sizeof(Level));
		state->current_level = level;
	}
	else {
		memset(level, 0, sizeof(Level));
	}

	val = cJSON_GetObjectItem(jlevel, "id");
	if (cJSON_IsNumber(val)) level->id = val->valueint;

	val = cJSON_GetObjectItem(jlevel, "name");
	if (cJSON_IsString(val)) strncpy(level->name, val->valuestring, sizeof(level->name));

	// --- Entities ---
	cJSON* jentities = cJSON_GetObjectItem(jlevel, "entities");
	if (cJSON_IsArray(jentities)) {
		int count = cJSON_GetArraySize(jentities);
		for (int i = 0; i < count && i < MAX_ENTITIES_PER_LEVEL; i++) {
			cJSON* je = cJSON_GetArrayItem(jentities, i);
			if (!cJSON_IsObject(je)) continue;

			val = cJSON_GetObjectItem(je, "id");
			if (!cJSON_IsNumber(val)) continue;
			int id = val->valueint;

			Entity* base = EntityRegistry_CloneByID(id);
			if (!base) continue;

			cJSON* jmeta = cJSON_GetObjectItem(je, "metadata");
			if (cJSON_IsObject(jmeta)) {
				JSON_ToMD(jmeta, &base->metadata);
			}

			level->entities[level->entity_amount++] = base;
		}
	}

	// --- Difficulty ---
	cJSON* jdifficulty = cJSON_GetObjectItem(root, "difficulty");
	val = jdifficulty ? cJSON_GetObjectItem(jdifficulty, "id") : 0;
	if (cJSON_IsNumber(val)) { SetDifficulty(&g_difficulty, val->valueint); }

	state->current_level = level;
	state->player = player;

	cJSON_Delete(root);
	return 1;
}
