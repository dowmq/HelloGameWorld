#include "CUIIndexStage.h"
#include "CUIImage.h"
#include "InputHandler.h"
#include "SpriteProvider.h"
CUIIndexStage::CUIIndexStage(CIndexStage& stage_, CUI* owner_) 
	: CUIComponent(owner_), stage(stage_)
{
	CUIImage* back = new CUIImage(SpriteProvider::UIID::index_stage_Background,
		0, 0, false, this);
	Component.push_back(back);

	CUIImage* new_game = new CUIImage(SpriteProvider::UIID::index_stage_new_game,
		0, 0, true, this);
	float width = new_game->GetWidth();
	float height = new_game->GetHeight();
	float screen_y = 350;
	new_game->SetScreenPos(400 - width/2, screen_y);
	events[IndexStageEvent::new_game] = CUIHandler::Instance().RegisterEventUI(this,
		[](CUI* ui) {
			if (InputHandler::Instance().is_left_click()) {
				CUIIndexStage* ui_stage = reinterpret_cast<CUIIndexStage*>(ui);
				ui_stage->GetStage().NewGame();
			}
		}, 400 - width / 2, screen_y, width, height);
	Component.push_back(new_game);
	screen_y += height;
	screen_y += 30;
	
	CUIImage* prev_game = new CUIImage(SpriteProvider::UIID::index_stage_prev_game,
		0, 0, true, this);
	width = prev_game->GetWidth();
	height = prev_game->GetHeight();
	prev_game->SetScreenPos(400 - width / 2, screen_y);
	events[IndexStageEvent::prev_game] = CUIHandler::Instance().RegisterEventUI(this,
		[](CUI* ui) {
			if (InputHandler::Instance().is_left_click()) {
				CUIIndexStage* ui_stage = reinterpret_cast<CUIIndexStage*>(ui);
				ui_stage->GetStage().PrevGame();
			}
		}, 400 - width / 2, screen_y, width, height);
	Component.push_back(prev_game);
	screen_y += height;
	screen_y += 30;

	CUIImage* exit = new CUIImage(SpriteProvider::UIID::index_stage_exit,
		0, 0, true, this);
	width = exit->GetWidth();
	height = exit->GetHeight();
	exit->SetScreenPos(400 - width / 2, screen_y);
	events[IndexStageEvent::exit] = CUIHandler::Instance().RegisterEventUI(this,
		[](CUI* ui) {
			if (InputHandler::Instance().is_left_click()) {
				CUIIndexStage* ui_stage = reinterpret_cast<CUIIndexStage*>(ui);
				ui_stage->GetStage().Exit();
			}
		}, 400 - width / 2, screen_y, width, height);
	Component.push_back(exit);
}

CIndexStage& CUIIndexStage::GetStage() { return stage; }


CUIIndexStage::~CUIIndexStage()
{
	for (auto& event_ : events) {
		CUIHandler::Instance().DeRegisterEventUI(event_->event_id);
	}
}