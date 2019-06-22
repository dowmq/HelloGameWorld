#include "ActionMap.h"
#include "InputHandler.h"
#include <exception>
#include <stdexcept>

void ActionMap::SetActionKeyState(KeySet action_key, int state)
{
	action_key_state[action_key] = state;
	if (state == key_state::KEY_UP) {
		UpdateKeyUpTime(action_key);
	}
}

DWORD ActionMap::GetActionKeyState(KeySet action_key)
{
	return action_key_state[action_key];
}

void ActionMap::UpdateKeyUpTime(KeySet action_key)
{
	last_input_key_up_time[action_key].Update();
}

double ActionMap::GetCurrentAndLastKeyUpTimeGap(KeySet action_key)
{
	internal_timer.Update();
	double time = (internal_timer - last_input_key_up_time[action_key]) * 1000.0;
	return time; 
}