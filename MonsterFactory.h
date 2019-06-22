#pragma once
#include "CMonster.h"
class MonsterFactory
{
public:
	static CMonster* CreateGoblin(float logical_x, float logical_y);
};
