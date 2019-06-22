#include "CUINPCTalk.h"
#include "CUINPC.h"
#include "CWindow.h"
#include "CUIHandler.h"
#include "InputHandler.h"

std::array< CUIButton*, CUIButton::Type::count>& CUINPCTalk::GetButtons() {
	return buttons;
}

CUINPCTalk::CUINPCTalk(int window_id_, std::vector<CUIString*>&& strings, std::string window_name, int sprite_id,
	float screen_x_, float screen_y_, float width_, float height_, CUI* owner)
	: CUIWindow(window_id_, window_name, screen_x_, screen_y_, width_, height_, owner)
{
	type = AdditionalOption::None;
	CreateBasicUI(screen_x_, screen_y_, std::move(strings), sprite_id);
}
CUINPCTalk::CUINPCTalk(int window_id_, std::vector<CUIString*>&& strings, std::string window_name, int sprite_id,
	float screen_x_, float screen_y_, CUI* owner,
	CUINPCTalk::AdditionalOption type_)
	: CUIWindow(window_id_, window_name, screen_x_, screen_y_, 0, 0, owner)
{
	type = type_;
	CreateBasicUI(screen_x_, screen_y_, std::move(strings), sprite_id);
	CreateAddtionalButton(type);
}

void CUINPCTalk::CreateBasicUI(float screen_x, float screen_y, std::vector<CUIString*>&& strings, int sprite_id)
{
	for (auto& element : button_events) {
		element = nullptr;
	}
	client_x = screen_x;
	client_y = screen_y + 17;


	float default_width = 100;
	float default_height = 50;

	float image_width = 0;
	float image_height = 0;

	float string_max_width = 0;
	float string_sum_height = 0;

	float space_width = 0;

	CUIImage* npc_image = new CUIImage(sprite_id, true, this);
	Component.push_back(npc_image);
	image_width = npc_image->GetWidth();
	image_height = npc_image->GetHeight();

	float temp_x = client_x + 10;
	space_width += 10;

	float ui_string_max_length = 0.0f;
	temp_x += npc_image->GetWidth() + 10;
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

	width = max(default_width, image_width + string_max_width + space_width);
	height = max(default_height, max(image_height, string_sum_height));

	float temp_image_height = height;
	float temp_y = client_y + temp_image_height / 2 - npc_image->GetHeight() / 2;
	npc_image->SetScreenPos(client_x + 10, temp_y);
}

void CUINPCTalk::CreateAddtionalButton(CUINPCTalk::AdditionalOption type)
{
	if (type == CUINPCTalk::AdditionalOption::None)
	{
		return;
	}
	switch (type)
	{
		case CUINPCTalk::AdditionalOption::YesNo:
		{
			CUIButton* btn_yes = new CUIButton(CUIButton::Type::Button_Yes, this);
			CUIButton* btn_no = new CUIButton(CUIButton::Type::Button_No, this);
			buttons[CUIButton::Type::Button_Yes] = btn_yes;
			buttons[CUIButton::Type::Button_No] = btn_no;

			float btn_y = client_y + height;
			height += btn_yes->GetHeight() + 5;
			float btn_no_x_pos = client_x + width - 5 - btn_no->GetWidth();
			btn_no->SetScreenPos(btn_no_x_pos, btn_y);
			btn_yes->SetScreenPos(btn_no_x_pos - 5 - btn_yes->GetWidth(), btn_y);
			Component.push_back(btn_no);
			Component.push_back(btn_yes);

			button_events[CUIButton::Type::Button_Yes] = CUIHandler::Instance().RegisterEventUI(this,
				[&btn_yes](CUI* obj) {
					if (InputHandler::Instance().is_left_click())
					{
						CUINPCTalk* talkwindow = (CUINPCTalk*)(obj);
						talkwindow->GetButtons()[CUIButton::Type::Button_Yes]->Invoke();
					}
				}, 
				btn_no_x_pos - 5 - btn_yes->GetWidth(), btn_y,
				btn_yes->GetWidth(), btn_yes->GetHeight());
			button_events[CUIButton::Type::Button_No] = CUIHandler::Instance().RegisterEventUI(this,
				[](CUI* obj) {
					if (InputHandler::Instance().is_left_click())
					{
						CUINPCTalk* talkwindow = reinterpret_cast<CUINPCTalk*>(obj);
						talkwindow->GetButtons()[CUIButton::Type::Button_No]->Invoke();
					}
				},
				btn_no_x_pos, btn_y,
				btn_no->GetWidth(), btn_no->GetHeight());
			events.push_back(button_events[CUIButton::Type::Button_Yes]);
			events.push_back(button_events[CUIButton::Type::Button_No]);
			break;
		}
		case CUINPCTalk::AdditionalOption::OK:
		{
			CUIButton* btn_ok = new CUIButton(CUIButton::Type::Button_Ok, this);
			buttons[CUIButton::Type::Button_Ok] = btn_ok;

			float btn_y = client_y + height;
			height += btn_ok->GetHeight() + 5;
			float btn_no_x_pos = client_x + width - 5 - btn_ok->GetWidth();
			btn_ok->SetScreenPos(btn_no_x_pos, btn_y);
			button_events[CUIButton::Type::Button_Ok] = CUIHandler::Instance().RegisterEventUI(this,
				[](CUI* obj) {
					if (InputHandler::Instance().is_left_click())
					{
						CUINPCTalk* talkwindow = reinterpret_cast<CUINPCTalk*>(obj);
						talkwindow->GetButtons()[CUIButton::Type::Button_Ok]->Invoke();
					}
				},
				btn_no_x_pos, btn_y,
				btn_ok->GetWidth(), btn_ok->GetHeight());
			Component.push_back(btn_ok);
			events.push_back(button_events[CUIButton::Type::Button_Ok]);
			break;
		}
	}
}

bool CUINPCTalk::Release()
{
	//for (auto& button : buttons) {
	//	if (button != nullptr) {
	//		button->Release();
	//		delete button; button = nullptr;
	//	}
	//}
	return CUIWindow::Release();
}