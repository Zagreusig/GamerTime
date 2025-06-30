#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Items.h"
#include "Inventory.h"
#include "MetDat.h"

#define MAX_STATUS_EFFECTS 16
#define MAX_BUFFS 8
#define MAX_HINDERS 8
#define MAX_ARMOR_SLOTS 4
#define MAX_ACCESSORY_SLOTS 2
#define MAX_WEAPON_SLOTS 2



typedef struct Equipment_struct {
	Item* armor[MAX_ARMOR_SLOTS];
	Item* accessories[MAX_ACCESSORY_SLOTS];
	Item* weapons[MAX_WEAPON_SLOTS];
}Equipment;

typedef struct Player_Status_struct {
	char* name;
	double strength;
	int duration;
	int stacks;
} PlayerStatus;

typedef struct Player_struct {
	const char* name;
	
	int id;
	int hp;
	int dmg;
	int persuasion;
	int stealth;

	PlayerStatus status_effects[MAX_STATUS_EFFECTS];
	int status_amount;

	Inventory inventory;
	Equipment equipment;
	MD metadata;

}Player;


void Player_Init(Player* p);
void Player_AddStatus(Player* p, const char* effect, double strength, int duration);
int Player_HasStatus(Player* p, const char* name);
int Player_RemoveStatus(Player* p, const char* name);
void Player_ClearStatus(Player* p);
void PrintPlayer(const Player* p);
void Player_EquipItem(Player* p, Item* item);
void Player_UnequipSlot(Player* p, const char* slot_type, int index);

#endif