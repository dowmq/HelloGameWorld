#include "CWindow.h"

LRESULT CWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_SYSKEYDOWN:
		{
			if (wParam == VK_MENU)
				return true;
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);// WM_QUIT 메세지큐 등록
			break;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool CWindow::MyRegisterClass()
{
	WNDCLASSEXW wcex;
	wcex.lpfnWndProc = (WNDPROC)&CWindow::MsgProc;
	wcex.lpszClassName = L"KGCA";
	wcex.hInstance = m_hInstance;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(nullptr, IDC_WAIT);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_DEMOWINDOW);
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	if (!RegisterClassExW(&wcex))
		return false;
	return true;
}
bool  CWindow::InitWindow(HINSTANCE hInstance,
	const TCHAR* pTitleName,
	int iX, int iY, int iWidth, int iHeight)
{
	m_hInstance = hInstance;
	// 1) 윈도우 레지스터 등록
	if (MyRegisterClass() == false)
		return false;

	RECT rt = { 0,0, iWidth, iHeight };
	AdjustWindowRect(&rt, m_dwStyle, FALSE);

	// 2) 윈도우 생성
	//HWND hWnd = CreateWindowEx(WS_EX_TOOLWINDOW,
	m_hWnd = CreateWindow(
		L"KGCA",
		(pTitleName) ? pTitleName : L"Sample Window",
		//WS_POPUP | WS_SYSMENU,
		m_dwStyle,
		iX, iY,
		rt.right - rt.left,
		rt.bottom - rt.top, NULL, NULL,
		m_hInstance,
		NULL);
	if (m_hWnd == NULL) 
		return 0;

	GetClientRect(m_hWnd, &m_rtClient);
	GetWindowRect(m_hWnd, &m_rtWindow);

	CenterWindow();
	ShowWindow(m_hWnd, SW_SHOW);

	// DC 얻기
	WindowDeviceContext.hScreenDC = GetDC(m_hWnd);
	WindowDeviceContext.hOffScreenDC = CreateCompatibleDC(WindowDeviceContext.hScreenDC);
	WindowDeviceContext.hOffScreenBitmap = CreateCompatibleBitmap(
		WindowDeviceContext.hScreenDC, m_rtClient.right, m_rtClient.bottom);
	SelectObject(WindowDeviceContext.hOffScreenDC, WindowDeviceContext.hOffScreenBitmap);

	WindowDeviceContext.bkColor = RGB(255, 255, 255);
	WindowDeviceContext.hbrBack = CreateSolidBrush(WindowDeviceContext.bkColor);
	SelectObject(WindowDeviceContext.hOffScreenDC, WindowDeviceContext.hbrBack);

	WindowDeviceContext.hPen = CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
	SelectObject(WindowDeviceContext.hOffScreenDC, WindowDeviceContext.hPen);

	// 폰트
	WindowDeviceContext.fontDefault = CreateFont(20, 0, 0, FW_BOLD,
		0, 0, 0, 0,
		HANGEUL_CHARSET, 3, 2, 1,
		VARIABLE_PITCH | FF_ROMAN,
		TEXT("고딕"));
	SelectObject(WindowDeviceContext.hOffScreenDC, WindowDeviceContext.fontDefault);
	return true;
}
void CWindow::CenterWindow()
{
	int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	int x = (iScreenWidth - (m_rtWindow.right - m_rtWindow.left)) / 2;
	int y = (iScreenHeight - (m_rtWindow.bottom - m_rtWindow.top)) / 2;
	MoveWindow(m_hWnd, x, y, m_rtWindow.right - m_rtWindow.left, m_rtWindow.bottom - m_rtWindow.top, true);
}

void CWindow::Release()
{
	DeleteObject(WindowDeviceContext.hPen);
	DeleteObject(WindowDeviceContext.fontDefault);
	DeleteObject(WindowDeviceContext.hOffScreenBitmap);
	DeleteObject(WindowDeviceContext.hbrBack);
	ReleaseDC(m_hWnd, WindowDeviceContext.hOffScreenDC);
	ReleaseDC(m_hWnd, WindowDeviceContext.hScreenDC);
}
CWindow::CWindow()
{
	m_dwStyle = WS_OVERLAPPEDWINDOW;
}


CWindow::~CWindow()
{
}

void CWindow::DeviceContext::PrepareRendering()
{
	PatBlt(hOffScreenDC,
		0, 0, 
		CWindow::Instance().m_rtClient.right,
		CWindow::Instance().m_rtClient.bottom, 
		PATCOPY);
}

void CWindow::DeviceContext::Rendering_OffScreen()
{
	BitBlt(hScreenDC, 
		0, 0,
		CWindow::Instance().m_rtClient.right,
		CWindow::Instance().m_rtClient.bottom,
		hOffScreenDC,
		0,0, SRCCOPY);
}