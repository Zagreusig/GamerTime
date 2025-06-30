#define _CRT_SECURE_NO_WARNINGS

#include "Level.h"
#include "Difficulty.h"
#include "items.h"
#include "Entity.h"
#include "Inventory.h"
#include "json_loader.h"
#include "OnStart.h"
#include <stdio.h>

void OnStart(Player* p, Item* item, Entity* ent, Level* level) {
    Player_Init(p, DEFAULT_STATS);
    printf("Player intialized.\n");
    int entities_num;
    int items_num;

    entities_num = LoadEntitiesJSON("data/Entities.json", ent, MAX_ENTITIES);
    items_num = LoadItemJSON("data/Items.json", item, MAX_ITEMS);
    ItemRegistry_LoadFromJSON("data/Items.json");
    EntityRegistry_LoadFromJSON("data/Entities.json");

    SetDifficulty(&config, DIFFICULTY_NORMAL);

    for (int i = 0; i < entities_num; i++) {
        ScaleEntityStats(&ent[i], config);
    }

    Level_Init(DEFAULT_LEVEL);

    printf("Loaded %d Entities.\n", entities_num);
    printf("Loaded %d Items.\n", items_num);
    printf("Item registry created.\n");
    printf("Entity registry created.\n");
    printf("Difficulty set.\n");
    printf("Entities stats scaled by %0.0lf percent.\n", (config.enemy_multiplier * 100));
    printf("Level initialized\n");

    printf("\n\nPrinting Debug\n\n");

    PrintPlayer(p);
    PrintDifficulty(&config);
    for (int i = 0; i < items_num; i++) {
        PrintItem(&item[i]);
    }
    for (int i = 0; i < entities_num; i++) {
        PrintEntity(&ent[i]);

    }
}