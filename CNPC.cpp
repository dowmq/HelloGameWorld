#include "CNPC.h"
#include "CUINPC.h"

int			CNPC::GetNPCID() { return npc_data.npc_id; }
int			CNPC::GetNPCScriptID() { return npc_data.npc_script_id; }
int			CNPC::GetNPCSpriteID() { return npc_data.npc_sprite_id; }
std::string CNPC::GetNPCName() { return npc_data.npc_name; }
CUINPC&					CNPC::GetNPCUI() { return *npc_ui; }
CUINPCQuestStatus&		CNPC::GetNPCUpperQuestUI() { return *npc_upper_quest_ui; }

CNPC::CNPC(NPCData::NPCID npcid, float logical_x, float logical_y)
	: npc_data(DataProvider__NPCData(npcid))
{
	//area.Init(logical_x, logical_y);
	area.FieldObjectInit(logical_x, logical_y);
	npc_ui = new CUINPC(npc_data, area.screen_point.x, area.screen_point.y, nullptr);
	npc_upper_quest_ui = new CUINPCQuestStatus(*this, nullptr);
}

CNPC::~CNPC()
{
	if (npc_ui != nullptr) {
		npc_ui->Release();
		delete npc_ui;
	}
	if (npc_upper_quest_ui != nullptr) {
		npc_upper_quest_ui->Release();
		delete npc_upper_quest_ui;
	}
}


bool CNPC::Init()
{
	npc_ui->Init();
	return true;
}

bool CNPC::Frame()
{
	npc_ui->Frame();
	if (npc_upper_quest_ui != nullptr)
		npc_upper_quest_ui->Frame();
	return true;
}

bool CNPC::Position_X_Frame()
{
	return true;
}

bool CNPC::Position_Y_Frame()
{
	return true;
}

bool CNPC::Render()
{
	npc_ui->Render();
	if (npc_upper_quest_ui != nullptr)
		npc_upper_quest_ui->Render();
	return true;
}

bool CNPC::Release()
{
	if (npc_ui != nullptr) {
		npc_ui->Release();
		delete npc_ui;
	}
	if (npc_upper_quest_ui != nullptr) {
		npc_upper_quest_ui->Release();
		delete npc_upper_quest_ui;
	}
	return true;
}

bool CNPC::EffectedbyGravity(float gravity)
{
	return true;
}

fPoint& CNPC::ModifyScreenPointFromLogicalToScreen()
{
	fPoint& point = CFieldObject::ModifyScreenPointFromLogicalToScreen();
	npc_ui->SetScreenPos(point.x, point.y);
	npc_upper_quest_ui->SetScreenPos(point.x, point.y);
	return point;
}

CNPC* CNPC::CreateNPC(NPCData::NPCID npcid, float logical_x, float logical_y)
{
	return new CNPC(npcid, logical_x, logical_y);
}

bool CNPC::SetNPCUpperQuestUI(CUINPCQuestStatus::Status status, CQuest& quest)
{
	if (npc_upper_quest_ui == nullptr)
		return false;
	npc_upper_quest_ui->SetNPCQuestUIStatus(status, quest);
	return true;
}

bool CNPC::ResetNPCUpperQuestUI()
{
	if (npc_upper_quest_ui != nullptr) {
		npc_upper_quest_ui->Release();
		delete npc_upper_quest_ui;
		npc_upper_quest_ui = nullptr;
		npc_upper_quest_ui = new CUINPCQuestStatus(*this, nullptr);
		return true;
	}
	return false;
}

float CNPC::GetWidth() {
	return npc_ui->GetWidth();
}

float CNPC::GetHeight() {
	return npc_ui->GetHeight();
}