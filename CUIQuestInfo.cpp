#include "CUIQuestInfo.h"
#include "StringData.h"


CUIQuestInfo::CUIQuestInfo(CUI* owner_) : CUI(owner_)
{
	right_upper_explain_ui = new CUIString(DataProvider__StringData(42), 
		CUICharColor::orange, 550, 5, this);
	ui_quest_name = nullptr;
}

CUIQuestInfo::~CUIQuestInfo()
{
	if (right_upper_explain_ui) {
		right_upper_explain_ui->Release();
		delete right_upper_explain_ui; 
	}
	if (ui_quest_name) {
		ui_quest_name->Release();
		delete ui_quest_name;
	}
	if (ui_quest_status_message.empty() == false) {
		for (auto& element : ui_quest_status_message)
		{
			element->Release();
			delete element; element = nullptr;
		}
		ui_quest_status_message.clear();
	}
}

bool CUIQuestInfo::Init()
{
	if (right_upper_explain_ui) {
		right_upper_explain_ui->Init();
	}
	if (ui_quest_name) {
		ui_quest_name->Init();
	}
	if (ui_quest_status_message.empty() == false) {
		for (auto& element : ui_quest_status_message)
		{
			element->Init();
		}
	}
	return true;
}

bool CUIQuestInfo::Frame()
{
	if (right_upper_explain_ui) {
		right_upper_explain_ui->Frame();
	}
	if (ui_quest_name) {
		ui_quest_name->Frame();
	}
	if (ui_quest_status_message.empty() == false) {
		for (auto& element : ui_quest_status_message)
		{
			element->Frame();
		}
	}
	return true;
}

bool CUIQuestInfo::Render()
{
	if (right_upper_explain_ui) {
		right_upper_explain_ui->Render();
	}
	if (ui_quest_name) {
		ui_quest_name->Render();
	}
	if (ui_quest_status_message.empty() == false) {
		for (auto& element : ui_quest_status_message)
		{
			element->Render();
		}
	}
	return true;
}

bool CUIQuestInfo::Release()
{
	if (right_upper_explain_ui) {
		right_upper_explain_ui->Release();
		delete right_upper_explain_ui; right_upper_explain_ui = nullptr;
	}
	if (ui_quest_name) {
		ui_quest_name->Release();
		delete ui_quest_name; ui_quest_name = nullptr;
	}
	if (ui_quest_status_message.empty() == false) {
		for (auto& element : ui_quest_status_message)
		{
			element->Release();
			delete element; element = nullptr;
		}
		ui_quest_status_message.clear();
	}
	return true;
}

bool CUIQuestInfo::UpdateQuestUI(CQuest& quest)
{
	ReleaseCurrentStatusUI();
	CreateQuestStatusUI(quest);
	return true;
}

bool CUIQuestInfo::CreateQuestStatusUI(CQuest& quest)
{
	ui_quest_name = new CUIString(quest.GetQuestName(),
		CUICharColor::white, 550, 15, this);
	std::string str = quest.GetQuestStatusMessage();
	int index = 0;
	auto escape_char = str.find_first_of('\\');
	do
	{
		escape_char = str.find_first_of('\\');
		if (escape_char == std::string::npos) {
			ui_quest_status_message.emplace_back(new CUIString(
				str, CUICharColor::white, 550, 25 + index * 10, this));
			break;
		}
		ui_quest_status_message.emplace_back(new CUIString(
			str.substr(0, escape_char), CUICharColor::white, 550, 25 + index * 10, this
		));
		str = str.substr(escape_char + 1);
		index++;

	} while (true);
	return true;
}

bool CUIQuestInfo::ReleaseCurrentStatusUI()
{
	if (ui_quest_name) {
		ui_quest_name->Release();
		delete ui_quest_name; ui_quest_name = nullptr;
	}
	if (ui_quest_status_message.empty() == false) {
		for (auto& element : ui_quest_status_message)
		{
			element->Release();
			delete element; element = nullptr;
		}
		ui_quest_status_message.clear();
	}
	return true;
}