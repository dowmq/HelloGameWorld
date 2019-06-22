#include <Windows.h>
#include "CWindow.h"
#include "GameLoop.h"
#include "TextFileParser.h"
#include <iostream>
#include "SpriteProvider.h"
#include "CItemDataProvider.h"
#include "StringData.h"
#include "NPCTalkScriptData.h"
#include "QuestData.h"
#include "NPCData.h"
#include "SoundData.h"
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	DataProvider<StringData>::Instance();
	DataProvider<NPCTalkScriptData>::Instance();
	DataProvider<QuestData>::Instance();
	DataProvider<NPCData>::Instance();
	DataProvider<SoundData>::Instance();
	SpriteProvider::Instance();
	CItemDataProvider::Instance();

	auto& window = CWindow::Instance();
	std::string title = DataProvider__StringData(7);
	std::wstring w_title(title.begin(), title.end());
	window.InitWindow(hInstance, w_title.c_str(), 0, 0);
	window.MyRegisterClass();
	window.CenterWindow();
	GameLoop::Instance().Run();
}
