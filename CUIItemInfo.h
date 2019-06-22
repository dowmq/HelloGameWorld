#pragma once
#include "CUIWindow.h"
#include "CItem.h"
#include "CUIImage.h"
#include <array>
class CUIItemInfo : public CUIWindow
{
public:
	CUIItemInfo(int window_id, CItem* item, std::string window_name,
		float screen_x, float screen_y, float width, float height, CUI* owner);
	~CUIItemInfo();

	// Child class adjust uis pos and event UI
	virtual bool		MoveWindowFunction(float move_x, float move_y) override;
	virtual bool		Render() override;
	virtual bool		Release() override;
	float				client_x;
	float				client_y;
	CUIString*			item_name;
	CUIImage*			item_image;
	CUIString*			item_type;
	std::array<CUIString*, CItem::ItemOption::option_count> options_string;
};

