#pragma once
#include "CFieldObject.h"
#include "Sprite.h"
#include "CPlayerCharacter.h"
#include "CMonster.h"
class CPlayerAttackObject : public CFieldObject
{
public:
	CPlayerAttackObject(CPlayerCharacter* owner_, 
		double dead_time, bool is_right_);
	~CPlayerAttackObject();

	virtual bool			Init() override;
	virtual bool			Frame() override;
	virtual bool			Render() override;
	virtual bool			Release() override;

	virtual bool			CollisionWith(CMonster* obj);


	CPlayerCharacter&		GetOwner() { return *owner; }
	Sprite&					GetSprite() { return *image; }
protected:
	int						attack_count;
	bool					is_right;
	Sprite*					image;
	CPlayerCharacter*		owner;
	double					dead_time;

	TimeHandler*			frame;
};

