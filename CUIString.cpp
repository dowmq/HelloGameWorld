#include "CUIString.h"
#include <algorithm>
CUIString::CUIString(std::string str, int CUIChar_Color, float x, float y, CUI* owner_)
	: CUIComponent(owner_)
{
	screen_x = x;
	screen_y = y;
	width = 0;
	height = 6;
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	int xpos = 0;
	for (unsigned i = 0; i < str.length(); ++i)
	{
		CUIChar* char_ = new CUIChar(str[i], CUIChar_Color, this);
		if (char_->image->GetSprite()->GetCurrentFrameImageRect().left == 0) {
			char_->SetScreenPos(x + xpos, y);
			xpos += 5;
			width += 5;
		}
		else {
			char_->SetScreenPos(x + xpos, y);
			xpos += 8;
			width += 8;
		}
		Component.push_back(char_);
	}
}

bool CUIString::SetScreenPos(float new_x, float new_y)
{
	screen_x = new_x;
	screen_y = new_y;
	size_t index = 0;
	int xpos = 0;
	for (auto iter = Component.begin(); iter != Component.end(); ++iter)
	{
		CUIChar* char_ = reinterpret_cast<CUIChar*>(*iter);
		if (char_->image->GetSprite()->GetCurrentFrameImageRect().left == 0) {
			char_->SetScreenPos(new_x + xpos, new_y);
			xpos += 5;
		}
		else {
			char_->SetScreenPos(new_x + xpos, new_y);
			xpos += 8;
		}
		//char_->area.SetScreenPos(x + (index * 10), y);
		++index;
	}
	return true;
}