#include "CHPGauge.h"
#include "SpriteProvider.h"

CHPGauge::CHPGauge(float x, float y, CUI* owner_)
	: CUIComponent(owner_), player(CPlayerCharacter::Instance())
{
	CUIImage* image = new CUIImage(SpriteProvider::UIID::HPEXPFrame, x - 2, y - 2, true, this);
	Component.push_back(image);
	CHPPiece* one = new CHPPiece(this);
	one->SetScreenPos(x, y);
	Component.push_back(one);
	LONG width = one->image->GetSprite()->GetCurrentFrameImageRect().right;
	for (unsigned i = 1; i < 100; i++) {
		CHPPiece* copy = one->Clone();
		copy->SetScreenPos(x + width * i, y);
		Component.push_back(copy);
	}
}

bool CHPGauge::Render()
{
	Component[0]->Render();
	int hp = player.GetStat()[CPlayerCharacter::CharacterStat::HP];
	if (hp > 0) {
		for (auto iter = Component.begin() + 1; iter != Component.begin() + (hp % 101);
			++iter)
		{
			(*iter)->Render();
		}
	}
	return true;
}

CHPPiece::CHPPiece(CUI* owner_) : CUI(owner_)
{
	image = new CUIImage(SpriteProvider::UIID::HPPiece, 0, 0, true, this);
}

CHPPiece::CHPPiece(const CHPPiece& copy) : CUI(copy.owner)
{
	image = new CUIImage(*copy.image);
	screen_x = copy.screen_x;
	screen_y = copy.screen_y;
}

bool CHPPiece::SetScreenPos(float x, float y)
{
	screen_x = x;
	screen_y = y;
	image->SetScreenPos(x, y);
	return true;
}

bool CHPPiece::Render()
{
	image->Render();
	return true;
}

bool CHPPiece::Init() { return true; }
bool CHPPiece::Frame() { return true; }
bool CHPPiece::Release() 
{ 
	if (image) {
		image->Release();
		delete image; image = nullptr;
	}
	return true; 
}

CHPPiece* CHPPiece::Clone()
{
	CHPPiece* piece = new CHPPiece(this->owner);
	piece->image = new CUIImage(*this->image);
	return piece;
}