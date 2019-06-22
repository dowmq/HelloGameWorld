#include "TimeHandler.h"
#include "CWindow.h"

bool TimeHandler::Init()
{
	time_current.Update();
	return true;
}

bool TimeHandler::Release()
{
	return true;
}
void TimeHandler::SetTimerFunction(double miliseconds,
	std::function<void(CFieldObject*)> func,
	CFieldObject* this_)
{
	reserved_time_and_function.emplace_back(miliseconds / 1000, func, this_);
}

bool TimeHandler::Frame()
{
	time_current.Update();
	auto elapseTick = time_current - time_before;
	double frame_per_sec = elapseTick;
	time_sum += frame_per_sec;
	frame_time += frame_per_sec;
	second_per_frame = frame_per_sec;
	fGameTime = time_sum;
	if (reserved_time_and_function.empty() == false)
	{
		for (auto iter = reserved_time_and_function.begin();
					iter != reserved_time_and_function.end();)
		{
			auto& reserve_time = std::get<0>(*iter);
			reserve_time -= elapseTick;
			if (reserve_time <= 0)
			{
				auto& function = std::get<1>(*iter);
				auto& this_pointer = std::get<2>(*iter);
				function(this_pointer);
				iter = reserved_time_and_function.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	if (frame_time >= 1.0)
	{
		game_FPS = frame_count;
		frame_count = 0;
		frame_time -= 1.0;
		_stprintf_s(m_buffer, TEXT("Timer:[%10.4f], FPS:[%d]"), time_sum, game_FPS);
	}
	frame_count++;
	time_before = time_current;
	return true;
}

bool  TimeHandler::Render() {

	auto OffDC = CWindow::Instance().GetOffScreenDC();
	//SetBkColor(OffDC, RGB(128, 128, 128));
	//SetTextColor(OffDC, RGB(0, 0, 0));
	//SetBkMode(OffDC, TRANSPARENT);
	TextOut(OffDC, 250, 0, m_buffer, _tcslen(m_buffer));
	return true;
};