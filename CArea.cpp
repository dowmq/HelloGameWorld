#include "CArea.h"
#include <cmath>
#include "CPlayerCharacter.h"

const double CArea::pi = std::acos(-1);
fPoint CArea::PlayerScreenFixedPoint = { 400.0f , 300.0f };

void CArea::Init(float logical_x, float logical_y)
{
	logical_init_point.x = logical_x;
	logical_init_point.y = logical_y;
	logical_point.x = logical_x;
	logical_point.y = logical_y;
	prev_logical_point.x = logical_point.x;
	prev_logical_point.y = logical_point.y;
	screen_point.x = logical_x;
	screen_point.y = 600 - logical_y;
	rt = { 0, };
}

void CArea::FieldObjectInit(float logical_x, float logical_y)
{
	logical_init_point.x = logical_x;
	logical_init_point.y = logical_y;
	logical_point.x = logical_x;
	logical_point.y = logical_y;
	prev_logical_point.x = logical_point.x;
	prev_logical_point.y = logical_point.y;
	CArea& player_point = CPlayerCharacter::Instance().GetArea();
	screen_point.x = logical_point.x + PlayerScreenFixedPoint.x - player_point.GetLogicalPos().x;
	screen_point.y = PlayerScreenFixedPoint.y - (logical_point.y - player_point.GetLogicalPos().y);
	rt = { 0, };
}

void CArea::SetScreenPos(float screen_x, float screen_y)
{
	screen_point.x = screen_x;
	screen_point.y = screen_y;
}

void CArea::SetLogicalPos(float logical_x, float logical_y) {
	prev_logical_point.x = logical_point.x;
	prev_logical_point.y = logical_point.y;
	logical_point.x = logical_x;
	logical_point.y = logical_y;
}

fPoint& CArea::GetScreenPos() { return screen_point;  }
fPoint&	CArea::GetLogicalPos() { return logical_point; }
fPoint& CArea::GetLogicalInitialPos() { return logical_init_point; }

void CArea::Modify_xPoint(float logical_x)
{
	prev_logical_point.x = logical_point.x;
	logical_point.x = logical_x ;
}
void CArea::Modify_yPoint(float logical_y)
{
	prev_logical_point.y = logical_point.y;
	logical_point.y = logical_y;
}

void CArea::ModifyScreenPointFromLogicalToScreen()
{
	/*screen_point.x = logical_point.x - (other.logical_point.x - other.logical_init_point.x);
	screen_point.y = 600 - (logical_point.y - (other.logical_point.y - other.logical_init_point.y));*/
	/*screen_point.x = logical_point.x - (other.logical_point.x - PlayerCameraPoint.x);
	screen_point.y = 600 - (logical_point.y - (other.logical_point.y - PlayerCameraPoint.y));*/
	
	//CArea& player_point = CPlayerCharacter::Instance().GetArea();
	//fPoint& player_scrren_point = CPlayerCharacter::Instance().GetPlayerScreenFixedPoint();
	//screen_point.x = logical_point.x - (player_point.logical_point.x - player_scrren_point.x);
	//screen_point.y = 600 - (logical_point.y - (player_point.logical_point.y - player_scrren_point.y));

	CArea& player_point = CPlayerCharacter::Instance().GetArea();
	screen_point.x = logical_point.x + PlayerScreenFixedPoint.x - player_point.GetLogicalPos().x;
	screen_point.y = PlayerScreenFixedPoint.y - (logical_point.y - player_point.GetLogicalPos().y);
}

bool CArea::Is_InScreenDrawRect(CArea& other)
{
	if (other.logical_point.x <= logical_point.x + 600 && other.logical_point.x >= logical_point.x - 600 &&
		other.logical_point.y <= logical_point.y + 600 && other.logical_point.y >= logical_point.y - 600)
	{
		return true;
	}
	return false;
}

bool CArea::is_InCollisionDrawRect(CArea& other)
{
	
	if (other.logical_point.x <= logical_point.x + rt.right + 55 && other.logical_point.x >= logical_point.x - 55 &&
		other.logical_point.y <= logical_point.y + rt.bottom + 55 && other.logical_point.y >= logical_point.y - 55)
	{
		return true;
	}
	return false;
}

bool CArea::is_Collision(CArea& other, float revise_value)
{
	float half_x_length = (float)rt.right / 2.0f;
	float this_center_x = logical_point.x + half_x_length;
	float half_y_length = (float)rt.bottom / 2.0f;
	float this_center_y = logical_point.y - half_y_length;

	float o_half_x_length = (float)other.rt.right / 2.0f;
	float other_center_x = other.logical_point.x + o_half_x_length;
	float o_half_y_length = (float)other.rt.bottom / 2.0f;
	float other_center_y = other.logical_point.y - o_half_y_length;

	if (half_x_length + o_half_x_length > std::abs(this_center_x - other_center_x) * revise_value
		&& half_y_length + o_half_y_length > std::abs(this_center_y - other_center_y) * revise_value
		)
	{
		return true;
	}
	return false;
}

float CArea::GetLogicalMiddleX()
{
	return logical_point.x + rt.right / 2.0f;
}

float CArea::GetLogicalMiddleY()
{
	return logical_point.y - rt.bottom / 2.0f;
}

bool CArea::is_X_Colision(CArea& other, float revise_value)
{
	float half_x_length = (float)rt.right / 2.0f;
	float this_center_x = logical_point.x + half_x_length;

	float o_half_x_length = (float)other.rt.right / 2.0f;
	float other_center_x = other.logical_point.x + o_half_x_length;


	if (half_x_length + o_half_x_length > std::abs(this_center_x - other_center_x) * revise_value)
	{
		return true;
	}
	return false;
}

bool CArea::is_Y_Colision(CArea& other, float revise_value)
{
	float half_y_length = (float)rt.bottom / 2.0f;
	float this_center_y = logical_point.y - half_y_length;

	float o_half_y_length = (float)other.rt.bottom / 2.0f;
	float other_center_y = other.logical_point.y - o_half_y_length;

	if (half_y_length + o_half_y_length > std::abs(this_center_y - other_center_y) * revise_value)
	{
		return true;
	}
	return false;
}

bool CArea::is_in_RECT(float x, float y)
{
	if ((x >= rt.left && x <= rt.left + rt.right)
		&& (y >= rt.top && y <= rt.top + rt.bottom))
	{
		return true;
	}
	return false;
}

bool CArea::is_in_Range(CArea& other, float maximum_distance, float maximum_degree)
{
	float this_mid_x = this->GetLogicalMiddleX();
	float this_mid_y = this->GetLogicalMiddleY();

	float other_mid_x = other.GetLogicalMiddleX();
	float other_mid_y = other.GetLogicalMiddleY();

	if (sqrt(pow(this_mid_x - other_mid_x, 2) + pow(this_mid_y - other_mid_y, 2)) > maximum_distance) {
		return false;
	}
	//double rad = atan2(this_mid_x - other_mid_x, this_mid_y - other_mid_y);
	//if (((rad * 180) / CArea::pi) > maximum_degree) {
	//	return false;
	//}
	return true;
}

bool CArea::Is_in_RECT(float left, float top, float right, float bottom, 
	float x, float y)
{
	if ((x >= left && x <= left + right) 
		&& (y >= top && y <= top + bottom))
	{
		return true;
	}
	return false;
}