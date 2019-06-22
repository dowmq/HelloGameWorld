#pragma once
#include "CFieldObject.h"
#include "SpriteFactory.h"
#include "CArea.h"
#include "InputHandler.h"
#include "CMonster.h"
#include "TSingleton.h"
#include "CQuest.h"
#include <array>
#include <optional>
#include "CPlayerDecorateEffect.h"

class CItem;

class CPlayerCharacter : public CFieldObject, public TSingleton<CPlayerCharacter>
{
	friend class TSingleton<CPlayerCharacter>;
public:
	enum CharacterSprite {
		walking_left, walking_right,
		running_left, running_right,
		attack_left, attack_right,
		jump_left, jump_right,
		fall_left, fall_right,
		stop_left, stop_right,
		damaged_left, damaged_right,
		die_left, die_right,
		sprite_count
	};
	enum CharacterStat {
		LV, EXP, HP, MP, attack_power, defense_power, speed, gold,
		stat_count
	};
	enum MotionState {
		Motion_LOCK, is_right, stop, walk, running, jump, fall, attack, damaged, die,
		motion_count
	};
public:
	~CPlayerCharacter();
	virtual bool			Init() override;
	virtual bool			Frame() override;
	virtual bool			Position_X_Frame() override;
	virtual bool			Position_Y_Frame() override;
	virtual bool			Render() override;
	virtual bool			Release() override;
	virtual bool			EffectedbyGravity(float gravity) override;

	bool					UpdateFieldNPCUpperQuestUI();

	std::vector<CQuest*>&	GetQuestList();
	std::optional<CQuest>	GetQuest(int qid);

	bool					CollisionWithMonster(CMonster* mob);
	bool					KillMonster(CMonster* mob);
	bool					GainExpFunction(float exp);
	bool					ProcessLevelUp();
	bool					ProcessKillMonsterQuest(CMonster* mob);
	bool					GainItem(CItem* item);
	bool					ProcessFieldItemPickUp(ActionMap& action_map);
	bool					PlayerDeadFunction();

	bool					SetEquipItemAbility(CItem* item);
	bool					SetUnEquipItemAbility(CItem* item);
	bool					AddCharacterStat(CharacterStat stat, float change_value);
	bool					SetCharacterStat(CharacterStat stat, float value);
	void					SetMotion(Sprite* new_motion);
	void					SetMotionLock();
	void					SetMotionUnLock();
	void					SetJumpStateToFall();
	void					AttackStateCancle();
	void					SetJumpStateCancle();
	void					SetFallCancle();
	void					SetDeadStateCancle();
	void					SetMotionLockCancle();
	void					SetDamagedMotionCancle();
	inline bool				GetMotionState(uint64_t& flag, MotionState MotionState);
	inline void				SetMotionState(uint64_t& flag, MotionState MotionState, bool value);

	std::array<float, CharacterStat::stat_count>& GetStat() { return stats; }
private:
	CPlayerCharacter();
	bool				DirectionHandle(ActionMap& action_map);
	bool				JumpHandle(ActionMap& action_map);
	bool				AttackHandle(ActionMap& action_map);
	bool				RunningHandle(ActionMap& action_map);
	bool				SetMotionToSprite();
	bool				SetSpriteMotionToMovePositionX();
	bool				SetSpriteMotionToMovePositionY();
	bool				is_state_that_motion_cannot_change();
	bool				is_cannot_set_direction_state();

	Sprite*						current;
	uint64_t					new_motion_flag;
	uint64_t					previous_motion_flag;
	InputHandler*				input;
	TimeHandler*				frame;
	TimeFragment				jump_time_point;
	TimeFragment				jump_elapse_time_point;
	CPlayerDecorateEffect		decorate_effect;
	
	std::vector<CQuest*> quest_list;
	std::array<Sprite*, CharacterSprite::sprite_count> sprites;
	std::array<float, CharacterStat::stat_count> stats;
};