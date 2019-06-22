#pragma once
#include "Bitmap.h"
#include "TimeHandler.h"
#include "CArea.h"
#include  <vector>
class Sprite
{
	friend class SpriteFactory;
	Sprite() = default;
	
public:
	~Sprite();
	Sprite(const Sprite&) = delete;
	void							Draw(CArea area, DWORD dwMode);
	void							Draw(float x, float y, DWORD dwMode);
	void							Reset();
	RECT&							GetCurrentFrameImageRect() { return imageList[current_frame]->GetImageRect(); }

	bool							Frame();
	void							DrawAlpha(CArea area);
	void							DrawAlpha(float x, float y);
	bool							Release();
	Sprite*							Clone();

	int								sprite_id;
	TimeHandler*					timer;
	int								number_of_frame;
	int								current_frame;
	bool							is_dead_sprite;
	bool							is_loop_sprite;
	float							life_time;
	float							elapsed_time;
	float							image_change_time;
	float							elapsed_time_per_frame;
	std::vector<CBitmap*>			imageList;

	std::string						text_file_name;
	int								nIndex = 0;
	int								width = 0;
	int								interval = 0;
};

