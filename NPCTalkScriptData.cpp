#include "NPCTalkScriptData.h"
#include "StringData.h"
#include "DataProvider.h"
#include "CUIString.h"

bool NPCTalkScriptData::Release()
{
	return true;
}

int NPCTalkScriptData::GetId() { return npc_talk_script_id; }

bool NPCTalkScriptData::SetData(const std::string& lhs_type_name, const std::string& rhs_type_value)
{
	if (lhs_type_name == DataProvider__StringData(13))
	{
		npc_talk_script_id = std::atoi(rhs_type_value.c_str());
	}
	else if (lhs_type_name == DataProvider__StringData(15))
	{
		std::string conversation = rhs_type_value;
		while (true)
		{
			auto find_escape_sequence = conversation.find_first_of(92);
			if (find_escape_sequence == std::string::npos) {
				break;
			}
			npc_conversation.push_back(conversation.substr(0, find_escape_sequence));
			conversation = conversation.substr(find_escape_sequence + 1);
		}
		npc_conversation.push_back(conversation);
	}
	return true;
}

std::vector<CUIString*> NPCTalkScriptData::GetScriptData()
{
	std::vector<CUIString*> temp;
	for (auto& _string : npc_conversation)
	{
		temp.emplace_back(new CUIString(_string, CUICharColor::white, 0, 0, nullptr));
	}
	return std::move(temp);
}

std::string NPCTalkScriptData::GetInitializeFileName() {
	return DataProvider__StringData(3);
}