#include "CUIGameStage.h"
#include "CUIChatLog.h"
#include "CUICharacterInfo.h"
#include "CUIInventory.h"
#include "CUIBackground.h"


CUIGameStage::CUIGameStage(CGameStage& stage_, CUI* owner_)
	: CUIComponent(owner_), stage(stage_)
{
}

CGameStage& CUIGameStage::GetStage() { return stage; }

bool CUIGameStage::Init()
{
	game_UIs[GameUIType::UIBackground] = new CUIBackground(0, 480, 800, 450, BackgroundColor::black, this);
	game_UIs[GameUIType::ChatLog] = new CUIChatLog(160, 485, 800, 105, this);
	game_UIs[GameUIType::CharacterInfo] = new CUICharacterInfo(5, 485, this);
	game_UIs[GameUIType::Inventory] = new CUIInventory(485, 485, this);

	for (auto& element : game_UIs)
	{
		Component.push_back(element);
	}
	return CUIComponent::Init();
}

bool CUIGameStage::Frame()
{
	return CUIComponent::Frame();
}

bool CUIGameStage::Render()
{
	return CUIComponent::Render();
}

bool CUIGameStage::Release()
{
	return CUIComponent::Release();
}

bool CUIGameStage::UpdateCharacterStatUI(CPlayerCharacter::CharacterStat stat, float value)
{
	CUICharacterInfo* characterinfo = reinterpret_cast<CUICharacterInfo*>(game_UIs[GameUIType::CharacterInfo]);
	characterinfo->SetCharacterStat(stat, value);
	return true;
}