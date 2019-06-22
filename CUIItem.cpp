#include "CUIItem.h"
#include "SpriteFactory.h"
#include "CItemDataProvider.h"

CUIItem::CUIItem(size_t itemid, CUI* owner_) : CUIComponent(owner_)
{
	CItem* item = CItemDataProvider::Instance().GetItemByItemId(itemid);
}

bool CUIItem::Render()
{
	//BasicSprite->DrawAlpha(400, 500);
	return true;
}


CUIItem::~CUIItem()
{
	//delete BasicSprite;
	//BasicSprite = nullptr;
}
