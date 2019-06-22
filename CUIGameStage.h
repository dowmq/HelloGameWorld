#pragma once
#include "CUIComponent.h"
#include "CGameStage.h"
#include "CPlayerCharacter.h"
#include <array>
class CUIGameStage : public CUIComponent
{
public:
	enum GameUIType {
		UIBackground, // 아래쪽 UI 나오는 걸 검게 칠함
		CharacterInfo,
		ChatLog,
		Inventory,
		//FieldInfo,
		
		count
	};

	CUIGameStage(CGameStage& stage_, CUI* owner_);
	~CUIGameStage() = default;

	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;

	bool				UpdateCharacterStatUI(CPlayerCharacter::CharacterStat stat, float value);
	CGameStage&			GetStage();
private:
	CGameStage&			stage;

	std::array<CUI*, GameUIType::count> game_UIs;
};

