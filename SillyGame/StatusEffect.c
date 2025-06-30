#define _CRT_SECURE_NO_WARNINGS

#include "StatusEffects.h"
#include <string.h>

StatusEffect KnownEffects[MAX_KNOWN_STATUS_EFFECTS] = {
	{"Weakness", 0.75, 1.0, 1, 0},
	{"Strength", 1.25, 1.0, 1, 0},
	{"Berserk", 1.5, 1.0, 1, 0},
	{"Exhausted", 0.5, 1.0, 1, 0},
	{"Poison", 1.0, 0.2, 1, 5},
	{"Regen", 1.0, 0.15, 1, 3}
};

int KnownEffectCount = 5;

const StatusEffect* GetStatusEffect(const char* name) {
	for (int i = 0; i < KnownEffectCount; i++) {
		if (strcmp(KnownEffects[i].name, name) == 0) {
			return &KnownEffects[i];
		}
	}
	return NULL;
}

int Status_IsStackable(const char* name) {
	const StatusEffect* effect = GetStatusEffect(name);
	return effect ? effect->stackable : 0;
}

int Status_StackLimit(const char* name) {
	const StatusEffect* effect = GetStatusEffect(name);
	return effect ? effect->stacks : 0;
}

double Status_GetEffectMulti(const char* name) {
	const StatusEffect* effect = GetStatusEffect(name);
	return effect ? effect->effect_multiplier : 1.0;
}

double Status_GetStrengthMulti(const char* name) {
	const StatusEffect* effect = GetStatusEffect(name);
	return effect ? effect->effect_strength_multiplier : 1.0;
}