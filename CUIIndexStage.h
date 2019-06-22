#pragma once
#include "CUIComponent.h"
#include "CIndexStage.h"
#include "CUIHandler.h"
#include <array>
class CUIIndexStage : public CUIComponent
{
	enum IndexStageEvent {
		new_game,
		prev_game,
		exit,
		size_of_event
	};
public:
	CUIIndexStage(CIndexStage& stage_, CUI* owner_);
	~CUIIndexStage();

	CIndexStage&		GetStage();
private:
	CIndexStage&		stage;
	std::array<CUIHandler::EventUI*, IndexStageEvent::size_of_event> events;
};

