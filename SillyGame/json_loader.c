#define _CRT_SECURE_NO_WARNINGS

#include "GameState.h"
#include "Inventory.h"
#include "Items.h"
#include "MetDat.h"
#include "json_loader.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* ReadFile(const char* path) {
	FILE* file = fopen(path, "rb");
	if (!file) {
		fprintf(stderr, "Failed to open file: %s\n", path);
		return NULL;
	}

	// Get file length
	if (fseek(file, 0, SEEK_END) != 0) {
		fclose(file);
		fprintf(stderr, "Failed to seek to end of file: %s\n", path);
		return NULL;
	}

	long len = ftell(file);
	if (len < 0) {
		fclose(file);
		fprintf(stderr, "Failed to get file length: %s\n", path);
		return NULL;
	}

	rewind(file);

	// Allocate buffer with +1 for null terminator
	char* data = (char*)malloc(len + 1);
	if (!data) {
		fclose(file);
		fprintf(stderr, "Memory allocation failed for file: %s\n", path);
		return NULL;
	}

	// Read file contents
	size_t read = fread(data, 1, len, file);
	fclose(file);

	if (read != (size_t)len) {
		fprintf(stderr, "Failed to read full file: %s\n", path);
		free(data);
		return NULL;
	}

	data[len] = '\0'; // Null-terminate for safe string usage
	return data;
}


int LoadItemJSON(const char* path, Item* items, int max_items) {
	char* data = ReadFile(path);
	if (!data) { return 0; }

	cJSON* root = cJSON_Parse(data);
	if (!root) { return 0; }

	int count = cJSON_GetArraySize(root);
	for (int i = 0; i < count && i < max_items; i++) {
		cJSON* entry = cJSON_GetArrayItem(root, i);

		int id = cJSON_GetObjectItem(entry, "id")->valueint;
		const char* name = cJSON_GetObjectItem(entry, "name")->valuestring;
		int durability = cJSON_GetObjectItem(entry, "durability")->valueint;
		int damage = cJSON_GetObjectItem(entry, "damage")->valueint;
		const char* desc = cJSON_GetObjectItem(entry, "description")->valuestring;
		int stage1 = cJSON_GetObjectItem(entry, "primaryStage")->valueint;
		int stage2 = cJSON_GetObjectItem(entry, "secondaryStage")->valueint;

		CreateItem(&items[i], id, name, durability, damage, desc);
		MD_SetInt(&items[i].metadata, "PrimaryStage", stage1);
		MD_SetInt(&items[i].metadata, "SecondaryStage", stage2);
	}

	cJSON_Delete(root);
	free(data);
	return count;
}

int LoadEntitiesJSON(const char* path, Entity* entities, int max_entities) {
	char* data = ReadFile(path);
	if (!data) { return 0; }

	cJSON* root = cJSON_Parse(data);
	if (!root) { return 0; }

	int count = cJSON_GetArraySize(root);
	for (int i = 0; i < count && i < max_entities; i++) {
		cJSON* entry = cJSON_GetArrayItem(root, i);
	
		int id = cJSON_GetObjectItem(entry, "id")->valueint;
		const char* name = cJSON_GetObjectItem(entry, "name")->valuestring;
		int baseHP = cJSON_GetObjectItem(entry, "baseHP")->valueint;
		int baseDMG = cJSON_GetObjectItem(entry, "baseDMG")->valueint;
		const char* rarity = cJSON_GetObjectItem(entry, "rarity")->valuestring;
		int opinion = cJSON_GetObjectItem(entry, "opinion")->valueint;
		int stage1 = cJSON_GetObjectItem(entry, "primaryStage")->valueint;
		int stage2 = cJSON_GetObjectItem(entry, "secondaryStage")->valueint;
		double weight = cJSON_GetObjectItem(entry, "spawnWeight")->valuedouble;

		CreateEntity(&entities[i], id, name, baseHP, baseDMG, rarity, opinion, stage1, stage2, weight);
	}

	cJSON_Delete(root);
	free(data);
	return count;
}

// Converts our MetaData tags -> cJSON objects [:
cJSON* MD_ToJSON(const MD* md) {
	if (!md) return NULL;

	cJSON* obj = cJSON_CreateObject();
	if (!obj) return NULL;

	for (int i = 0; i < md->tag_amt; i++) {
		MD_Tag* tag = &md->tags[i];
		if (!tag->key) continue;

		switch (tag->type) {
		case TAG_INT:
			cJSON_AddNumberToObject(obj, tag->key, tag->int_val);
			break;
		case TAG_DOUBLE:
			cJSON_AddNumberToObject(obj, tag->key, tag->double_val);
			break;
		case TAG_STRING:
			if (tag->string_val)
				cJSON_AddStringToObject(obj, tag->key, tag->string_val);
			break;
		default:
			break;
		}
	}
	return obj;
}


// Converting JSON -> MD tags
void JSON_ToMD(cJSON* json, MD* md) {
	if (!json || !md) return;  // null safety

	cJSON* item = NULL;
	cJSON_ArrayForEach(item, json) {
		const char* key = item->string;
		if (!key) continue;

		if (cJSON_IsNumber(item)) {
			double num = item->valuedouble;

			// Attempt to preserve decimal precision
			if ((double)((int)num) == num) {
				MD_SetInt(md, key, (int)num);  // ensure correct cast
			}
			else {
				MD_SetDouble(md, key, num);
			}
		}
		else if (cJSON_IsString(item)) {
			if (item->valuestring) {
				MD_SetString(md, key, item->valuestring);
			}
		}
		else {
			fprintf(stderr, "[JSON_ToMD] Unsupported type for key: %s\n", key);
		}
	}
}


int GameState_Load_state(GameState* state, const char* path) {
	char* data = ReadFile(path);
	if (!data) { return 0; }

	cJSON* root = cJSON_Parse(data);
	if (!root) { free(data); return 0; }

	cJSON* jplayer = cJSON_GetObjectItem(root, "player");
	if (!jplayer) { return 0; }

	Player* p = &state->player;

	p->name = _strdup(cJSON_GetObjectItem(jplayer, "name")->valuestring);
	p->hp = cJSON_GetObjectItem(jplayer, "hp")->valueint;
	p->dmg = cJSON_GetObjectItem(jplayer, "dmg")->valueint;
	p->persuasion = cJSON_GetObjectItem(jplayer, "persuasion")->valueint;
	p->stealth = cJSON_GetObjectItem(jplayer, "stealth")->valueint;

	// Status Effects
	cJSON* jstats = cJSON_GetObjectItem(jplayer, "status_effects");
	cJSON* se = NULL;
	cJSON_ArrayForEach(se, jstats) {
		const char* name = cJSON_GetObjectItem(se, "name")->valuestring;
		double strength = cJSON_GetObjectItem(se, "strength")->valuedouble;
		int duration = cJSON_GetObjectItem(se, "duration")->valueint;
		int stack = cJSON_GetObjectItem(se, "stack")->valueint;
		Player_AddStatus(p, name, strength, duration);
		p->status_effects[p->status_amount - 1].stacks = stack;
	}

	// Inventory
	cJSON* jinv = cJSON_GetObjectItem(jplayer, "inventory");
	cJSON* item = NULL;
	cJSON_ArrayForEach(item, jinv) {
		int id = cJSON_GetObjectItem(item, "id")->valueint;
		cJSON* jmeta = cJSON_GetObjectItem(item, "metadata");
		
		if (!jmeta) { continue; }

		MD metadat;
		metadat.tag_amt = 0;
		JSON_ToMD(jmeta, &metadat);

		Item* item = malloc(sizeof(Item));
		
		if (!item) { continue; }

		Item_Create_FromMD(item, id, &metadat);
		Inventory_AddItem(&p->inventory, item);
	}

	// Equipment
	cJSON* jequip = cJSON_GetObjectItem(jplayer, "equipment");

	LOAD_EQUIP_SLOT(armor, p->equipment.armor, MAX_ARMOR_SLOTS);
	LOAD_EQUIP_SLOT(accessories, p->equipment.accessories, MAX_ACCESSORY_SLOTS);
	LOAD_EQUIP_SLOT(weapons, p->equipment.weapons, MAX_WEAPON_SLOTS);

	// FIXME: ADD ENTITY LOGIC

	cJSON_Delete(root);
	free(data);
	return 1;
}

void GameState_Init() {
	memset(&g_save_state, 0, sizeof(GameState));
	Level_Init(g_save_state.current_level, 0, "Default");

}

void GameState_Del(GameState* state) {
	free(state->player);
	free(state->current_level);
	free(state);
}