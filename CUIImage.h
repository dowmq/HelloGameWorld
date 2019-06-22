#pragma once
#include "CUI.h"
#include "Sprite.h"
#include <string>
class CUIImage : public CUI
{
public:
	CUIImage(int sprite_id, float screen_x_, float screen_y_, bool isAlpha_, CUI* owner_);
	CUIImage(int sprite_id, bool isAlpha_, CUI* owner_);
	CUIImage(Sprite* sprite, bool isAlpha_, CUI* owner_);
	~CUIImage();

	CUIImage(const CUIImage&);
	CUIImage(CUIImage&& other) noexcept;
	CUIImage& operator=(CUIImage&& other) noexcept;

	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;

	virtual bool		SetWidth(float width_) override;
	virtual bool		SetHeight(float height_) override;

	Sprite*				GetSprite() { return sprite; }
protected:
	bool				isAlpha;
	Sprite*				sprite;
};

