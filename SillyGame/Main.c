#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "Entity.h"
#include "Items.h"
#include "Inventory.h"
#include "StatusEffects.h"
#include "GameState.h"
#include "OnStart.h"
#include "Level.h"
#include "Difficulty.h"
#include "json_loader.h"

int main(void) {
    Level* level = malloc(sizeof(Level));
    Entity* entities = malloc(sizeof(Entity) * MAX_ENTITIES);
    Item* items = malloc(sizeof(Item) * MAX_ITEMS);
    Player* player = malloc(sizeof(Player));

    if (!level || !entities || !items || !player) {
        fprintf(stderr, "Memory allocation failure.\n");
        return 1;
    }

    OnStart(player, items, entities, level);


    return 0;
}