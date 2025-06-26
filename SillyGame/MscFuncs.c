#include <stdio.h>
#include <stdlib.h>
#include "Definitions.h"
#include "Items.h"
#include "Inventory.h"
#include "Entity.h"
#include "DifficultyConfig.h"

void WipeMem() {
    // FIXME: Finish this.
    //        also figure out what this is going to do LOL.
}

void ErrFunc() {
	printf("Something went wrong...\n");
	printf("Exiting.\n");
	WipeMem();
}



void DebugStage(Stage* passed) {
    if (passed == NULL) {
        printf("Stage does not exist or was not initialized.\n");
        exit(1);
    }
    printf("\n");
    printf("Stage ID: %d\n", passed->id);
    printf("Enemies:\n");\
        for (int i = 0; i < MAX_ENEMIES; i++) {
        if (passed->Enemies.enemy == NULL) {
            break;
        }
        printf("%s\n", passed->Enemies.enemy[i]); 
    }
    printf("Bosses:\n");
    for (int i = 0; i < MAX_BOSSES; i++) {
        if (passed->Bosses.boss == NULL) {
            break;
        }
        printf("%s\n", passed->Bosses.boss[i]);
    }
    printf("Items:\n");
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (passed->Items.item == NULL) {
            break;
        }
        printf("%s\n", passed->Items.item[i]);
    }
}

void DebugItem(Item* passed) {
    if (passed == NULL) {
        printf("Item does not exist or was not initialized.\n");
        exit(1);
    }
    printf("Item Name: %s\n", passed->name);
    printf("Item Description: %s\n", passed->description);
    printf("Item ID: %d\n", passed->id);
    printf("Item HP: %d\n", passed->hp);
    printf("Item DMG: %d\n", passed->dmg);
}

void DebugDifficultyConfig(const Difficulty* config) {
    if (config == NULL) {
        printf("Difficulty config was not initialized correctly.\n");
        exit(1);
    }

    if (config->code == 0) {
        printf("Mode: Easy\n");
    }
    if (config->code == 1) {
        printf("Mode: Normal\n");
    }
    if (config->code == 2) {
        printf("Mode: Hard\n");
    }
    if (config->code == 3) {
        printf("Mode: Ultrahard\n");
    }
    
    printf("\"Evil factor\": %0.2lf\n", config->evilfactor);
    printf("\"Silly Factor\": %0.2lf\n", config->sillyfactor);
    printf("\"Player factor\": %0.2lf\n", config->playerfactor);
    printf("\"Likeability\": %0.2lf\n", config->likeability);
}

//void ClearInputBuffer() {
//    int c;
//    while ((c = getchar()) != '\n' && c = EOF);
//}