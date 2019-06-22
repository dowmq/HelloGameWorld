#pragma once
#include "CUIComponent.h"
#include "CUIBackground.h"
#include "CUIHandler.h"
#include "CUIString.h"
#include <string>
#include <array>

class CUIWindowBody;
class CUIWindow : public CUIComponent
{
public:
	CUIWindow(int window_id_, std::string window_name,
		float screen_x, float screen_y, float width, float height, CUI* owner);
	virtual ~CUIWindow();

	enum CUIWIndowEvent {
		move_window,
		close,
		get_priority,
		count
	};
	enum CUIWindowLock {
		work_lock,
		overlapped, // 겹친 부분을 클릭햇을때 동시 이벤트 방지
	};
	static uint64_t			entire_uiwindow_lock_flag;
	static int				current_job_window_id;
	virtual bool			SetScreenPos(float x, float y) override;

	static inline bool		GetCUIWindowLock(uint64_t& flag, CUIWindowLock);
	static inline void		SetCUIWindowLock(uint64_t& flag, CUIWindowLock, bool value);
	inline std::vector<CUIHandler::EventUI*>&	GetEvent();
	inline CUIHandler::EventUI&					GetCUIWindowEvent(CUIWindow::CUIWIndowEvent CUIWIndowEvent);
	void										DeRegisterAllEvent();

	virtual bool								Init() override;
	virtual bool								Frame() override;
	virtual bool								Render() override;
	virtual bool								Release() override;

	
	virtual bool								MoveWindowFunction(float move_x, float move_y);

	virtual bool								CloseWindow();
	int											GetWindowId() { return window_id; }
protected:
	virtual bool								PostCloseFunction();

	int											window_id;
	std::string									window_name;
	std::vector<CUIHandler::EventUI*>			events;
	CUIBackground*								title_bar;
	CUIString*									title_string;
	CUIImage*									window_close;
	CUIBackground*								background_body;
};	

