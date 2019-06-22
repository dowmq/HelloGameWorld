#pragma once
#include "CUIComponent.h"
#include "Sprite.h"
#include "CPlayerCharacter.h"
#include "CUIImage.h"

class CEXPGauge : public CUIComponent
{
	CPlayerCharacter&	player;
public:
	CEXPGauge(float x, float y, CUI* owner_);
	~CEXPGauge() = default;

	virtual bool		Render() override;
};


class CEXPPiece : public CUI
{
	friend class CEXPGauge;
public:
	explicit			CEXPPiece(CUI* owner_);
	CEXPPiece*			Clone();

	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;

	virtual bool		SetScreenPos(float x, float y) override;
private:
	CEXPPiece(const CEXPPiece&);
	CUIImage*			image;
};