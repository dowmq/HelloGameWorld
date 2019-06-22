#pragma once
#include "CUI.h"
#include "CUIImage.h"
enum CUICharColor
{
	white,
	orange,
	yellow
};

class CUIChar : public CUI
{
	friend class CUIString;
public:
	CUIChar(char char_, int CUIChar_Color, CUI* owner_);
	~CUIChar();
	CUIChar(CUIChar&& other) noexcept;
	CUIChar& operator=(CUIChar&& other) noexcept;

	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
	virtual bool		SetScreenPos(float x, float y);
private:
	CUIImage*			image;
	char				character;
};

