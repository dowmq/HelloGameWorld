#include "CUIInvenSlotBehaveList.h"
#include "CUIHandler.h"
#include "CUIInventory.h"
#include "InputHandler.h"
#include "CUIItemInfo.h"
#include "SpriteProvider.h"
CUIInvenSlotBehaveList::CUIInvenSlotBehaveList(bool is_equipSlot, float owner_x, float owner_y, CUI* owner_)
	: CUI(owner_)
{
	screen_x = owner_x - 60;
	if (is_equipSlot) 
	{
		screen_y = owner_y - 48;
		image = new CUIImage(SpriteProvider::UIID::equipSlotBehaveList, screen_x, screen_y, false, this);

		// UNEQUIP
		auto unequip_event = CUIHandler::Instance().RegisterEventUI(this,
			[](CUI* behavelist) {
				if (InputHandler::Instance().is_left_click())
				{
					CUIInvenSlotBehaveList* blist = reinterpret_cast<CUIInvenSlotBehaveList*>(behavelist);
					CUIInvenSlot* slot = reinterpret_cast<CUIInvenSlot*>(behavelist->GetOwner());
					CUIInventory* inven = reinterpret_cast<CUIInventory*>(slot->GetOwner());
					inven->UnEquipitem(slot);
					blist->DeRegisterAllEvent();
					CUIHandler::Instance().RequestToReleaseInCurrentUI(behavelist);
					CUIInvenSlot::entire_slot_ui_lock = false;
				}
			}, screen_x, screen_y, 60, 24);
		event_ids.emplace(SlotBehaveList::uneuip, unequip_event->event_id);

		// ITEMINFO
		auto iteminfo_event = CUIHandler::Instance().RegisterEventUI(this,
			[](CUI* behavelist) {
				if (InputHandler::Instance().is_left_click())
				{
					CUIInvenSlotBehaveList* blist = reinterpret_cast<CUIInvenSlotBehaveList*>(behavelist);
					CUIInvenSlot* slot = reinterpret_cast<CUIInvenSlot*>(behavelist->GetOwner());
					//slot->CreateCUIItemInfo();
					if (CUIHandler::Instance().isRegistedUIWindow(slot->item->GetObjectId()) == false) {
						CUIItemInfo* info = new CUIItemInfo(slot->item->GetObjectId(), slot->item, "item information",
							slot->GetScreenX() - 150, slot->GetScreenY() - 200, 150, 90, nullptr);
						CUIHandler::Instance().RegisterUIWindow(info->GetWindowId(), info);
					}
					blist->DeRegisterAllEvent();
					CUIHandler::Instance().RequestToReleaseInCurrentUI(behavelist);
					CUIInvenSlot::entire_slot_ui_lock = false;
				}
			}, screen_x, screen_y + 24, 60, 24);
		event_ids.emplace(SlotBehaveList::item_info, iteminfo_event->event_id);
	}
	else 
	{
		screen_y = owner_y - 72;
		image = new CUIImage(SpriteProvider::UIID::storeSlotBehaveList, screen_x, screen_y, false, this);

		// EQUIP
		auto equip_event = CUIHandler::Instance().RegisterEventUI(this,
			[](CUI* behavelist) {
				if (InputHandler::Instance().is_left_click())
				{
					CUIInvenSlotBehaveList* blist = reinterpret_cast<CUIInvenSlotBehaveList*>(behavelist);
					CUIInvenSlot* slot = reinterpret_cast<CUIInvenSlot*>(behavelist->GetOwner());
					CUIInventory* inven = reinterpret_cast<CUIInventory*>(slot->GetOwner());
					inven->Equipitem(slot);
					blist->DeRegisterAllEvent();
					CUIHandler::Instance().RequestToReleaseInCurrentUI(behavelist);
					CUIInvenSlot::entire_slot_ui_lock = false;
				}
			}, screen_x, screen_y, 60, 24);
		event_ids.emplace(SlotBehaveList::equip, equip_event->event_id);

		// ITEMINFO
		auto iteminfo_event = CUIHandler::Instance().RegisterEventUI(this,
			[](CUI* behavelist) {
				if (InputHandler::Instance().is_left_click())
				{
					CUIInvenSlotBehaveList* blist = reinterpret_cast<CUIInvenSlotBehaveList*>(behavelist);
					CUIInvenSlot* slot = reinterpret_cast<CUIInvenSlot*>(behavelist->GetOwner());
					if (CUIHandler::Instance().isRegistedUIWindow(slot->item->GetObjectId()) == false) {
						CUIItemInfo* info = new CUIItemInfo(slot->item->GetObjectId(), slot->item, "item information",
							slot->GetScreenX() - 150, slot->GetScreenY() - 200, 150, 90, nullptr);
						CUIHandler::Instance().RegisterUIWindow(info->GetWindowId(), info);
					}
					blist->DeRegisterAllEvent();
					CUIHandler::Instance().RequestToReleaseInCurrentUI(behavelist);
					CUIInvenSlot::entire_slot_ui_lock = false;
				}
			}, screen_x, screen_y + 24, 60, 24);
		event_ids.emplace(SlotBehaveList::item_info, iteminfo_event->event_id);

		//DISCARD
		auto discard_event = CUIHandler::Instance().RegisterEventUI(this,
			[](CUI* behavelist) {
				if (InputHandler::Instance().is_left_click())
				{
					CUIInvenSlotBehaveList* blist = reinterpret_cast<CUIInvenSlotBehaveList*>(behavelist);
					CUIInvenSlot* slot = reinterpret_cast<CUIInvenSlot*>(behavelist->GetOwner());
					slot->DiscardItem();
					blist->DeRegisterAllEvent();
					CUIHandler::Instance().RequestToReleaseInCurrentUI(behavelist);
					CUIInvenSlot::entire_slot_ui_lock = false;
				}
			}, screen_x, screen_y + 48, 60, 24);
		event_ids.emplace(SlotBehaveList::discard, discard_event->event_id);
	}
}

bool CUIInvenSlotBehaveList::DeRegisterAllEvent()
{
	for (auto& element : event_ids)
	{
		CUIHandler::Instance().DeRegisterEventUI(element.second);
	}
	event_ids.clear();
	return true;
}


CUIInvenSlotBehaveList::~CUIInvenSlotBehaveList()
{
	if (image) {
		image->Release();
		delete image; image = nullptr;
	}
}

bool CUIInvenSlotBehaveList::Init()
{
	if (image != nullptr) {
		image->Init();
	}
	return true;
}

bool CUIInvenSlotBehaveList::Frame()
{
	if (image != nullptr) {
		image->Frame();
	}
	return true;
}

bool CUIInvenSlotBehaveList::Render()
{
	if (image != nullptr) {
		image->Render();
	}
	return true;
}

bool CUIInvenSlotBehaveList::Release()
{
	if (image != nullptr) {
		image->Release();
		delete image; image = nullptr;
	}
	return true;
}