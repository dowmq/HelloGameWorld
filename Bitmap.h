#pragma once
#include <Windows.h>
#include <string>
#include "CArea.h"
class CBitmap
{
	friend class SpriteFactory;
	CBitmap() = default;
public:
	CBitmap(const CBitmap&) = delete;
	// ºñÆ®¸Ê ÇÚµé
	HBITMAP					m_hBitmap;
	HDC						m_hMemDC;
	BITMAP					m_BitmapInfo;
	RECT					bitmap_rect;
	HDC						offScreenDC;

	int						m_Alpha = 255;
public:
	bool					SetAlpha(int alpha) { m_Alpha = alpha; };
	bool					DrawBitBlt(CArea area, DWORD iMode = SRCCOPY);
	bool					DrawBitBlt(float x, float y, DWORD iMode = SRCCOPY);
	bool					DrawAlphaBlend(CArea area);
	bool					DrawAlphaBlend(float x, float y);
	bool					Release();

	RECT&					GetImageRect() { return bitmap_rect; }

	static CBitmap*			Create_24bit_bitmap(std::wstring filename);
	~CBitmap();
};
