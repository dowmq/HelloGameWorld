#pragma once
#include "CUI.h"
#include "CUIImage.h"
#include <array>
#include "CUIHandler.h"
#include "CQuest.h"
class CNPC;

class CUINPCQuestStatus : public CUI
{
public:
	enum Status
	{
		is_nothing = 0,
		is_can_start,
		is_progressing,
		is_can_complete,

		count
	};
	CUINPCQuestStatus(CNPC& npc_, CUI* owner_);
	~CUINPCQuestStatus();

	virtual bool		SetScreenPos(float x, float y) override;
	void				SetNPCQuestUIStatus(CUINPCQuestStatus::Status stat, CQuest& quest);
	
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
	CNPC&				GetNPC();
private:
	CUIImage* CreateNPCQuestUIStatus(CUINPCQuestStatus::Status stat);
	CUIHandler::EventUI* CreateEventUI(CUINPCQuestStatus::Status stat, CUIImage* rend_unit, CQuest& quest);

	CNPC&					npc;
	CUIImage*				rendering_unit;
	CUIHandler::EventUI*	binded_event;
	std::array< CUIImage*, CUINPCQuestStatus::Status::count> npc_upper_quest_status;
};