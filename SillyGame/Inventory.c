#define _CRT_SECURE_NO_WARNINGS

#include "Inventory.h"
#include "Items.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InventoryInit(Inventory* inv) {
	inv->totalItems = 0;
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		inv->items[i] = NULL;
	}
}

int Inventory_AddItem(Inventory* inv, Item* item) {
	if (inv->totalItems >= INVENTORY_SIZE) { return 0; }
	inv->items[inv->totalItems++] = item;
	return 1;
}

void Inventory_RemoveItem(Inventory* inv, int index) {
	if (index < 0 || index >= inv->totalItems) return;
	for (int i = index; i < inv->totalItems - 1; i++) {
		inv->items[i] = inv->items[i + 1];
	}
	inv->items[--inv->totalItems] = NULL;
}

int Inv_RemoveItem_ByID(Inventory* inv, int id) {
	for (int i = 0; i < inv->totalItems; i++) {
		if (inv->items[i]->id == id) {
			Inventory_RemoveItem(inv, i);
			return 1;
		}
	}
	return 0;
}

int Inv_RemoveItem_ByName(Inventory* inv, const char* name) {
	for (int i = 0; i < inv->totalItems; i++) {
		char ItemName[32];
		if (MD_GetString(&inv->items[i]->metadata, "Name", ItemName, sizeof(ItemName))) {
			if (strcmp(ItemName, name) == 0) {
				Inventory_RemoveItem(inv, i);
				return 1;
			}
		}
	}
	return 0;
}


void PrintInventory(const Inventory* inv) {
	printf("\nInventory (%d items):\n", inv->totalItems);
	for (int i = 0; i < inv->totalItems; i++) {
		char name[32];
		MD_GetString(&inv->items[i]->metadata, "Name", name, sizeof(name));
		printf("[Position %d] %s\n", i, name);
	}
	printf("\n");
}