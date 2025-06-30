#include "Difficulty.h"
#include "Level.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Level_Init(Level* level, int id, const char* name) {
    level->id = id;
    level->name = name;
    level->entity_amount = 0;
}

void Level_AddEntityIfMatch(Level* level, Entity* e) {
    int primary = 0, secondary = 0;
    if (MD_GetInt(&e->metadata, "PrimaryStage", &primary) &&
        MD_GetInt(&e->metadata, "SecondaryStage", &secondary)) {

        if (primary == level->id || secondary == level->id) {
            if (level->entity_amount < MAX_STAGE_ENTITIES) {
                level->entities[level->entity_amount++] = e;
            }
        }
    }
}

void Level_PrintEntities(const Level* level) {
    printf("Level %d: %s\n", level->id, level->name);
    printf("Entities in this level:\n");
    for (int i = 0; i < level->entity_amount; ++i) {
        char name[64];
        MD_GetString(&level->entities[i]->metadata, "Name", name, sizeof(name));
        printf("  - [%d] %s\n", level->entities[i]->id, name);
    }
}

Entity* Level_GrabRandEntity(Level* level) {
    int total_weight = 0;
    int weights[MAX_STAGE_ENTITIES];

    for (int i = 0; i < level->entity_amount; ++i) {
        int weight = 1;
        MD_GetInt(&level->entities[i]->metadata, "SpawnWeight", &weight);
        weights[i] = weight;
        total_weight += weight;
    }

    int r = rand() % total_weight;
    int running = 0;

    for (int i = 0; i < level->entity_amount; ++i) {
        running += weights[i];
        if (r < running) {
            return level->entities[i];
        }
    }
    return NULL;
}

void Level_AddItemIfMatch(Level* level, Item* item) {
    int primary = 0, secondary = 0;
    if (MD_GetInt(&item->metadata, "PrimaryStage", &primary) &&
        MD_GetInt(&item->metadata, "SecondaryStage", &secondary)) {

        if (primary == level->id || secondary == level->id) {
            if (level->item_amount < MAX_LEVEL_ITEMS) {
                level->items[level->item_amount++] = item;
            }
        }
    }
}