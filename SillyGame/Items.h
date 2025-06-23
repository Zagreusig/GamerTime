#pragma once
#ifndef ITEMS_H
#define ITEMS_H

typedef struct Item_struct {
	char name[50];
	char description[500];
	int id;
	int hp;
	int dmg;
}Item;

Item* ItemIDsMaster(int code);
Item* DullBlade(Item* passed);
Item* SharpSword(Item* passed);
Item* HerosSword(Item* passed);


#endif