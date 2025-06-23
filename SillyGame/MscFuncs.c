#include <stdio.h>
#include "Inventory.h"
#include "Entity.h"
#include "DifficultyConfig.h"

void WipeMem() {

}

void ErrFunc() {
	printf("Something went wrong...\n");
	printf("Exiting.\n");
	WipeMem();
}

void DebugItem(Item* passed) {
    printf("Item Name: %s\n", passed->name);
    printf("Item Description: %s\n", passed->description);
    printf("Item ID: %d\n", passed->id);
    printf("Item HP: %d\n", passed->hp);
    printf("Item DMG: %d\n", passed->dmg);
}

void DebugDifficultyConfig(const Difficulty* config) {
    if (config->code == 0) {
        printf("Mode: Easy");
    }
    if (config->code == 1) {
        printf("Mode: Normal");
    }
    if (config->code == 2) {
        printf("Mode: Hard");
    }
    if (config->code == 3) {
        printf("Mode: Ultrahard");
    }
    
    printf("\"Evil factor\": %0.2lf\n", config->evilfactor);
    printf("\"Silly Factor\": %0.2lf", config->sillyfactor);
    printf("\"Player factor\": %0.2lf\n", config->playerfactor);
    printf("\"Likeability\": %0.2lf\n", config->likeability);
}

//void ClearInputBuffer() {
//    int c;
//    while ((c = getchar()) != '\n' && c = EOF);
//}