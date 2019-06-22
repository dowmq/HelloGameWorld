#include "CUI.h"

CUI::CUI(CUI* owner_) : owner(owner_) { }

CUI::CUI(const CUI& other)
{
	owner = other.owner;
}

bool CUI::SetScreenPos(float x, float y)
{
	screen_x = x;
	screen_y = y;
	return true;
}

bool CUI::AddScreenPos(float x, float y)
{
	return SetScreenPos(screen_x + x, screen_y + y);
}

float CUI::GetScreenX() { return screen_x; }
float CUI::GetScreenY() { return screen_y; }
float CUI::GetWidth() { return width; }
float CUI::GetHeight() { return height; }
bool CUI::SetWidth(float width_) { width = width_; return true; }
bool CUI::SetHeight(float height_) { height = height_; return true; }