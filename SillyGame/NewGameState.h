#pragma once
#ifndef TEST_H
#define TEST_H

#include "Player.h"
#include "Items.h"
#include "Entity.h"
#include "Level.h"
#include <stdbool.h>

#define MAX_ENTITIES_PER_LEVEL 32
#define MAX_ITEMS_IN_LEVEL 64

#define MAX_CHECK_FLAGS 8

typedef struct Flags_Struct {
	char key[32];
	bool statement;
}Flags;

typedef struct Game_state_struct {
	Player* player;
	Level* current_level;
	Flags flags[MAX_CHECK_FLAGS];
}GameState;

extern GameState g_save_state;

void GameState_Save_state(GameState* state, const char* filepath);
int GameState_Load_State(GameState* state, const char* filepath);
void SetSavePath(const char* path, int num);
int SaveFileCheck(int slot);
char* FilePathList(int num);
int SaveFileCountCheck();
int SaveFileDelete(const char* path);

#define SERIALIZE_SLOT_ARRAY(name, arr, count)\
do { \
    cJSON* j##name = cJSON_CreateArray(); \
    if (!j##name) { fprintf(stderr, "[saving] Failed to create array for slot '%s'.\n", #name); exit(1); } \
    for (int i = 0; i < count; i++) { \
        if (arr[i]) { \
            cJSON* itemObj = cJSON_CreateObject(); \
            if (!itemObj) continue; \
            cJSON_AddNumberToObject(itemObj, "id", arr[i]->id); \
            cJSON* jmeta = MD_ToJSON(&arr[i]->metadata); \
            if (!jmeta) continue; \
            cJSON_AddItemToObject(itemObj, "metadata", jmeta); \
            cJSON_AddItemToArray(j##name, itemObj); \
        } else { \
            cJSON_AddItemToArray(j##name, cJSON_CreateNull()); \
        } \
    } \
    cJSON_AddItemToObject(jequip, #name, j##name); \
} while (0)

#define DESERIALIZE_SLOT_ARRAY(name, arr, max) \
do { \
	cJSON* j##name = cJSON_GetObjectItem(jequip, #name); \
	if (j##name && cJSON_IsArray(j##name)) { \
		int len = cJSON_GetArraySize(j##name); \
		for (int i = 0; i < len && i < max; i++) { \
			cJSON* slot = cJSON_GetArrayItem(j##name, i); \
			if (!slot || !cJSON_IsObject(slot)) { arr[i] = NULL; continue; } \
			cJSON* jid = cJSON_GetObjectItem(slot, "id"); \
			cJSON* jmeta = cJSON_GetObjectItem(slot, "metadata"); \
			if (!cJSON_IsNumber(jid) || !cJSON_IsObject(jmeta)) { arr[i] = NULL; continue; } \
			const Item* base = ItemRegistry_GetByID(jid->valueint); \
			if (!base) { arr[i] = NULL; continue; } \
			arr[i] = malloc(sizeof(Item)); \
			memcpy(arr[i], base, sizeof(Item)); \
			JSON_ToMD(jmeta, &arr[i]->metadata); \
		} \
	} \
} while (0)


#endif