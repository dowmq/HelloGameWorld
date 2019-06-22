#include "CUIHandler.h"
#include "InputHandler.h"
#include "CArea.h"
#include "CUIWindow.h"
#include "CPlayerCharacter.h"
#include "CUINoticeWindow.h"
#include "NPCTalkScriptData.h"
#include <algorithm>
CUIHandler::CUIHandler() : CUI(nullptr)
{
}


CUIHandler::~CUIHandler()
{
}

bool CUIHandler::Init()
{
	return true;
}

bool CUIHandler::Frame()
{
	if (reserve_to_delete.empty() == false) {
		// 돌다가 iterator 바껴서 뒤질 것 같은 존나 쌔한 느낌 드네;;
		for (auto& deleted : reserve_to_delete) {
			CUI* parent = deleted->GetOwner();
			if (parent == this) { // 스파게티가 되어 간다 야호!!!
				CUIWindow* window = reinterpret_cast<CUIWindow*>(deleted);
				this->DeRegisterUIWindow(window->GetWindowId());
			}
			else {
				parent->DeleteElement(deleted); // parent 가 nullptr이라 죽음(0xFFFFFFFF)
			}
		}
		reserve_to_delete.clear();
	}
	if (reset_to_delete_event_ui.empty() == false) {
		for (auto event_id : reset_to_delete_event_ui) {
			delete event_ui[event_id]; event_ui[event_id] = nullptr;
			event_ui.erase(event_id);
		}
	}

	CheckEventUI();

	return true;
}

bool CUIHandler::Render()
{
	for (auto& window : registed_ui_windows) {
			window->Render();
	}
	return true;
}

bool CUIHandler::Release()
{
	for (auto& obj : reserve_to_delete) {
		delete obj;
		obj = nullptr;
	}
	reserve_to_delete.clear();

	for (auto& registed_event : event_ui)
	{
		delete registed_event.second;
		registed_event.second = nullptr;
	}
	event_ui.clear();

	for (auto& window : registed_ui_windows) {
		if (window != nullptr) {
			delete window; window = nullptr;
		}
	}
	registed_ui_windows.clear();
	return true;
}

CUIHandler::EventUI* CUIHandler::RegisterEventUI(CUI* obj_, 
	std::function<void(CUI*)> callback_,
	float x_, float y_, float width_, float height_)
{
	static size_t new_id = 0;
	EventUI* new_event = new EventUI(new_id, obj_, callback_, x_, y_, width_, height_);
	event_ui.emplace(new_id, new_event);
	new_id++;
	return new_event;
}

bool CUIHandler::DeRegisterEventUI(size_t id_of_eventUI)
{
	reset_to_delete_event_ui.push_back(id_of_eventUI);
	return true;
}

bool CUIHandler::RequestToReleaseInCurrentUI(CUI* detached)
{
	reserve_to_delete.push_back(detached);
	return true;
}

bool CUIHandler::CheckEventUI()
{
	float mouse_x = InputHandler::Instance().GetMousePos().x;
	float mouse_y = InputHandler::Instance().GetMousePos().y;
	if (event_ui.empty() == true)
		return false;

	if (registed_ui_windows.empty() == false) {
		auto& events = registed_ui_windows.back()->GetEvent();
		for (auto event_ : events) {
			if (event_ != nullptr) {
				auto iter = event_ui.find(event_->event_id);
				if (iter != event_ui.end()) {
					auto& ui = event_ui[event_->event_id];
					if (CArea::Is_in_RECT(ui->x, ui->y, ui->width, ui->height, mouse_x, mouse_y) == true)
					{
						ui->callback(ui->obj);
					}
				}
			}
		}
	}

	for (auto& element : event_ui)
	{
		auto& ui = element.second;
		if (ui != nullptr) {
			if (registed_ui_windows.empty() == false && ui->obj == registed_ui_windows.back())
				continue;
			if (CArea::Is_in_RECT(ui->x, ui->y, ui->width, ui->height, mouse_x, mouse_y) == true)
			{
				if (registed_ui_windows.empty() == false
					&& CArea::Is_in_RECT(registed_ui_windows.back()->GetScreenX(), registed_ui_windows.back()->GetScreenY(),
						registed_ui_windows.back()->GetWidth(), registed_ui_windows.back()->GetHeight(), mouse_x, mouse_y) == true)
				{
					continue;
				}
				ui->callback(ui->obj);
			}
		}
	}
	return true;
}

bool CUIHandler::isRegistedUIWindow(int window_id) {
	auto iter = std::find_if(registed_ui_windows.begin(), registed_ui_windows.end(),
		[=](CUIWindow* ui) {return ui->GetWindowId() == window_id; });
	if (iter == registed_ui_windows.end()) {
		return false;
	}
	return true;
}

bool CUIHandler::RegisterUIWindow(int window_id, CUIWindow* window)
{
	auto iter = std::find_if(registed_ui_windows.begin(), registed_ui_windows.end(),
		[=](CUIWindow* ui) {return ui->GetWindowId() == window_id; });
	if (iter == registed_ui_windows.end()) {
		window->SetOwner(this);
		window->Init();
		registed_ui_windows.emplace_back(window);
		return true;
	}
	delete window;
	return false;
}

bool CUIHandler::DeRegisterUIWindow(int window_id)
{
	auto iter = std::find_if(registed_ui_windows.begin(), registed_ui_windows.end(),
		[=](CUIWindow* ui) {return ui->GetWindowId() == window_id; });
	//auto iter = registed_ui_windows.find(window_id);
	if(iter != registed_ui_windows.end())
	{
		delete (*iter); (*iter) = nullptr;
		registed_ui_windows.erase(iter);
		return true;
	}
	return false;
}

bool CUIHandler::SetWindowPriority(int window_id)
{
	auto iter = std::find_if(registed_ui_windows.begin(), registed_ui_windows.end(),
		[=](CUIWindow* ui) {return ui->GetWindowId() == window_id; });
	//auto iter = registed_ui_windows.find(window_id);
	if (iter != registed_ui_windows.end())
	{
		CUIWindow* window = (*iter);
		registed_ui_windows.erase(iter);
		registed_ui_windows.push_back(window);
		return true;
	}
	return false;
}

bool CUIHandler::CreateNoticeUIWindow(float screen_x, float screen_y, std::string window_name,
	int script_id, std::function<void(void* obj)> invoke_function, void* obj)
{
	static int notice_window_id = 300000;
	std::vector<CUIString*> string_data = DataProvider__ScriptData(script_id);
	CUINoticeWindow* window =
		new CUINoticeWindow(notice_window_id, std::move(string_data), window_name, screen_x, screen_y, nullptr,
			invoke_function, obj);
	this->RegisterUIWindow(notice_window_id, window);
	notice_window_id++;
	return true;
}