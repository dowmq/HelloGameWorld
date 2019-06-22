#pragma once
#include "TimeHandler.h"
#include <array>
#include <Windows.h>

class ActionMap
{
	friend class InputHandler;
public:
	enum KeySet
	{
		w_key,
		s_key,
		a_key,
		d_key,
		z_key,
		space_key,
		escape_key,
		left_click,
		middle_click,
		right_click,

		count
	};
	void					SetActionKeyState(KeySet ActionKeyList, int state);
	DWORD					GetActionKeyState(KeySet ActionKeyList);
	double					GetCurrentAndLastKeyUpTimeGap(KeySet ActionKeyList);
private:
	ActionMap() = default;
	~ActionMap() = default;
	void					UpdateKeyUpTime(KeySet action_key);

	TimeFragment								internal_timer;
	std::array<DWORD, KeySet::count>			action_key_state;
	std::array<TimeFragment, KeySet::count>		last_input_key_up_time;
};