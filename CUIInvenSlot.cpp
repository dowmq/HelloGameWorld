#include "CUIInvenSlot.h"
#include "CUIInventory.h"
#include "CArea.h"
#include "CUIItemInfo.h"
#include "SpriteProvider.h"
bool CUIInvenSlot::entire_slot_ui_lock = false;

CUIInvenSlot::CUIInvenSlot(float screen_x_, float screen_y_, float width_, float height_,
	int EquipSlot, size_t index_, CUI* owner_) : CUIComponent(owner_)
{
	screen_x = screen_x_;
	screen_y = screen_y_;
	width = width_;
	height = height_;
	index = index_;
	SpriteProvider::UIID ui_id = SpriteProvider::UIID::invenslot_store;
	switch (EquipSlot)
	{
		case CUIInventory::SlotType::store:
			ui_id = SpriteProvider::UIID::invenslot_store;
			break;
		case CUIInventory::SlotType::weapon:
			ui_id = SpriteProvider::UIID::invenslot_attack;
			break;
		case CUIInventory::SlotType::armor:
			ui_id = SpriteProvider::UIID::invenslot_body;
			break;
		case CUIInventory::SlotType::feet:
			ui_id = SpriteProvider::UIID::invenslot_feet;
			break;
		case CUIInventory::SlotType::head:
			ui_id = SpriteProvider::UIID::invenslot_head;
			break;
		case CUIInventory::SlotType::ring:
			ui_id = SpriteProvider::UIID::invenslot_ring;
			break;
		case CUIInventory::SlotType::torch:
			ui_id = SpriteProvider::UIID::invenslot_ring;
			break;
		default:
			break;
	}
	CUIImage* slot_image = new CUIImage(ui_id, screen_x, screen_y, true, this);
	Component.push_back(slot_image);
}

CUIInvenSlot::~CUIInvenSlot()
{
}

bool CUIInvenSlot::Render()
{
	if (item != nullptr) {
		item->GetSprite()->DrawAlpha(image_pos_x, image_pos_y);
	}
	return CUIComponent::Render();
}

bool CUIInvenSlot::Release()
{
	if (item != nullptr) {
		delete item; item = nullptr;
	}
	return CUIComponent::Release();
}

bool CUIInvenSlot::StoreItem(CItem* item_) { 
	if (item != nullptr || item_  == nullptr) {
		return false;
	}
	item = item_;
	RECT& rt = item->GetSprite()->GetCurrentFrameImageRect();
	image_pos_x = screen_x + ((width - rt.right) / 2);
	image_pos_y = (screen_y + 1) + ((height - rt.bottom) / 2);
	return true; 
}

CItem* CUIInvenSlot::DetachItem()
{
	CItem* temp = this->item;
	item = nullptr;
	return temp;
}

bool CUIInvenSlot::DiscardItem()
{
	if (item != nullptr) {
		delete item; item = nullptr;
	}
	return true;
}