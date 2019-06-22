#include "CUIBackground.h"
#include "SpriteProvider.h"

CUIBackground::CUIBackground(float screen_x_, float screen_y_, float width_, float height_, BackgroundColor color, CUI* owner_)
	: CUIComponent(owner_)
{
	screen_x = screen_x_;
	screen_y = screen_y_;
	width = width_;
	height = height_;
	SpriteProvider::UIID id = SpriteProvider::UIID::UIBackground_black;
	switch (color)
	{
		case black:
			id = SpriteProvider::UIID::UIBackground_black;
			break;
		case navy_gray:
			id = SpriteProvider::UIID::UIBackground_navy_gray;
			break;
		case gray:
			id = SpriteProvider::UIID::UIBackground_gray;
			break;
		case blue:
			id = SpriteProvider::UIID::UIBackground_blue;
			break;
	}
	image = new CUIImage(id, screen_x_, screen_y_, false, this);
	RECT& rt = image->GetSprite()->GetCurrentFrameImageRect();
	rt.left = 0;
	rt.top = 0;
	rt.right = width_;
	rt.bottom = height_;
	Component.push_back(image);
}

bool CUIBackground::SetWidth(float width_)
{
	image->GetSprite()->GetCurrentFrameImageRect().right = width_;
	return CUI::SetWidth(width_);
}

bool CUIBackground::SetHeight(float height_)
{
	image->GetSprite()->GetCurrentFrameImageRect().bottom = height_;
	return CUI::SetHeight(height_);
}