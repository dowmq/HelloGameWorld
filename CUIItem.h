#pragma once
#include "CUIComponent.h"

class CUIItem : public CUIComponent
{
public:
	CUIItem(size_t itemid, CUI* owner_);
	~CUIItem();

	virtual bool Render() override;
};

