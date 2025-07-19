#define _CRT_SECURE_NO_WARNINGS

#include "GameState.h"
#include "Inventory.h"
#include "Items.h"
#include "MetDat.h"
#include "json_loader.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* ReadFile(const char* path) {
	FILE* file = fopen(path, "rb");
	if (!file) {
		fprintf(stderr, "Failed to open file: %s\n", path);
		return NULL;
	}

	// Get file length
	if (fseek(file, 0, SEEK_END) != 0) {
		fclose(file);
		fprintf(stderr, "Failed to seek to end of file: %s\n", path);
		return NULL;
	}

	long len = ftell(file);
	if (len < 0) {
		fclose(file);
		fprintf(stderr, "Failed to get file length: %s\n", path);
		return NULL;
	}

	rewind(file);

	// Allocate buffer with +1 for null terminator
	char* data = (char*)malloc(len + 1);
	if (!data) {
		fclose(file);
		fprintf(stderr, "Memory allocation failed for file: %s\n", path);
		return NULL;
	}

	// Read file contents
	size_t read = fread(data, 1, len, file);
	fclose(file);

	if (read != (size_t)len) {
		fprintf(stderr, "Failed to read full file: %s\n", path);
		free(data);
		return NULL;
	}

	data[len] = '\0'; // Null-terminate for safe string usage
	return data;
}


int LoadItemJSON(const char* path, Item* items, int max_items) {
	char* data = ReadFile(path);
	if (!data) { return 0; }

	cJSON* root = cJSON_Parse(data);
	if (!root) { return 0; }

	int count = cJSON_GetArraySize(root);
	for (int i = 0; i < count && i < max_items; i++) {
		cJSON* entry = cJSON_GetArrayItem(root, i);

		int id = cJSON_GetObjectItem(entry, "id")->valueint;
		const char* name = cJSON_GetObjectItem(entry, "name")->valuestring;
		int durability = cJSON_GetObjectItem(entry, "durability")->valueint;
		int damage = cJSON_GetObjectItem(entry, "damage")->valueint;
		const char* desc = cJSON_GetObjectItem(entry, "description")->valuestring;
		int stage1 = cJSON_GetObjectItem(entry, "primaryStage")->valueint;
		int stage2 = cJSON_GetObjectItem(entry, "secondaryStage")->valueint;

		CreateItem(&items[i], id, name, durability, damage, desc);
		MD_SetInt(&items[i].metadata, "PrimaryStage", stage1);
		MD_SetInt(&items[i].metadata, "SecondaryStage", stage2);
	}

	cJSON_Delete(root);
	free(data);
	return count;
}

int LoadEntitiesJSON(const char* path, Entity* entities, int max_entities) {
	char* data = ReadFile(path);
	if (!data) { return 0; }

	cJSON* root = cJSON_Parse(data);
	if (!root) { return 0; }

	int count = cJSON_GetArraySize(root);
	for (int i = 0; i < count && i < max_entities; i++) {
		cJSON* entry = cJSON_GetArrayItem(root, i);
	
		int id = cJSON_GetObjectItem(entry, "id")->valueint;
		const char* name = cJSON_GetObjectItem(entry, "name")->valuestring;
		int baseHP = cJSON_GetObjectItem(entry, "baseHP")->valueint;
		int baseDMG = cJSON_GetObjectItem(entry, "baseDMG")->valueint;
		const char* rarity = cJSON_GetObjectItem(entry, "rarity")->valuestring;
		int opinion = cJSON_GetObjectItem(entry, "opinion")->valueint;
		int stage1 = cJSON_GetObjectItem(entry, "primaryStage")->valueint;
		int stage2 = cJSON_GetObjectItem(entry, "secondaryStage")->valueint;
		double weight = cJSON_GetObjectItem(entry, "spawnWeight")->valuedouble;

		CreateEntity(&entities[i], id, name, baseHP, baseDMG, rarity, opinion, stage1, stage2, weight);
	}

	cJSON_Delete(root);
	free(data);
	return count;
}
