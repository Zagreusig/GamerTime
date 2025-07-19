#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include "Entity.h"
#include "Items.h"
#include "GameState.h"

#define DEFAULT_LEVEL &level, 0, "Default"

#define MAX_ENTITIES_PER_LEVEL 32
#define MAX_ITEMS_IN_LEVEL 64

typedef struct Level_struct {
    int id;
    char name[32];
    Entity* entities[MAX_ENTITIES_PER_LEVEL];
    int entity_amount;
    Item* items[MAX_ITEMS_IN_LEVEL];
    int item_amount;
} Level;

void Level_Init(Level* level, int id, const char* name);
void Level_AddEntityIfMatch(Level* level, Entity* e);
void Level_PrintEntities(const Level* stage);
void Level_AddItemIfMatch(Level* level, Item* item);
Entity* Level_GrabRandEntity(Level* level);


#endif
