#include "CField.h"
#include "MonsterFactory.h"
#include "CBackground.h"
#include <algorithm>
#include "SpriteProvider.h"
#include "CItemDataProvider.h"
#include <algorithm>
#include <random>
#include "SoundHandler.h"
#include "CGameStage.h"

float CField::GetFieldGravity() { return field_gravity; }
void CField::SetFieldGravity(float gravity) { field_gravity = gravity; }
const std::vector<CNPC*>& CField::GetAllRegistedNPC() { return npcs; }
std::vector<CField::CFieldItem*>& CField::GetFieldItemList() { return field_items; }
int CField::GetFieldId() { return field_id; }
int CField::GetFieldMusicId() { return field_music_id; }
void CField::SetFieldMusicId(int music_id) { field_music_id = music_id; }

CField::CField() {
	field_music_id = 700010;
}

CField::CFieldItem::CFieldItem(size_t itemid)
{
	static size_t field_item_id = 0;
	this->item = CItemDataProvider::Instance().GetItemByItemId(itemid);
	item_object_id = field_item_id;
	field_item_id++;
}

bool CField::CFieldItem::Render() {
	if (item != nullptr) {
		item->GetSprite()->DrawAlpha(area);
	}
	return true;
}

bool CField::Init()
{
	for (auto& obj : backgrounds) {
		obj->Init();
	}
	for (auto& obj : field_monsters)
	{
		obj->monster->Init();
	}
	for (auto& obj : tiles)
	{
		obj->Init();
	}
	for (auto& obj : npcs)
	{
		obj->Init();
	}
	for (auto& obj : portals) {
		obj->Init();
	}
	return true;
}

bool CField::Frame()
{
	field_timer.Update();
	for (auto background : backgrounds) {
		background->Frame();
	}
	CPlayerCharacter::Instance().Frame();
	for (auto& attack : player_attacks) {
		attack->Frame();
	}
	for (auto& tile : tiles)
		tile->Frame();
	for (auto& field_mob : field_monsters) {
		if (field_mob->is_dead) 
		{
			if ((field_timer - field_mob->time_point_dead) * 1000 >= field_mob->respawn_time) 
			{
				field_mob->is_dead = false;
			}
		}
		else 
		{
			field_mob->monster->Frame();
			if (CPlayerCharacter::Instance().area.is_Collision(field_mob->monster->area, 1.15f)) {
				CPlayerCharacter::Instance().CollisionWithMonster(field_mob->monster);
			}
			for (auto& attack : player_attacks) {
				if (attack->area.is_Collision(field_mob->monster->area)) {
					attack->CollisionWith(field_mob->monster);
				}
			}
		}
	}
	for (auto& npc : npcs)
		npc->Frame();
	for (auto& portal : portals) {
		portal->Frame();
	}

	Position_Y_Collision();
	Position_X_Collision();
	
	if (field_items.size() >= 25) {
		ClearFieldItemList();
	}
	return true;
}

bool CField::Render()
{
	for (auto& back : backgrounds) {
		back->Render();
	}
	for (auto& obj : tiles)
	{
		if (CPlayerCharacter::Instance().area.Is_InScreenDrawRect(obj->area)) {
			obj->ModifyScreenPointFromLogicalToScreen();
			obj->Render();
		}	
	}
	for (auto& field_mob : field_monsters)
	{
		if (field_mob->is_dead == false)
		{
			if (CPlayerCharacter::Instance().area.Is_InScreenDrawRect(field_mob->monster->area)) {
				field_mob->monster->ModifyScreenPointFromLogicalToScreen();
				field_mob->monster->Render();
			}
		}
	}
	for (auto& obj : npcs)
	{
		if (CPlayerCharacter::Instance().area.Is_InScreenDrawRect(obj->area)) {
			obj->ModifyScreenPointFromLogicalToScreen();
			obj->Render();
		}
	}

	CPlayerCharacter::Instance().Render();

	for (auto& item : field_items)
	{
		if (item->item != nullptr) {
			if (CPlayerCharacter::Instance().area.Is_InScreenDrawRect(item->area)) {
				item->area.ModifyScreenPointFromLogicalToScreen();
				item->Render();
			}
		}
	}

	
	for (auto& attack : player_attacks) {
		attack->Render();
	}
	for (auto& portal : portals) {
		portal->ModifyScreenPointFromLogicalToScreen();
		portal->Render();
	}
	return true;
}

bool CField::Release()
{
	for (auto& back : backgrounds) {
		back->Release();
		delete back; back = nullptr;
	}
	for (auto& field_mob : field_monsters)
	{
		field_mob->monster->Release();
		delete field_mob->monster;
		delete field_mob;
	}
	for (auto& obj : tiles)
	{
		obj->Release();
		delete obj;
	}
	for (auto& attack_obj : player_attacks) {
		delete attack_obj;
	}
	for (auto& portal : portals) {
		portal->Release();
		delete portal;
	}
	backgrounds.clear();
	field_monsters.clear();
	tiles.clear();
	player_attacks.clear();
	portals.clear();
	return true;
}

CField::~CField()
{
	Release();
}

bool CField::Position_X_Collision()
{

	for (auto& field_mob : field_monsters) {
		CArea prev = field_mob->monster->GetArea();
		field_mob->monster->Position_X_Frame();
		ObjectCollision_X(field_mob->monster, prev);
	}
	for (auto& npc : npcs) {
		CArea prev = npc->GetArea();
		npc->Position_X_Frame();
		ObjectCollision_X(npc, prev);
	}
	CArea prev = CPlayerCharacter::Instance().GetArea();
	CPlayerCharacter::Instance().Position_X_Frame();
	ObjectCollision_X(&CPlayerCharacter::Instance(), prev);
	return true;
}

bool CField::Position_Y_Collision()
{
	for (auto& field_mob : field_monsters) {
		field_mob->monster->EffectedbyGravity(field_gravity);
		CArea prev = field_mob->monster->GetArea();
		field_mob->monster->Position_Y_Frame();
		ObjectCollision_Y(field_mob->monster, prev);
	}
	for (auto& npc : npcs) {
		npc->EffectedbyGravity(field_gravity);
		CArea prev = npc->GetArea();
		npc->Position_Y_Frame();
		ObjectCollision_Y(npc, prev);
	}
	CPlayerCharacter::Instance().EffectedbyGravity(field_gravity);
	CArea prev = CPlayerCharacter::Instance().GetArea();
	CPlayerCharacter::Instance().Position_Y_Frame();
	if (ObjectCollision_Y(&CPlayerCharacter::Instance(), prev)) {
		CPlayerCharacter::Instance().SetJumpStateCancle();
	}
	return true;
}

bool CField::ObjectCollision_X(CFieldObject* obj, CArea& prev)
{
	bool collision = false;
	std::vector<CTile*> collTiles;
	for (auto& tile : tiles) {
		if (obj->area.Is_InScreenDrawRect(tile->area)) {
			if (obj->area.is_Collision(tile->area)) {
				collision = true;
				collTiles.push_back(tile);
			}
		}
	}
	if (collTiles.size() > 1)
	{
		bool is_same_row = true;
		float screen_y = collTiles[0]->area.screen_point.y;
		for (auto& tile : collTiles) {
			if (screen_y != tile->area.screen_point.y) {
				is_same_row = false;
			}
		}
		if (is_same_row) {
			return false;
		}
	}
	if (collision == true) {
		obj->area.Modify_xPoint(obj->area.logical_point.x > collTiles[0]->area.logical_point.x
			? collTiles[0]->area.logical_point.x + collTiles[0]->area.rt.right
			: collTiles[0]->area.logical_point.x - obj->area.rt.right);
	}
	return true;
}

bool CField::ObjectCollision_Y(CFieldObject* obj, CArea& prev)
{
	bool bottom_collision = false;
	bool collision = false;
	std::vector<CTile*> collTiles;
	for (auto& tile : tiles) {
		if (obj->area.Is_InScreenDrawRect(tile->area)) {
			if (obj->area.is_Collision(tile->area)) {
				collision = true;
				collTiles.push_back(tile);
			}
		}
	}
	if (collTiles.size() > 1)
	{
		bool is_same_column = true;
		float screen_x = collTiles[0]->area.screen_point.x;
		for (auto& tile : collTiles) {
			if (screen_x != tile->area.screen_point.x) {
				is_same_column = false;
			}
		}
		if (is_same_column) {
			return false;
		}
		if (prev.logical_point.y >= obj->area.logical_point.y) {
			std::sort(collTiles.begin(), collTiles.end(), [](CTile* t1, CTile* t2) {
				return t1->GetArea().screen_point.y > t2->GetArea().screen_point.y;
				});
		}
		else
		{
			std::sort(collTiles.begin(), collTiles.end(), [](CTile* t1, CTile* t2) {
				return t1->GetArea().screen_point.y < t2->GetArea().screen_point.y;
				});
		}
	}

	if (collision == true) {
		obj->area.Modify_yPoint(obj->area.logical_point.y > collTiles[0]->area.logical_point.y
			? collTiles[0]->area.logical_point.y + obj->area.rt.bottom
			: collTiles[0]->area.logical_point.y - obj->area.rt.bottom);
	}
	if (collision == true && obj->area.logical_point.y > collTiles[0]->area.logical_point.y)
	{
		bottom_collision = true;
	}
	return bottom_collision;
}

bool CField::PushPlayerAttack(CPlayerAttackObject* attack)
{
	player_attacks.push_back(attack);
	return true;
}

bool CField::RemovePlayerAttack(CPlayerAttackObject* attack)
{
	auto iter = std::find(player_attacks.begin(), player_attacks.end(), attack);
	if (iter == player_attacks.end())
	{
		return false;
	}
	CPlayerAttackObject* removed = *iter;
	delete removed;
	player_attacks.erase(iter);
	return true;
}

std::string& CField::GetFieldName() { return field_name; }
void		 CField::SetFieldName(std::string field_name_) { field_name = field_name_; }
std::string& CField::GetLevelOfDifficulty() { return level_of_difficulty; }
void		 CField::SetLevelOfDifficulty(std::string difficulty) { level_of_difficulty = difficulty; }

bool CField::RegisterFieldBackground(CFieldObject* background_)
{
	backgrounds.push_back(background_);
	return true;
}

bool CField::RegisterRespawnMonster(CMonster* mob, double respawn_time)
{
	mob->SetField(this);
	field_monsters.emplace_back(new CFieldMonster{ mob, respawn_time, false });
	return true;
}

bool CField::DeadMonster(CMonster* mob)
{
	//auto iter = std::find(monsters.begin(), monsters.end(), mob);
	auto iter = std::find_if(field_monsters.begin(), field_monsters.end(),
		[&](CFieldMonster* field_mob) {
			return field_mob->monster == mob;
		});
	if (iter == field_monsters.end())
	{
		return false;
	}
	CFieldMonster* field_mob = *iter;
	SprinkleItemOverTheField(field_mob->monster);
	field_mob->is_dead = true;
	field_mob->time_point_dead.Update();
	field_mob->monster->Reset();
	return true;
}

bool CField::SprinkleItemOverTheField(CMonster* mob)
{
	std::vector<size_t> itemlist(GetSprinkleItemList(mob));
	if (itemlist.empty() == true) {
		return false;
	}
	std::vector< CFieldItem*> monster_items;
	for (size_t itemid : itemlist) {
		monster_items.push_back(new CFieldItem(itemid));
	}
	fPoint monster_pos = mob->GetArea().GetLogicalPos();
	for (size_t i = 1; i <= itemlist.size(); ++i)
	{
		float pos_x = (monster_pos.x + mob->GetArea().rt.right / 2);
		// Â¦¼ö
		if (itemlist.size() % 2 == 0) {
			pos_x += ((int)(i - (itemlist.size() / 2) - 1) * 25);
		}
		else // È¦¼ö
		{
			pos_x += ((int)(i - (itemlist.size() / 2) - 2) * 25);
		}
		monster_items[i - 1]->area.Init(pos_x,
			monster_items[i-1]->item->GetSprite()->GetCurrentFrameImageRect().bottom
			+ monster_pos.y - mob->GetArea().rt.bottom);
	}
	field_items.insert(field_items.end(), monster_items.begin(), monster_items.end());
	return true;
}

void CField::ClearFieldItemList()
{
	for (auto& field_item : field_items) {
		delete field_item->item;
		delete field_item;
	}
	field_items.clear();
}

std::vector<size_t> CField::GetSprinkleItemList(CMonster* mob)
{
	std::vector<size_t> item_list;
	auto& items = mob->GetMonsterDropProbabilityAndItemId();

	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(field_timer.time.time_since_epoch()).count();
	std::mt19937_64 rng(millis);
	std::uniform_int_distribution<__int64> dist(0, 1000000);
	for (auto& pair_itemid_Itemid : items)
	{
		size_t dice = dist(rng);
		if (pair_itemid_Itemid.first >= dice) {
			item_list.push_back(pair_itemid_Itemid.second);
		}
	}
	return std::move(item_list);
}
bool CField::RegisterNPC(CNPC* npc)
{
	npc->SetField(this);
	npcs.push_back(npc);
	return true;
}

bool CField::RegisterTile(CTile* tile)
{
	tile->SetField(this);
	tiles.push_back(tile);
	return true;
}

bool CField::RegisterPortal(CPortal* portal)
{
	portal->SetField(this);
	portals.push_back(portal);
	return false;
}

void CField::PlayerEnterTheMap()
{
	SoundHandler::Instance().Play(field_music_id);
}

void CField::PlayerLeaveTheMap()
{
	SoundHandler::Instance().Stop(field_music_id);
}