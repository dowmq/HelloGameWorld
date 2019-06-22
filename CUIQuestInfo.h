#pragma once
#include "CUI.h"
#include "CQuest.h"
#include "CUIString.h"
#include <vector>
class CUIQuestInfo : public CUI
{
public:
	CUIQuestInfo(CUI* owner_);
	~CUIQuestInfo();

	virtual bool			Init() override;
	virtual bool			Frame() override;
	virtual bool			Render() override;
	virtual bool			Release() override;

	bool					ReleaseCurrentStatusUI();
	bool					UpdateQuestUI(CQuest& quest);
private:
	bool					CreateQuestStatusUI(CQuest& quest);
	
	CUIString*				right_upper_explain_ui;
	CUIString*				ui_quest_name;
	std::vector<CUIString*>	ui_quest_status_message;
};