#pragma once
#include "TSingleton.h"
#include "Sprite.h"
#include <unordered_map>
#include "StringData.h"
class SpriteProvider : public TSingleton< SpriteProvider>
{
	friend class TSingleton< SpriteProvider>;
	SpriteProvider();
public:
	enum UIID
	{
		index_stage_Background = 10000,
		index_stage_new_game = 10001,
		index_stage_prev_game = 10002,
		index_stage_exit = 10003,
		equipSlotBehaveList = 10004,
		storeSlotBehaveList = 10005,
		UIBackground_black = 10006,
		UIBackground_blue = 10007,
		UIBackground_gray = 10008,
		UIBackground_navy_gray = 10009,
		HPEXPFrame = 10010,
		HPPiece = 10011,
		EXPPiece = 10012,
		UIChar_0 = 10013,
		UIChar_a = 10014,
		UIWindow_close = 10015,
		invenslot_attack = 10016,
		invenslot_body = 10017,
		invenslot_feet = 10018,
		invenslot_head = 10019,
		invenslot_ring = 10020,
		invenslot_store = 10021,
		invenslot_torch = 10022,
		npc_quest_can_start = 10023,
		npc_quest_proceed = 10024,
		npc_quest_can_complete = 10025,
		btn_yes = 10026,
		btn_no = 10027,
		btn_ok = 10028,
	};

	enum CharacterID
	{
		attack_effect_left = 20000,
		attack_effect_right = 20001,
		attack_left = 20002,
		attack_right = 20003,
		damaged_left = 20004,
		damaged_right = 20005,
		die_left = 20006,
		die_right = 20007,
		fall_left = 20008,
		fall_right = 20009,
		jump_left = 20010,
		jump_right = 20011,
		run_left = 20012,
		run_right = 20013,
		stop_left = 20014,
		stop_right = 20015,
		walk_left = 20016,
		walk_right = 20017,
	};
	enum NPCID
	{
		Codex = 30000,
		Conjurer = 30001,
		DungeonMaster = 30002,
	};
	enum MonsterID
	{
		Goblin_stop_left = 40000,
		Goblin_stop_right = 40001,
		Goblin_walk_left = 40002,
		Goblin_walk_right = 40003,
		Goblin_die = 40004,
	};
	enum TileID
	{
		normal = 50000,
		wood_row = 50001,
		wood_column = 50002,
	};
	enum ETCID
	{
		back_cave = 60000,
		sky = 60001,
		portal = 60002,
		back_castle = 60003,
		level_up = 60004,
	};
	~SpriteProvider();

	Sprite* GetUISpriteByUIID(UIID SpriteID);
	Sprite* GetCharacterSpriteBySpriteID(CharacterID SpriteID);
	Sprite* GetNPCSpriteBySpriteID(NPCID SpriteID);
	Sprite* GetMonsterSpriteBySpriteID(MonsterID SpriteID);
	Sprite* GetTileSpriteBySpriteID(TileID SpriteID);
	Sprite* GetETCSpriteBySpriteID(ETCID SpriteID);

	Sprite* GetSprite(size_t id);
private:
	bool Initialize(std::string filename);
	std::unordered_map<size_t, std::string> stored_sprite_data;
};

