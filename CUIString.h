#pragma once
#include "CUIComponent.h"
#include "CUIChar.h"
#include <string>
class CUIString : public CUIComponent
{
public:
	CUIString(std::string str, int CUIChar_Color, float x, float y, CUI* owner_);
	~CUIString() = default;

	virtual bool SetScreenPos(float new_x, float new_y) override;
};

