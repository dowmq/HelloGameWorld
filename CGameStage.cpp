#include "CGameStage.h"
#include "CUIGameStage.h"
#include "CFieldFactory.h"
#include "SoundHandler.h"
CGameStage::CGameStage()
{
	player = &CPlayerCharacter::Instance();
	game_ui = new CUIGameStage(*this, nullptr);

	current_field = CFieldFactory::Instance().CreateStartMap();
	current_field->Init();
	stored_field.insert(std::make_pair(current_field->GetFieldId(), current_field));

	auto gobline_castle = CFieldFactory::Instance().CreateGoblinCastle();
	gobline_castle->Init();
	stored_field.insert(std::make_pair(gobline_castle->GetFieldId(), gobline_castle));


	player->SetField(current_field);
	player->UpdateFieldNPCUpperQuestUI();

	field_info = new CUIFieldInfo(*current_field);
	quest_info = new CUIQuestInfo(nullptr);
}

bool CGameStage::Init()
{
	game_ui->Init();
	player->Init();
	current_field->Init();
	field_info->Init();
	quest_info->Init();
	current_field->PlayerEnterTheMap();
	return true;
}

bool CGameStage::Frame()
{
	current_field->Frame();
	game_ui->Frame();
	field_info->Frame();
	quest_info->Frame();
	return true;
}

bool CGameStage::Render()
{
	current_field->Render();
	game_ui->Render();
	field_info->Render();
	quest_info->Render();
	return true;
}

bool CGameStage::Release()
{
	for (auto& pair_id_CField : stored_field) {
		if (pair_id_CField.second != nullptr) {
			pair_id_CField.second->Release();
			delete pair_id_CField.second; pair_id_CField.second = nullptr;
		}
	}
	stored_field.clear();
	if (game_ui) {
		game_ui->Release();
		delete game_ui; game_ui = nullptr;
	}
	if (field_info) {
		field_info->Release();
		delete field_info; field_info = nullptr;
	}
	if (quest_info) {
		quest_info->Release();
		delete quest_info; quest_info = nullptr;
	}
	player->Release();
	return true;
}

CGameStage::~CGameStage()
{
	for (auto& pair_id_CField : stored_field) {
		if (pair_id_CField.second != nullptr) {
			pair_id_CField.second->Release();
			delete pair_id_CField.second;
		}
	}
	stored_field.clear();
	if (game_ui) {
		game_ui->Release();
		delete game_ui; game_ui = nullptr;
	}
	if (field_info) {
		field_info->Release();
		delete field_info; field_info = nullptr;
	}
	if (quest_info) {
		quest_info->Release();
		delete quest_info; quest_info = nullptr;
	}
}

CUIGameStage& CGameStage::GetGameUI()
{
	return *game_ui;
}

CUIFieldInfo& CGameStage::GetFieldInfoUI()
{
	return *field_info;
}

CUIQuestInfo& CGameStage::GetQuestInfoUI()
{
	return *quest_info;
}

bool CGameStage::PlayerChangeField(int field_id, float new_logical_x, float new_logical_y)
{
	CField* new_field = stored_field.at(field_id);
	new_field->Init();

	current_field->PlayerLeaveTheMap();
	current_field = new_field;
	current_field->PlayerEnterTheMap();

	field_info->Update(*new_field);

	player->SetField(current_field);
	player->SetLogicalPoint(new_logical_x, new_logical_y);
	player->UpdateFieldNPCUpperQuestUI();
	
	return true;
}

CField& CGameStage::GetField(int field_id) {
	return *stored_field[field_id];
}