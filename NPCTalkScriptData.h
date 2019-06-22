#pragma once
#include "Data.h"
#include <string>
#include <vector>
#include "DataProvider.h"
#define DataProvider__ScriptData(script_id) (*DataProvider<NPCTalkScriptData>::Instance().GetData(script_id)).GetScriptData()
class CUIString;
class NPCTalkScriptData : public Data
{
public:
	enum ScriptID
	{
		Codex_Talk = 850001,
		Conjurer_Talk = 850002,
		DungeonMaster_Talk = 850003,
		QuestTalk_ConversationwithDungeonMaster_Start = 860002,
		QuestTalk_ConversationwithDungeonMaster_End = 860003,
		QuestTalk_GoblinHunt_Start = 860004,
		QuestTalk_GoblinHunt_End = 860003,
		Notice_die = 860005,
	};
	NPCTalkScriptData() = default;
	~NPCTalkScriptData() = default;

	bool		SetData(const std::string& lhs_type_name, const std::string& rhs_type_value);
	bool		Release();
	int	GetId();
	static std::string GetInitializeFileName();
	std::vector<CUIString*> GetScriptData();
private:
	int npc_talk_script_id;
	std::vector<std::string> npc_conversation;
};

