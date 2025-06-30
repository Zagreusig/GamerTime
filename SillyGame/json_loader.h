#pragma once
#ifndef JSON_LOADER_H
#define JSON_LOADER_H

#include "GameState.h"
#include "Items.h"
#include "Entity.h"
#include "cJSON.h"
#include "MetDat.h"

int LoadItemJSON(const char* path, Item* items, int max_items);
int LoadEntitiesJSON(const char* path, Entity* entities, int max_entities);

cJSON* MD_ToJSON(const MD* meta);
void JSON_ToMD(cJSON* json, MD* meta);

int GameState_Load_state(GameState* state, const char* path);

#endif