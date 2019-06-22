#pragma once
#include <vector>
#include <unordered_map>
#include "CStage.h"
#include "CPlayerCharacter.h"
#include "CField.h"
#include "CUIFieldInfo.h"
#include "CUIQuestInfo.h"
class CUIGameStage;

class CGameStage : public CStage, public TSingleton<CGameStage>
{
	friend class TSingleton<CGameStage>;
public:
	~CGameStage();
	virtual bool						Init() override;
	virtual bool						Frame() override;
	virtual bool						Render() override;
	virtual bool						Release() override;

	// �ʵ带 ������ �ΰ� �ʵ带 �ٲٰ� �÷��̾ set�ϴ� �� �߰��ؾ��ҵ�.
	bool								PlayerChangeField(int field_id, float new_logical_x, float new_logical_y);

	CField&								GetField(int field_id);
	CUIGameStage&						GetGameUI();
	CUIFieldInfo&						GetFieldInfoUI();
	CUIQuestInfo&						GetQuestInfoUI();
private:
	CGameStage();
	CPlayerCharacter*	player;
	CUIGameStage*		game_ui;
	CField*				current_field;
	CUIFieldInfo*		field_info;
	CUIQuestInfo*		quest_info;

	std::unordered_map<size_t, CField*> stored_field;
};

