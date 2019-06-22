#pragma once
#include "CItem.h"
#include "CUIComponent.h"
#include "CUIImage.h"

class CUIInventory;

class CUIInvenSlot : public CUIComponent
{
public:
	CUIInvenSlot(float screen_x_, float screen_y_, float width_, float height_,
		int EquipSlot, size_t index_, CUI* owner_);
	~CUIInvenSlot();


	virtual bool Render() override;
	virtual bool Release() override;

	size_t			index;

	CItem*			item;

	bool			StoreItem(CItem* item);
	CItem*			DetachItem();
	bool			DiscardItem();

	static bool		entire_slot_ui_lock;
private:
	float			image_pos_x;
	float			image_pos_y;
};

