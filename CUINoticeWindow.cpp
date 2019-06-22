#include "CUINoticeWindow.h"
#include "InputHandler.h"
#include "CUIHandler.h"

CUIButton& CUINoticeWindow::GetButton() { return *button; }

CUINoticeWindow::CUINoticeWindow(int window_id_, std::vector<CUIString*>&& strings, std::string window_name,
	float screen_x_, float screen_y_, CUI* owner,
	std::function<void(void*)> ok_invoke_function, void* invoke_obj)
	: CUIWindow(window_id_, window_name, screen_x_, screen_y_, 0, 0, owner)
{
	CreateBasicUI(screen_x_, screen_y_, std::move(strings));
	CreateAddtionalButton(ok_invoke_function, invoke_obj);
}

void CUINoticeWindow::CreateBasicUI(float screen_x, float screen_y, std::vector<CUIString*>&& strings)
{
	client_x = screen_x;
	client_y = screen_y + 17;


	float default_width = 100;
	float default_height = 50;

	float string_max_width = 0;
	float string_sum_height = 0;

	float space_width = 0;

	float temp_x = client_x + 10;
	space_width += 10;

	float ui_string_max_length = 0.0f;

	space_width += 10;
	for (size_t i = 0; i < strings.size(); ++i)
	{
		strings[i]->SetScreenPos(temp_x, screen_y + 17 + 5 + i * 10);
		string_max_width = max(string_max_width, strings[i]->GetWidth());
		string_sum_height += 10;
		Component.push_back(strings[i]);
	}
	string_sum_height += 10;
	space_width += 10;

	width = max(default_width,  + string_max_width + space_width);
	height = max(default_height, string_sum_height);
}

void CUINoticeWindow::CreateAddtionalButton(std::function<void(void*)> ok_invoke_function, void* invoke_obj)
{
	button = new CUIButton(CUIButton::Type::Button_Ok, this);
	float btn_y = client_y + height;
	height += button->GetHeight() + 5;
	float btn_no_x_pos = client_x + width - 5 - button->GetWidth();
	button->SetScreenPos(btn_no_x_pos, btn_y);
	button->SetInvokeFunction(ok_invoke_function, invoke_obj);
	button_event = CUIHandler::Instance().RegisterEventUI(this,
		[](CUI* obj) {
			if (InputHandler::Instance().is_left_click())
			{
				CUINoticeWindow* notice_window = reinterpret_cast<CUINoticeWindow*>(obj);
				notice_window->GetButton().Invoke();
				notice_window->CloseWindow();
			}
		},
		btn_no_x_pos, btn_y,
			button->GetWidth(), button->GetHeight());
	Component.push_back(button);
	events.push_back(button_event);
}