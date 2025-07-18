#define _CRT_SECURE_NO_WARNINGS
#include "Difficulty.h"
#include <stdio.h>
#include <string.h>

DConfig g_difficulty = { 0 };

void Difficulty_Init() {
	SetDifficulty(&g_difficulty, DIFFICULTY_NORMAL);
}

void SetDifficulty(DConfig* config, Difficulty diff) {
	switch (diff)
	{
	default:
		break;
	case DIFFICULTY_EASY:
		strcpy(config->read_diff, "Easy");
		config->enemy_multiplier = 0.75;
		config->player_multiplier = 1.25;
		config->npc_multiplier = 0.75;
		config->jester_chance = 0.03;
		break;
	case DIFFICULTY_NORMAL:
		strcpy(config->read_diff, "Normal");
		config->enemy_multiplier = 1;
		config->player_multiplier = 1;
		config->npc_multiplier = 1;
		config->jester_chance = 0.06;
		break;
	case DIFFICULTY_HARD:
		strcpy(config->read_diff, "Hard");
		config->enemy_multiplier = 1.15;
		config->player_multiplier = 0.9;
		config->npc_multiplier = 1.25;
		config->jester_chance = 0.1;
		break;
	case DIFFICULTY_ULTRAHARD:
		strcpy(config->read_diff, "Ultrahard");
		config->enemy_multiplier = 1.25;
		config->player_multiplier = 0.85;
		config->npc_multiplier = 1.5;
		config->jester_chance = 0.15;
		break;
	case DIFFICULTY_CUSTOM:
		strcpy(config->read_diff, "Custom");
		printf("Enter float values x.xx:\n");
		printf("Enemy multiplier: ");
		(void)scanf(" %0.2lf", &config->enemy_multiplier);
		printf("\nPlayer multiplier: ");
		(void)scanf(" %0.2lf", &config->player_multiplier);
		printf("\nNPC multiplier: ");
		(void)scanf(" %0.2lf", &config->npc_multiplier);
		printf("\nJester: ");
		(void)scanf(" %0.2lf", &config->jester_chance);
		break;
	case DIFFICULTY_CHALLENGE:
		strcpy(config->read_diff, "Debug Challenge");
		config->enemy_multiplier = 5.0;
		config->player_multiplier = 0.25;
		config->npc_multiplier = 9.0;
		config->jester_chance = 0.4;
		break;
	}

}

void CustomDifficulty(DConfig* config, double player, double enemies, double npcs, double jester) {
	config->diff = DIFFICULTY_CUSTOM;
	strcpy(config->read_diff, "Custom");
	config->enemy_multiplier = enemies;
	config->player_multiplier = player;
	config->npc_multiplier = npcs;
	config->jester_chance = jester;
}

void GamblerDifficulty(DConfig* config) {
	config->diff = DIFFICULTY_CHALLENGE;
	strcpy(config->read_diff, "Challenge: Gambler");
	config->enemy_multiplier = 1;
	config->player_multiplier = 1;
	config->npc_multiplier = 1;
	config->jester_chance = 0.5;
}

void PrintDifficulty(const DConfig* config) {
	printf("Difficulty: %s", config->read_diff);
	printf("\nEnemy multiplier: %0.2lf", config->enemy_multiplier);
	printf("\nPlayer multiplier: %0.2lf", config->player_multiplier);
	printf("\nNpc multiplier: %0.2lf", config->npc_multiplier);
	printf("\nJester chance: %0.3lf", config->jester_chance);
	printf("\n\n");
}