/*
*
*
*
*
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Entity.h"
#include "Inventory.h"
#include "Items.h"
#include "DifficultyConfig.h"
#include "MscFuncs.h"

int main(void) {
    Difficulty* diff = malloc(sizeof(Difficulty));
    Item item;

    for (int i = 0; i < 3; i++) {
        item = ItemIDsMaster(i);
        DebugItem(&item);
    }

    diff = DifficultySet();
    if (diff == NULL) {
        printf("Malloc err.\n");
        return 1;
    }

    DebugDifficultyConfig(diff);

    return 0;
}
