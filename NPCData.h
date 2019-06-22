#pragma once
#include "Data.h"
#include <string>
#include "StringData.h"
#define DataProvider__NPCData(npc_id) (*DataProvider<NPCData>::Instance().GetData(npc_id))
class NPCData : public Data
{
	friend class CNPC;
	friend class CUINPC;
public:
	enum NPCID
	{
		Codex			= 800001,
		Conjurer		= 800002,
		DungeonMaster	= 800003,
	};
	NPCData() = default;
	~NPCData() = default;

	bool			SetData(const std::string& lhs_type_name, const std::string& rhs_type_value);
	bool			Release();
	int				GetId();

	const std::string&	GetName() const;
	const std::string&	GetExplain() const;
	const int			GetScriptId() const;
	const int			GetSpriteId() const;
	static std::string	GetInitializeFileName();
private:
	int				npc_id;
	std::string		npc_name;
	std::string		npc_explain;
	int				npc_script_id;
	int				npc_sprite_id;
};

