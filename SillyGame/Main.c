/*
*
*
*
*
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Stages.h"
#include "StructDefinitions.h"
#include "Entity.h"
#include "Inventory.h"
#include "Items.h"
#include "DifficultyConfig.h"
#include "Debug.h"
#include "MscFuncs.h"
#include "OnStart.h"

int main(void) {
    Difficulty* config = (Difficulty*)malloc(sizeof(Difficulty));
    Stage* stages[3] = { (Stage*)calloc(1, (3 * sizeof(Stage))) };
    char yesNoChoice;

    for (int i = 0; i < 3; i++) {
        stages[i] = StageInfoReference(i);
    }
    

    
    printf("Would you like to preset a difficulty?\n");
    printf("(y / n): ");
    while (scanf("%c", &yesNoChoice) != 1) {
        printf("Not a valid character.");
    }
    if (yesNoChoice == 'y' || yesNoChoice == 'Y') {
        int i;
        printf("Select difficulty code: ");
        (void)scanf("%d", &i);
        config = DifficultySet(i);
    }
    else {
        printf("\nDifficulty will not be initialized.\n\n"); 
    }
    DebugMenus(config);


    return 0;
}
