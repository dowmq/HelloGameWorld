#include "CBackground.h"
#include "SpriteProvider.h"

CBackground::CBackground(float logical_x, float logical_y, int sprite_id, bool alpha)
{
	area.Init(logical_x, logical_y);
	move_speed = 0;
	sprite = SpriteProvider::Instance().GetETCSpriteBySpriteID((SpriteProvider::ETCID)sprite_id);
	is_alpha = alpha;
}

CBackground::~CBackground()
{
	if (sprite) {
		sprite->Release();
		delete sprite; sprite = nullptr;
	}
}

bool CBackground::Init()
{
	return true;
}
bool CBackground::Frame()
{
	sprite->Frame();
	return true;
}

bool CBackground::Render()
{
	if (is_alpha) {
		sprite->DrawAlpha(area);
	} else {
		// #define SRCCOPY             (DWORD)0x00CC0020 /* dest = source      
		sprite->Draw(area, 0x00CC0020);
	}
	
	return true;
}

bool CBackground::Release()
{
	if (sprite) {
		sprite->Release();
		delete sprite; sprite = nullptr;
	}
	return true;
}

bool CBackground::EffectedbyGravity(float gravity)
{
	return true;
}