#include "CPlayerCharacter.h"
#include "TimeHandler.h"
#include "TimeFragment.h"
#include "CUIChatLog.h"
#include <cmath>
#include <utility>
#include "CItemDataProvider.h"
#include "CUIInventory.h"
#include "SpriteFactory.h"
#include "CField.h"
#include "CItem.h"
#include "CGameStage.h"
#include "CUIGameStage.h"
#include "QuestData.h"
#include "CMonster.h"
#include "SoundHandler.h"
#include "CUINPCTalk.h"
#include "NPCTalkScriptData.h"

CPlayerCharacter::CPlayerCharacter() : decorate_effect(*this)
{
	//area.Init(0, 50);
	area.SetLogicalPos(0, 50);
	area.SetScreenPos(400, 300);

	#define SpriteProvider__GetSprite SpriteProvider::Instance().GetCharacterSpriteBySpriteID
	this->sprites[CPlayerCharacter::CharacterSprite::walking_left] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::walk_left);
	this->sprites[CPlayerCharacter::CharacterSprite::walking_right] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::walk_right);
	this->sprites[CPlayerCharacter::CharacterSprite::running_left] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::run_left);
	this->sprites[CPlayerCharacter::CharacterSprite::running_right] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::run_right);
	this->sprites[CPlayerCharacter::CharacterSprite::attack_left] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::attack_left);
	this->sprites[CPlayerCharacter::CharacterSprite::attack_right] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::attack_right);
	this->sprites[CPlayerCharacter::CharacterSprite::jump_left] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::jump_left);
	this->sprites[CPlayerCharacter::CharacterSprite::jump_right] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::jump_right);
	this->sprites[CPlayerCharacter::CharacterSprite::stop_left] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::stop_left);
	this->sprites[CPlayerCharacter::CharacterSprite::stop_right] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::stop_right);
	this->sprites[CPlayerCharacter::CharacterSprite::damaged_left] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::damaged_left);
	this->sprites[CPlayerCharacter::CharacterSprite::damaged_right] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::damaged_right);
	this->sprites[CPlayerCharacter::CharacterSprite::die_left] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::die_left);
	this->sprites[CPlayerCharacter::CharacterSprite::die_right] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::die_right);
	this->sprites[CPlayerCharacter::CharacterSprite::fall_left] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::fall_left);
	this->sprites[CPlayerCharacter::CharacterSprite::fall_right] =
		SpriteProvider__GetSprite(SpriteProvider::CharacterID::fall_right);
	move_speed = 130.0f;
	stats[LV] = 100;
	stats[HP] = 100.0f;
	stats[MP] = 100.0f;
	stats[attack_power] = 10.0f;
	stats[defense_power] = 10.0f;
	stats[CharacterStat::speed] = 0.0f;
	stats[EXP] = 80.0f;

	auto& qdata = DataProvider<QuestData>::Instance().GetAllData();
	for (auto& element : qdata)
	{
		quest_list.emplace_back(new CQuest(*element.second, *this));
	}
}

CPlayerCharacter::~CPlayerCharacter()
{
	for (auto& quest : quest_list)
	{
		delete quest;
	}
}

bool CPlayerCharacter::GetMotionState(uint64_t& flag, MotionState MotionState) {
	return (flag >> MotionState) & 0x01;
}

void CPlayerCharacter::SetMotionState(uint64_t& flag, MotionState MotionState, bool value) {
	value ? flag |= 1i64 << MotionState : flag &= ~(1i64 << MotionState);
}

bool CPlayerCharacter::Init()
{
	current = sprites[walking_right];
	input = &InputHandler::Instance();
	frame = &TimeHandler::Instance();
	CGameStage::Instance().GetGameUI().UpdateCharacterStatUI(LV, stats[LV]);
	CGameStage::Instance().GetGameUI().UpdateCharacterStatUI(attack_power, stats[attack_power]);
	CGameStage::Instance().GetGameUI().UpdateCharacterStatUI(defense_power, stats[defense_power]);
	CGameStage::Instance().GetGameUI().UpdateCharacterStatUI(speed, stats[speed]);

	if (CUIInventory::inventory != nullptr) {
		CItem* item = CItemDataProvider::Instance().GetItemByItemId(5);
		CUIInventory::inventory->StoreItem(item);
		item = CItemDataProvider::Instance().GetItemByItemId(105);
		CUIInventory::inventory->StoreItem(item);
		CItem* item1 = CItemDataProvider::Instance().GetItemByItemId(207);
		CUIInventory::inventory->StoreItem(item1);
		item1 = CItemDataProvider::Instance().GetItemByItemId(308);
		CUIInventory::inventory->StoreItem(item1);
		item1 = CItemDataProvider::Instance().GetItemByItemId(403);
		CUIInventory::inventory->StoreItem(item1);
		item1 = CItemDataProvider::Instance().GetItemByItemId(504);
		CUIInventory::inventory->StoreItem(item1);
	}
	decorate_effect.Init();
	return true;
}

bool CPlayerCharacter::EffectedbyGravity(float gravity)
{
	if (current == sprites[jump_right] || current == sprites[jump_left]) {
		return false;
	}
	area.Modify_yPoint(area.logical_point.y - gravity * min(0.1, frame->GetSecondPerFrame()));
	return true;
}
const double pi = std::acos(-1);
bool CPlayerCharacter::Frame()
{
	auto& action_map = input->GetActionKey();
	new_motion_flag = previous_motion_flag;

	if (GetMotionState(previous_motion_flag, MotionState::Motion_LOCK) == true) {
		return false;
	}
	
	DirectionHandle(action_map);
	RunningHandle(action_map);
	JumpHandle(action_map);
	AttackHandle(action_map);
	ProcessFieldItemPickUp(action_map);

	SetMotionToSprite();

	previous_motion_flag = new_motion_flag;

	current->Frame();
	area.rt = current->GetCurrentFrameImageRect();

	decorate_effect.Frame();
	return true;
}

bool CPlayerCharacter::Position_X_Frame()
{
	if (GetMotionState(previous_motion_flag, MotionState::Motion_LOCK) == true) {
		return false;
	}
	SetSpriteMotionToMovePositionX();
	return true;
}

bool CPlayerCharacter::Position_Y_Frame()
{
	if (GetMotionState(previous_motion_flag, MotionState::Motion_LOCK) == true) {
		return false;
	}
	SetSpriteMotionToMovePositionY();
	return true;
}

bool CPlayerCharacter::Render()
{
	current->DrawAlpha(area);
	decorate_effect.Render();
	return true;
}

bool CPlayerCharacter::Release()
{
	current->Release();
	for (auto& quest : quest_list)
	{
		delete quest; quest = nullptr;
	}
	quest_list.clear();
	decorate_effect.Release();
	return true;
}

void CPlayerCharacter::SetMotion(Sprite* new_motion)
{
	if (current != new_motion) {
		Sprite* temp = current;
		current = new_motion;
		temp->Reset();
	}
}

void CPlayerCharacter::SetMotionLock()
{
	SetMotionState(previous_motion_flag, MotionState::Motion_LOCK, true);
}

void CPlayerCharacter::SetMotionUnLock()
{
	SetMotionState(previous_motion_flag, MotionState::Motion_LOCK, false);
}

void CPlayerCharacter::AttackStateCancle()
{
	if (GetMotionState(previous_motion_flag, MotionState::attack) == true) {
		SetMotionState(previous_motion_flag, MotionState::attack, false);
	}
}

void CPlayerCharacter::SetJumpStateCancle()
{
	// if already cancled...
	if (GetMotionState(previous_motion_flag, MotionState::jump) == true) {
		SetMotionState(previous_motion_flag, MotionState::jump, false);
	}
	if (GetMotionState(previous_motion_flag, MotionState::fall) == true) {
		SetMotionState(previous_motion_flag, MotionState::fall, false);
	}
}

void CPlayerCharacter::SetFallCancle()
{
	if (GetMotionState(previous_motion_flag, MotionState::fall) == true) {
		SetMotionState(previous_motion_flag, MotionState::fall, false);
	}
}

void CPlayerCharacter::SetDeadStateCancle()
{
	if (GetMotionState(previous_motion_flag, MotionState::die) == true) {
		SetMotionState(previous_motion_flag, MotionState::die, false);
	}
}

void CPlayerCharacter::SetJumpStateToFall()
{
	if (0.5 > (jump_elapse_time_point - jump_time_point))
		return;

	if (GetMotionState(previous_motion_flag, MotionState::jump) == true) {
		SetMotionState(previous_motion_flag, MotionState::fall, true);
	}
}

void CPlayerCharacter::SetMotionLockCancle()
{
	if (GetMotionState(previous_motion_flag, MotionState::Motion_LOCK) == true) {
		SetMotionState(previous_motion_flag, MotionState::Motion_LOCK, false);
	}
}

void CPlayerCharacter::SetDamagedMotionCancle()
{
	if (GetMotionState(previous_motion_flag, MotionState::damaged) == true) {
		SetMotionState(previous_motion_flag, MotionState::damaged, false);
	}
}

bool CPlayerCharacter::is_state_that_motion_cannot_change()
{
	if (GetMotionState(previous_motion_flag, MotionState::fall) ||
		GetMotionState(previous_motion_flag, MotionState::attack) ||
		GetMotionState(previous_motion_flag, MotionState::jump) //||
		//GetMotionState(previous_motion_flag, MotionState::damaged)
		)
	{
		return true;
	}
	return false;
}

bool CPlayerCharacter::is_cannot_set_direction_state()
{
	if (GetMotionState(previous_motion_flag, MotionState::attack) //||
		//GetMotionState(previous_motion_flag, MotionState::damaged)
		)
	{
		return true;
	}
	return false;
}

bool CPlayerCharacter::DirectionHandle(ActionMap& action_map)
{
	if (is_cannot_set_direction_state()) {
		return false;
	}
	if (action_map.GetActionKeyState(ActionMap::KeySet::a_key) == KEY_HOLD) {
		SetMotionState(new_motion_flag, MotionState::walk, true);
		SetMotionState(new_motion_flag, MotionState::is_right, false);
		SetMotionState(new_motion_flag, MotionState::stop, false);
	} else if (action_map.GetActionKeyState(ActionMap::KeySet::d_key) == KEY_HOLD) {
		SetMotionState(new_motion_flag, MotionState::walk, true);
		SetMotionState(new_motion_flag, MotionState::is_right, true);
		SetMotionState(new_motion_flag, MotionState::stop, false);
	} else {
		SetMotionState(new_motion_flag, MotionState::walk, false);
		SetMotionState(new_motion_flag, MotionState::running, false);
		SetMotionState(new_motion_flag, MotionState::stop, true);
	}
	return true;
}

bool CPlayerCharacter::RunningHandle(ActionMap& action_map)
{
	if (is_state_that_motion_cannot_change()) {
		return false;
	}
	if (GetMotionState(new_motion_flag, MotionState::walk)
		&& GetMotionState(previous_motion_flag, MotionState::is_right) 
		== GetMotionState(new_motion_flag, MotionState::is_right))
	{
		double d_time = action_map.GetCurrentAndLastKeyUpTimeGap(ActionMap::KeySet::d_key);
		double a_time = action_map.GetCurrentAndLastKeyUpTimeGap(ActionMap::KeySet::a_key);
		//TCHAR debugTest[256] = { 0, };
		//_stprintf_s(debugTest, TEXT("dtime, atime : %f , %f"), d_time, a_time);
		//TextOut(CWindow::Instance().GetScreenDC(), 0, 200, debugTest, _tcslen(debugTest));
		if ( (GetMotionState(new_motion_flag, MotionState::is_right) == true && d_time < 300)
			||  (GetMotionState(new_motion_flag, MotionState::is_right) == false && a_time < 300) )
		{
			SetMotionState(new_motion_flag, MotionState::running, true);
		}
	}
	return true;
}

bool CPlayerCharacter::JumpHandle(ActionMap& action_map)
{
	if (is_state_that_motion_cannot_change()) {
		return false;
	}
	if (action_map.GetActionKeyState(ActionMap::KeySet::space_key) == KEY_HOLD
		&& GetMotionState(previous_motion_flag, MotionState::attack) == false) 
	{
		SetMotionState(new_motion_flag, MotionState::jump, true);
		SoundHandler::Instance().Play(SoundData::Type::Jump);
		frame->SetTimerFunction(1000,
			[=](CFieldObject* this_) {
				CPlayerCharacter* player = reinterpret_cast<CPlayerCharacter*>(this_);
				player->SetJumpStateToFall();
			}, this);
		jump_time_point.Update();
	}
	return true;
}

bool CPlayerCharacter::AttackHandle(ActionMap& action_map)
{
	//if (is_state_that_motion_cannot_change()) {
	//	return false;
	//}
	if (action_map.GetActionKeyState(ActionMap::KeySet::escape_key) == KEY_HOLD
		&& GetMotionState(previous_motion_flag, MotionState::attack) == false
		&& GetMotionState(previous_motion_flag, MotionState::jump) == false) 
	{
		SetMotionState(new_motion_flag, MotionState::attack, true);
		SetMotionState(new_motion_flag, MotionState::walk, false);
		SetMotionState(new_motion_flag, MotionState::running, false);
		this->field->PushPlayerAttack(new CPlayerAttackObject(this, 600, 
			GetMotionState(new_motion_flag, MotionState::is_right)));
		frame->SetTimerFunction(600,
			[](CFieldObject* this_) {
				CPlayerCharacter* player = reinterpret_cast<CPlayerCharacter*>(this_);
				player->AttackStateCancle();
			}, this);
	}
	return true;
}

bool CPlayerCharacter::SetMotionToSprite()
{
	bool is_right = GetMotionState(new_motion_flag, MotionState::is_right);
	bool is_stop = GetMotionState(new_motion_flag, MotionState::stop);
	bool is_walk = GetMotionState(new_motion_flag, MotionState::walk);
	bool is_run = GetMotionState(new_motion_flag, MotionState::running);
	bool isjump = GetMotionState(new_motion_flag, MotionState::jump);
	bool is_attack = GetMotionState(new_motion_flag, MotionState::attack);
	bool is_damaged = GetMotionState(new_motion_flag, MotionState::damaged);
	bool is_die = GetMotionState(new_motion_flag, MotionState::die);
	bool is_fall = GetMotionState(new_motion_flag, MotionState::fall);

	if (is_fall == true) {
		SetMotion(sprites[is_right ? fall_right : fall_left]);
		return true;
	}

	if (isjump == true)
	{
		SetMotion(sprites[is_right ? jump_right : jump_left]);
		return true;
	}

	if (is_run) {
		SetMotion(sprites[is_right ? running_right : running_left]);
		return true;
	}

	if (is_attack == true) {
		SetMotion(sprites[is_right ? attack_right : attack_left]);
		return true;
	}

	if (is_walk == true) {
		SetMotion(sprites[is_right ? walking_right : walking_left]);
		return true;
	}
	SetMotion(sprites[is_right ? stop_right : stop_left]);
	return true;
}

bool CPlayerCharacter::SetSpriteMotionToMovePositionX()
{
	bool is_right = GetMotionState(new_motion_flag, MotionState::is_right);
	if (GetMotionState(new_motion_flag, MotionState::walk) == true)
	{
		double running_bonus = 0.0;
		if (GetMotionState(new_motion_flag, MotionState::running) == true) {
			running_bonus = 35.0f;
		}
		area.Modify_xPoint(is_right ?
			area.logical_point.x + (stats[CharacterStat::speed] + running_bonus + move_speed) * frame->GetSecondPerFrame()
			: area.logical_point.x - (stats[CharacterStat::speed] + running_bonus + move_speed) * frame->GetSecondPerFrame());
	}
	return true;
}

bool CPlayerCharacter::SetSpriteMotionToMovePositionY()
{
	if (current == sprites[jump_right] || current == sprites[jump_left])
	{
		double running_bonus = 0.0;
		if (GetMotionState(new_motion_flag, MotionState::running)) {
			running_bonus = 50.0;
		}
		jump_elapse_time_point.Update();
		double elapse_time = (jump_elapse_time_point - jump_time_point) * pi;
		area.Modify_yPoint(area.logical_point.y + (running_bonus + 325.0f) * frame->GetSecondPerFrame() * cos(elapse_time) 
			//+ (running_bonus + 325.0f) * frame->GetSecondPerFrame()
		);
	}
	return true;
}

bool CPlayerCharacter::CollisionWithMonster(CMonster* mob)
{
	if (mob->Is_dead()) {
		return false;
	}
	if (GetMotionState(previous_motion_flag, MotionState::damaged) ||
		GetMotionState(previous_motion_flag, MotionState::die) ||
		GetMotionState(previous_motion_flag, MotionState::Motion_LOCK))
	{
		return false;
	}
	bool is_right = mob->GetArea().GetScreenPos().x >= area.GetScreenPos().x;
	float damage = 20.0f;// stat[CharacterStat::defense_power];
	SetMotionState(previous_motion_flag, MotionState::Motion_LOCK, true);

	// 몬스터와 부딪혔을 때 데미지
	stats[CharacterStat::HP] -= damage;
	if (stats[CharacterStat::HP] > 0) 
	{
		SetMotion(sprites[is_right ? damaged_right : damaged_left]);
		SetMotionState(previous_motion_flag, MotionState::damaged, true);
		// 0.7초간 움직임 방지
		frame->SetTimerFunction(700,
			[](CFieldObject* this_) {
				CPlayerCharacter* player = reinterpret_cast<CPlayerCharacter*>(this_);
				player->SetMotionLockCancle();
				player->SetMotionState(player->previous_motion_flag, CPlayerCharacter::MotionState::stop, true);
			}, this);
		// 3초간 데미지를 받지 않게
		frame->SetTimerFunction(3000,
			[](CFieldObject* this_) {
				CPlayerCharacter* player = reinterpret_cast<CPlayerCharacter*>(this_);
				player->SetDamagedMotionCancle();
			}, this);
	}
	else 
	{
		SetMotion(sprites[is_right ? die_right : die_left]);
		SetMotionState(previous_motion_flag, MotionState::die, true);
		PlayerDeadFunction();

	}
	return true;
}

bool CPlayerCharacter::KillMonster(CMonster* mob)
{
	ProcessKillMonsterQuest(mob);
	GainExpFunction(mob->GetMonsterStats()[CMonster::StatType::EXP]);
	return true;
}

bool CPlayerCharacter::GainExpFunction(float exp)
{
	stats[CharacterStat::EXP] += exp;
	if (CUIChatLog::instance_ != nullptr) {
		static std::string gain_exp_msg = DataProvider__StringData(34);
		char buffer[256] = { 0, };
		sprintf_s(buffer, 256, gain_exp_msg.c_str(), static_cast<int>(exp));
		std::string str(buffer);
		CUIChatLog::instance_->StoreString(str);
	}
	if (stats[CharacterStat::EXP] >= 100)
	{
		size_t number_of_level_up = stats[CharacterStat::EXP] / 100;
		stats[CharacterStat::EXP] -= 100.0 * number_of_level_up;
		for (size_t i = 0; i < number_of_level_up; ++i)
		{
			ProcessLevelUp();
		}
	}
	return true;
}

bool CPlayerCharacter::ProcessLevelUp()
{
	AddCharacterStat(CharacterStat::LV, 1);
	SetCharacterStat(CharacterStat::HP, 100.0f);
	decorate_effect.AddEffect(35, 5, 0, -30, 1500, SpriteProvider::ETCID::level_up);
	if (CUIChatLog::instance_ != nullptr) {
		static std::string level_up_msg = DataProvider__StringData(35);
		CUIChatLog::instance_->StoreString(level_up_msg);
	}
	return true;
}

bool CPlayerCharacter::ProcessKillMonsterQuest(CMonster* mob)
{
	auto iter = std::find_if(quest_list.begin(), quest_list.end(),
		[&mob](CQuest* quest) {
			if (quest->GetQuestStatus() == CQuest::Status::is_prossessing
				&& quest->GetQuestType() == QuestData::Type::HUNT)
			{
				if ((*quest->GetQuestAdditionalCondition())[0] == mob->GetId()) {
					return true;
				}
			}
			return false;
		});
	if (iter != quest_list.end())
	{
		auto number_of_remain_monster = --(*(*iter)->GetQuestAdditionalCondition())[1];
		if (number_of_remain_monster <= 0) {
			(*iter)->SetQuestStatus(CQuest::Status::is_can_completed);
			UpdateFieldNPCUpperQuestUI();
		}
		else {
			(*iter)->SetQuestStatus(CQuest::Status::is_prossessing);
		}
	}
	return true;
}

bool CPlayerCharacter::GainItem(CItem* item)
{
	if (CUIInventory::inventory != nullptr) {
		return CUIInventory::inventory->StoreItem(item);
	}
	return false;
}

bool CPlayerCharacter::ProcessFieldItemPickUp(ActionMap& action_map)
{
	if (action_map.GetActionKeyState(ActionMap::KeySet::z_key) != KEY_HOLD) {
		return false;
	}
	auto& field_items = field->GetFieldItemList();
	if (field_items.empty() == true) {
		return false;
	}

	for (auto iter = field_items.begin(); iter != field_items.end();)
	{
		if (area.is_in_Range((*iter)->area, 5, 0) == true) {
			if (GainItem((*iter)->item) == true)
			{
				(*iter)->item = nullptr;
				iter = field_items.erase(iter);
				SoundHandler::Instance().Play(SoundData::Type::PickUpItem);
				continue;
			}
		}
		++iter;
	}
	return true;
}

// enum ItemOption { lvConstrain, grade, incAtt, incDfn, incSpeed, option_count};
bool CPlayerCharacter::SetEquipItemAbility(CItem* item)
{
	AddCharacterStat(CharacterStat::attack_power, item->GetItemOption(CItem::ItemOption::incAtt));
	AddCharacterStat(CharacterStat::defense_power, item->GetItemOption(CItem::ItemOption::incDfn));
	AddCharacterStat(CharacterStat::speed, item->GetItemOption(CItem::ItemOption::incSpeed));
	return true;
}

bool CPlayerCharacter::SetUnEquipItemAbility(CItem* item)
{
	AddCharacterStat(CharacterStat::attack_power, -item->GetItemOption(CItem::ItemOption::incAtt));
	AddCharacterStat(CharacterStat::defense_power, -item->GetItemOption(CItem::ItemOption::incDfn));
	AddCharacterStat(CharacterStat::speed, -item->GetItemOption(CItem::ItemOption::incSpeed));
	return true;
}

bool CPlayerCharacter::SetCharacterStat(CharacterStat stat, float value)
{
	stats[stat] = value;
	return true;
}

bool CPlayerCharacter::AddCharacterStat(CharacterStat stat, float value)
{
	SetCharacterStat(stat, stats[stat] + value);
	if (value != 0)
	{
		CGameStage::Instance().GetGameUI().UpdateCharacterStatUI(stat, stats[stat]);
	}
	return true;
}

std::vector<CQuest*>& CPlayerCharacter::GetQuestList()
{
	return quest_list;
}


std::optional<CQuest> CPlayerCharacter::GetQuest(int qid)
{
	auto iter = std::find_if(quest_list.begin(), quest_list.end(),
		[=](CQuest* quest) {
			return quest->GetQuestID() == qid;
		});
	if (iter != quest_list.end())
	{
		return **iter; // ** zz
	}
	return std::nullopt;
}

bool CPlayerCharacter::UpdateFieldNPCUpperQuestUI()
{
	auto npcs = field->GetAllRegistedNPC();
	std::vector<std::pair<CNPC*, CQuest*>> can_start;
	std::vector<std::pair<CNPC*, CQuest*>> proceed;
	std::vector<std::pair<CNPC*, CQuest*>> can_complete;
	for (auto& npc : npcs)
	{
		for (auto& quest : quest_list)
		{
			if (quest->GetQuestStatus() == CQuest::Status::is_completed) {
				continue;
			}
			if (quest->GetQuestStartNPCID() == npc->GetNPCID()) {
				if (quest->GetQuestStatus() == CQuest::Status::is_not_started) {
					if (quest->GetQuestStartConditionQuestID() == 0
						|| GetQuest(quest->GetQuestStartConditionQuestID()).value().GetQuestStatus() == CQuest::Status::is_completed)
					{
						can_start.push_back(std::make_pair(npc, quest));
					}
				}
			}
			else if (quest->GetQuestEndNPCID() == npc->GetNPCID())
			{
				if (quest->GetQuestStatus() == CQuest::Status::is_can_completed || 
					(quest->GetQuestStatus() == CQuest::Status::is_prossessing && 
						quest->GetQuestType() == QuestData::Type::CONVERSATION)) {
					can_complete.push_back(std::make_pair(npc, quest));
				}
				else if (quest->GetQuestStatus() == CQuest::Status::is_prossessing) {
					proceed.push_back(std::make_pair(npc, quest));
				}
			}
		}
	}
	if (proceed.empty() == false) {
		for (auto& pair_npc_quest : proceed) {
			pair_npc_quest.first->SetNPCUpperQuestUI(CUINPCQuestStatus::Status::is_progressing, *pair_npc_quest.second);
		}
	}
	if (can_start.empty() == false) {
		for (auto& pair_npc_quest : can_start) {
			pair_npc_quest.first->SetNPCUpperQuestUI(CUINPCQuestStatus::Status::is_can_start, *pair_npc_quest.second);
		}
	}
	if (can_complete.empty() == false) {
		for (auto& pair_npc_quest : can_complete) {
			pair_npc_quest.first->SetNPCUpperQuestUI(CUINPCQuestStatus::Status::is_can_complete, *pair_npc_quest.second);
		}
	}
	return true;
}

bool CPlayerCharacter::PlayerDeadFunction()
{
	CUIHandler::Instance().CreateNoticeUIWindow(300, 100, 
		DataProvider__StringData(60), NPCTalkScriptData::ScriptID::Notice_die,
		[](void* obj) {
			TimeHandler::Instance().SetTimerFunction(0,
				[](CFieldObject* obj) {
					CGameStage::Instance().PlayerChangeField(0, 0, 50);
					CPlayerCharacter::Instance().SetMotionUnLock();
					CPlayerCharacter::Instance().SetCharacterStat(CPlayerCharacter::CharacterStat::HP, 100.0);
					CPlayerCharacter::Instance().SetDeadStateCancle();
				}, nullptr);
		}, this);
	return true;
}