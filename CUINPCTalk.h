#pragma once
#include "CUIWindow.h"
#include "CUIString.h"
#include "CUIButton.h"
#include <vector>
#include <array>
#include <string>
#include <functional>
class CUINPCTalk : public CUIWindow
{
public:
	enum AdditionalOption {
		None = 0,
		YesNo = 1,
		OK = 2,

		count
	};
	CUINPCTalk(int window_id_, std::vector<CUIString*>&& strings, std::string window_name, 
		int sprite_id, float screen_x, float screen_y,
		float width, float height, CUI* owner);
	CUINPCTalk(int window_id_, std::vector<CUIString*>&& strings, std::string window_name,
		int sprite_id, float screen_x, float screen_y, CUI* owner,
		CUINPCTalk::AdditionalOption button = CUINPCTalk::AdditionalOption::None);
	~CUINPCTalk() = default;
	virtual bool			Release();
	
	std::array< CUIButton*, CUIButton::Type::count>& GetButtons();
private:
	void		CreateBasicUI(float screen_x, float screen_y, std::vector<CUIString*>&& strings, int sprite_id);
	void		CreateAddtionalButton(CUINPCTalk::AdditionalOption type);

	float		client_x, client_y;
	AdditionalOption		type;
	std::array< CUIButton*, CUIButton::Type::count> buttons;
	std::array<CUIHandler::EventUI*, CUIButton::Type::count> button_events;
};

