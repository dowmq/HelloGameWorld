#pragma once
#include "CUIComponent.h"
#include <string>
#include <list>
class CUIChatLog : public CUIComponent
{
	std::list<std::string> stored_chatting;
public:
	static CUIChatLog*		instance_;
	virtual bool			Frame() override;
	bool					StoreString(std::string& str);

	CUIChatLog(float screen_x, float screen_y, float width_, float height_, CUI* owner_);
	~CUIChatLog();
};

