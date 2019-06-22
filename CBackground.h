#pragma once
#include "CFieldObject.h"
#include "Sprite.h"
class CBackground : public CFieldObject
{
public:
	CBackground(float logical_x, float logical_y, int sprite_id, bool alpha = true);
	~CBackground();

	virtual bool			Init() override;
	virtual bool			Frame() override;
	virtual bool			Render() override;
	virtual bool			Release() override;
	virtual bool			EffectedbyGravity(float gravity) override;
private:
	bool					is_alpha;
	Sprite*					sprite;
};