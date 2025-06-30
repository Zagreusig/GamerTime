#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "MetDat.h"
#include "Difficulty.h"

#define MAX_ENTITIES 16

typedef struct Entity_struct {
    int id;
    MD metadata;
} Entity;

void CreateEntity(Entity* e, int id, const char* name, int base_hp, int base_dmg, const char* rarity, int opinion, int primary_stage, int secondary_stage, double spawn_weight);
void ScaleEntityStats(Entity* e, DConfig config);
void PrintEntity(const Entity* e);

// Entity Reference Registry

#define MAX_ENTITY_REGISTRY_SIZE 256

typedef struct Entity_Registry_struct {
    Entity entities[MAX_ENTITY_REGISTRY_SIZE];
    int numEnt;
}EntityRegistry;

extern EntityRegistry g_entity_registry;

void EntityRegistry_LoadFromJSON(const char* path);
const Entity* EntityRegistry_GetByID(int id);
Entity* EntityRegistry_CloneByID(int id);


#endif