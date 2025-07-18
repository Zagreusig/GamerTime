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

extern EntityRegistry g_entity_registry;
extern ItemRegistry g_item_registry;
extern GameState g_save_state;

int main(void) {
    OnStart();
    NewGame();
    GameState_Save_state(&g_save_state, "data/savestate.json");
    
    int load;
    load = GameState_Load_State(&g_save_state, "data/savestate.json");

    if (load == 0) { printf("err"); exit(1); }

    if (g_save_state.current_level) { printf("[ State ] Verified Level data.\n"); }
    if (g_save_state.player) { printf("[ State ] Verified Player data.\n"); }

    PrintPlayer(&g_save_state.player);
    printf("\n[ Level ] ID: %d", g_save_state.current_level->id);
    printf("\n[ Level ] Name: %s", g_save_state.current_level->name);
    printf("\n[ Level ] Ent count: %d", g_save_state.current_level->entity_amount);
    printf("\n[ Level ] Item count: %d", g_save_state.current_level->item_amount);

    printf("\n All Tests passed.\n");

    return 0;
}