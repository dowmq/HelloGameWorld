#include "CUICharacterInfo.h"
#include "CHPGauge.h"
#include "CEXPGauge.h"
#include "CUIBackground.h"
CUICharacterInfo::CUICharacterInfo(float screen_x_, float screen_y_, CUI* owner_)
	: CUI(owner_)
{
	screen_x = screen_x_;
	screen_y = screen_y_;
}


CUICharacterInfo::~CUICharacterInfo()
{
	for (auto& element : basic_info)
	{
		if (element != nullptr) {
			element->Release();
			delete element; element = nullptr;
		}
	}
	for (auto& element : numeric_info)
	{
		if (element != nullptr) {
			element->Release();
			delete element; element = nullptr;
		}
	}
}

bool CUICharacterInfo::Init()
{
	basic_info[character_info_string] = new CUIString("character info", CUICharColor::yellow, screen_x, screen_y, this);
	basic_info[lv_string] = new CUIString("lv", CUICharColor::white, screen_x + 5, screen_y + 10, this);
	basic_info[hp_string] = new CUIString("hp", CUICharColor::white, screen_x + 5, screen_y + 20, this);
	basic_info[hp_gague] = new CHPGauge(screen_x + 40, screen_y + 22, this);
	basic_info[exp_string] = new CUIString("exp ", CUICharColor::white, screen_x + 5, screen_y + 30, this);
	basic_info[exp_gaque] = new CEXPGauge(screen_x + 40, screen_y + 32, this);
	basic_info[attack_string] = new CUIString("attack", CUICharColor::white, screen_x + 5, screen_y + 40, this);
	basic_info[defense_string] = new CUIString("defense", CUICharColor::white, screen_x + 5, screen_y + 50, this);
	basic_info[speed_string] = new CUIString("speed", CUICharColor::white, screen_x + 5, screen_y + 60, this);

	for (int i = 0; i < Numerical_InfoString::size_of_numerical; ++i)
		numeric_info[i] = nullptr;
	for (auto& element : basic_info)
	{
		element->Init();
	}
	return true;
}

bool CUICharacterInfo::Frame()
{
	for (auto& element : basic_info)
	{
		element->Frame();
	}
	for (auto& element : numeric_info)
	{
		element->Frame();
	}
	return true;
}

bool CUICharacterInfo::Render()
{
	for (auto& element : basic_info)
	{
		element->Render();
	}
	for (auto& element : numeric_info)
	{
		element->Render();
	}
	return true;
}

bool CUICharacterInfo::Release()
{
	for (auto& element : basic_info)
	{
		element->Release();
		delete element; element = nullptr;
	}
	for (auto& element : numeric_info)
	{
		element->Release();
		delete element; element = nullptr;
	}
	return true;
}

bool CUICharacterInfo::SetCharacterStat(CPlayerCharacter::CharacterStat stat, int value)
{
	float screen_x = 0.0;
	float screen_y = 0.0;
	switch (stat)
	{
		case CPlayerCharacter::CharacterStat::LV:
			if (numeric_info[Lv] != nullptr) {
				delete numeric_info[Lv]; numeric_info[Lv] = nullptr;
			}
			screen_x = basic_info[lv_string]->GetScreenX() + basic_info[lv_string]->GetWidth() + 10;
			screen_y = basic_info[lv_string]->GetScreenY();
			numeric_info[Lv] = new CUIString(std::to_string(value), CUICharColor::white, screen_x, screen_y, this);
			numeric_info[Lv]->Init();
			break;
		case CPlayerCharacter::CharacterStat::attack_power:
			if (numeric_info[attack] != nullptr) {
				delete numeric_info[attack]; numeric_info[attack] = nullptr;
			}
			screen_x = basic_info[attack_string]->GetScreenX() + basic_info[attack_string]->GetWidth() + 10;
			screen_y = basic_info[attack_string]->GetScreenY();
			numeric_info[attack] = new CUIString(std::to_string(value), CUICharColor::white, screen_x, screen_y, this);
			numeric_info[attack]->Init();
			break;
		case CPlayerCharacter::CharacterStat::defense_power:
			if (numeric_info[defense] != nullptr) {
				delete numeric_info[defense]; numeric_info[defense] = nullptr;
			}
			screen_x = basic_info[defense_string]->GetScreenX() + basic_info[defense_string]->GetWidth() + 10;
			screen_y = basic_info[defense_string]->GetScreenY();
			numeric_info[defense] = new CUIString(std::to_string(value), CUICharColor::white, screen_x, screen_y, this);
			numeric_info[defense]->Init();
			break;
		case CPlayerCharacter::CharacterStat::speed:
			if (numeric_info[speed] != nullptr) {
				delete numeric_info[speed]; numeric_info[speed] = nullptr;
			}
			screen_x = basic_info[speed_string]->GetScreenX() + basic_info[speed_string]->GetWidth() + 10;
			screen_y = basic_info[speed_string]->GetScreenY();
			numeric_info[speed] = new CUIString(std::to_string(value), CUICharColor::white, screen_x, screen_y, this);
			numeric_info[speed]->Init();
			break;
	}
	return true;
}