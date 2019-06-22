#include "QuestData.h"


QuestData::~QuestData()
{
	if (additional_condition != nullptr)
	{
		if(additional_condition->empty() == false)
			additional_condition->clear();
		delete additional_condition;
	}
}

bool QuestData::Release()
{
	if (additional_condition != nullptr)
	{
		if (additional_condition->empty() == false)
			additional_condition->clear();
		delete additional_condition; additional_condition = nullptr;
	}
	return true;
}

int QuestData::GetId() { return quest_id; }

bool QuestData::SetData(const std::string& lhs_type_name, const std::string& rhs_type_value)
{

	if (lhs_type_name == DataProvider__StringData(16)) {
		quest_id = std::atoi(rhs_type_value.c_str());
	}
	else if (lhs_type_name == DataProvider__StringData(17)) {
		quest_name = rhs_type_value;
	} 
	else if(lhs_type_name == DataProvider__StringData(18)) {
		quest_type = (QuestData::Type)std::atoi(rhs_type_value.c_str());
	}
	else if (lhs_type_name == DataProvider__StringData(19)) {
		if (additional_condition == nullptr) {
			additional_condition = new std::vector<int>(); additional_condition->resize(2);
		}
		additional_condition->operator[](0) = (std::atoi(rhs_type_value.c_str()));
	}
	else if (lhs_type_name == DataProvider__StringData(20)) {
		if (additional_condition == nullptr) {
			additional_condition = new std::vector<int>(); additional_condition->resize(2);
		}
		additional_condition->operator[](1) = (std::atoi(rhs_type_value.c_str()));
	}
	else if (lhs_type_name == DataProvider__StringData(21)) {
		start_condition_qeust_id = std::atoi(rhs_type_value.c_str());
	}
	else if (lhs_type_name == DataProvider__StringData(22)) {
		start_npc_id = std::atoi(rhs_type_value.c_str());
	}
	else if (lhs_type_name == DataProvider__StringData(23)) {
		start_npc_script_id = std::atoi(rhs_type_value.c_str());
	}
	else if (lhs_type_name == DataProvider__StringData(24)) {
		end_npc_id = std::atoi(rhs_type_value.c_str());
	}
	else if (lhs_type_name == DataProvider__StringData(25)) {
		end_npc_script_id = std::atoi(rhs_type_value.c_str());
	}
	else if (lhs_type_name == DataProvider__StringData(26)) {
		clear_exp_reward = std::atoi(rhs_type_value.c_str());
	}
	else if (lhs_type_name == DataProvider__StringData(27)) {
		clear_item_reward = std::atoi(rhs_type_value.c_str());
	}
	return true;
}

std::string QuestData::GetInitializeFileName() {
	return DataProvider__StringData(4);
}