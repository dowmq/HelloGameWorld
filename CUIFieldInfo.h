#pragma once
#include "CUI.h"
#include "CUIString.h"
#include <array>
#include "CField.h"

class CUIFieldInfo : public CUI
{
	enum FieldInfoList
	{
		ui_field_name,
		field_name,
		ui_difficulty,
		difficulty,

		size_of_list
	};
public:
	explicit CUIFieldInfo(CField& field);
	~CUIFieldInfo();

	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;

	bool				Update(CField& field_);
	bool				CreateUI(CField& field_);
private:
	std::array<CUIString*, FieldInfoList::size_of_list> uis;
};

