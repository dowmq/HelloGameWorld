#include "CUIImage.h"
#include "SpriteProvider.h"


CUIImage::CUIImage(int sprite_id, float screen_x_, float screen_y_, bool isAlpha_,CUI* owner_)
	: CUI(owner_), isAlpha(isAlpha_)
{
	screen_x = screen_x_;
	screen_y = screen_y_;
	sprite = SpriteProvider::Instance().GetSprite(sprite_id);
	width = sprite->GetCurrentFrameImageRect().right;
	height = sprite->GetCurrentFrameImageRect().bottom;
}

CUIImage::CUIImage(int sprite_id, bool isAlpha_, CUI* owner_)
	: CUI(owner_), isAlpha(isAlpha_)
{
	screen_x = 0;
	screen_y = 0;
	sprite = SpriteProvider::Instance().GetSprite(sprite_id);
	width = sprite->GetCurrentFrameImageRect().right;
	height = sprite->GetCurrentFrameImageRect().bottom;
}

CUIImage::CUIImage(Sprite* sprite_, bool isAlpha_, CUI* owner_)
	: CUI(owner_), isAlpha{ isAlpha_ }
{
	sprite = sprite_->Clone();
	width = sprite->GetCurrentFrameImageRect().right;
	height = sprite->GetCurrentFrameImageRect().bottom;
}

CUIImage::CUIImage(const CUIImage& copy) : CUI(copy.owner)
{
	isAlpha = copy.isAlpha;
	sprite = copy.sprite->Clone();
}

CUIImage::CUIImage(CUIImage&& other) noexcept
	: CUI(other.owner)
{
	other.owner = nullptr;
	this->isAlpha = other.isAlpha;
	this->sprite = other.sprite;
	other.sprite = nullptr;
}

CUIImage& CUIImage::operator=(CUIImage&& other) noexcept
{
	if (this != &other)
	{
		delete this->sprite;

		this->owner = other.owner;
		this->isAlpha = other.isAlpha;
		this->sprite = other.sprite;
		other.owner = nullptr;
		other.sprite = nullptr;
	}
	return *this;
}

CUIImage::~CUIImage()
{
	if (sprite) {
		sprite->Release();
		delete sprite; sprite = nullptr;
	}
}

bool CUIImage::SetWidth(float width_)
{
	sprite->GetCurrentFrameImageRect().right = width_;
	width = width_;
	return true;
}

bool CUIImage::SetHeight(float height_)
{
	sprite->GetCurrentFrameImageRect().bottom = height_;
	height = height_;
	return true;
}

bool CUIImage::Init()
{
	return true;
}

bool CUIImage::Frame()
{
	sprite->Frame();
	return true;
}

bool CUIImage::Render()
{
	if (isAlpha)
		sprite->DrawAlpha(screen_x, screen_y);
	else
		sprite->Draw(screen_x, screen_y, SRCCOPY);
	return true;
}

bool CUIImage::Release()
{
	if (sprite) {
		sprite->Release();
		delete sprite; sprite = nullptr;
	}
	return true;
}