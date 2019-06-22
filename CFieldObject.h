#pragma once
#include "CArea.h"

class CField;

class CFieldObject
{
	friend class CField;
public:
	virtual					~CFieldObject() = default;
	virtual bool			Init() = 0;
	virtual bool			Frame() = 0;
	virtual bool			Render() = 0;
	virtual bool			Release() = 0;
	virtual bool			Position_X_Frame();
	virtual bool			Position_Y_Frame();

	virtual bool			EffectedbyGravity(float gravity);

	virtual fPoint&			ModifyScreenPointFromLogicalToScreen();

	virtual bool			InitLogicalPoint(float logical_x, float logical_y);
	virtual bool			SetLogicalPoint(float x, float y);
	virtual bool			AddLogicalPointX(float x);
	virtual bool			AddLogicalPointY(float y);

	virtual float			GetWidth();
	virtual float			GetHeight();
	virtual bool			GetDamaged(float damage);
	virtual bool			Reset();
	float					GetMoveSpeed();
	float					SetMoveSpeed(float speed);
	CField*					SetField(CField* new_field);
	CField&					GetField();
	CArea&					GetArea();
protected:
	CField*		field;
	CArea		area;
	float		move_speed;
};