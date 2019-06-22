#pragma once
#include "Sprite.h"
#include "CPlayerCharacter.h"
#include "CUIImage.h"
#include <array>
#include <string>


class CItem
{
	friend class CItemDataProvider;
	CItem();
	
	CItem& operator=(const CItem&) = delete;
public:
	CItem(const CItem&) = default;
	enum ItemType { ring, head, weapon, armor, torch, feet, consume, etc };
	enum ItemOption { lvConstrain, grade, incAtt, incDfn, incSpeed, option_count};
	
	~CItem();
	CItem*			Clone();

	int				GetObjectId() { return object_id; }
	Sprite*			GetSprite() { return image; }
	size_t			GetItemid() { return id; }
	std::string		GetItemName() { return name; }
	ItemType		GetType() { return type; }
	float			GetItemOption(CItem::ItemOption option_type) { return item_option[option_type];}
private:
	int				object_id;

	size_t			id;
	std::string		name;
	ItemType		type;
	std::array<float, ItemOption::option_count> item_option;

	Sprite*			image;
};

