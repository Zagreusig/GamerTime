#define _CRT_SECURE_NO_WARNINGS

#include "Player.h"
#include "Inventory.h"
#include "StatusEffects.h"
#include "Difficulty.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Player player = { '\0' };

void Player_Init(Player* p, const char* name) {
	p->id = 0;
	if (!name) {
		p->name = "Unnamed player";
	}
	else {
		p->name = name;
	}
	p->hp = 100;
	p->dmg = 5;
	p->persuasion = 0;
	p->stealth = 0;
	p->status_amount = 0;
	p->metadata.tag_amt = 0;
	memset(&p->equipment, 0, sizeof(Equipment));
	InventoryInit(&p->inventory);
}

int Player_HasStatus(const Player* p, const char* name) {
	for (int i = 0; i < p->status_amount; i++) {
		if (strcmp(p->status_effects[i].name, name) == 0) {
			return 1;
		}
	}
	return 0;
}

int Player_RemoveStatus(Player* p, const char* name) {
	for (int i = 0; i < p->status_amount; i++) {
		if (strcmp(p->status_effects[i].name, name) == 0) {
			free(p->status_effects[i].name);
			for (int j = i; j < p->status_amount - 1; j++){
				p->status_effects[j] = p->status_effects[j + 1];
			}
			p->status_amount--;
			return 1;
		}
	}
	return 0;
}

void Player_AddStatus(Player* p, const char* effect, double strength, int duration) {
	const StatusEffect* stat = GetStatusEffect(effect);
	if (!stat) return;

	for (int i = 0; i < p->status_amount; i++) {
		PlayerStatus* exists = &p->status_effects[i];
		if (strcmp(exists->name, effect) == 0) {
			if (stat->stackable) {
				int limit = stat->stacks;
				if (limit == 0 || exists->stacks < limit) {
					exists->stacks += 1;
					exists->duration += duration;
					exists->strength += strength;
					printf("[ Status ] Gained a stack of %s. (Stacks: %d)\n",
						effect, exists->stacks);
				}
			}
			else {
				if (strength > exists->strength) {
					exists->strength = strength;
					printf("[ Status ] %s is strengthened!\n", effect);
				}
				exists->duration += duration;
				printf("[ Status ] %s has extended its duration!\n", effect);
			}
			return;
		}
	}

	if (p->status_amount >= MAX_STATUS_EFFECTS) { return; }

	PlayerStatus* new_status = &p->status_effects[p->status_amount++];
	new_status->name = _strdup(effect);
	new_status->strength = strength;
	new_status->duration = duration;
	new_status->stacks = stat->stackable ? 1 : 0;

	printf("[ Status ] Gained %s (%0.2lfx for %d actions.\n", effect, strength, duration);

}

void Player_ClearStatus(Player* p) {
	for (int i = 0; i < p->status_amount; i++) {
		free(p->status_effects[i].name);
	}
	p->status_amount = 0;
}

void Player_UpdateStatusEffect(Player* p) {
	for (int i = 0; i < p->status_amount; i++) {
		p->status_effects[i].duration--;

		if (p->status_effects[i].duration <= 0) {
			printf("[ Status ] %s's effect has worn off.\n", p->status_effects[i].name);
			free(p->status_effects[i].name);
			for (int j = i; j < p->status_amount - 1; j++) {
				p->status_effects[j] = p->status_effects[j + 1];
			}
			p->status_amount--;
		}
		else {
			i++;
		}
	}
}

void Player_ResolveStatusEffects(Player* p, const DConfig* config) {
	for (int i = 0; i < p->status_amount; i++) {
		const char* name = p->status_effects[i].name;
		const StatusEffect* stat = GetStatusEffect(name);

		if (!stat) continue;

		// Right now the effect of the status is the strength * the multiplier of the effect, then * the number of stacks and * against the difficulty multiplier.
		double tick_strength_damage = ((p->status_effects[i].strength * stat->effect_strength_multiplier) * stat->stacks) * config->enemy_multiplier;
		double tick_strength_health = ((p->status_effects[i].strength * stat->effect_strength_multiplier) * stat->stacks) * config->player_multiplier;


		if (strcmp(name, "Poison") == 0) {
			p->hp -= (int)tick_strength_damage;
			printf("[ Damage ]  Took %d poison damage!\n", (int)tick_strength_damage);
			printf("HP is now %d / 100\n", p->hp);
		}
		if (strcmp(name, "Regen") == 0) {
			p->hp += (int)tick_strength_health;
			printf("[ Healing ]  Healed %d hp!\n", (int)tick_strength_health);
			printf("HP is now %d / 100\n", p->hp);
		}
		// Can add more per effect logics here!
	}
}

void PrintPlayer(const Player* p) {
	printf("Player [%d]: %s\n", p->id, p->name);
	printf("HP: %d\n", p->hp);
	printf("DMG: %d\n", p->dmg);
	printf("Persuasion: %d\n", p->persuasion);
	printf("Stealth: %d\n", p->stealth);

	printf("\nStatus Effects : \n");
	if (p->status_amount == 0) {
		printf("None.\n");
	}
	else {
		for (int i = 0; i < p->status_amount; i++) {
			printf("%s\n", p->status_effects[i].name);
		}
		printf("\n");
	}
	PrintInventory(&p->inventory);
}

void Player_EquipItem(Player* p, Item* item) {
	char type[32];
	if (!MD_GetString(&item->metadata, "Type", type, sizeof(type))) { return; }

	if (strcmp(type, "Armor") == 0) {
		for (int i = 0; i < MAX_ARMOR_SLOTS; i++) {
			if (p->equipment.armor[i] == NULL) {
				p->equipment.armor[i] = item;
				return;
			}
		}
	}
	else if (strcmp(type, "Accessory") == 0) {
		for (int i = 0; i < MAX_ACCESSORY_SLOTS; i++) {
			if (p->equipment.accessories[i] == NULL) {
				p->equipment.accessories[i] = item;
				return;
			}
		}
	}
	else if (strcmp(type, "Weapon") == 0) {
		for (int i = 0; i < MAX_WEAPON_SLOTS; i++) {
			if (p->equipment.weapons[i] == NULL) {
				p->equipment.weapons[i] = item;
				return;
			}
		}
	}
	else {
		printf("You don't have enough slots!\n");
	}
}

void Player_UnequipSlot(Player* p, const char* type, int index) {
	if (strcmp(type, "Armor") == 0 && index < MAX_ARMOR_SLOTS) {
		p->equipment.armor[index] = NULL;
	}
	else if (strcmp(type, "Accessory") == 0 && index < MAX_ACCESSORY_SLOTS) {
		p->equipment.accessories[index] = NULL;
	}
	else if (strcmp(type, "Weapon") == 0 && index < MAX_WEAPON_SLOTS) {
		p->equipment.weapons[index] = NULL;
	}
}