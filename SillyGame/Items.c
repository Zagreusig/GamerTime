#include "Items.h"
#include "MetDat.h"
#include "json_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ItemRegistry g_item_registry = { 0 };

void CreateItem(Item* item, int id, const char* name, int damage, int durability, const char* description) {
	item->id = id;
	item->metadata.tag_amt = 0;

	MD_SetString(&item->metadata, "Name", name);
	MD_SetString(&item->metadata, "Description", description);
	MD_SetInt(&item->metadata, "Damage", damage);
	MD_SetInt(&item->metadata, "Durability", durability);
}

void Item_Create_FromMD(Item* item, int id, MD* md) {
    item->id = id;
    item->metadata.tag_amt = md->tag_amt;
    for (int i = 0; i < md->tag_amt; i++) {
        item->metadata.tags[i] = md->tags[i];
    }
}

void PrintItem(const Item* item) {
    char name[128], desc[128];
    int durability, damage;

    MD_GetString((MD*)&item->metadata, "Name", name, sizeof(name));
    MD_GetString((MD*)&item->metadata, "Description", desc, sizeof(desc));
    MD_GetInt((MD*)&item->metadata, "Durability", &durability);
    MD_GetInt((MD*)&item->metadata, "Damage", &damage);

    printf("Item ID: %d\n", item->id);
    printf("Name: %s\n", name);
    printf("Durability: %d\n", durability);
    printf("Damage: %d\n", damage);
    printf("Description: %s\n", desc);
}

void ItemRegistry_LoadFromJSON(const char* path) {
    g_item_registry.numitems = LoadItemJSON(path, g_item_registry.items, MAX_ITEM_REGISTRY_SIZE);
}

const Item* ItemRegistry_GetByID(int id) {
    for (int i = 0; i < g_item_registry.numitems; i++) {
        if (g_item_registry.items[i].id == id) {
            return &g_item_registry.items[i];
        }
    }
    return NULL;
}

Item* ItemRegistry_CloneByID(int id) {
    const Item* base = ItemRegistry_GetByID(id);
    if (!base) { return NULL; }

    Item* clone = malloc(sizeof(Item));
    if (!clone) { return NULL; }

    memcpy(clone, base, sizeof(Item));
    return clone;
}