#pragma once
#include "TSingleton.h"
#include "CItem.h"
#include <map>
class CItemDataProvider : public TSingleton<CItemDataProvider>
{
	friend class TSingleton<CItemDataProvider>;
public:
	CItemDataProvider();
	~CItemDataProvider();

	CItem* GetItemByItemId(size_t itemid);
	std::map<size_t, CItem*>& GetAllItems();

private:
	bool Initialize(std::string filename);

	
	// itemid, CItem*
	std::map<size_t, CItem*> items;
};