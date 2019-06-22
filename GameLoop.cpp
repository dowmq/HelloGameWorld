#include "GameLoop.h"
#include "CWindow.h"
#include "InputHandler.h"
#include "SoundHandler.h"
#include <Windows.h>
GameLoop::GameLoop()
{
	indexStage = &CIndexStage::Instance();
	gameStage = &CGameStage::Instance();
}


GameLoop::~GameLoop()
{
}

bool GameLoop::Run()
{
	if (!Init()) {
		return false;
	}
	MSG msg = { 0, };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			Input->MsgProc(msg);
		}
		else
		{
			Frame();
			Render();
		}
	}
	Release();
	return true;
}

bool GameLoop::Init()
{
	Input = &InputHandler::Instance();
	Timer = &TimeHandler::Instance();
	uihandler = &CUIHandler::Instance();

	SoundHandler::Instance().Init();

	currentStage = indexStage;
	currentStage->Init();

	Input->Init();
	Timer->Init();
	uihandler->Init();

	return true;
}

bool GameLoop::Frame()
{
	TimeHandler::Instance().Frame();
	InputHandler::Instance().Frame();
	SoundHandler::Instance().Frame();
	CUIHandler::Instance().Frame();
	currentStage->Frame();

	return true;
}

bool GameLoop::Render()
{
	PreRender();
	
	currentStage->Render();
	TimeHandler::Instance().Render();
	CUIHandler::Instance().Render();


	PostRender();
	return true;
}

// 자원 해제는 자원 생성의 역순으로 해야 안전하다.
bool GameLoop::Release()
{
	TimeHandler::Instance().Release();
	CUIHandler::Instance().Release();

	CWindow::Instance().Release();
	currentStage->Release();
	return true;
}


void GameLoop::PreRender()
{
	CWindow::Instance().WindowDeviceContext.PrepareRendering();
	return;
}

void GameLoop::PostRender()
{
	//if (m_bDebug == true)
	//{
	//	DebugRender();
	//}
	CWindow::Instance().WindowDeviceContext.Rendering_OffScreen();
	return;
}

bool GameLoop::StartNewGame()
{
	indexStage->Release();
	currentStage = gameStage;
	currentStage->Init();
	return true;
}

bool GameLoop::StartPrevGame()
{
	return true;
}