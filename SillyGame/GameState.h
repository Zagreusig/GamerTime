#pragma once
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Player.h"
#include "Items.h"
#include "Entity.h"
#include "Level.h"

#define MAX_ENTITIES_PER_LEVEL 64
#define MAX_ITEMS_IN_LEVEL 128

typedef struct Game_State_struct {
	Player player;

	Entity* active_entities[MAX_ENTITIES_PER_LEVEL];
	int entity_amount;

	Item* level_items[MAX_ITEMS_IN_LEVEL];
	int level_items_amount;

	Level* current_level;
	// TODO:
	// int flags[NUM_FLAGS]
	// quests?
}GameState;

void GameState_Init(GameState* state);
void GameState_Del(GameState* state);

void GameState_Save_state(GameState* state, const char* filepath);
int GameState_Load_state(GameState* state, const char* filepath);

#endif