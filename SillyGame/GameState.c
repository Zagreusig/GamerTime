#include "cJSON.h"
#include "json_loader.h"
#include "Player.h"
#include "GameState.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern ItemRegistry g_item_registry;

void GameState_Save_state(GameState* state, const char* filepath) {
	cJSON* root = cJSON_CreateObject();
	
	Player* player = &state->player;
	memset(player, 0, sizeof(Player));
	cJSON* jplayer = cJSON_CreateObject();

	cJSON_AddStringToObject(jplayer, "name", player->name);
	cJSON_AddNumberToObject(jplayer, "hp", player->hp);
	cJSON_AddNumberToObject(jplayer, "dmg", player->dmg);
	cJSON_AddNumberToObject(jplayer, "persuasion", player->persuasion);

	// Entities
	cJSON* jentities = cJSON_CreateArray();
		for (int i = 0; i < state->entity_amount; i++){
			Entity* e = state->active_entities[i];
			if (!e) { continue };

			cJSON* je = cJSON_CreateObject();
			cJSON_AddNumberToObject(je, "id", e->id);
			cJSON_AddItemToObject(je, "metadata", MD_ToJSON(&e->metadata));
			cJSON_AddItemToArray(jentities, je);
		}
		cJSON_AddItemToObject(root, "entities", jentities);


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

int GameState_Load_State(GameState* state, const char* path) {
	FILE* file = fopen(path, "rb");
	if (!file) { return 0; }

	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	rewind(file);
	
	char* data = malloc(len + 1);
	fread(data, 1, len, file);
	data[len] = '\0';
	fclose(file);

	cJSON* root = cJSON_Parse(data);
	free(data);

	cJSON* jplayer = cJSON_GetObjectItem(root, "player");
	if (!jplayer) {
		cJSON_Delete(root);
		return 0;
	}

	Player* player = &state->player;
	memset(player, 0, sizeof(Player));

	strncpy(player->name, cJSON_GetObjectItem(jplayer, "name")->valuestring, sizeof(player->name));
	player->hp = cJSON_GetObjectItem(jplayer, "hp")->valueint;
	player->dmg = cJSON_GetObjectItem(jplayer, "dmg")->valueint;
	player->persuasion = cJSON_GetObjectItem(jplayer, "persuasion")->valueint;
	player->stealth = cJSON_GetObjectItem(jplayer, "stealth")->valueint;

	// Entities
	cJSON* jentities = cJSON_GetObjectItem(root, "entities");
	if (jentities) {
		int count = cJSON_GetArraySize(jentities);
		for (int i = 0; i < count && i < MAX_ENTITIES_PER_LEVEL; i++) {
			cJSON* je = cJSON_GetArrayItem(jentities, i);
			if (!cJSON_IsObject(je)) { continue; }

			int id = cJSON_GetObjectItem(je, "id")->valueint;
			cJSON* jmeta = cJSON_GetObjectItem(je, "metadata");
			
			Entity* base = EntityRegistry_CloneByID(id);
			if (!base) { continue; }

			JSON_ToMD(jmeta, &base->metadata);
			state->active_entities[state->entity_amount++] = base;
		}
	}


	// Status effects
	cJSON* jstatuses = cJSON_GetObjectItem(jplayer, "status_effects");
	if (jstatuses) {
		int count = cJSON_GetArraySize(jstatuses);
		for (int i = 0; i < count && i < MAX_STATUS_EFFECTS; i++) {
			cJSON* jstatus = cJSON_GetArrayItem(jstatuses, i);
			PlayerStatus* effect = &player->status_effects[i];
			strncpy(effect->name, cJSON_GetObjectItem(jstatus, "name")->valuestring, sizeof(effect->name));
			effect->strength = cJSON_GetObjectItem(jstatus, "strength")->valuedouble;
			effect->duration = cJSON_GetObjectItem(jstatus, "duration")->valueint;
			effect->stacks = cJSON_GetObjectItem(jstatus, "stacks")->valueint;
		}
		player->status_amount = count;
	}

	// Inventory
	InventoryInit(&player->inventory);
	cJSON* jinv = cJSON_GetObjectItem(jplayer, "inventory");
	if (jinv) {
		int count = cJSON_GetArraySize(jinv);
		for (int i = 0; i < count && i < INVENTORY_SIZE; i++) {
			cJSON* jitem = cJSON_GetArrayItem(jinv, i);
			int id = cJSON_GetObjectItem(jitem, "id")->valueint;
			cJSON* jmeta = cJSON_GetObjectItem(jitem, "metadata");

			const Item* base = ItemRegistry_GetByID(id);
			if (!base) { continue; }

			Item* new_item = malloc(sizeof(Item));
			memcpy(new_item, base, sizeof(Item));
			JSON_ToMD(jmeta, &new_item->metadata);
			Inventory_AddItem(&player->inventory, new_item);
		}
	}

	// Equipment
	cJSON* jequip = cJSON_GetObjectItem(jplayer, "equipment");
	if (jequip) {
#define DESERIALIZE_SLOT_ARRAY(name, arr, max)\
		do {\
			cJSON* j##name = cJSON_GetObjectItem(jequip, #name);\
			if (j##name){\
				int len = cJSON_GetArraySize(j##name); \
				for (int i = 0; i < len && i < max; i++){ \
					cJSON* slot = cJSON_GetArrayItem(j##name, i); \
					if (!cJSON_IsObject(slot)) {arr[i] = NULL; continue; } \
					int id = cJSON_GetObjectItem(slot, "id")->valueint; \
					cJSON* jmeta = cJSON_GetObjectItem(slot, "metadata"); \
					const Item* base = ItemRegistry_GetByID(id); \
					if (!base) {arr[i] = NULL; continue; } \
					arr[i] = malloc(sizeof(Item)); \
					memcpy(arr[i], base, sizeof(Item)); \
					JSON_ToMD(jmeta, &arr[i]->metadata); \
				}\
			}\
		} while (0) 
	
		DESERIALIZE_SLOT_ARRAY(armor, player->equipment.armor, MAX_ARMOR_SLOTS);
		DESERIALIZE_SLOT_ARRAY(accessories, player->equipment.accessories, MAX_ACCESSORY_SLOTS);
		DESERIALIZE_SLOT_ARRAY(weapons, player->equipment.weapons, MAX_WEAPON_SLOTS);
	}

	cJSON_Delete(root);
	return 1;
}