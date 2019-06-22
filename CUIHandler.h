#pragma once
#include "TSingleton.h"
#include "CUI.h"
#include <array>
#include <map>
#include <vector>
#include <unordered_map>
#include <functional>

class CUIWindow;

class CUIHandler : public TSingleton<CUIHandler>, public CUI
{
	friend class TSingleton<CUIHandler>;
public:
	CUIHandler();
	~CUIHandler();
	struct EventUI
	{
		size_t						event_id;
		CUI*						obj;
		std::function<void(CUI*)>	callback;
		float						x, y, width, height;
		EventUI() = default;
		EventUI(size_t id, CUI* obj_, std::function<void(CUI*)> callback_,
			float x_, float y_, float width_, float height_)
			: event_id(id), obj(obj_), callback(callback_)
			, x(x_), y(y_), width(width_), height(height_)
		{}
	};

	virtual bool			Init() override;
	virtual bool			Frame() override;
	virtual bool			Render() override;
	virtual bool			Release() override;

	bool					isRegistedUIWindow(int window_id);
	bool					RegisterUIWindow(int window_id, CUIWindow* window);
	bool					DeRegisterUIWindow(int window_id);
	bool					CreateNoticeUIWindow(float screen_x, float screen_y,
										std::string window_name, int script_id,
										std::function<void(void*)> invoke_function, void* obj);

	EventUI*				RegisterEventUI(CUI* obj_, std::function<void(CUI*)> callback_,
										float x_, float y_, float width_, float height_);
	bool					DeRegisterEventUI(size_t id_of_eventUI);
	bool					RequestToReleaseInCurrentUI(CUI* detached);
	bool					SetWindowPriority(int window_id);

private:
	bool					CheckEventUI();
	std::vector<CUIWindow*>						registed_ui_windows;
	std::unordered_map<size_t, EventUI*>		event_ui;
	std::vector<size_t>							reset_to_delete_event_ui;
	std::vector<CUI*>							reserve_to_delete;
};

