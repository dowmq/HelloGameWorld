#pragma once
#include "CPlayerCharacter.h"
#include "CPlayerAttackObject.h"
#include "CTile.h"
#include "CMonster.h"
#include "TimeFragment.h"
#include "CNPC.h"
#include <string>
#include <vector>
#include "CItem.h"
#include "CPortal.h"
class CFieldObject;

class CField
{
	friend class CFieldFactory;
public:
	struct CFieldMonster
	{
		CMonster*		monster;
		double			respawn_time;
		bool			is_dead;
		TimeFragment	time_point_dead;
	};
	struct CFieldItem
	{
		size_t			item_object_id;
		CItem*			item;
		CArea			area;
		explicit CFieldItem(size_t itemid);
		bool Render();

	};

	CField();

	virtual ~CField();
	virtual bool					Init();
	virtual bool					Frame();
	virtual bool					Render();
	virtual bool					Release();

	virtual bool					Position_X_Collision();
	virtual bool					Position_Y_Collision();

	virtual bool					ObjectCollision_X(CFieldObject* obj, CArea& prev);
	virtual bool					ObjectCollision_Y(CFieldObject* obj, CArea& prev);

	bool							PushPlayerAttack(CPlayerAttackObject* attack);
	bool							RemovePlayerAttack(CPlayerAttackObject* attack);

	bool							RegisterFieldBackground(CFieldObject* background);
	bool							RegisterRespawnMonster(CMonster* mob, double respawn_time);
	bool							RegisterNPC(CNPC* npc);
	const std::vector<CNPC*>&		GetAllRegistedNPC();
	bool							RegisterTile(CTile* tile);
	bool							RegisterPortal(CPortal* field);

	bool							DeadMonster(CMonster* mob);
	bool							SprinkleItemOverTheField(CMonster* mob);
	std::vector<size_t>				GetSprinkleItemList(CMonster* mob);
	void							ClearFieldItemList();
	std::vector< CFieldItem*>&		GetFieldItemList();

	int								GetFieldId();
	float							GetFieldGravity();
	void							SetFieldGravity(float gravity);
	std::string&					GetFieldName();
	void							SetFieldName(std::string field_name_);
	std::string&					GetLevelOfDifficulty();
	void							SetLevelOfDifficulty(std::string difficulty);
	int								GetFieldMusicId();
	void							SetFieldMusicId(int music_id);

	void							PlayerEnterTheMap();
	void							PlayerLeaveTheMap();
private:
	int									field_id;
	std::string							field_name;
	std::string							level_of_difficulty;
	int									field_music_id;

	TimeFragment						field_timer;

	std::vector<CFieldObject*>			backgrounds;
	std::vector<CTile*>					tiles;
	std::vector<CPlayerAttackObject*>	player_attacks;
	std::vector<CFieldMonster*>			field_monsters;
	std::vector<CFieldItem*>			field_items;
	std::vector<CNPC*>					npcs;
	std::vector<CPortal*>				portals;
	float								field_gravity;
};