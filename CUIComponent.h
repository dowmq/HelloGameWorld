#pragma once
#include "CUI.h"
class CUIComponent : public CUI
{
public:
	explicit CUIComponent(CUI* owner_);
	CUIComponent(const CUIComponent&);
	virtual ~CUIComponent();

	virtual bool			Init();
	virtual bool			Frame();
	virtual bool			Render();
	virtual bool			Release();
	
	virtual bool			AddElement(CUI* element);
	virtual bool			EmptyCompocnent();
	virtual size_t			GetComponentSize();
	virtual bool			DeleteElement(CUI* element) override;

	virtual bool			SetScreenPos(float x, float y) override;
protected:
	std::vector<CUI*>		Component;
};