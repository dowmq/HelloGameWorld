#include "NPCData.h"

bool NPCData::Release() { return true; }
int NPCData::GetId() { return npc_id; }
const std::string&	NPCData::GetName() const { return npc_name; }
const std::string&	NPCData::GetExplain() const { return npc_explain; }
const int			NPCData::GetScriptId() const { return npc_script_id; }
const int			NPCData::GetSpriteId() const { return npc_sprite_id; }

bool NPCData::SetData(const std::string& lhs_type_name, const std::string& rhs_type_value)
{
	if (lhs_type_name == DataProvider__StringData(10))
	{
		npc_id = std::atoi(rhs_type_value.c_str());
	}
	else if (lhs_type_name == DataProvider__StringData(11))
	{
		npc_name = rhs_type_value;
	}
	else if (lhs_type_name == DataProvider__StringData(12))
	{
		npc_explain = rhs_type_value;
	}
	else if (lhs_type_name == DataProvider__StringData(13))
	{
		npc_script_id = std::atoi(rhs_type_value.c_str());;
	}
	else if (lhs_type_name == DataProvider__StringData(14))
	{
		npc_sprite_id = std::atoi(rhs_type_value.c_str());;
	}
	return true;
}

std::string NPCData::GetInitializeFileName() {
	return DataProvider__StringData(0);
}