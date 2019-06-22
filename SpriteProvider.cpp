#include "SpriteProvider.h"
#include "TextFileParser.h"
#include "CWindow.h"
#include "SpriteFactory.h"
#include <string>

SpriteProvider::SpriteProvider()
{
	Initialize(DataProvider__StringData(1));
}

SpriteProvider::~SpriteProvider()
{
}

bool SpriteProvider::Initialize(std::string filename)
{
	try
	{
		auto result = TextFileParser::GetSerialString(filename, '/');
		for (auto& line : *(result.get()) )
		{
			stored_sprite_data.emplace(std::atoi(line[0].c_str()), 
				line[1]);
		}
	}
	catch (std::exception& e)
	{
		std::string str(e.what());
		std::wstring wstr(str.begin(), str.end());
		MessageBox(CWindow::Instance().m_hWnd, wstr.c_str(), TEXT("Exception"), MB_ICONERROR);
		return false;
	}
	return true;
}

Sprite* SpriteProvider::GetSprite(size_t id)
{
	try
	{
		Sprite* data = SpriteFactory::CreateSprite(stored_sprite_data.at(id));
		data->sprite_id = id;
		return data;
	}
	catch (std::exception& e)
	{
		std::string str(e.what());
		std::wstring wstr(str.begin(), str.end());
		MessageBox(CWindow::Instance().m_hWnd, wstr.c_str(), TEXT("Exception"), MB_ICONERROR);
		return nullptr;
	}
}

Sprite* SpriteProvider::GetUISpriteByUIID(UIID SpriteID)
{
	return GetSprite((int)SpriteID);
}

Sprite* SpriteProvider::GetCharacterSpriteBySpriteID(CharacterID SpriteID)
{
	return GetSprite((int)SpriteID);
}

Sprite* SpriteProvider::GetNPCSpriteBySpriteID(NPCID SpriteID)
{
	return GetSprite((int)SpriteID);
}

Sprite* SpriteProvider::GetMonsterSpriteBySpriteID(MonsterID SpriteID)
{
	return GetSprite((int)SpriteID);
}

Sprite* SpriteProvider::GetTileSpriteBySpriteID(TileID SpriteID)
{
	return GetSprite((int)SpriteID);
}

Sprite* SpriteProvider::GetETCSpriteBySpriteID(ETCID SpriteID)
{
	return GetSprite((int)SpriteID);
}