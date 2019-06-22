#include "CUIInventory.h"
#include "CUIString.h"
#include "CUIHandler.h"
#include "InputHandler.h"
#include "CUIInvenSlotBehaveList.h"
#include "CPlayerCharacter.h"
#include "TimeHandler.h"
CUIInventory* CUIInventory::inventory = nullptr;

CUIInventory::CUIInventory(float screen_x_, float screen_y_, CUI* owner_)
	: CUIComponent(owner_)
{
	screen_x = screen_x_;
	screen_y = screen_y_;
	width = height = 0;
	CUIString* string = new CUIString("character inventory", CUICharColor::yellow, screen_x, screen_y, this);
	CUIString* gold = new CUIString("gold", CUICharColor::white, screen_x + 180, screen_y, this);
	Component.push_back(string);
	Component.push_back(gold);

	float slot_start_x = screen_x + 5;
	float slot_start_y = screen_y + 10;
	for (int i = 0; i < CUIInventory::SlotType::size_of_equip_slot; ++i)
	{
		int row = i / 2;
		int column = i % 2;
		float x = slot_start_x + (column * 33);
		float y = slot_start_y + (row * 34);
		CUIInvenSlot* slot = new CUIInvenSlot(x, y, 33, 34, (CUIInventory::SlotType)i, 0, this);
		equip_slots[(CUIInventory::SlotType)i] = slot;
		Component.push_back(slot);
		CUIHandler::Instance().RegisterEventUI(slot, 
			[](CUI* ui) {
				CUIInvenSlot* slot = reinterpret_cast<CUIInvenSlot*>(ui);
				if (InputHandler::Instance().is_now_double_click())
				{
					CUIInventory* inven = reinterpret_cast<CUIInventory*>(ui->GetOwner());
					inven->UnEquipitem(slot);
					return;
				}
				if (InputHandler::Instance().is_right_click() 
					&& slot->item != nullptr
					&& slot->GetComponentSize() <= 1) 
				{
					CUIInvenSlotBehaveList* blist = new CUIInvenSlotBehaveList(true, slot->GetScreenX(), slot->GetScreenY(), slot);
					slot->AddElement(blist);
					// 장착해제/아이템정보 이렇게 이벤트 UI를 등록
					return;
				}
			}, x, y, 33, 34);
	}
	slot_start_x += 33 + 33 + 10;
	for (int i = 0; i < CUIInventory::SlotType::size_of_store_slot; ++i)
	{
		int row = i / (CUIInventory::SlotType::size_of_store_slot / 3);
		int column = i % (CUIInventory::SlotType::size_of_store_slot / 3);
		float x = slot_start_x + (column * 33);
		float y = slot_start_y + (row * 34);
		CUIInvenSlot* slot = new CUIInvenSlot(x, y, 33, 34, CUIInventory::SlotType::store, i,  this);
		store_slots[i] = slot;
		Component.push_back(slot);
		CUIHandler::Instance().RegisterEventUI(slot,
			[](CUI* ui) {
				if (CUIInvenSlot::entire_slot_ui_lock == false) {
					CUIInvenSlot* slot = reinterpret_cast<CUIInvenSlot*>(ui);
					if (InputHandler::Instance().is_now_double_click())
					{
						CUIInventory* inven = reinterpret_cast<CUIInventory*>(ui->GetOwner());
						inven->Equipitem(slot);
						return;
					}
					if (InputHandler::Instance().is_right_click()
						&& slot->item != nullptr
						&& slot->GetComponentSize() <= 1)
					{
						CUIInvenSlot::entire_slot_ui_lock = true;
						CUIInvenSlotBehaveList* blist = new CUIInvenSlotBehaveList(false, slot->GetScreenX(), slot->GetScreenY(), slot);
						slot->AddElement(blist);
						return;
					}
				}
			}, x, y, 33, 34);
	}
	inventory = this;
}

CUIInvenSlot* CUIInventory::GetEquipSlot(CUIInventory::SlotType type) {
	return equip_slots[type];
}

CUIInvenSlot* CUIInventory::GetStoreSlot(size_t index)
{
	if (index >= CUIInventory::SlotType::size_of_store_slot) {
		std::string str = "InventoryHandler::GetStore - Out Of Range... Index: ";
		str.append(std::to_string(index));
		throw std::out_of_range(str.c_str());
	}
	return equip_slots[index];
}

bool CUIInventory::StoreItem(CItem* item)
{
	for (CUIInvenSlot* slot : store_slots)
	{
		if (slot->item == nullptr) {
			slot->StoreItem(item);
			return true;
		}
	}
	return false;
}

bool CUIInventory::Equipitem(CUIInvenSlot* storeSlot)
{
	if (storeSlot->item == nullptr)
		return false;
	CItem* item = storeSlot->item;
	if (CPlayerCharacter::Instance().GetStat()[CPlayerCharacter::CharacterStat::LV]
		< item->GetItemOption(CItem::ItemOption::lvConstrain))
	{
		return false;
	}
	CUIInvenSlot* equipSlot = nullptr;
	CItem::ItemType type = item->GetType();
	switch (type)
	{
		case CItem::ItemType::ring:
			equipSlot = equip_slots[CUIInventory::SlotType::ring];
			break;
		case CItem::ItemType::head:
			equipSlot = equip_slots[CUIInventory::SlotType::head];
			break;
		case CItem::ItemType::weapon:
			equipSlot = equip_slots[CUIInventory::SlotType::weapon];
			break;
		case CItem::ItemType::armor:
			equipSlot = equip_slots[CUIInventory::SlotType::armor];
			break;
		case CItem::ItemType::torch:
			equipSlot = equip_slots[CUIInventory::SlotType::torch];
			break;
		case CItem::ItemType::feet:
			equipSlot = equip_slots[CUIInventory::SlotType::feet];
			break;
	}
	storeSlot->DetachItem();
	CItem* previous_equip_item = equipSlot->DetachItem();
	equipSlot->StoreItem(item);
	if (previous_equip_item != nullptr) {
		storeSlot->StoreItem(previous_equip_item);
		CPlayerCharacter::Instance().SetUnEquipItemAbility(previous_equip_item);
		CUIInvenSlot::entire_slot_ui_lock = true;
		TimeHandler::Instance().SetTimerFunction(300, [](CFieldObject* obj) {
			CUIInvenSlot::entire_slot_ui_lock = false;
			}, nullptr);

	}
	CPlayerCharacter::Instance().SetEquipItemAbility(item);
	return true;
}

bool CUIInventory::UnEquipitem(CUIInvenSlot* equipSlot)
{
	if (equipSlot->item == nullptr)
		return false;
	CUIInvenSlot* emptySlot = nullptr;
	for (auto& slot : store_slots)
	{
		if (slot->item == nullptr) {
			emptySlot = slot;
			break;
		}
	}
	// 비어 있는 공간이 없음.
	if (emptySlot == nullptr)
		return false;

	// 비어있는 슬롯으로 아이템을 이동한다.
	CItem* item = equipSlot->DetachItem();
	emptySlot->StoreItem(item);
	CPlayerCharacter::Instance().SetUnEquipItemAbility(item);
	return true;
}