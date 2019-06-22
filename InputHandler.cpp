#include "InputHandler.h"
#include "CWindow.h"
#include "CUIWindow.h"
DWORD InputHandler::Key(DWORD dwKey)
{
	SHORT sKey = GetAsyncKeyState(dwKey);
	return sKey;
}
DWORD InputHandler::KeyCheck(DWORD dwKey)
{
	SHORT sKey = GetAsyncKeyState(dwKey);
	// enum { KEY_FREE, KEY_PUSH, KEY_HOLD, KEY_UP}

	//  이전에 누른 적이 없고 호출 시점에는 눌려있는 상태
	if (sKey & 0x8000)
	{
		if (keyboard_state[dwKey] == KEY_FREE || keyboard_state[dwKey] == KEY_UP)
		{
			keyboard_state[dwKey] = KEY_PUSH;
		}
		else
		{
			keyboard_state[dwKey] = KEY_HOLD;
		}
	}
	else
	{
		if (keyboard_state[dwKey] == KEY_PUSH || keyboard_state[dwKey] == KEY_HOLD)
		{
			keyboard_state[dwKey] = KEY_UP;
		}
		else
		{
			keyboard_state[dwKey] = KEY_FREE;
		}
	}
	return keyboard_state[dwKey];
}
bool InputHandler::Init()
{
	ZeroMemory(keyboard_state, sizeof(DWORD) * 256);
	return true;
};
bool InputHandler::Frame()
{
	previous_mouse_pos = mouse_pos;
	// 마우스 point를 갱신
	GetCursorPos(&mouse_pos); // 화면스크린 좌표계
	ScreenToClient(CWindow::Instance().m_hWnd, &mouse_pos);

	action_map.SetActionKeyState(ActionMap::KeySet::w_key, KeyCheck(VK_UP));
	action_map.SetActionKeyState(ActionMap::KeySet::s_key, KeyCheck(VK_DOWN));
	action_map.SetActionKeyState(ActionMap::KeySet::a_key, KeyCheck(VK_LEFT));
	action_map.SetActionKeyState(ActionMap::KeySet::d_key, KeyCheck(VK_RIGHT));
	action_map.SetActionKeyState(ActionMap::KeySet::z_key, KeyCheck('Z'));
	action_map.SetActionKeyState(ActionMap::KeySet::escape_key, KeyCheck(VK_CONTROL));
	action_map.SetActionKeyState(ActionMap::KeySet::space_key, KeyCheck(VK_MENU));

	for (int iButton = 0; iButton < 3; iButton++)
	{
		if (previous_mouse_state[iButton] == KEY_PUSH)
		{
			if (current_mouse_state[iButton] == KEY_PUSH)
			{
				current_mouse_state[iButton] = KEY_HOLD;
			}
		}
		previous_mouse_state[iButton] = current_mouse_state[iButton];
	}
	action_map.SetActionKeyState(ActionMap::KeySet::left_click, current_mouse_state[0]);
	action_map.SetActionKeyState(ActionMap::KeySet::middle_click, current_mouse_state[1]);
	action_map.SetActionKeyState(ActionMap::KeySet::right_click, current_mouse_state[2]);
	return true;
};
bool InputHandler::Render() {
	//HDC hdc = GetDC(g_hWnd);
	//SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
	//SetTextColor(g_hOffScreenDC, RGB(0, 0, 255));
	//SetBkMode(g_hOffScreenDC, TRANSPARENT);
	//TextOut(g_hOffScreenDC, 0, 0, m_csBuffer, _tcslen(m_csBuffer));
	//ReleaseDC(g_hWnd, hdc);
	return true;
};
bool InputHandler::Release() 
{
	return true;
};
LRESULT InputHandler::MsgProc(MSG msg)
{
	current_mouse_state[0] = KEY_FREE;
	current_mouse_state[1] = KEY_FREE;
	current_mouse_state[2] = KEY_FREE;
	switch (msg.message)
	{
		case WM_LBUTTONDOWN:
		{
			current_mouse_state[0] = KEY_PUSH;
			is_mouse_holding = true;
			break;
		}
		case WM_LBUTTONUP:
		{
			current_mouse_state[0] = KEY_UP;
			is_mouse_holding = false;
			if (CUIWindow::GetCUIWindowLock(CUIWindow::entire_uiwindow_lock_flag, 
				CUIWindow::CUIWindowLock::work_lock) == true)
			{
				CUIWindow::SetCUIWindowLock(CUIWindow::entire_uiwindow_lock_flag,
					CUIWindow::CUIWindowLock::work_lock, false);
			}
			break;
		}
		case WM_MBUTTONDOWN:
		{
			current_mouse_state[1] = KEY_PUSH;
			break;
		}
		case WM_MBUTTONUP:
		{
			current_mouse_state[1] = KEY_UP;
			break;
		}
		case WM_RBUTTONDOWN:
		{
			current_mouse_state[2] = KEY_PUSH;
			break;
		}
		case WM_RBUTTONUP:
		{
			current_mouse_state[2] = KEY_UP;
			break;
		}
	}
	return 1;
}

bool InputHandler::is_left_click()
{
	if (action_map.GetActionKeyState(ActionMap::KeySet::left_click) == key_state::KEY_PUSH) {
	//if(current_mouse_state[0] == key_state::KEY_PUSH) {
		return true;
	}
	return false;
}

bool InputHandler::is_left_click_up()
{
	if (action_map.GetActionKeyState(ActionMap::KeySet::left_click) == key_state::KEY_UP) {
		return true;
	}
	return false;
}

bool InputHandler::is_left_click_holding()
{
	//if (action_map.GetActionKeyState(ActionMap::KeySet::left_click) == key_state::KEY_HOLD) {
	if(is_mouse_holding){
		return true;
	}
	return false;
}

bool InputHandler::is_right_click()
{
	if (action_map.GetActionKeyState(ActionMap::KeySet::right_click) == key_state::KEY_PUSH) {
		return true;
	}
	return false;
}

bool InputHandler::is_now_double_click()
{
	if (action_map.GetActionKeyState(ActionMap::KeySet::left_click) == key_state::KEY_HOLD)
	{
		auto time = action_map.GetCurrentAndLastKeyUpTimeGap(ActionMap::KeySet::left_click);
		if (time <= 40)
			return true;
		else
			return false;
	}
	return false;
}

POINT InputHandler::GetMousePosVariation()
{
	return std::move(POINT{ mouse_pos.x - previous_mouse_pos.x, mouse_pos.y - previous_mouse_pos.y });
}