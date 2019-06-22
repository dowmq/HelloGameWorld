#include "CQuest.h"
#include "CPlayerCharacter.h"
#include "StringData.h"
#include "NPCData.h"
#include "CGameStage.h"
#include "CItem.h"
#include "CItemDataProvider.h"
#include "CUIChatLog.h"
#include "StringData.h"
#include "SoundHandler.h"

const int CQuest::GetQuestID()  const { return quest_data.quest_id; }
const std::string& CQuest::GetQuestName()  const { return quest_data.quest_name; }
QuestData::Type CQuest::GetQuestType()  const { return quest_data.quest_type; }
std::vector<int>* CQuest::GetQuestAdditionalCondition()  const { return quest_data.additional_condition; }
int CQuest::GetQuestStartConditionQuestID()  const { return quest_data.start_condition_qeust_id; }
int CQuest::GetQuestStartNPCID()  const { return quest_data.start_npc_id; }
int CQuest::GetQuestEndNPCID()  const { return quest_data.end_npc_id; }
int CQuest::GetQuestStartScriptID()  const { return quest_data.start_npc_script_id; }
int CQuest::GetQuestEndScriptID()  const { return quest_data.end_npc_script_id; }
int CQuest::GetQuestEXPReward()  const { return quest_data.clear_exp_reward; }
int CQuest::GetQuestItemReward()  const { return quest_data.clear_item_reward; }

CQuest::CQuest(int qid, CPlayerCharacter& player_)
	: quest_data(DataProvider__QuestData(qid))
	, quest_status(CQuest::Status::is_not_started)
	, player(player_)
{
}

CQuest::CQuest(QuestData& quest_data_, CPlayerCharacter& player_)
	: quest_data(quest_data_)
	, quest_status(CQuest::Status::is_not_started)
	, player(player_)
{
}

CQuest::~CQuest()
{
}

CQuest::Status CQuest::GetQuestStatus()
{
	return quest_status;
}

bool CQuest::PlayerCanProgressQuest(CPlayerCharacter& player)
{
	if (player.GetQuest(GetQuestStartConditionQuestID()).value().GetQuestStatus() == CQuest::Status::is_completed)
	{
		return true;
	}
	return false;
}

const std::string& CQuest::GetQuestStatusMessage() const {
	return quest_status_message;
}

void CQuest::SetQuestStatusMessage(const std::string& message) {
	quest_status_message = message;
}
void CQuest::SetQuestStatusMessage(std::string&& message) {
	quest_status_message = message;
}

void CQuest::SetQuestStatus(Status q_status)
{
	quest_status = q_status;
	QuestStatusUpdate(quest_status);
}

void CQuest::QuestStatusUpdate(Status q_status)
{
	if (QuestData::Type::CONVERSATION == this->quest_data.quest_type) {
		if (q_status == Status::is_prossessing) {
			std::string msg = DataProvider__StringData(38);
			auto npc = DataProvider__NPCData(quest_data.end_npc_id);
			char buffer[128];
			sprintf_s(buffer, 128, msg.c_str(), npc.GetName().c_str());
			SetQuestStatusMessage(std::move(std::string(buffer)));
			CGameStage::Instance().GetQuestInfoUI().UpdateQuestUI(*this);
			return;
		}
	}
	else if (QuestData::Type::HUNT == this->quest_data.quest_type) {
		if (q_status == Status::is_prossessing) {
			std::string msg = DataProvider__StringData(39);
			char buffer[128];
			sprintf_s(buffer, 128, msg.c_str(), (*quest_data.additional_condition)[1]);
			SetQuestStatusMessage(std::move(std::string(buffer)));
			CGameStage::Instance().GetQuestInfoUI().UpdateQuestUI(*this);
			return;
		}
	}
	switch (q_status) {
		case Status::is_can_completed: {
			std::string msg = DataProvider__StringData(40);
			auto npc = DataProvider__NPCData(quest_data.end_npc_id);
			char buffer[128];
			sprintf_s(buffer, 128, msg.c_str(), npc.GetName().c_str());
			SetQuestStatusMessage(std::move(std::string(buffer)));
			CGameStage::Instance().GetQuestInfoUI().UpdateQuestUI(*this);
			break;
		}
		case Status::is_completed: {
			std::string msg = DataProvider__StringData(41);
			SetQuestStatusMessage(msg);
			CGameStage::Instance().GetQuestInfoUI().ReleaseCurrentStatusUI();
			PlayerQuestCompleteGetReward();
			SoundHandler::Instance().Play(SoundData::Type::QuestClear);
			break;
		}
	}
}

bool CQuest::PlayerQuestCompleteGetReward()
{
	auto& player = CPlayerCharacter::Instance();
	CItem* gain_item = CItemDataProvider::Instance().GetItemByItemId(quest_data.clear_item_reward);
	player.GainItem(gain_item);
	player.GainExpFunction(quest_data.clear_exp_reward);
	char buffer[128] = { 0, };
	sprintf_s(buffer, 128, DataProvider__StringData(44).c_str(), gain_item->GetItemName().c_str());
	std::string str(buffer);
	CUIChatLog::instance_->StoreString(str);
	return true;
}