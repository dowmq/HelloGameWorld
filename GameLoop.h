#pragma once
#include <vector>
#include "CIndexStage.h"
#include "CGameStage.h"
#include "TSingleton.h"
#include "CWindow.h"
#include "TimeHandler.h"
#include "InputHandler.h"
#include "CField.h"
#include "CUIHandler.h"


class GameLoop : public TSingleton<GameLoop>
{
	friend class TSingleton<GameLoop>;
public:
	bool Run();

	GameLoop();
	~GameLoop();

	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();

	void			PreRender();
	void			PostRender();


	bool			StartNewGame();
	bool			StartPrevGame();

private:
	InputHandler*	Input;
	TimeHandler*	Timer;
	CUIHandler*		uihandler;

	CIndexStage*	indexStage;
	CGameStage*		gameStage;
	CStage*			currentStage;
};

