#pragma comment(lib, "Msimg32.lib")
#include "Bitmap.h"
#include "CWindow.h"

bool CBitmap::DrawBitBlt(CArea area, DWORD iMode)
{
	BitBlt(offScreenDC, 
		area.screen_point.x, // The x-coordinate of the upper-left corner of the destination rectangle.
		area.screen_point.y, // The y-coordinate of the upper-left corner of the destination rectangle.
		bitmap_rect.right,	// The width of the source and destination rectangles.
		bitmap_rect.bottom,	// The height of the source and the destination rectangles.
		m_hMemDC, 
		bitmap_rect.left,	// The x-coordinate of the upper-left corner of the source rectangle.
		bitmap_rect.top,	// The y-coordinate of the upper-left corner of the source rectangle.
		iMode);
	return true;
}

bool CBitmap::DrawBitBlt(float x, float y, DWORD iMode)
{
	BitBlt(offScreenDC,
		x, // The x-coordinate of the upper-left corner of the destination rectangle.
		y, // The y-coordinate of the upper-left corner of the destination rectangle.
		bitmap_rect.right,	// The width of the source and destination rectangles.
		bitmap_rect.bottom,	// The height of the source and the destination rectangles.
		m_hMemDC,
		bitmap_rect.left,	// The x-coordinate of the upper-left corner of the source rectangle.
		bitmap_rect.top,	// The y-coordinate of the upper-left corner of the source rectangle.
		iMode);
	return true;
}

bool CBitmap::DrawAlphaBlend(CArea area)
{
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = m_Alpha;
	bf.AlphaFormat = AC_SRC_ALPHA;

	bool a = AlphaBlend(offScreenDC, 
		area.screen_point.x, // The x-coordinate of the upper-left corner of the destination rectangle.
		area.screen_point.y, // The y-coordinate of the upper-left corner of the destination rectangle
		bitmap_rect.right,	// The width of the destination rectangle
		bitmap_rect.bottom,	// The height of the destination rectangle
		m_hMemDC, 
		bitmap_rect.left,	// The x-coordinate of the upper-left corner of the source rectangle
		bitmap_rect.top,	// The y-coordinate of the upper-left corner of the source rectangle
		bitmap_rect.right,	// The width of the source rectangle
		bitmap_rect.bottom,	// The height of the source rectangle
		bf);
	return true;
}

bool CBitmap::DrawAlphaBlend(float x, float y)
{
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = m_Alpha;
	bf.AlphaFormat = AC_SRC_ALPHA;

	bool a = AlphaBlend(offScreenDC,
		x, // The x-coordinate of the upper-left corner of the destination rectangle.
		y, // The y-coordinate of the upper-left corner of the destination rectangle
		bitmap_rect.right,	// The width of the destination rectangle
		bitmap_rect.bottom,	// The height of the destination rectangle
		m_hMemDC,
		bitmap_rect.left,	// The x-coordinate of the upper-left corner of the source rectangle
		bitmap_rect.top,	// The y-coordinate of the upper-left corner of the source rectangle
		bitmap_rect.right,	// The width of the source rectangle
		bitmap_rect.bottom,	// The height of the source rectangle
		bf);
	return true;
}

bool CBitmap::Release()
{
	DeleteObject(m_hBitmap);
	DeleteObject(m_hMemDC);
	m_hBitmap = NULL;
	m_hMemDC = NULL;
	return true;
}

CBitmap::~CBitmap()
{
	if (m_hBitmap != NULL) {
		DeleteObject(m_hBitmap);
	}
	if (m_hMemDC != NULL) {
		DeleteObject(m_hMemDC);
	}
}
CBitmap* CBitmap::Create_24bit_bitmap(std::wstring filename)
{
	CBitmap* bitmap = new CBitmap;
	bitmap->m_hBitmap = (HBITMAP)LoadImage(
		CWindow::Instance().m_hInstance,
		filename.c_str(),
		IMAGE_BITMAP,
		0, 0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE);
	GetObject(bitmap->m_hBitmap, sizeof(BITMAP), &bitmap->m_BitmapInfo);
	
	bitmap->m_hMemDC = CreateCompatibleDC(CWindow::Instance().GetScreenDC());	
	SelectObject(bitmap->m_hMemDC, bitmap->m_hBitmap);

	bitmap->offScreenDC = CWindow::Instance().GetOffScreenDC();
	return bitmap;
}