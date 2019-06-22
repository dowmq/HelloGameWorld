#include "CUIChatLog.h"
#include "CUIString.h"
CUIChatLog* CUIChatLog::instance_ = nullptr;
CUIChatLog::CUIChatLog(float screen_x_, float screen_y_, float width_, float height_, CUI* owner_)
	: CUIComponent(owner_)
{
	screen_x = screen_x_;
	screen_y = screen_y_;
	width = width_;
	height = height_;
	CUIString* chatlog = new CUIString("chat log", CUICharColor::yellow, screen_x, screen_y, this);
	Component.push_back(chatlog);
}

bool CUIChatLog::Frame()
{
	if (instance_ == nullptr)
		instance_ = this;
	if (stored_chatting.empty() == false)
	{
		for (size_t i = 0; i < stored_chatting.size(); ++i)
		{
			std::string str = stored_chatting.front();
			stored_chatting.pop_front();
			CUIString* chat_message = new CUIString(str, CUICharColor::white, 200, 200, this);
			Component.push_back(chat_message);
		}
		size_t index = 0;
		for (auto iter = Component.rbegin(); iter != Component.rend() - 1;)
		{
			CUIString* string = reinterpret_cast<CUIString*>(*iter);
			string->SetScreenPos(screen_x + 5, screen_y + height - (index * 8));
			// 채팅 로그 부분을 넘어갔을 경우
			if (height - (index * 8) <  8)
			{
				string->Release();
				delete string; string = nullptr;
				auto not_reverse_iter = Component.erase(std::next(iter).base());
				iter = std::reverse_iterator(not_reverse_iter);
			}
			else
			{
				++iter;
			}
			++index;
		}
	}
	return true;
}

bool CUIChatLog::StoreString(std::string& str)
{
	stored_chatting.push_back(str);
	return true;
}

CUIChatLog::~CUIChatLog()
{
}
