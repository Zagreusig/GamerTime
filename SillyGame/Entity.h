#pragma once
#ifndef ENTITY_H
#define ENTITY_H

typedef struct Entity_struct {
	int type;
	/*
	* If Enemy: 
	* 0 = normal, 1 = elite
	* 
	* If NPC:
	* 0 = Helpful, 1 = Harmful, 2 = Jester
	* 
	* If Shopkeeper:
	* 0 = Stingy, 1 = Neutral, 2 = Generous
	*/

	int health;
	int damage;
	int id;

	char interact;

}Entity;


#endif