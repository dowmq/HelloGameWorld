#pragma once
#include "Sprite.h"
#include "CFieldObject.h"
#include "CArea.h"
#include "TimeHandler.h"
#include <array>
#include <vector>
#include <utility>
#include <mutex>

class CMonster : public CFieldObject
{
	enum MonsterSprite
	{
		walking_left, walking_right,
		stop_left, stop_right,
		die,
		count
	};
	friend class MonsterFactory;
public:
	enum StatType
	{
		HP,
		MP,
		EXP,
		Attack,
		defense,
		StatCount
	};
	CMonster(float logical_x, float logical_y);
	~CMonster();

	virtual bool			Init() override;
	virtual bool			Frame() override;
	virtual bool			Position_X_Frame() override;
	virtual bool			Position_Y_Frame() override;
	virtual bool			Render() override;
	virtual bool			Release() override;
	virtual bool			EffectedbyGravity(float gravity) override;

	virtual bool			GetDamaged(float damaged) override;
	virtual bool			Reset() override;
	bool					Is_dead();
	int						GetId();
	void					SetDirectionLock(bool value);
	std::vector<std::pair<size_t, size_t>>& GetMonsterDropProbabilityAndItemId();
	std::array<float, StatType::StatCount>& GetMonsterStats();
private:
	void					ReserveDirectionChangeUnlock();

	int						monster_id = 900001;
	bool					trace;
	bool					dead;
	bool					direction_change_lock;
	bool					character_is_on_right;

	std::array<Sprite*, MonsterSprite::count> sprites;
	std::array<float, StatType::StatCount> stats;

	static std::once_flag							call_once_flag;
	static std::vector<std::pair<size_t, size_t>>	pair_probability_dropitem;

	Sprite* current;
	TimeHandler* frame;
};

