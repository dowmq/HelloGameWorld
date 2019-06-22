#pragma once
#include "CUI.h"
#include "CPlayerCharacter.h"
#include "CUIString.h"
#include <array>
class CUICharacterInfo : public CUI
{
	enum BasicCharacterInfoUI
	{
		character_info_string,
		lv_string,
		hp_string,
		hp_gague,
		exp_string,
		exp_gaque,
		attack_string,
		defense_string,
		speed_string,

		size_of_basicInfo
	};
	enum Numerical_InfoString
	{
		Lv,
		attack,
		defense,
		speed,

		size_of_numerical
	};
public:
	CUICharacterInfo(float screen_x, float screen_y, CUI* owner_);
	~CUICharacterInfo();

	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;

	virtual bool		SetCharacterStat(CPlayerCharacter::CharacterStat stat, int value);

private:
	std::array<CUI*, BasicCharacterInfoUI::size_of_basicInfo> basic_info;
	std::array<CUI*, Numerical_InfoString::size_of_numerical> numeric_info;
};

