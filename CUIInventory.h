#pragma once
#include "CUIComponent.h"
#include "CItem.h"
#include "CUIInvenSlot.h"

class CUIInventory : public CUIComponent
{
public:
	enum SlotType {
		ring = 0, head, weapon, armor, torch, feet, size_of_equip_slot = 6,
		store = -1, size_of_store_slot = 21,
	};
	CUIInventory(float screen_x_, float screen_y_, CUI* owner_);
	~CUIInventory() = default;
	
	CUIInvenSlot*		GetEquipSlot(CUIInventory::SlotType type);
	CUIInvenSlot*		GetStoreSlot(size_t index);

	bool				StoreItem(CItem* item);
	bool				Equipitem(CUIInvenSlot* storeSlot);
	bool				UnEquipitem(CUIInvenSlot* equipSlot);

	std::array<CUIInvenSlot*, SlotType::size_of_equip_slot> equip_slots;
	std::array<CUIInvenSlot*, SlotType::size_of_store_slot> store_slots;

	static CUIInventory* inventory;
};

