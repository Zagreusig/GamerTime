#pragma once
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Player.h"
#include "Items.h"
#include "Entity.h"
#include "Level.h"
#include <stdbool.h>

#define MAX_ENTITIES_PER_LEVEL 32
#define MAX_ITEMS_IN_LEVEL 64

#define MAX_CHECK_FLAGS 8


typedef struct Flags_struct {
	char key[32];
	bool statement;
} Flags;

typedef struct Game_State_struct {
	Player* player;
	Level* current_level;
	Flags flags[MAX_CHECK_FLAGS];
} GameState;

extern GameState g_save_state;

void GameState_Init(GameState* state);
void GameState_Del(GameState* state);

void GameState_Save_state(GameState* state, const char* filepath);
int GameState_Load_state(GameState* state, const char* filepath);

#define SERIALIZE_SLOT_ARRAY(name, arr, count) \
	do { \
		cJSON* j##name = cJSON_CreateArray(); \
		for (int i = 0; i < count; i++){ \
		if (arr[i]){ \
			cJSON* itemObj = cJSON_CreateObject(); \
			cJSON_AddNumberToObject(itemObj, "id", arr[i]->id); \
			cJSON_AddItemToObject(itemObj, "metadata", MD_ToJSON(&arr[i]->metadata)); \
			cJSON_AddItemToArray(j##name, itemObj); \
			} else { \
			cJSON_AddItemToArray(j##name, cJSON_CreateNull()); \
			} \
		} \
		cJSON_AddItemToObject(jequip, #name, j##name); \
	} while(0)

#define DESERIALIZE_SLOT_ARRAY(name, arr, max)\
		do {\
			cJSON* j##name = cJSON_GetObjectItem(jequip, #name);\
			if (j##name){\
				int len = cJSON_GetArraySize(j##name); \
				for (int i = 0; i < len && i < max; i++){ \
					cJSON* slot = cJSON_GetArrayItem(j##name, i); \
					if (!cJSON_IsObject(slot)) {arr[i] = NULL; continue; } \
					int id = cJSON_GetObjectItem(slot, "id")->valueint; \
					cJSON* jmeta = cJSON_GetObjectItem(slot, "metadata"); \
					const Item* base = ItemRegistry_GetByID(id); \
					if (!base) {arr[i] = NULL; continue; } \
					arr[i] = malloc(sizeof(Item)); \
					memcpy(arr[i], base, sizeof(Item)); \
					JSON_ToMD(jmeta, &arr[i]->metadata); \
				}\
			}\
		} while (0)

#endif