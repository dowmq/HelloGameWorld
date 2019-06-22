#pragma once
#include "CUI.h"

#include "CUIImage.h"

#include <functional>
class CUIButton : public CUI
{
public:
	enum Type {
		Button_Yes,
		Button_No,
		Button_Ok,

		count
	};
	CUIButton(CUIButton::Type type_, CUI* owner_);
	~CUIButton();

	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
	virtual bool		SetScreenPos(float x, float y) override;
	bool				SetInvokeFunction(std::function<void(void*)> func_, void* obj_);
	void				Invoke();

private:
	CUIButton::Type		type;
	CUIImage*			image;
	void*				obj;
	std::function<void(void*)> func; 
};

