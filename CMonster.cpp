#include "CMonster.h"
#include "CPlayerCharacter.h"
#include "CField.h"
#include "CUIChatLog.h"
#include "StringData.h"
#include <time.h>
#include <stdlib.h>
#include "CItemDataProvider.h"
#include "SoundHandler.h"
#include "TimeHandler.h"
int CMonster::GetId() { return monster_id; }

std::once_flag CMonster::call_once_flag;
std::vector<std::pair<size_t, size_t>> CMonster::pair_probability_dropitem;

std::array<float, CMonster::StatType::StatCount>& CMonster::GetMonsterStats() {
	return stats;
}
std::vector<std::pair<size_t, size_t>>& CMonster::GetMonsterDropProbabilityAndItemId() {
	return pair_probability_dropitem;
}

CMonster::CMonster(float logical_x, float logical_y)
{
	std::call_once(CMonster::call_once_flag, []() {
		for (auto& pair_itemid_Citem : CItemDataProvider::Instance().GetAllItems())
		{
			pair_probability_dropitem.emplace_back(80000, pair_itemid_Citem.first);
		}
		});
	area.Init(logical_x, logical_y);
	direction_change_lock = false;
	character_is_on_right = false;
}

CMonster::~CMonster()
{
	if (current) {
		current->Release();
		delete current; current = nullptr;
	}
}

bool CMonster::Init()
{
	frame = &TimeHandler::Instance();
	current = sprites[walking_left];
	trace = false;
	dead = false;
	move_speed = 75.0f;
	stats[StatType::HP] = 100.0f;
	stats[StatType::MP] = 100.0f;
	stats[StatType::defense] = 10.0f;
	stats[StatType::EXP] = 17.0f;
	return true;
}
bool CMonster::Frame()
{
	if (current == sprites[die]) {
		return false;
	}
	if (stats[StatType::HP] <= 0) {
		current = sprites[die];
		dead = true;
		CPlayerCharacter::Instance().KillMonster(this);
		frame->SetTimerFunction(1500, 
			[](CFieldObject* obj) {
				CMonster* mob = reinterpret_cast<CMonster*>(obj);
				mob->GetField().DeadMonster(mob);
			}, this);
	}
	else
	{
		if (CPlayerCharacter::Instance().GetArea().is_in_Range(this->area, 400, 30)) {
			trace = true;
		}
		else
		{
			trace = false;
		}
		
		if (trace)
		{
			if (current == sprites[walking_right])
			{
				this->AddLogicalPointX(50.0f * frame->GetSecondPerFrame());
			}
			else
			{
				this->AddLogicalPointX(-50.0f * frame->GetSecondPerFrame());
			}
			if (direction_change_lock == false) {
				character_is_on_right =
					CPlayerCharacter::Instance().GetArea().GetLogicalPos().x > this->area.GetLogicalPos().x;
				ReserveDirectionChangeUnlock();
			}
			if (character_is_on_right)
			{
				current = sprites[walking_right];
				direction_change_lock = true;
			}
			else
			{
				current = sprites[walking_left];
				direction_change_lock = true;
			}
		}
	}
	current->Frame();
	area.rt = current->GetCurrentFrameImageRect();
	return true;
}

bool CMonster::Position_X_Frame()
{
	return true;
}
bool CMonster::Position_Y_Frame()
{
	return true;
}

bool CMonster::Render()
{
	current->DrawAlpha(area);
	return true;
}

bool CMonster::Release()
{
	if (current) {
		current->Release();
		delete current; current = nullptr;
	}
	return true;
}

bool CMonster::EffectedbyGravity(float gravity)
{
	area.logical_point.y -= gravity * min(0.1, frame->GetSecondPerFrame());
	return true;
}

bool CMonster::GetDamaged(float damaged)
{
	static std::string damage_msg = DataProvider__StringData(33);
	auto defense = stats[StatType::defense];
	srand(time(NULL));
	float d = (damaged - defense) *  ( ( ((float)(rand() % 6)) + 8.0f) / 10.0f) ;
	stats[StatType::HP] -= d;
	SoundHandler::Instance().Play(SoundData::Type::Damaged_goblin);
	if (CUIChatLog::instance_ != nullptr) {
		char buffer[256] = { 0, };
		sprintf_s(buffer, 256, damage_msg.c_str(), static_cast<int>(d));
		std::string str(buffer);
		CUIChatLog::instance_->StoreString(str);
	}
	return true;
}

bool CMonster::Is_dead() { return dead; }

bool CMonster::Reset()
{
	current = sprites[walking_left];
	trace = false;
	dead = false;
	move_speed = 75.0f;
	stats[StatType::HP] = 100.0f;
	stats[StatType::MP] = 100.0f;

	this->SetLogicalPoint(area.GetLogicalInitialPos().x, area.GetLogicalInitialPos().y);
	return true;
}

void CMonster::SetDirectionLock(bool value) { direction_change_lock = value; }

void CMonster::ReserveDirectionChangeUnlock()
{
	TimeHandler::Instance().SetTimerFunction(1000, 
		[](CFieldObject* obj) {
			CMonster* mob = reinterpret_cast<CMonster*>(obj);
			if (mob == nullptr) {
				return;
			}
			mob->SetDirectionLock(false);
		}, this);
}