#include "CPlayerDecorateEffect.h"
#include "CPlayerCharacter.h"
#include "SpriteProvider.h"
#include "TimeHandler.h"
CPlayerDecorateEffect::CPlayerDecorateEffect(CPlayerCharacter& player_)
	: player(player_)
{
}


CPlayerDecorateEffect::~CPlayerDecorateEffect()
{
}

CPlayerDecorateEffect::DecorateSprite::DecorateSprite(float relative_x, float relative_y, 
	float x_increment_, float y_increment_, double life_time_, Sprite* image_)
{
	relative_coordinate.x = relative_x;
	relative_coordinate.y = relative_y;
	x_increment = x_increment_;
	y_increment = y_increment_;
	life_time = life_time_;
	image = image_;
	timer.Update();
}

bool CPlayerDecorateEffect::DecorateSprite::Release() {
	if (image) 
		image->Release();
	return true;
}

bool CPlayerDecorateEffect::Init()
{
	return true;
}

bool CPlayerDecorateEffect::Frame()
{
	if (effects.empty() == false) {
		EffectTimer.Update();
		for (auto iter = effects.begin(); iter != effects.end();)
		{
			double time = ((EffectTimer - (*iter).timer) * 1000);
			if ((*iter).life_time <= time)
			{
				(*iter).Release();
				iter = effects.erase(iter);
			}
			else
			{
				(*iter).relative_coordinate.x += (*iter).x_increment * TimeHandler::Instance().GetSecondPerFrame();
				(*iter).relative_coordinate.y += (*iter).y_increment * TimeHandler::Instance().GetSecondPerFrame();
				(*iter).image->Frame();
				++iter;
			}
		}
	}
	return true;
}

bool CPlayerDecorateEffect::Render()
{
	if (effects.empty() == false) {
		fPoint player_pos = player.GetArea().GetScreenPos();
		for (auto& effect : effects) {
			effect.image->DrawAlpha(player_pos.x + effect.relative_coordinate.x, 
				player_pos.y + effect.relative_coordinate.y);
		}
	}
	return true;
}

bool CPlayerDecorateEffect::Release()
{
	if (effects.empty() == false) {
		for (auto& effect : effects) {
			effect.Release();
		}
	}
	effects.clear();
	return true;
}

bool CPlayerDecorateEffect::AddEffect(float relative_x, float relative_y, 
	float x_increment_, float y_increment, double life_time_, int sprite_id)
{
	effects.emplace_back(relative_x, relative_y, x_increment_, y_increment,
		life_time_, SpriteProvider::Instance().GetSprite(sprite_id));
	return true;
}