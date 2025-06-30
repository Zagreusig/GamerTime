#pragma once
#ifndef STATUS_EFFECTS_H
#define STATUS_EFFECTS_H

typedef struct Status_Effect_struct {
	const char* name;
	double effect_multiplier;
	double effect_strength_multiplier;
	int stackable;
	int stacks;
}StatusEffect;

#define MAX_KNOWN_STATUS_EFFECTS 16

extern StatusEffect KnownEffects[MAX_KNOWN_STATUS_EFFECTS];
extern int KnownEffectCount;

const StatusEffect* GetStatusEffect(const char* name);
int StatusIsStackable(const char* name);
int StatusStackLimit(const char* name);
double StatusGetEffectMulti(const char* name);
double StatusGetStrengthMulti(const char* name);


#endif