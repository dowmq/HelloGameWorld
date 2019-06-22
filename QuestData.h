#pragma once
#include "Data.h"
#include <string>
#include "StringData.h"
#define DataProvider__QuestData(qeust_id) (*DataProvider<QuestData>::Instance().GetData(qeust_id))

class QuestData : public Data
{
	friend class CQuest;
public:
	enum Type { CONVERSATION = 0, HUNT };
	QuestData() = default;
	~QuestData();
	bool		SetData(const std::string& lhs_type_name, const std::string& rhs_type_value);
	bool		Release();
	int			GetId();
	static std::string GetInitializeFileName();
private:
	Type						quest_type;
	int							quest_id;
	std::string					quest_name;

	std::vector<int>*			additional_condition = nullptr;
	int							start_condition_qeust_id;
	int							start_npc_id;
	int							start_npc_script_id;
	int							end_npc_id;
	int							end_npc_script_id;
	int							clear_exp_reward;
	int							clear_item_reward;


};

