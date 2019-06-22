#pragma once
#include <Windows.h>
#include "TSingleton.h"

class CWindow : public TSingleton<CWindow>
{
	friend class TSingleton<CWindow>;
	struct DeviceContext
	{
		HDC				hScreenDC;
		HDC				hOffScreenDC;
		HBITMAP			hOffScreenBitmap;
		DWORD			bkColor;
		HBRUSH			hbrBack;
		HANDLE			hPen;
		HFONT			fontDefault;

		void			PrepareRendering();
		void			Rendering_OffScreen();
	};
public:
	HINSTANCE			m_hInstance;
	HWND				m_hWnd;
	RECT				m_rtClient;
	RECT				m_rtWindow;
	DWORD				m_dwStyle;

	DeviceContext WindowDeviceContext;
public:
	bool				InitWindow( HINSTANCE hInstance, const TCHAR* szTitleName = 0,
									int iX = 0, int iY = 0, int iWidth = 800, int iHeight = 600);
	bool				MyRegisterClass();
	void				CenterWindow();
	HDC					GetScreenDC() { return WindowDeviceContext.hScreenDC; }
	HDC					GetOffScreenDC() { return WindowDeviceContext.hOffScreenDC; }
	HBITMAP				GetOffScreenBitmap() { return WindowDeviceContext.hOffScreenBitmap; }
	void				Release();

	static LRESULT		MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	CWindow();
	~CWindow();
};

