#pragma once
#include "CUIWindow.h"
#include "CUIButton.h"
#include <functional>

class CUINoticeWindow : public CUIWindow
{
public:
	CUINoticeWindow(int window_id_, std::vector<CUIString*>&& strings,
		std::string window_name, float screen_x, float screen_y, CUI* owner,
		std::function<void(void*)> ok_invoke_function, void* invoke_obj);
	~CUINoticeWindow() = default;

	CUIButton& GetButton();
private:
	void		CreateBasicUI(float screen_x, float screen_y, std::vector<CUIString*>&& strings);
	void		CreateAddtionalButton(std::function<void(void*)> ok_invoke_function, void* invoke_obj);

	float		client_x, client_y;
	CUIButton*	button;
	CUIHandler::EventUI* button_event;
};