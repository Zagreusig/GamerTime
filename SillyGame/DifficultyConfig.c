#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "DifficultyConfig.h"
/* 
* 0 = Easy, 1 = Normal, 2 = Hard, 3 = Ultrahard.
* Easy:
* Enemy hp 75%, Enemy damage 50%, player dmg 150%, tool/armor durability reduced by 50%
* Shop prices reduced, increased healing effectiveness, reduced negative status time, decreased prank item chance.
*/ 

Difficulty* DifficultySet(int difficulty) {
	Difficulty* diffSettngs = (Difficulty*)malloc(sizeof(Difficulty));
	if (diffSettngs == NULL) {
		return NULL;
	}
	

	if (difficulty == 0) {
		diffSettngs->code = 0;
		diffSettngs->evilfactor = 0.5;
		diffSettngs->likeability = 1.25;
		diffSettngs->playerfactor = 1.5;
		diffSettngs->sillyfactor = 0.03;
		
		return diffSettngs;
	}
	if (difficulty == 1) {
		diffSettngs->code = 1;
		diffSettngs->evilfactor = 1;
		diffSettngs->likeability = 1;
		diffSettngs->sillyfactor = 0.05;
		diffSettngs->playerfactor = 1;

		return diffSettngs;
	}
	if (difficulty == 2) {
		diffSettngs->code = 2;
		diffSettngs->evilfactor = 1.15;
		diffSettngs->likeability = 0.85;
		diffSettngs->playerfactor = 0.9;
		diffSettngs->sillyfactor = 0.09;
		
		return diffSettngs;
	}
	
	if (difficulty == 3) {
		diffSettngs->code = 3;
		diffSettngs->evilfactor = 1.5;
		diffSettngs->likeability = 0.65;
		diffSettngs->playerfactor = 0.75;
		diffSettngs->sillyfactor = 0.15;
		
		return diffSettngs;
	}
}

