#define _CRT_SECURE_NO_WARNINGS

#include "Difficulty.h"
#include "Level.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Level_Init(Level* level, int id, const char* name) {
    if (id && name) {
        level->id = id;
        strcpy(level->name, name);
    }
    else {
        level->id = 0;
        strcpy(level->name, "Default Name");
    }
    level->entity_amount = 0;
    level->item_amount = 0;
    
    
    //FIXME: Remove (here for debug)
    int countere = 0, counteri = 0;
    for (int i = 0; i < MAX_ENTITIES_PER_LEVEL; i++) {
        if (EntityRegistry_CloneByID == NULL) { break; }
        else {
            level->entities[i] = EntityRegistry_CloneByID(i);
            countere++;
        }
    }
    for (int i = 0; i < MAX_ITEMS_IN_LEVEL; i++) {
        if (ItemRegistry_CloneByID(i) == NULL) { break; }
        else {
            level->items[i] = ItemRegistry_CloneByID(i);
            counteri++;
        }
    }
    level->item_amount = counteri;
    level->entity_amount = countere;


    // FIXME: Add in lookup by level id for monsters and init them here.
    /*for (int i = 0; i < MAX_ENTITIES_PER_LEVEL; i++) { level->entities[i] = NULL; }
    for (int i = 0; i < MAX_ITEMS_IN_LEVEL; i++) { level->items[i] = NULL; }*/
}

void Level_AddEntityIfMatch(Level* level, Entity* e) {
    int primary = 0, secondary = 0;
    if (MD_GetInt(&e->metadata, "PrimaryStage", &primary) &&
        MD_GetInt(&e->metadata, "SecondaryStage", &secondary)) {

        if (primary == level->id || secondary == level->id) {
            if (level->entity_amount < MAX_ENTITIES_PER_LEVEL) {
                level->entities[level->entity_amount++] = e;
            }
        }
    }
}

void Level_PrintEntities(const Level* level) {
    printf("Level %d: %s\n", level->id, level->name);
    printf("Entities in this level:\n");
    for (int i = 0; i < level->entity_amount; ++i) {
        PrintEntity(level->entities[i]);
    }
}

void Level_PrintItems(const Level* level) {
    printf("Level %d: %s\n", level->id, level->name);
    printf("Items in this level:\n");
    for (int i = 0; i < level->item_amount; ++i) {
        char name[64];
        MD_GetString(&level->entities[i]->metadata, "Name", name, sizeof(name));
        printf(" - [%d] %s\n", level->items[i]->id, name);
    }
}

Entity* Level_GrabRandEntity(Level* level) {
    int total_weight = 0, weights[MAX_ENTITIES_PER_LEVEL] = { 0 };

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
            if (level->item_amount < MAX_ITEMS_IN_LEVEL) {
                level->items[level->item_amount++] = item;
            }
        }
    }
}