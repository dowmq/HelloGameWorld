#pragma once
#include "CFieldObject.h"
#include <functional>
#include <string>
#include <vector>
#include "Sprite.h"
class CUI
{
	friend class CUIHandler;
public:
	explicit CUI(CUI* owner_);
	CUI(const CUI&);
	virtual ~CUI() = default;

	virtual bool			Init() = 0;
	virtual bool			Frame() = 0;
	virtual bool			Render() = 0;
	virtual bool			Release() = 0;
			void			SetOwner(CUI* owner_) { owner = owner_; }
			CUI*			GetOwner() { return owner; }
	// called by UIHandler
	virtual bool			DeleteElement(CUI* element) { return true; }

	virtual bool			SetScreenPos(float x, float y);
			bool			AddScreenPos(float x, float y);
	virtual bool			SetWidth(float width_);
	virtual bool			SetHeight(float height_);
	float					GetScreenX();
	float					GetScreenY();
	float					GetWidth();
	float					GetHeight();
protected:
	CUI*					owner;
	float screen_x;
	float screen_y;
	float width;
	float height;
};