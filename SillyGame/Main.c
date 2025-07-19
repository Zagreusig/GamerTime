#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Entity.h"
#include "Items.h"
#include "Inventory.h"
#include "StatusEffects.h"
#include "NewGameState.h"
#include "GameState.h"
#include "OnStart.h"
#include "Level.h"
#include "Difficulty.h"
#include "json_loader.h"

extern EntityRegistry g_entity_registry;
extern ItemRegistry g_item_registry;
extern GameState g_save_state;
extern char* savepath;

int main(void) {
    OnStart();
    NewGame();

    return 0;
}