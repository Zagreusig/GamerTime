#include "cJSON.h"
#include "json_loader.h"
#include "Player.h"
#include "GameState.h"
#include <stdio.h>
#include <stdlib.h>

void GameState_Save_state(Player* player, const char* filepath) {
	cJSON* root = cJSON_CreateObject();
	cJSON* jplayer = cJSON_CreateObject();

	cJSON_AddStringToObject(jplayer, "name", player->name);
	cJSON_AddNumberToObject(jplayer, "hp", player->hp);
	cJSON_AddNumberToObject(jplayer, "dmg", player->dmg);
	cJSON_AddNumberToObject(jplayer, "persuasion", player->persuasion);

	// Status Effects
	cJSON* jstatuses = cJSON_CreateArray();
	for (int i = 0; i < player->status_amount; i++) {
		PlayerStatus* stat = &player->status_effects[i];
		cJSON* jstatus = cJSON_CreateObject();
		cJSON_AddStringToObject(jstatus, "name", stat->name);
		cJSON_AddNumberToObject(jstatus, "strength", stat->strength);
		cJSON_AddNumberToObject(jstatus, "duration", stat->duration);
		cJSON_AddNumberToObject(jstatus, "stacks", stat->stacks);
		cJSON_AddItemToArray(jstatuses, jstatus);
	}
	cJSON_AddItemToObject(jplayer, "status_effects", jstatuses);

	// Inventory
	cJSON* jinv = cJSON_CreateArray();
	for (int i = 0; i < player->inventory.totalItems; i++) {
		Item* item = player->inventory.items[i];
		cJSON* jitem = cJSON_CreateObject();
		cJSON_AddNumberToObject(jitem, "id", item->id);
		cJSON_AddItemToObject(jitem, "metadata", MD_ToJSON(&item->metadata));
		cJSON_AddItemToArray(jinv, jitem);
	}
	cJSON_AddItemToObject(jplayer, "inventory", jinv);

	//Equipment
	cJSON* jequip = cJSON_CreateObject();
	#define SERIALIZE_SLOT_ARRAY(name, arr, count) \
	do { \
		cJSON* j##name = cJSON_CreateArray(); \
		for (int i = 0; i < count; i++){ \
		if (arr[i]){ \
			cJSON* itemObj = cJSON_CreateObject(); \
			cJSON_AddNumberToObject(itemObj, "id", arr[i]->id); \
			cJSON_AddItemToObject(itemObj, "metadata", MD_ToJSON(&arr[i]->metadata)); \
			cJSON_AddItemToArray(j##name, itemObj); \
			} else { \
			cJSON_AddItemToArray(j##name, cJSON_CreateNull()); \
			} \
		} \
		cJSON_AddItemToObject(jequip, #name, j##name); \
	} while(0)

	SERIALIZE_SLOT_ARRAY(armor, player->equipment.armor, MAX_ARMOR_SLOTS);
	SERIALIZE_SLOT_ARRAY(accessories, player->equipment.accessories, MAX_ACCESSORY_SLOTS);
	SERIALIZE_SLOT_ARRAY(weapons, player->equipment.weapons, MAX_WEAPON_SLOTS);

	cJSON_AddItemToObject(jplayer, "equipment", jequip);
	cJSON_AddItemToObject(root, "player", jplayer);

	char* json_string = cJSON_Print(root);
	FILE* file = fopen(filepath, "w");
	if (file) {
		fprintf(file, "%s\n", json_string);
		fclose(file);
	}

	cJSON_Delete(root);
	free(json_string);
}