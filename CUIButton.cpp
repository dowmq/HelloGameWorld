#include "CUIButton.h"
#include "SpriteProvider.h"
CUIButton::CUIButton(CUIButton::Type type_, CUI* owner_)
	: CUI(owner_), type(type_)
{
	screen_x = 0;
	screen_y = 0;
	Sprite* sprite = nullptr;
	switch (type)
	{
		case CUIButton::Type::Button_Yes:
			sprite = SpriteProvider::Instance().GetSprite(SpriteProvider::UIID::btn_yes);
			break;
		case CUIButton::Type::Button_No:
			sprite = SpriteProvider::Instance().GetSprite(SpriteProvider::UIID::btn_no);
			break;
		case CUIButton::Type::Button_Ok:
			sprite = SpriteProvider::Instance().GetSprite(SpriteProvider::UIID::btn_ok);
			break;
	}
	image = new CUIImage(sprite->sprite_id, false, this);
	width = image->GetWidth();
	height = image->GetHeight();
}


CUIButton::~CUIButton()
{
	if (image != nullptr) {
		delete image; image = nullptr;
	}
}


bool CUIButton::Init()
{
	return true;
}

bool CUIButton::Frame()
{
	if (image != nullptr) {
		image->Frame();
	}
	return true;
}

bool CUIButton::Render()
{
	if (image != nullptr) {
		image->Render();
	}
	return true;
}

bool CUIButton::Release()
{
	if (image != nullptr) {
		delete image; image = nullptr;
	}
	return true;
}

bool CUIButton::SetScreenPos(float x, float y)
{
	screen_x = x;
	screen_y = y;
	image->SetScreenPos(x, y);
	return true;
}

bool CUIButton::SetInvokeFunction(std::function<void(void*)> func_, void* obj_)
{
	func = func_;
	obj = obj_;
	return true;
}

void CUIButton::Invoke()
{
	func(obj);
}