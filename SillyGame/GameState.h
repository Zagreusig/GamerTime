#pragma once
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Player.h"

typedef struct Game_State_struct {
	Player player;
	// TODO:
	// Level current level + data
	// int flags[NUM_FLAGS]
}GameState;

void GameState_Save_state(Player* player, const char* filepath);
int GameState_Load_state(GameState* state, const char* filepath);

#endif