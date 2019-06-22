#include "CIndexStage.h"
#include "CUIIndexStage.h"
#include "CWindow.h"
#include "GameLoop.h"
#include "SoundHandler.h"
CIndexStage::CIndexStage()
{
	ui = new CUIIndexStage(*this, nullptr);
}


CIndexStage::~CIndexStage()
{
	if (ui) {
		ui->Release();
		delete ui; ui = nullptr;
	}
}


bool CIndexStage::Init()
{
	SoundHandler::Instance().Play(SoundData::Type::MureungHill);
	ui->Init();
	return true;
}

bool CIndexStage::Frame()
{
	ui->Frame();
	return true;
}

bool CIndexStage::Render()
{
	ui->Render();
	return true;
}

bool CIndexStage::Release()
{
	if (ui) {
		ui->Release();
		delete ui; ui = nullptr;
	}
	SoundHandler::Instance().Stop(SoundData::Type::MureungHill);
	return true;
}

bool CIndexStage::NewGame()
{
	GameLoop::Instance().StartNewGame();
	return true;
}

bool CIndexStage::PrevGame()
{
	GameLoop::Instance().StartPrevGame();
	return true;
}

bool CIndexStage::Exit()
{
	SendMessage(CWindow::Instance().m_hWnd, WM_DESTROY, 0, 0);
	return true;
}