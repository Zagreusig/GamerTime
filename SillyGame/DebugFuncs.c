#define _CRT_SECURE_NO_WARNINGS

#include "Debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ErrHandler(int macro, int key) {
	char type[24] = { 0 };
	char section[36] = { 0 };
	int for_exit = macro;

	switch (macro) {
	case 1:
		strcpy(&type, "Gamestate");
		break;
	case 2:
		strcpy(&type, "Player");
		break;
	case 3:
		strcpy(&type, "Level");
		break;
	case 4:
		strcpy(&type, "Difficulty");
		break;
	case 5:
		strcpy(&type, "Item");
		break;
	case 6:
		strcpy(&type, "Entity");
		break;
	case 7:
		strcpy(&type, "Inventory");
		break;
	case 8:
		strcpy(&type, "Status Effect");
		break;
	default:
		printf("Unknown error has occured.\n");
		exit(-1);
	}

	switch (key) {
	default:
		printf("Unknown error has occured.\n");
		exit(-1);
	}
}