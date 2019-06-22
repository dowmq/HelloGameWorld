#include "CUINPC.h"
#include "CUIImage.h"
#include "InputHandler.h"
#include "CUINPCTalk.h"
#include "SpriteProvider.h"
#include "NPCTalkScriptData.h"
CUINPC::CUINPC(const NPCData& npc_data_,
	float screen_x_, float screen_y_, CUI* owner_) : CUI(owner_), npc_data(npc_data_)
{
	screen_x = screen_x_;
	screen_y = screen_y_;
	npc_image = new CUIImage(npc_data.npc_sprite_id, screen_x_, screen_y_, true, this);
	width = npc_image->GetWidth();
	height = npc_image->GetHeight();

	// NPCTalkBox Create Event
	CUIHandler::EventUI* event_ = CUIHandler::Instance().RegisterEventUI(this,
		[](CUI* ui) {
			CUINPC* npc = reinterpret_cast<CUINPC*>(ui);
			if (InputHandler::Instance().is_now_double_click())
			{
				//auto temp = NPCTalkScriptDataProvider::Instance().GetNpcTalkScript(npc->npc_data.npc_script_id);
				auto temp = DataProvider__ScriptData(npc->npc_data.npc_script_id);
				CUINPCTalk* talkwindow = new CUINPCTalk(npc->npc_data.npc_id, 
					std::move(temp), 
					"npc talk", 
					npc->npc_image->GetSprite()->sprite_id,
					100, 100, npc);
				CUIHandler::Instance().RegisterUIWindow(npc->npc_data.npc_id, talkwindow);
			}
		}, screen_x, screen_y, width, height);
	events[CUINPCEvent::double_clicked] = event_;

	upper_npc_name_string = new CUIString(npc_data.npc_name, CUICharColor::white, 0, 0, this);
	float string_width = upper_npc_name_string->GetWidth();
	float string_height = upper_npc_name_string->GetHeight();
	upper_npc_name_string->SetScreenPos(screen_x + ((width - string_width) / 2), screen_y - 3 - string_height);

	lower_npc_explain_string = new CUIString(npc_data.npc_explain, CUICharColor::white, 0, 0, this);
	string_width = lower_npc_explain_string->GetWidth();
	string_height = lower_npc_explain_string->GetHeight();
	lower_npc_explain_string->SetScreenPos(screen_x + ((width - string_width) / 2), screen_y + height + 3);
}

bool CUINPC::Init()
{
	return true;
}

bool CUINPC::Frame()
{
	npc_image->Frame();
	return true;
}

bool CUINPC::Render()
{
	npc_image->Render();
	upper_npc_name_string->Render();
	lower_npc_explain_string->Render();
	return true;
}

bool CUINPC::Release()
{
	if (npc_image) {
		npc_image->Release();
		delete npc_image; npc_image = nullptr;
	}
	if (upper_npc_name_string) {
		upper_npc_name_string->Release();
		delete upper_npc_name_string; upper_npc_name_string = nullptr;
	}
	if (lower_npc_explain_string) {
		lower_npc_explain_string->Release();
		delete lower_npc_explain_string; lower_npc_explain_string = nullptr;
	}
	return true;
}

bool CUINPC::DeleteElement(CUI* element)
{
	return false;
}

bool CUINPC::SetScreenPos(float x, float y)
{
	npc_image->SetScreenPos(x, y);
	upper_npc_name_string->SetScreenPos(x + ((width - upper_npc_name_string->GetWidth()) / 2), y - 3 - upper_npc_name_string->GetHeight());
	lower_npc_explain_string->SetScreenPos(x + ((width - lower_npc_explain_string->GetWidth()) / 2), y + height + 3);
	for (CUIHandler::EventUI* event_ : events)
	{
		event_->x = x;
		event_->y = y;
	}
	return true;
}

CUIImage* CUINPC::GetNPCImage() { return npc_image; }

CUINPC::~CUINPC()
{
	if (npc_image) {
		npc_image->Release();
		delete npc_image; npc_image = nullptr;
	}
	if (upper_npc_name_string) {
		upper_npc_name_string->Release();
		delete upper_npc_name_string; upper_npc_name_string = nullptr;
	}
	if (lower_npc_explain_string) {
		lower_npc_explain_string->Release();
		delete lower_npc_explain_string; lower_npc_explain_string = nullptr;
	}
}
