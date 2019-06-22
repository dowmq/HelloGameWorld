#pragma once
#include "CFieldObject.h"
#include "Sprite.h"
#include "SpriteProvider.h"

class CTile : public CFieldObject
{
	CTile(const CTile&) = default;
public:
	static size_t reference_count;
	~CTile();

	virtual bool			Init() override;
	virtual bool			Frame() override;
	virtual bool			Render() override;
	virtual bool			Release() override;
	virtual bool			EffectedbyGravity(float gravity) override;
	CTile*					Clone();

	static CTile*			CreateCTile(SpriteProvider::TileID tileid,
							float logical_x, float logical_y);
private:
	CTile(float logical_x, float logical_y);

	void					SetSprite(Sprite* sprite_);
	Sprite*					sprite;
};

