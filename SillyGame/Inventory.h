#pragma once
#ifndef INVENTORY_H
#define INVENTORY_H

#include "items.h"

#define INVENTORY_SIZE 16

typedef struct Inventory_struct {
	Item* items[INVENTORY_SIZE];
	int totalItems;
}Inventory;

void InventoryInit(Inventory* inv);
int Inventory_AddItem(Inventory* inv, Item* item);
void Inventory_RemoveItem(Inventory* inv, int index);
int Inv_RemoveItem_ByName(Inventory* inv, const char* name);
int Inv_RemoveItem_ByID(Inventory* inv, int id);
void PrintInventory(const Inventory* inv);

#endif