#include "CUIChar.h"
#include "CUIImage.h"
#include "SpriteProvider.h"
CUIChar::CUIChar(char char_, int CUICharColor, CUI* owner_) : CUI(owner_), character(char_)
{
	if (char_ >= '!' && char_ <= '9')
	{
		
		image = new CUIImage(SpriteProvider::UIID::UIChar_0, 0, 0, true, this);
		RECT& rt = image->GetSprite()->GetCurrentFrameImageRect();
		rt.left += (char_ - '!') * 9;
		rt.top += CUICharColor * 18;
	}
	else if (char_ >= 'a' && char_ <= 'z')
	{
		image = new CUIImage(SpriteProvider::UIID::UIChar_a, 0, 0, true, this);
		RECT& rt = image->GetSprite()->GetCurrentFrameImageRect();
		rt.left += (char_ - 'a') * 9;
		rt.top += CUICharColor * 18;
	}
	else
	{
		image = new CUIImage(SpriteProvider::UIID::UIChar_a, 0, 0, true, this);
		RECT& rt = image->GetSprite()->GetCurrentFrameImageRect();
		rt.left = 0;
		rt.top = 0;
	}
}

CUIChar::CUIChar(CUIChar&& other) noexcept : CUI(other.owner)
{
	this->image = other.image;
	this->character = other.character;
	other.image = nullptr;
	other.character = 0;
}
CUIChar& CUIChar::operator=(CUIChar&& other) noexcept
{
	if (this != &other)
	{
		this->image = other.image;
		this->character = other.character;
		other.image = nullptr;
		other.character = 0;
	}
	return *this;
}

bool CUIChar::SetScreenPos(float x, float y)
{ 
	screen_x = x;
	screen_y = y;
	image->SetScreenPos(screen_x, screen_y);
	return true; 
}


bool CUIChar::Init()
{
	image->Init();
	return true;
}

bool CUIChar::Frame()
{
	image->Frame();
	return true;
}

bool CUIChar::Render()
{
	image->Render();
	return true;
}

bool CUIChar::Release()
{
	if (image) {
		image->Release();
		delete image; image = nullptr;
	}
	return true;
}

CUIChar::~CUIChar()
{
	if (image) {
		image->Release();
		delete image; image = nullptr;
	}
}
