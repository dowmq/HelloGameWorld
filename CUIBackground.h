#pragma once
#include "CUIComponent.h"
#include "CUIImage.h"
enum BackgroundColor { black, navy_gray, gray, blue };
class CUIBackground : public CUIComponent
{
public:
	CUIBackground(float screen_x, float screen_y, float width, float height, 
		BackgroundColor color, CUI* owner_);
	virtual bool			SetWidth(float width_);
	virtual bool			SetHeight(float height_);

private:
	CUIImage*				image;
};