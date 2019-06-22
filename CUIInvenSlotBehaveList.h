#pragma once
#include "CUI.h"
#include "CUIImage.h"
#include <vector>
#include <map>
class CUIInvenSlotBehaveList : public CUI
{	
	float screen_x, screen_y;
public:
	enum SlotBehaveList { equip, uneuip, item_info, discard };

	CUIInvenSlotBehaveList(bool is_equipSlot, float owner_x, float owner_y, CUI* owner_);
	~CUIInvenSlotBehaveList();

	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;

	bool				DeRegisterAllEvent();
	std::map<size_t, size_t>& GetEventIds() { return event_ids; }

	CUIImage* image;
	std::map<size_t, size_t> event_ids;
};

