#pragma once
#ifndef ITEMS_H
#define ITEMS_H

#include "MetDat.h"

#define MAX_ITEMS 16
typedef struct Item_struct{
	int id;
	MD metadata;
}Item;

void CreateItem(Item* item, int id, const char* name, int damage, int durability, const char* description);
void PrintItem(const Item* item);
void Item_Create_FromMD(Item* item, int id, MD* md);

// Item Registry (for references)

#define MAX_ITEM_REGISTRY_SIZE 256

typedef struct Item_Registry_struct {
	Item items[MAX_ITEM_REGISTRY_SIZE];
	int numitems;
}ItemRegistry;

extern ItemRegistry g_item_registry;

void ItemRegistry_LoadFromJSON(const char* path);
const Item* ItemRegistry_GetByID(int id);
Item* ItemRegistry_CloneByID(int id);

#endif