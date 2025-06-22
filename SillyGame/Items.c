#define _CRT_SECURE_NO_WARNINGS

#include "Items.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Table_Struct {
    Item* itemOne;
    Item* itemTwo;
    Item* itemThree;
}Table;


Item LootDrop(int stageID, int rarity) {
    Item tbd = (Item*)malloc(sizeof(Item));
    Table FirstStage = (Table*)malloc(sizeof(Table));
    Table SecondStage = (Table*)malloc(sizeof(Table));
    Table ThirdStage = (Table*)malloc(sizeof(Table));
    if (tbd == NULL || FirstStage == NULL || SecondStage == NULL || ThirdStage == NULL) {
        printf("\nMalloc err LootDrop()\n");
        exit(1);
    }

    switch (stageID)
    {
    default:
        break;
    case 0:
        switch (rarity)
        {
        default:
            printf("\nRarity in LootDrop() err\n");
            break;
        case 0:
            tbd = GetRare1();
        case 1:

        case 2:

        }
    case 1:
        switch (rarity)
        {
        default:
            printf("\nRarity in LootDrop() err\n");
            break;
        case 0:

        case 1:

        case 2:

        }
    case 2:

        switch (rarity)
        {
        default:
            printf("\nRarity in LootDrop() err\n");
            break;
        case 0:

        case 1:

        case 2:

        }
    }
   

}

Item DullBlade() {
	Item dullBlade;
	dullBlade.id = 0;
	strcpy(dullBlade.name, "Dull Blade");
	strcpy(dullBlade.description, "A dull blade.\n");
    strcat(dullBlade.description, "\t\tNothing special about it.\n");
    strcat(dullBlade.description, "\t\tThis is a third line of text.");
    dullBlade.hp = 3;
    dullBlade.dmg = 2;
	return dullBlade;
}

Item SharpSword() {
    Item sharpSword;
    sharpSword.id = 1;
    strcpy(sharpSword.name, "Sharp Sword");
    strcpy(sharpSword.description, "A sharp sword.\n");
    strcat(sharpSword.description, "\t\tMuch better than your last tool.\n");
    strcat(sharpSword.description, "\t\tSimply better.\n");
    sharpSword.hp = 6;
    sharpSword.dmg = 5;
    return sharpSword;
}

Item HerosSword() {
    Item herosSword;
    herosSword.id = 2;
    strcpy(herosSword.name, "Hero's Sword");
    strcpy(herosSword.description, "The Legendary Hero's Sword.\n");
    strcat(herosSword.description, "\t\tThe infamous sword the Hero used to\n");
    strcat(herosSword.description, "\t\tdefeat the awful beast and save the land.\n");
    herosSword.hp = 15;
    herosSword.dmg = 20;
    return herosSword;
}

Item ItemIDsMaster(int code) {
    Item call;
    if (code == 0) {
        call = DullBlade();
        return call;
    }
    if (code == 1) {
        call = SharpSword();
        return call;
    }
    if (code == 2) {
        call = HerosSword();
        return call;
    }
}