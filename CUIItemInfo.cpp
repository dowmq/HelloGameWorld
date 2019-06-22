#include "CUIItemInfo.h"
#include <string>
#include "CItemDataProvider.h"

CUIItemInfo::CUIItemInfo(int window_id, CItem* item, std::string window_name,
	float screen_x_, float screen_y_, float width, float height, CUI* owner)
	: CUIWindow(window_id, window_name, screen_x_, screen_y_, width, height, owner)
{
	client_x = screen_x_;
	client_y = screen_y_ + 17;

	int grade = item->GetItemOption(CItem::ItemOption::grade);

	item_name = new CUIString(item->GetItemName(), grade > 3 ? CUICharColor::yellow : CUICharColor::white
		, 0, 0, this);
	item_image = new CUIImage(item->GetSprite() , true, this);
	std::string str("");
	switch (item->GetType())
	{
		case CItem::ItemType::ring:
			str = "type ring";
			break;
		case CItem::ItemType::head:
			str = "type head";
			break;
		case CItem::ItemType::weapon:
			str = "type weapon";
			break;
		case CItem::ItemType::armor:
			str = "type aromor";
			break;
		case CItem::ItemType::torch:
			str = "type torch";
			break;
		case CItem::ItemType::feet:
			str = "type feet";
			break;
	}
	item_type = new CUIString(str, CUICharColor::white, 0, 0, this);

	str = "grade ";
	str.append(std::to_string((int)item->GetItemOption(CItem::ItemOption::grade)));
	options_string[CItem::ItemOption::grade] = new CUIString(str, CUICharColor::white, 0, 0, this);
	str = "req lv ";
	str.append(std::to_string((int)item->GetItemOption(CItem::ItemOption::lvConstrain)));
	options_string[CItem::ItemOption::lvConstrain] = new CUIString(str, CUICharColor::white, 0, 0, this);
	str = "inc att ";
	str.append(std::to_string((int)item->GetItemOption(CItem::ItemOption::incAtt)));
	options_string[CItem::ItemOption::incAtt] = new CUIString(str, CUICharColor::white, 0, 0, this);
	str = "inc def ";
	str.append(std::to_string((int)item->GetItemOption(CItem::ItemOption::incDfn)));
	options_string[CItem::ItemOption::incDfn] = new CUIString(str, CUICharColor::white, 0, 0, this);
	str = "inc spd ";
	str.append(std::to_string((int)item->GetItemOption(CItem::ItemOption::incSpeed)));
	options_string[CItem::ItemOption::incSpeed] = new CUIString(str, CUICharColor::white, 0, 0, this);

	float temp_x = client_x;
	float temp_y = client_y;

	temp_y += 2;
	item_name->SetScreenPos(temp_x + 5, temp_y);
	
	temp_x += 2; temp_y += 10;
	item_image->SetScreenPos(temp_x + 8, temp_y + 21);
	
	//temp_y += 34;
	item_type->SetScreenPos(temp_x + 33 + 5, temp_y);
	temp_y += 10;
	options_string[CItem::ItemOption::grade]->SetScreenPos(temp_x + 33 + 5, temp_y);
	temp_y += 10;
	options_string[CItem::ItemOption::lvConstrain]->SetScreenPos(temp_x + 33 + 5, temp_y);

	temp_y += 10;
	options_string[CItem::ItemOption::incAtt]->SetScreenPos(temp_x + 33 + 5, temp_y);
	temp_y += 10;
	options_string[CItem::ItemOption::incDfn]->SetScreenPos(temp_x + 33 + 5, temp_y);
	temp_y += 10;
	options_string[CItem::ItemOption::incSpeed]->SetScreenPos(temp_x + 33 + 5, temp_y);
}

bool CUIItemInfo::MoveWindowFunction(float move_x, float move_y)
{
	if (item_name)
		item_name->AddScreenPos(move_x, move_y);
	if (item_image)
		item_image->AddScreenPos(move_x, move_y);
	if (item_type)
		item_type->AddScreenPos(move_x, move_y);

	for (auto& element : options_string) {
		if(element)
			element->AddScreenPos(move_x, move_y);
	}
	return true;
}

bool CUIItemInfo::Render()
{
	CUIWindow::Render();
	if (item_name) {
		item_name->Render();
	}
	if (item_image) {
		item_image->Render();
	}
	if(item_type)
		item_type->Render();
	for (CUIString* str : options_string) {
		if(str)
			str->Render();
	}
	return true;
}

bool CUIItemInfo::Release()
{
	if (item_name != nullptr) {
		item_name->Release();
		delete item_name; item_name = nullptr;
	}
	if (item_image != nullptr) {
		item_image->Release();
		delete item_image; item_image = nullptr;
	}
	if (item_type != nullptr) {
		item_type->Release();
		delete item_type; item_type = nullptr;
	}

	for (CUIString* str : options_string) {
		if (str != nullptr) {
			str->Release();
			delete str; str = nullptr;
		}
	}
	return CUIWindow::Release();
}

CUIItemInfo::~CUIItemInfo()
{
	if (item_name != nullptr) {
		item_name->Release();
		delete item_name; item_name = nullptr;
	}
	if (item_image != nullptr) {
		item_image->Release();
		delete item_image; item_image = nullptr;
	}
	if (item_type != nullptr) {
		item_type->Release();
		delete item_type; item_type = nullptr;
	}

	for (CUIString* str : options_string) {
		if (str != nullptr) {
			str->Release();
			delete str; str = nullptr;
		}
	}
}