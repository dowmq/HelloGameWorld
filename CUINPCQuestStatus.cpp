#include "CUINPCQuestStatus.h"
#include "SpriteProvider.h"
#include "CNPC.h"
#include "InputHandler.h"
#include "CUINPCTalk.h"
#include "NPCTalkScriptData.h"
#include "CPlayerCharacter.h"
#include "SoundHandler.h"

CNPC& CUINPCQuestStatus::GetNPC() { return npc; }

CUINPCQuestStatus::CUINPCQuestStatus(CNPC& npc_, CUI* owner_)
	: CUI(owner_), npc(npc_)
{
	screen_x = 0;
	screen_y = 0;
	for (auto& element : npc_upper_quest_status)
	{
		element = nullptr;
	}
	rendering_unit = nullptr;
}


CUINPCQuestStatus::~CUINPCQuestStatus()
{
	if (binded_event != nullptr) {
		CUIHandler::Instance().DeRegisterEventUI(binded_event->event_id);
	}
	for (auto& element : npc_upper_quest_status)
	{
		if (element != nullptr)
			delete element;
	}
}

void CUINPCQuestStatus::SetNPCQuestUIStatus(CUINPCQuestStatus::Status stat, CQuest& quest)
{
	if (stat == CUINPCQuestStatus::Status::is_nothing) {
		rendering_unit = nullptr;
		return;
	}
	if (npc_upper_quest_status[stat] == nullptr)
	{
		npc_upper_quest_status[stat] = CreateNPCQuestUIStatus(stat);
	}
	rendering_unit = npc_upper_quest_status[stat];
	CreateEventUI(stat, rendering_unit, quest);
}

CUIImage* CUINPCQuestStatus::CreateNPCQuestUIStatus(CUINPCQuestStatus::Status stat)
{
	if (npc_upper_quest_status[stat] != nullptr) {
		return npc_upper_quest_status[stat];
	}
	switch (stat)
	{
		case CUINPCQuestStatus::Status::is_can_start:
			npc_upper_quest_status[stat] =
				new CUIImage(SpriteProvider::UIID::npc_quest_can_start, true, this);
			break;
		case CUINPCQuestStatus::Status::is_progressing:
			npc_upper_quest_status[stat] =
				new CUIImage(SpriteProvider::UIID::npc_quest_proceed, true, this);
			break;
		case CUINPCQuestStatus::Status::is_can_complete:
			npc_upper_quest_status[stat] =
				new CUIImage(SpriteProvider::UIID::npc_quest_can_complete, true, this);
			break;
	}
	return npc_upper_quest_status[stat];
}

CUIHandler::EventUI* CUINPCQuestStatus::CreateEventUI(CUINPCQuestStatus::Status stat, CUIImage* rend_unit, CQuest& quest)
{
	if (binded_event != nullptr)
	{
		CUIHandler::Instance().DeRegisterEventUI(binded_event->event_id);
		binded_event = nullptr;
	}
	int script_id = 0;
	CUINPCTalk::AdditionalOption option = CUINPCTalk::AdditionalOption::None;
	if (stat == CUINPCQuestStatus::Status::is_can_start) {
		script_id = quest.GetQuestStartScriptID();
		option = CUINPCTalk::AdditionalOption::YesNo;
	} else if (stat == CUINPCQuestStatus::Status::is_can_complete) {
		script_id = quest.GetQuestEndScriptID();
		option = CUINPCTalk::AdditionalOption::OK;
	} else {
		return nullptr;
	}

	binded_event = CUIHandler::Instance().RegisterEventUI(this,
		[&quest, script_id, option](CUI* ui) {
			if (InputHandler::Instance().is_now_double_click())
			{
				CUINPCQuestStatus* this_ = reinterpret_cast<CUINPCQuestStatus*>(ui);
				CNPC& npc = this_->GetNPC();
				auto temp = DataProvider__ScriptData(script_id);
				CUINPCTalk* talkwindow = new CUINPCTalk(quest.GetQuestID(),
					std::move(temp),
					DataProvider__StringData(30),
					npc.GetNPCUI().GetNPCImage()->GetSprite()->sprite_id,
					200, 100,
					this_, option);
				if (option == CUINPCTalk::AdditionalOption::YesNo)
				{
					talkwindow->GetButtons()[CUIButton::Type::Button_Yes]->SetInvokeFunction(
						[&quest, &npc](void* obj) {
							CUINPCTalk* talkwindow = (CUINPCTalk*)(obj);
							quest.SetQuestStatus(CQuest::Status::is_prossessing);
							npc.ResetNPCUpperQuestUI();
							CPlayerCharacter::Instance().UpdateFieldNPCUpperQuestUI();
							SoundHandler::Instance().Play(SoundData::Type::BtMouseClick);
							talkwindow->CloseWindow();
						}, talkwindow
					);
					talkwindow->GetButtons()[CUIButton::Type::Button_No]->SetInvokeFunction(
						[](void* obj) {
							CUINPCTalk* talkwindow = reinterpret_cast<CUINPCTalk*>(obj);
							SoundHandler::Instance().Play(SoundData::Type::BtMouseClick);
							talkwindow->CloseWindow();
						}, talkwindow
					);
				}
				else if (option == CUINPCTalk::AdditionalOption::OK)
				{
					talkwindow->GetButtons()[CUIButton::Type::Button_Ok]->SetInvokeFunction(
						[&quest, &npc](void* obj) {
							CUINPCTalk* talkwindow = reinterpret_cast<CUINPCTalk*>(obj);
							SoundHandler::Instance().Play(SoundData::Type::BtMouseClick);
							quest.SetQuestStatus(CQuest::Status::is_completed);
							npc.ResetNPCUpperQuestUI();
							CPlayerCharacter::Instance().UpdateFieldNPCUpperQuestUI();
							talkwindow->CloseWindow();
						}, talkwindow
					);
				}
				CUIHandler::Instance().RegisterUIWindow(quest.GetQuestID(), talkwindow);
			}
		}, 
		rend_unit->GetScreenX(), rend_unit->GetScreenY(), rend_unit->GetWidth(), rend_unit->GetHeight());
	return binded_event;
}

bool CUINPCQuestStatus::SetScreenPos(float x, float y)
{
	if (rendering_unit != nullptr) {
		rendering_unit->SetScreenPos(x + npc.GetNPCUI().GetWidth() / 2 - rendering_unit->GetWidth() / 2,
			y - rendering_unit->GetHeight() - 10);
	}
	if (binded_event != nullptr) {
		binded_event->x = x + npc.GetNPCUI().GetWidth() / 2 - rendering_unit->GetWidth() / 2;
		binded_event->y = y - rendering_unit->GetHeight() - 10;
	}
	return true;
}

bool CUINPCQuestStatus::Init()
{
	return true;
}

bool CUINPCQuestStatus::Frame()
{
	if (rendering_unit != nullptr) {
		rendering_unit->Frame();
	}
	return true;
}

bool CUINPCQuestStatus::Render()
{
	if (rendering_unit != nullptr) {
		rendering_unit->Render();
	}
	return true;
}

bool CUINPCQuestStatus::Release()
{
	if (binded_event != nullptr) {
		CUIHandler::Instance().DeRegisterEventUI(binded_event->event_id);
	}
	for (auto& element : npc_upper_quest_status)
	{
		if (element != nullptr)
			delete element; element = nullptr;
	}
	return true;
}