#include "CUIFieldInfo.h"
#include "StringData.h"
#include <cmath>

CUIFieldInfo::CUIFieldInfo(CField& field) 
	: CUI(nullptr)
{
	screen_x = 5;
	screen_y = 5;

	CreateUI(field);
}


CUIFieldInfo::~CUIFieldInfo()
{
	for (auto& ui : uis) {
		if (ui != nullptr) {
			ui->Release();
			delete ui;
		}
	}
}

bool CUIFieldInfo::Init()
{
	for (auto& ui : uis) {
		ui->Init();
	}
	return true;
}

bool CUIFieldInfo::Frame()
{
	for (auto& ui : uis) {
		ui->Frame();
	}
	return true;
}

bool CUIFieldInfo::Render()
{
	for (auto& ui : uis) {
		ui->Render();
	}
	return true;
}

bool CUIFieldInfo::Release()
{
	for (auto& ui : uis) {
		ui->Release();
		delete ui; ui = nullptr;
	}
	return true;
}

bool CUIFieldInfo::Update(CField& field)
{
	Release();

	CreateUI(field);
	return true;
}

bool CUIFieldInfo::CreateUI(CField& field)
{
	float temp_y = screen_y;
	uis[FieldInfoList::ui_field_name] = new CUIString(DataProvider__StringData(28),
		CUICharColor::orange, screen_x, screen_y, this);
	uis[FieldInfoList::field_name] = new CUIString(field.GetFieldName(),
		CUICharColor::white, screen_x + 10 + uis[FieldInfoList::ui_field_name]->GetWidth(), screen_y, this);

	temp_y += uis[FieldInfoList::field_name]->GetHeight();
	temp_y += 5;
	uis[FieldInfoList::ui_difficulty] = new CUIString(DataProvider__StringData(29),
		CUICharColor::orange, screen_x, temp_y, this);
	uis[FieldInfoList::difficulty] = new CUIString(field.GetLevelOfDifficulty(),
		CUICharColor::white, screen_x + 10 + uis[FieldInfoList::ui_difficulty]->GetWidth(), temp_y, this);
	width = max(uis[FieldInfoList::ui_field_name]->GetWidth() + 10 + uis[FieldInfoList::field_name]->GetWidth(),
		uis[FieldInfoList::ui_difficulty]->GetWidth() + 10 + uis[FieldInfoList::difficulty]->GetWidth());
	height = 5 + uis[FieldInfoList::field_name]->GetHeight() + uis[FieldInfoList::difficulty]->GetHeight();
	return true;
}