#include "CUIWindow.h"
#include "InputHandler.h"
#include "CWindow.h"
#include "SpriteProvider.h"
uint64_t CUIWindow::entire_uiwindow_lock_flag = 0;
int CUIWindow::current_job_window_id = 0;

CUIWindow::CUIWindow(int window_id_, std::string window_name_,
	float screen_x_, float screen_y_, float width_, float height_, CUI* owner) 
	: CUIComponent(owner)
{
	screen_x = screen_x_;
	screen_y = screen_y_;
	width = width_;
	height = height_;
	window_id = window_id_;
	window_name = window_name_;
	events.resize(CUIWIndowEvent::count);
}


CUIWindow::~CUIWindow()
{
	if (title_bar) {
		title_bar->Release();
		delete title_bar; title_bar = nullptr;
	}
	if (title_string) {
		title_string->Release();
		delete title_string; title_string = nullptr;
	}
	if (window_close) {
		window_close->Release();
		delete window_close; window_close = nullptr;
	}
	if (background_body) {
		background_body->Release();
		delete background_body; background_body = nullptr;
	}
}

bool CUIWindow::Init()
{
	title_bar = new CUIBackground(screen_x, screen_y, width, 17, BackgroundColor::blue, this);
	window_close = new CUIImage(SpriteProvider::UIID::UIWindow_close, screen_x + width - 17, screen_y, false, this);
	title_string = new CUIString(window_name, CUICharColor::white, screen_x + 3, screen_y + 4, this);
	background_body = new CUIBackground(screen_x, screen_y + 17, width, height, BackgroundColor::black, this);

	CUIHandler& uihandler = CUIHandler::Instance();
	// 제목 표시줄
	events[CUIWIndowEvent::move_window] = uihandler.RegisterEventUI(this,
		[](CUI* ui) {
			CUIWindow* window = reinterpret_cast<CUIWindow*>(ui);
			if (CUIWindow::current_job_window_id == window->GetWindowId() ||
				CUIWindow::GetCUIWindowLock(CUIWindow::entire_uiwindow_lock_flag, CUIWindowLock::work_lock) == false) {
				if (InputHandler::Instance().is_left_click_holding())
				{
					POINT&& mouse_pos_variable = InputHandler::Instance().GetMousePosVariation();
					if (window->GetScreenY() + 17 + window->GetHeight() + mouse_pos_variable.y >= 480) {
						mouse_pos_variable.y = 480 - window->GetScreenY() - window->GetHeight() - 17;
					}
					for (auto event_ : window->GetEvent())
					{
						if (event_ != nullptr) {
							event_->x = event_->x + mouse_pos_variable.x;
							event_->y = event_->y + mouse_pos_variable.y;
						}
					}
					window->AddScreenPos(mouse_pos_variable.x, mouse_pos_variable.y);
					window->MoveWindowFunction(mouse_pos_variable.x, mouse_pos_variable.y);
					CUIWindow::current_job_window_id = window->GetWindowId();
					CUIWindow::SetCUIWindowLock(CUIWindow::entire_uiwindow_lock_flag, CUIWindowLock::work_lock, true);
				}
			}
		},
		screen_x, screen_y, width - 17, 17);
	// UIWindow 간의 렌더링 우선순위 설정(클릭시 맨 위로 가게)
	events[CUIWIndowEvent::get_priority] = uihandler.RegisterEventUI(this,
		[](CUI* ui) {
			CUIWindow* window = reinterpret_cast<CUIWindow*>(ui);
			if (CUIWindow::current_job_window_id == window->GetWindowId() ||
				CUIWindow::GetCUIWindowLock(CUIWindow::entire_uiwindow_lock_flag, CUIWindowLock::work_lock) == false)
			{
				if (InputHandler::Instance().is_left_click())
				{
					CUIHandler::Instance().SetWindowPriority(window->GetWindowId());
					CUIWindow::current_job_window_id = window->GetWindowId();
					CUIWindow::SetCUIWindowLock(CUIWindow::entire_uiwindow_lock_flag, CUIWindowLock::work_lock, true);
				}
			}
		}, screen_x, screen_y, width, height + 17);
	// X Button(Close)
	events[CUIWIndowEvent::close] = uihandler.RegisterEventUI(this,
		[](CUI* ui) {
			CUIWindow* window = reinterpret_cast<CUIWindow*>(ui);
			if (InputHandler::Instance().is_left_click())
			{
				window->CloseWindow();
			}
		}, screen_x + width - 17, screen_y, 17, 17);
	return true;
}

bool CUIWindow::Frame()
{
	if (title_bar)
		title_bar->Frame();
	if (title_string)
		title_string->Frame();
	if (window_close)
		window_close->Frame();
	if (background_body)
		background_body->Frame();
	return CUIComponent::Frame();
}

bool CUIWindow::Render()
{
	if (title_bar)
		title_bar->Render();
	if (title_string)
		title_string->Render();
	if (window_close)
		window_close->Render();
	if (background_body)
		background_body->Render();
	return CUIComponent::Render();
}

bool CUIWindow::Release()
{
	if (title_bar) {
		title_bar->Release();
		delete title_bar; title_bar = nullptr;
	}
	if (title_string) {
		title_string->Release();
		delete title_string; title_string = nullptr;
	}
	if (window_close) {
		window_close->Release();
		delete window_close; window_close = nullptr;
	}
	if (background_body) {
		background_body->Release();
		delete background_body; background_body = nullptr;
	}
	return CUIComponent::Release();
}
bool CUIWindow::GetCUIWindowLock(uint64_t& flag, CUIWindowLock lock) {
	return (flag >> lock) & 0x01;
}

void CUIWindow::SetCUIWindowLock(uint64_t& flag, CUIWindowLock lock, bool value) {
	value ? flag |= 1i64 << lock : flag &= ~(1i64 << lock);
}

CUIHandler::EventUI& CUIWindow::GetCUIWindowEvent(CUIWindow::CUIWIndowEvent CUIWIndowEvent)
{
	return *(events[CUIWIndowEvent]);
}

//std::array<CUIHandler::EventUI*, CUIWindow::CUIWIndowEvent::count>& CUIWindow::GetEvent() {
//	return events;
//}

std::vector<CUIHandler::EventUI*>& CUIWindow::GetEvent() {
	return events;
}

void CUIWindow::DeRegisterAllEvent()
{
	for (auto& element : events)
	{
		if (element != nullptr) {
			CUIHandler::Instance().DeRegisterEventUI(element->event_id);
		}
		element = nullptr;
	}
}

bool CUIWindow::SetScreenPos(float x, float y)
{
	screen_x = x;
	screen_y = y;
	title_bar->SetScreenPos(x, y);
	window_close->SetScreenPos(x + width - 17, y);
	title_string->SetScreenPos(x + 3, y + 4);
	background_body->SetScreenPos(x, y + 17);
	return true;
}

bool CUIWindow::MoveWindowFunction(float move_x, float move_y)
{
	for (auto& element : Component) {
		element->AddScreenPos(move_x, move_y);
	}
	return true;
}

bool CUIWindow::PostCloseFunction()
{
	return true;
}

bool CUIWindow::CloseWindow()
{
	DeRegisterAllEvent();
	CUIHandler::Instance().RequestToReleaseInCurrentUI(this);
	PostCloseFunction();
	return true;
}