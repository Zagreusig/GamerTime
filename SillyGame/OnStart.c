#define _CRT_SECURE_NO_WARNINGS

#include "NewGameState.h"
#include "GameState.h"
#include "json_loader.h"
#include "Items.h"
#include "Entity.h"
#include "Player.h"
#include "Difficulty.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External global registries
extern ItemRegistry g_item_registry;
extern EntityRegistry g_entity_registry;
extern DConfig g_difficulty;
extern GameState g_save_state;
extern char* savepath;

void OnStart() {

	// === Load Item Registry ===
	memset(&g_item_registry, 0, sizeof(ItemRegistry));
	int item_count = LoadItemJSON("data/items.json", g_item_registry.items, MAX_ITEM_REGISTRY_SIZE);
	g_item_registry.numitems = item_count;
	printf("[ Initilizing ] Loaded %d items into registry.\n", item_count);

	// === Load Entity Registry ===
	memset(&g_entity_registry, 0, sizeof(EntityRegistry));
	int entity_count = LoadEntitiesJSON("data/entities.json", g_entity_registry.entities, MAX_ENTITY_REGISTRY_SIZE);
	g_entity_registry.numEnt = entity_count;
	printf("[ Initilizing ] Loaded %d entities into registry.\n", entity_count);
	
	// === GameState initialization ===
	memset(&g_save_state, 0, sizeof(GameState));
	printf("[ Initilizing ] Initialized GameState Successfully.\n");


	printf("[ Initilizing ] Checking for Save Files.\n");

	if (SaveFileCountCheck() < 0) { printf("[ Initilizing ] No save files found.\n"); }
	else { printf("[ Initilizing ] Found %d save files.\n", SaveFileCountCheck()); }

	SetSavePath("data/savefile1.json", NULL);

	// FIXME: There will be specific (global) registries that are for each class of enemy / item / level.
	
}


void NewGame() {
	Player* player = (Player*)malloc(sizeof(Player));
	Level* level = (Level*)malloc(sizeof(Level));
	if (!player || !level) { printf("Malloc err, NewGame()\n"); exit(1); }

	int file = 1, valid = 0;

	printf("What save file would you like to use? (1-4)\n");
	do {
		if (!scanf(" %d", &file)) {
			printf("Invalid selection. Retry.\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
		}
		else { valid = 1; }
	} while (valid == 0);

	if (SaveFileCheck(file) == 1) {
		char ch = 'N';
		int c;
		valid = 0;
		printf("There is already a save file, would you like to overwrite? (y/N)\n");
		do {
			if (!scanf(" %c", &ch)) {
				printf("Must be y or n.\n");
				while ((c = getchar()) != '\n' && c != EOF);
			}
			else { valid = 1; }
		} while (valid == 0);

		if (ch == 'y' || ch == 'Y') { SetSavePath(NULL, file); }
		else { SetSavePath(NULL, file + 1); }
	}

	char name[32] = { '\0' };

	printf("Enter your name!\n");
	(void)scanf(" %s", &name);

	Player_Init(player, name);
	Level_Init(level, NULL, NULL);
	
	g_save_state.player = player;
	g_save_state.current_level = level;

}

void LoadGame() {
	int choice = 0;
	int valid = 0;
	int c;

	switch (SaveFileCountCheck()) {
	case 0:
		printf("No save files, creating new file.\n");
		SetSavePath(NULL, 1);
		break;
	case 1:
		printf("One save file found, Loading.\n");
		SetSavePath(NULL, 1);
		break;
	case 2:
		printf("Found two saves, which would you like to use?\n");
		printf("Save 1 or 2? (Please input a number)\n");
		goto input_choice;
	case 3:
		printf("Found three saves, which would you like to use?\n");
		printf("Save 1, 2 or 3? (Please input a number)\n");
		goto input_choice;
	case 4:
		printf("Found four saves, which would you like to use?\n");
		printf("Save 1, 2, 3, or 4? (Please input a number)\n");
		goto input_choice;
	}

input_choice:
	do {
		if (!scanf("%d", &choice)) {
			printf("Invalid selection. Retry.\n");
			while ((c = getchar()) != '\n' && c != EOF);
		}
		else {
			valid = 1;
		}
	} while (valid = 0);
	if (valid == 1) { SetSavePath(NULL, choice); }


	GameState_Load_State(&g_save_state, savepath);
}