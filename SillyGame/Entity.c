#define _CRT_SECURE_NO_WARNINGS

#include "Entity.h"
#include "Difficulty.h"
#include "json_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>  // for floor()

EntityRegistry g_entity_registry = { 0 };


void CreateEntity(Entity* e, int id, const char* name, int base_hp, int base_dmg, const char* rarity, int opinion, int primary_stage, int secondary_stage, double spawn_weight) {
    e->id = id;
    e->metadata.tag_amt = 0;

    MD_SetString(&e->metadata, "Name", name);
    MD_SetString(&e->metadata, "Rarity", rarity);

    // Constant stats
    MD_SetInt(&e->metadata, "BaseHP", base_hp);
    MD_SetInt(&e->metadata, "BaseDMG", base_dmg);

    // Will change stats
    MD_SetInt(&e->metadata, "CurrentHP", base_hp);
    MD_SetInt(&e->metadata, "ActualDMG", base_dmg);
    MD_SetInt(&e->metadata, "Opinion", opinion);

    // Stage data
    MD_SetInt(&e->metadata, "PrimaryStage", primary_stage);
    MD_SetInt(&e->metadata, "SecondaryStage", secondary_stage);

    // For Rouge Like mode
    MD_SetDouble(&e->metadata, "SpawnWeight", spawn_weight);
}

void ScaleEntityStats(Entity* e, DConfig config) {
    int base_hp, base_dmg;
    double multiplier = config.enemy_multiplier;

    if (MD_GetInt(&e->metadata, "BaseHP", &base_hp) &&
        MD_GetInt(&e->metadata, "BaseDMG", &base_dmg)) {

        int scaled_hp = (int)floor(base_hp * multiplier);
        int scaled_dmg = (int)floor(base_dmg * multiplier);

        MD_SetInt(&e->metadata, "CurrentHP", scaled_hp);
        MD_SetInt(&e->metadata, "CurrentDMG", scaled_dmg);
    }
}

void PrintEntity(const Entity* e) {
    char name[64], rarity[32];
    int base_hp, base_dmg, curr_hp, curr_dmg, opinion;

    MD_GetString((MD*)&e->metadata, "Name", name, sizeof(name));
    MD_GetString((MD*)&e->metadata, "Rarity", rarity, sizeof(rarity));

    MD_GetInt((MD*)&e->metadata, "BaseHP", &base_hp);
    MD_GetInt((MD*)&e->metadata, "BaseDMG", &base_dmg);
    MD_GetInt((MD*)&e->metadata, "CurrentHP", &curr_hp);
    MD_GetInt((MD*)&e->metadata, "CurrentDMG", &curr_dmg);
    MD_GetInt((MD*)&e->metadata, "Opinion", &opinion);

    printf("Entity ID: %d\n", e->id);
    printf("Name: %s\n", name);
    printf("Rarity: %s\n", rarity);
    printf("Base Stats - HP: %d, DMG: %d\n", base_hp, base_dmg);
    printf("Current Stats - HP: %d, DMG: %d\n", curr_hp, curr_dmg);
    printf("Opinion: %d\n", opinion);
}

void EntityRegistry_LoadFromJSON(const char* path) {
    g_entity_registry.numEnt = LoadEntitiesJSON(path, g_entity_registry.entities, MAX_ENTITY_REGISTRY_SIZE);
}

const Entity* EntityRegistry_GetByID(int id) {
    for (int i = 0; i < g_entity_registry.numEnt; i++) {
        if (g_entity_registry.entities[i].id == id) {
            return &g_entity_registry.entities[i];
        }
    }
    return NULL;
}

Entity* EntityRegistry_CloneByID(int id) {
    const Entity* base = EntityRegistry_GetByID(id);
    if (!base) { return NULL; }

    Entity* clone = malloc(sizeof(Entity));
    if (!clone) { return NULL; }

    memcpy(clone, base, sizeof(Entity));
    return clone;
}