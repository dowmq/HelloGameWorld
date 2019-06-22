#pragma once
#include "Sprite.h"
#include <vector>
#include "TimeFragment.h"

class CPlayerCharacter;

class CPlayerDecorateEffect
{
	struct DecorateSprite
	{
		fPoint			relative_coordinate;
		float			x_increment;
		float			y_increment;
		double			life_time;
		Sprite*			image;
		TimeFragment	timer;
		DecorateSprite(float relative_x, float relative_y, float x_increment_, float y_increment,
			double life_time_, Sprite* image_);
		bool			Release();
	};
public:
	CPlayerDecorateEffect(CPlayerCharacter& player_);
	~CPlayerDecorateEffect();

	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();

	bool		AddEffect(float relative_x, float relative_y, 
		float x_increment_, float y_increment, double life_time_, int sprite_id);
private:
	CPlayerCharacter&	player;
	TimeFragment		EffectTimer;

	std::vector<DecorateSprite> effects;
};

