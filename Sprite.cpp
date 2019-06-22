#include "Sprite.h"
#include "CWindow.h"
#include <tchar.h>
#include "CWindow.h"
#include "SpriteFactory.h"

void  Sprite::DrawAlpha(CArea area)
{
	imageList[current_frame]->DrawAlphaBlend(area);
}

void Sprite::DrawAlpha(float x, float y)
{
	imageList[current_frame]->DrawAlphaBlend(x, y);
}

void  Sprite::Draw(CArea area, DWORD dwMode)
{
	imageList[current_frame]->DrawBitBlt(area, dwMode);
}

void Sprite::Draw(float x, float y, DWORD dwMode)
{
	imageList[current_frame]->DrawBitBlt(x, y, dwMode);
}

bool Sprite::Frame()
{
	if (is_loop_sprite == false)
	{
		life_time -= timer->GetSecondPerFrame();
		elapsed_time += timer->GetSecondPerFrame();;
		if (life_time <= 0) {
			is_dead_sprite = true;
		}
	}
	if (is_dead_sprite == true) {
		return true;
	}
	if (number_of_frame <= 1) {
		return true;
	}

	elapsed_time_per_frame += timer->GetSecondPerFrame();
	if (elapsed_time_per_frame > image_change_time)
	{
		current_frame++;
		if (current_frame >= number_of_frame)
		{
			current_frame = 0;
		}
		elapsed_time_per_frame = 0.0f;
	}
	return true;
}

void Sprite::Reset()
{
	current_frame = 0;
	is_dead_sprite = false;
	life_time = life_time + elapsed_time;
}

bool Sprite::Release()
{
	if (imageList.empty() == false) {
		for (auto& element : imageList)
		{
			if (element) {
				element->Release();
				delete element;
			}
		}
		imageList.clear();
	}
	return true;
}

Sprite::~Sprite()
{
	Release();
}

Sprite* Sprite::Clone() {
	Sprite* new_sprite = SpriteFactory::CreateSprite(text_file_name, '/', nIndex, width, interval);
	new_sprite->nIndex = this->nIndex;
	new_sprite->width = this->width;
	new_sprite->interval = this->interval;
	return new_sprite;
}