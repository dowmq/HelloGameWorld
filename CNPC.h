#pragma once
#include "CFieldObject.h"
#include "CUI.h"
#include "CUINPCQuestStatus.h"
#include <string>
#include "NPCData.h"
#include "CQuest.h"
#include "CUINPC.h"

class CNPC : public CFieldObject
{
	CNPC(NPCData::NPCID npcid, float logical_x, float logical_y);
public:
	virtual bool			Init() override;
	virtual bool			Frame() override;
	virtual bool			Position_X_Frame() override;
	virtual bool			Position_Y_Frame() override;
	virtual bool			Render() override;
	virtual bool			Release() override;
	virtual bool			EffectedbyGravity(float gravity) override;
	virtual fPoint&			ModifyScreenPointFromLogicalToScreen() override;
	
	bool					SetNPCUpperQuestUI(CUINPCQuestStatus::Status status, CQuest& quest);
	bool					ResetNPCUpperQuestUI();

	virtual float			GetWidth() override;
	virtual float			GetHeight() override;
	int						GetNPCID();
	int						GetNPCScriptID();
	int						GetNPCSpriteID();
	std::string				GetNPCName();
	CUINPC&					GetNPCUI();
	CUINPCQuestStatus&		GetNPCUpperQuestUI();

	~CNPC();
	static CNPC* CreateNPC(NPCData::NPCID npcid, float logical_x, float logical_y);
private:
	NPCData&					npc_data;
	CUINPC*						npc_ui;
	CUINPCQuestStatus*			npc_upper_quest_ui;
};