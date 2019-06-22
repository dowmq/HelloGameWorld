#include "CPortal.h"
#include "SpriteProvider.h"
#include "CField.h"
#include "InputHandler.h"
#include "TimeHandler.h"
#include "CGameStage.h"
CPortal::CPortal(int target_field_id_, float logical_x, float logical_y)
{
	target_field_id = target_field_id_;
	new_logical_x = logical_x;
	new_logical_y = logical_y;
	area.Init(logical_x, logical_y);
	portal_image = SpriteProvider::Instance().GetSprite(SpriteProvider::ETCID::portal);
}

CPortal::CPortal(int target_field_id_, float logical_x, float logical_y,
	float _new_logical_x, float _new_logical_y)
{
	target_field_id = target_field_id_;
	new_logical_x = logical_x;
	new_logical_y = logical_y;
	area.Init(logical_x, logical_y);
	portal_image = SpriteProvider::Instance().GetSprite(SpriteProvider::ETCID::portal);
	SetNewFieldCharacterPosition(_new_logical_x, _new_logical_y);
}


CPortal::~CPortal()
{
	if (portal_image) {
		portal_image->Release();
		delete portal_image; portal_image = nullptr;
	}
}

bool CPortal::Init()
{
	return true;
}

bool CPortal::Frame()
{
	if (portal_image)
		portal_image->Frame();
	CheckPlayerStatus(CPlayerCharacter::Instance());
	return true;
}

bool CPortal::Render()
{
	if (portal_image)
		portal_image->DrawAlpha(area);
	return true;
}

bool CPortal::Release()
{
	if (portal_image) {
		portal_image->Release();
		delete portal_image; portal_image = nullptr;
	}
	return true;
}

bool CPortal::SetNewFieldCharacterPosition(float _new_logical_x, float _new_logical_y)
{
	new_logical_x = _new_logical_x;
	new_logical_y = _new_logical_y;
	return true;
}

bool CPortal::CheckPlayerStatus(CPlayerCharacter& player)
{
	if (InputHandler::Instance().GetActionKey().GetActionKeyState(ActionMap::KeySet::w_key)
		!= key_state::KEY_PUSH || CanMove(player) == false)
	{
		return false;
	}
	if (player.GetArea().is_in_Range(area, 100, 0) == true) {
		ReservePlayerChangeField();
	}
	return true;
}

bool CPortal::CanMove(CPlayerCharacter& player) {
	return true;
}

void CPortal::ReservePlayerChangeField() {
	TimeHandler::Instance().SetTimerFunction(0,
		[=](CFieldObject* obj) {
			CGameStage::Instance().PlayerChangeField(target_field_id, new_logical_x, new_logical_y);
		}, nullptr);
}