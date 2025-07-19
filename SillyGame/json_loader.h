#pragma once
#ifndef JSON_LOADER_H
#define JSON_LOADER_H

#include "NewGameState.h"
#include "Items.h"
#include "Entity.h"
#include "cJSON.h"
#include "MetDat.h"

int LoadItemJSON(const char* path, Item* items, int max_items);
int LoadEntitiesJSON(const char* path, Entity* entities, int max_entities);

cJSON* MD_ToJSON(const MD* meta);
void JSON_ToMD(cJSON* json, MD* meta);


#define LOAD_EQUIP_SLOT(name, arr, count) \
        do { \
            cJSON* jarr = cJSON_GetObjectItem(jequip, #name); \
            for (int i = 0; i < count && i < cJSON_GetArraySize(jarr); ++i) { \
                cJSON* eq = cJSON_GetArrayItem(jarr, i); \
                if (cJSON_IsNull(eq)) { arr[i] = NULL; continue; } \
                int id = cJSON_GetObjectItem(eq, "id")->valueint; \
                cJSON* jmeta = cJSON_GetObjectItem(eq, "metadata"); \
                Item* it = malloc(sizeof(Item)); \
                if (!it) continue; \
				Item_Create_FromMD(it, id, &jmeta); \
                JSON_ToMD(jmeta, &it->metadata); \
                arr[i] = it; \
            } \
        } while (0)

#endif