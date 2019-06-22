#pragma once
#include <Windows.h>

struct fPoint
{
	float x;
	float y;
};

struct CArea
{
	fPoint			prev_logical_point;
	fPoint			logical_point;
	fPoint			screen_point;
	RECT			rt;

	void			FieldObjectInit(float logical_x, float logical_y);
	void			Init(float logical_x, float logical_y);

	void			SetScreenPos(float screen_x, float screen_y);
	void			SetLogicalPos(float logical_x, float logical_y);
	fPoint&			GetScreenPos();
	fPoint&			GetLogicalPos();
	fPoint&			GetLogicalInitialPos();
	float			GetLogicalMiddleX();
	float			GetLogicalMiddleY();

	void			Modify_xPoint(float logical_x);
	void			Modify_yPoint(float logical_y);
	void			ModifyScreenPointFromLogicalToScreen();

	bool			Is_InScreenDrawRect(CArea& other);
	bool			is_InCollisionDrawRect(CArea& other);
	bool			is_Collision(CArea& area, float revise_value = 1.0f);

	bool			is_X_Colision(CArea& area, float revise_value = 1.0f);
	bool			is_Y_Colision(CArea& area, float revise_value = 1.0f);

	// return - either the given (x, y) is in CArea::rt or not.
	bool			is_in_RECT(float x, float y);
	static bool		Is_in_RECT(float left, float top, float right, float bottom, 
		float x, float y);

	bool			is_in_Range(CArea& other, float maximum_distance, float maximum_degree);

	static const double pi;
	static fPoint		PlayerScreenFixedPoint;
private:
	fPoint				logical_init_point;
};

