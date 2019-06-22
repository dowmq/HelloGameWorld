#pragma once
#include <Windows.h>
#include "ActionMap.h"
#include "TSingleton.h"

enum key_state { KEY_FREE, KEY_PUSH, KEY_HOLD, KEY_UP };

class InputHandler : public TSingleton<InputHandler>
{
	friend class TSingleton<InputHandler>;
	InputHandler() = default;
private:
	DWORD				keyboard_state[256];
	DWORD				current_mouse_state[3];
	DWORD				previous_mouse_state[3];
	POINT				mouse_pos;
	POINT				previous_mouse_pos;
	ActionMap			action_map;
public:
	bool				Init();
	bool				Frame();
	bool				Render();
	bool				Release();
	DWORD				Key(DWORD dwKey);
	LRESULT				MsgProc(MSG msg);

	bool				is_left_click();
	bool				is_left_click_up();
	bool				is_left_click_holding();
	bool				is_right_click();
	bool				is_now_double_click();
	POINT				GetMousePosVariation();
	POINT				GetMousePos() { return mouse_pos; }
	ActionMap&			GetActionKey() { return action_map; }
private:
	bool				is_mouse_holding;
	DWORD				KeyCheck(DWORD dwKey);
public:
	~InputHandler() = default;
};

