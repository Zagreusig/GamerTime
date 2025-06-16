#pragma once
#ifndef DIFF_CONFIG_H
#define DIFF_CONFIG_H

typedef struct Difficulty_struct {
	int code;
	
	double evilfactor;
	// Controls the bonus for all negative things.
	double sillyfactor;
	// Controls the fake items.
	double playerfactor;
	// Controls player things (health related, dmg related, etc.)
	double likeability;
	// RNG bonus for player chance interactions.
	}Difficulty;

Difficulty* DifficultySet();

#endif