#include "MonsterFactory.h"
#include "CWindow.h"
#include <string>
#include <exception>
#include "SpriteProvider.h"

CMonster* MonsterFactory::CreateGoblin(float logical_x, float logical_y)
{
	try
	{
		CMonster* monster = new CMonster(logical_x, logical_y);
		monster->sprites[CMonster::MonsterSprite::walking_left] = 
			SpriteProvider::Instance().GetMonsterSpriteBySpriteID(SpriteProvider::MonsterID::Goblin_walk_left);
		monster->sprites[CMonster::MonsterSprite::walking_right] = 
			SpriteProvider::Instance().GetMonsterSpriteBySpriteID(SpriteProvider::MonsterID::Goblin_walk_right);
		monster->sprites[CMonster::MonsterSprite::stop_left] = 
			SpriteProvider::Instance().GetMonsterSpriteBySpriteID(SpriteProvider::MonsterID::Goblin_stop_left);
		monster->sprites[CMonster::MonsterSprite::stop_right] = 
			SpriteProvider::Instance().GetMonsterSpriteBySpriteID(SpriteProvider::MonsterID::Goblin_stop_right);
		monster->sprites[CMonster::MonsterSprite::die] = 
			SpriteProvider::Instance().GetMonsterSpriteBySpriteID(SpriteProvider::MonsterID::Goblin_die);

		return monster;
	}
	catch (std::exception& e)
	{
		std::string str(e.what());
		std::wstring wstr(str.begin(), str.end());
		MessageBox(CWindow::Instance().m_hWnd, wstr.c_str(), TEXT("Exception"), MB_ICONERROR);
	}
	return nullptr;
}