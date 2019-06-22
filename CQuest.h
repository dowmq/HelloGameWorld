#pragma once
//#include "QuestDataProvider.h"
#include <vector>
#include <string>
#include "QuestData.h"
class CPlayerCharacter;

class CQuest
{
public:
	enum class Status : unsigned long
	{
		is_not_started = 0,
		is_prossessing = 1,
		is_can_completed = 2,
		is_completed = 3
	};
	explicit CQuest(int qid, CPlayerCharacter& player_);
	explicit CQuest(QuestData& quest_data_, CPlayerCharacter& player_);
	~CQuest();

	bool				PlayerCanProgressQuest(CPlayerCharacter& player);
	bool				PlayerQuestCompleteGetReward();

	void				SetQuestStatus(Status q_status);
	Status				GetQuestStatus();
	const int			GetQuestID() const;
	const std::string&	GetQuestName() const;
	QuestData::Type		GetQuestType() const;
	std::vector<int>*	GetQuestAdditionalCondition() const;
	int					GetQuestStartConditionQuestID() const;
	int					GetQuestStartNPCID() const;
	int					GetQuestEndNPCID() const;
	int					GetQuestStartScriptID() const;
	int					GetQuestEndScriptID() const;
	int					GetQuestEXPReward() const;
	int					GetQuestItemReward() const;
	const std::string&	GetQuestStatusMessage() const;
	void				SetQuestStatusMessage(const std::string& message);
	void				SetQuestStatusMessage(std::string&& message);
private:
	void				QuestStatusUpdate(Status q_status);

	Status								quest_status;
	std::string							quest_status_message;
	const QuestData&					quest_data;
	CPlayerCharacter&					player;
};

