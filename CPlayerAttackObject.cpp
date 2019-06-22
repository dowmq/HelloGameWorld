#include "CPlayerAttackObject.h"
#include "TimeHandler.h"
#include "CField.h"
#include "SpriteProvider.h"
#include "SoundHandler.h"
CPlayerAttackObject::CPlayerAttackObject(CPlayerCharacter* owner_, double dead_time_, bool is_right_)
	: owner(owner_), dead_time(dead_time_), is_right(is_right_), image(nullptr)
{
	Init();
}


CPlayerAttackObject::~CPlayerAttackObject()
{
	if (image != nullptr) {
		delete image;
	}
}

bool CPlayerAttackObject::Init()
{
	if (is_right)
	{
		image = SpriteProvider::Instance().GetCharacterSpriteBySpriteID(SpriteProvider::CharacterID::attack_effect_right);
	}
	else
	{
		image = SpriteProvider::Instance().GetCharacterSpriteBySpriteID(SpriteProvider::CharacterID::attack_effect_left);
	}
	frame = &TimeHandler::Instance();
	frame->SetTimerFunction(dead_time, 
		[](CFieldObject* this_) {
			CPlayerAttackObject* attack = reinterpret_cast<CPlayerAttackObject*>(this_);
			attack->GetOwner().GetField().RemovePlayerAttack(attack);
		}, this);
	attack_count = 1;
	SoundHandler::Instance().Play(SoundData::Type::Attack);
	return true;
}

bool CPlayerAttackObject::Frame()
{
	image->Frame();
	area.rt = image->GetCurrentFrameImageRect();

	CArea& owner_area = owner->GetArea();
	RECT& rt = owner_area.rt;
	if (is_right)
	{
		area.Modify_xPoint(owner_area.GetLogicalPos().x + rt.right);
	}
	else
	{
		area.Modify_xPoint(owner_area.GetLogicalPos().x - image->GetCurrentFrameImageRect().right);
	}
	area.Modify_yPoint(owner_area.GetLogicalPos().y - (rt.bottom / 2) + (image->GetCurrentFrameImageRect().bottom / 2));
	area.ModifyScreenPointFromLogicalToScreen();
	return true;
}

bool CPlayerAttackObject::Render()
{
	image->DrawAlpha(area);	
	return true;
}

bool CPlayerAttackObject::Release()
{
	if (image != nullptr) {
		delete image; image = nullptr;
	}
	return true;
}

bool CPlayerAttackObject::CollisionWith(CMonster* mob)
{
	if (mob->Is_dead())
		return false;
	
	if (is_right)
	{
		mob->AddLogicalPointX(mob->GetMoveSpeed() * frame->GetSecondPerFrame());
	}
	else
	{
		mob->AddLogicalPointX(-mob->GetMoveSpeed() * frame->GetSecondPerFrame());
	}
	if (attack_count-- <= 0) {
		return false;
	}
	auto player_attack_power = owner->GetStat()[CPlayerCharacter::CharacterStat::attack_power];
	mob->GetDamaged(player_attack_power);
	return true;
}