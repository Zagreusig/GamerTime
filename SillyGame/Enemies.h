#pragma once
#ifndef ENEMIES_H
#define ENEMIES_H
#include "StructDefinitions.h"

Enemy* BossClass(int id);
Enemy* EliteClass(int id);
Enemy* CommonClass(int id);
Enemy* SecretSecret();
Enemy* GetEnemyMulti(Difficulty* modifier);

#endif