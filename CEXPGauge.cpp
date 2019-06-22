#include "CEXPGauge.h"
#include "SpriteProvider.h"
#include "CPlayerCharacter.h"

CEXPGauge::CEXPGauge(float x, float y, CUI* owner_)
	: CUIComponent(owner_), player(CPlayerCharacter::Instance())
{
	CUIImage* image = new CUIImage(SpriteProvider::UIID::HPEXPFrame,
		x - 2, y - 2, true, this);
	Component.push_back(image);
	CEXPPiece* one = new CEXPPiece(this);
	one->SetScreenPos(x, y);
	Component.push_back(one);
	LONG width = one->image->GetSprite()->GetCurrentFrameImageRect().right;
	for (unsigned i = 1; i < 100; i++) {
		CEXPPiece* copy = one->Clone();
		copy->SetScreenPos(x + width * i, y);
		Component.push_back(copy);
	}
}

bool CEXPGauge::Render()
{
	Component[0]->Render();
	int exp = player.GetStat()[CPlayerCharacter::CharacterStat::EXP];
	if (exp > 0) {
		for (auto iter = Component.begin(); iter != Component.begin() + (exp % 101);
			++iter)
		{
			(*iter)->Render();
		}
	}
	return true;
}

CEXPPiece::CEXPPiece(CUI* owner_) : CUI(owner_)
{
	image = new CUIImage(SpriteProvider::UIID::EXPPiece, 0, 0, true, this);
}

CEXPPiece::CEXPPiece(const CEXPPiece& copy) : CUI(copy.owner)
{
	image = new CUIImage(*copy.image);
	screen_x = copy.screen_x;
	screen_y = copy.screen_y;
}

bool CEXPPiece::SetScreenPos(float x, float y)
{
	screen_x = x;
	screen_y = y;
	image->SetScreenPos(x, y);
	return true;
}

bool CEXPPiece::Render()
{
	image->Render();
	//BasicSprite->DrawAlpha(area);
	return true;
}

bool CEXPPiece::Init() { return true; }
bool CEXPPiece::Frame() { return true; }
bool CEXPPiece::Release()
{
	if (image) {
		image->Release();
		delete image; image = nullptr;
	}
	return true;
}

CEXPPiece* CEXPPiece::Clone()
{
	CEXPPiece* piece = new CEXPPiece(this->owner);
	piece->image = new CUIImage(*this->image);
	return piece;
}