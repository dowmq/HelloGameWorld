#pragma once
#include "CUI.h"
#include "CUIHandler.h"
#include "CUIString.h"
#include <array>
#include "NPCData.h"
class CUINPCTalk;

class CUINPC : public CUI
{
public:
	enum CUINPCEvent { double_clicked, count };
	CUINPC(const NPCData& npc_data_, float screen_x_, float screen_y_, CUI* owner);
	~CUINPC();

	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;

	virtual bool		DeleteElement(CUI* element) override;
	virtual bool		SetScreenPos(float x, float y) override;

	CUIImage*			GetNPCImage();
	const NPCData&		npc_data;
	CUIImage*			npc_image;
	CUIString*			upper_npc_name_string;
	CUIString*			lower_npc_explain_string;
	std::array< CUIHandler::EventUI*, CUINPCEvent::count> events;
};

