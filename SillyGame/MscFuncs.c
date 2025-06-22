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

void DebugItem(const Item* item) {
    printf("ID: %d\n", item->id);
    printf("Name: %s\n", item->name);
    printf("Description: %s\n", item->description);
    printf("Durrability: %d\n", item->hp);
    printf("Damage: %d\n\n", item->dmg);
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