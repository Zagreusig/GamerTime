#pragma once
#ifndef ONSTART_H
#define ONSTART_H
#include "StructDefinitions.h"

ITable* InitLootTables(ITable* table, int stageID);
EYTable* InitEnemyTable(EYTable* table);
BTable* InitBossTable(BTable* table);
ENTTable* InitEntityTable(ENTTable* table);

#endif