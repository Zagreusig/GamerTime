#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "StructDefinitions.h"
#include "Items.h"
#include "DifficultyConfig.h"
#include "Enemies.h"
#include "Entity.h"
#include "MscFuncs.h"
#include "Debug.h"
#include "Stages.h"

void DebugMenus(Difficulty* config) {
	int selection;
	Difficulty* debugDifficulty = (Difficulty*)malloc(sizeof(Difficulty));
	Item* debugItem = (Item*)malloc(sizeof(Item));
	Stage* debugStage = (Stage*)malloc(sizeof(Stage));

	printf("\n\n");
	printf("DEBUG MENUS\n");
	printf("Debug Item: 1\n");
	printf("Debug Entity/Enemy: 2\n");
	printf("Difficulty Config: 3\n");
	printf("Enter selection: ");
	while (scanf("%d", &selection) != 1) {
		printf("Err. Invalid selection.\n");
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
	}
	switch (selection) {
	case 1:
		printf("\nItems:\n\n");
		printf("Dull blade: 0\n");
		printf("Sharp sword: 1\n");
		printf("Hero's sword: 2\n");
		printf("Enter code for item debug: ");
		(void)scanf("%d", &selection);
		debugItem = ItemIDsMaster(selection);
		DebugItem(debugItem);
		break;
	case 2:
		printf("\n\nFIXME: Finish\n");
		break;
	case 3:
		printf("\n\n");
		if (config != NULL) {
			printf("Difficulty printout:\n");
			DebugDifficultyConfig(config);
		}
		else {
			printf("Difficulty options:\n");
			printf("Easy: 0\n");
			printf("Normal: 1\n");
			printf("Hard: 2\n");
			printf("Ultrahard: 3\n");
			printf("Enter code for difficulty config: ");
			(void)scanf("%d", &selection);
			debugDifficulty = DifficultySet(selection);
			DebugDifficultyConfig(debugDifficulty);
		}
		break;
	case 4:
		printf("\n\n");
		printf("Debugging stage data\n");
		printf("Stage ID (0 - 2): ");
		(void)scanf("%d", &selection);
		debugStage = InitStage(debugStage, selection);
		DebugStage(debugStage);

	}
}
