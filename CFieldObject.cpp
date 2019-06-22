#include "CFieldObject.h"
#include "CField.h"

bool CFieldObject::Position_X_Frame()
{
	return true;
}

bool CFieldObject::Position_Y_Frame()
{
	return true;
}

bool CFieldObject::EffectedbyGravity(float gravity)
{
	return true;
}

fPoint& CFieldObject::ModifyScreenPointFromLogicalToScreen()
{
	area.ModifyScreenPointFromLogicalToScreen();
	return area.GetScreenPos();
}

bool CFieldObject::InitLogicalPoint(float logical_x, float logical_y)
{
	area.Init(logical_x, logical_y);
	return true;
}

bool CFieldObject::SetLogicalPoint(float x, float y)
{
	area.Modify_xPoint(x);
	area.Modify_yPoint(y);
	return true;
}

bool CFieldObject::AddLogicalPointX(float x)
{
	area.Modify_xPoint(area.logical_point.x  + x);
	return true;
}

bool CFieldObject::AddLogicalPointY(float y)
{
	area.Modify_yPoint(area.logical_point.y + y);
	return true;
}

float CFieldObject::GetMoveSpeed() 
{
	return move_speed;
}

float CFieldObject::SetMoveSpeed(float speed)
{
	float temp = move_speed;
	move_speed = speed;
	return temp;
}

CField* CFieldObject::SetField(CField* new_field)
{
	CField* temp = field;
	field = new_field;
	return temp;
}

CField& CFieldObject::GetField()
{
	return *field;
}

CArea& CFieldObject::GetArea()
{
	return area;
}

bool CFieldObject::GetDamaged(float damage)
{
	return true;
}

bool CFieldObject::Reset()
{
	return true;
}

float CFieldObject::GetWidth()
{
	return area.rt.right;
}

float CFieldObject::GetHeight()
{
	return area.rt.bottom;
}