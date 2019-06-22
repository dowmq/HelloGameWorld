#pragma once
#include "TSingleton.h"
#include "CField.h"

class CFieldFactory : public TSingleton<CFieldFactory>
{
	friend class TSingleton<CFieldFactory>;
	CFieldFactory() = default;
public:
	~CFieldFactory() = default;

	CField*			CreateStartMap();
	CField*			CreateGoblinCastle();
};

