#pragma once
#include "CUIComponent.h"
#include "Sprite.h"
#include "CPlayerCharacter.h"
#include "CUIImage.h"

class CHPGauge : public CUIComponent
{
	CPlayerCharacter&	player;
public:
	CHPGauge(float x, float y, CUI* owner_);
	~CHPGauge() = default;

	virtual bool		Render() override;
};


class CHPPiece : public CUI
{
	friend class CHPGauge;
public:
	explicit CHPPiece(CUI* owner_);
	CHPPiece*			Clone();
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;

	virtual bool		SetScreenPos(float x, float y) override;
private:
	CHPPiece(const CHPPiece&);
	CUIImage*			image;
};