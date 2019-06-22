#include "CFieldFactory.h"
#include "CBackground.h"
#include "MonsterFactory.h"
#include "StringData.h"
#include "CArea.h"
#include "CPortal.h"
#include "SoundData.h"
CField* CFieldFactory::CreateStartMap()
{
	CField* field = new CField();
	field->field_id = 0;
	field->SetFieldName(DataProvider__StringData(54));
	field->SetLevelOfDifficulty(DataProvider__StringData(55));
	field->SetFieldMusicId(SoundData::Type::AboveTheTreetops);

	field->RegisterFieldBackground(new CBackground(0, 600, SpriteProvider::ETCID::sky));

	CTile* tile = CTile::CreateCTile(SpriteProvider::TileID::wood_row, 150, 210);
	CNPC* codex = CNPC::CreateNPC(NPCData::NPCID::Codex, 0, 0);
	CArea& tile_area = tile->GetArea();
	codex->GetArea().SetLogicalPos(
		tile_area.GetLogicalPos().x + tile->GetWidth() / 2 - codex->GetWidth() / 2,
		tile_area.GetLogicalPos().y + codex->GetHeight());
	field->RegisterNPC(codex);
	field->RegisterTile(tile);

	CPortal* portal = new CPortal(1, -200, 95, 30, 50);
	field->RegisterPortal(portal);

	for (int i = -14; i < 15; ++i)
	{
		CTile* clone_bottom = tile->Clone();
		clone_bottom->GetArea().Init(i * 30, 0);
		field->RegisterTile(clone_bottom);
		CTile* clone_top = tile->Clone();
		clone_top->GetArea().Init(i * 30, 600);
		field->RegisterTile(clone_top);
	}
	for (size_t i = 1; i <= 20; ++i) {
		CTile* clone_left = tile->Clone();
		clone_left->GetArea().Init(-420, i * 30);
		field->RegisterTile(clone_left);
		CTile* clone_right = tile->Clone();
		clone_right->GetArea().Init(420, i * 30);
		field->RegisterTile(clone_right);
	}
	field->SetFieldGravity(325.0f);
	return field;
}

CField* CFieldFactory::CreateGoblinCastle()
{
	CField* field = new CField();
	field->field_id = 1;
	field->SetFieldName(DataProvider__StringData(56));
	field->SetLevelOfDifficulty(DataProvider__StringData(55));
	field->RegisterFieldBackground(new CBackground(0, 600, SpriteProvider::ETCID::back_cave));
	field->RegisterFieldBackground(new CBackground(0, 600, SpriteProvider::ETCID::back_castle));
	field->SetFieldMusicId(SoundData::Type::CrimsonTower);

	field->RegisterNPC(CNPC::CreateNPC(NPCData::NPCID::Conjurer, -200, 40));
	field->RegisterNPC(CNPC::CreateNPC(NPCData::NPCID::DungeonMaster, 200, 50));

	CPortal* portal = new CPortal(0, 0, 95, -170, 50);
	field->RegisterPortal(portal);

	CMonster* monster = MonsterFactory::CreateGoblin(700, 120);
	field->RegisterRespawnMonster(monster, 6000);
	monster = MonsterFactory::CreateGoblin(800, 120);
	field->RegisterRespawnMonster(monster, 6000);
	monster = MonsterFactory::CreateGoblin(900, 120);
	field->RegisterRespawnMonster(monster, 6000);

	CTile* tile = CTile::CreateCTile(SpriteProvider::TileID::normal, -330, 0);
	field->RegisterTile(tile);

	int last_x = 0;
	int last_y = 0;
	int x = 0;
	for (x = -10; x < 10; ++x)
	{
		CTile* clone = tile->Clone();
		clone->GetArea().Init(x * 30, 0);
		field->RegisterTile(clone);
	}
	for (int i = 0; i < 20; ++i)
	{
		CTile* clone = tile->Clone();
		clone->GetArea().Init(-330, (i + 1) * 30 );
		field->RegisterTile(clone);
	}
	last_x = x * 30;
	
	int y = 0;
	for (y = 1; y <= 1; ++y)
	{
		CTile* clone = tile->Clone();
		clone->GetArea().Init(last_x - 30, y * 30);
		field->RegisterTile(clone);
	}
	last_y = y * 30;

	for (unsigned i = 0; i <= 30; ++i)
	{
		CTile* clone = reinterpret_cast<CTile*>(tile->Clone());
		clone->GetArea().Init(last_x + i * 30 - 30, last_y);
		field->RegisterTile(clone);
	}
	field->SetFieldGravity(325.0f);
	return field;
}