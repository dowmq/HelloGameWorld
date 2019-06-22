#pragma once
#include "TimeFragment.h"
#include "TSingleton.h"
#include <tchar.h>
#include <functional>
#include <vector>
#include <utility>

class CFieldObject;

class TimeHandler : public TSingleton<TimeHandler>
{
	friend class TSingleton<TimeHandler>;
	TimeHandler() = default;
public:
	~TimeHandler() = default;
	bool					Init();
	bool					Frame();
	bool					Render();
	bool					Release();
	size_t					GetFPS() { return game_FPS; }
	double					GetSecondPerFrame() { return second_per_frame; }

	void					SetTimerFunction(double miliseconds, std::function<void(CFieldObject*)>, CFieldObject*);
private:
	TimeFragment			time_before;
	TimeFragment			time_current;
	size_t					frame_count = 0;
	double					frame_time = 0.0;
	double					time_sum = 0.0;
	size_t					game_FPS = 0;
	TCHAR					m_buffer[_MAX_PATH] = { 0, };

	double					second_per_frame = 0.0;
	double					fGameTime = 0.0;

	std::vector<std::tuple<double, std::function<void(CFieldObject*)>, CFieldObject*>>
		reserved_time_and_function;
};
