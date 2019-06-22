#pragma once
#include "CStage.h"
#include "CUI.h"
#include "TSingleton.h"
class CUIIndexStage;

class CIndexStage : public CStage, public TSingleton<CIndexStage>
{
	friend class TSingleton<CIndexStage>;
public:
	~CIndexStage();

	virtual bool			Init() override;
	virtual bool			Frame() override;
	virtual bool			Render() override;
	virtual bool			Release() override;

	bool					NewGame();
	bool					PrevGame();
	bool					Exit();
private:
	CIndexStage();
	CUIIndexStage*			ui;
};

