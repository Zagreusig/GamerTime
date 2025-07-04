#pragma once
#ifndef DIFFICULTY_H
#define DIFFICULTY_H

typedef enum Difficulty_ENUM {
    DIFFICULTY_EASY,
    DIFFICULTY_NORMAL,
    DIFFICULTY_HARD,
    DIFFICULTY_ULTRAHARD,
    DIFFICULTY_CUSTOM,
    DIFFICULTY_CHALLENGE
} Difficulty;

typedef struct Difficulty_Config_struct {
    Difficulty diff;
    char read_diff[32];
    double player_multiplier;
    double enemy_multiplier;
    double npc_multiplier;
    double jester_chance;
}DConfig;

extern DConfig g_difficulty;

void SetDifficulty(DConfig* config, Difficulty diff);
void CustomDifficulty(DConfig* config, double player, double enemies, double npcs, double jester);
void GamblerDifficulty(DConfig* config);
void PrintDifficulty(const DConfig* config);

#endif