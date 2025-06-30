#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include "Entity.h"
#include "Items.h"

#define MAX_STAGE_ENTITIES 64
#define MAX_LEVEL_ITEMS 64
#define DEFAULT_LEVEL &level, 0, "Default"

typedef struct Level_struct{
    int id;
    const char* name;
    Entity* entities[MAX_STAGE_ENTITIES];
    int entity_amount;
    Item* items[MAX_LEVEL_ITEMS];
    int item_amount;
} Level;

void Level_Init(Level* level, int id, const char* name);
void Level_AddEntityIfMatch(Level* level, Entity* e);
void Level_PrintEntities(const Level* stage);
void Level_AddItemIfMatch(Level* level, Item* item);
Entity* Level_GrabRandEntity(Level* level);


#endif
