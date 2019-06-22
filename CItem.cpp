#include "CItem.h"
#include "CUIHandler.h"


CItem::CItem()
{
	static int oid = 30000;
	object_id = oid;
	oid++;
}


CItem::~CItem()
{
	if (image) {
		image->Release();
		delete image; image = nullptr;
	}
}

CItem* CItem::Clone()
{
	CItem* item = new CItem();
	item->id = this->id;
	item->name = this->name;
	item->type = this->type;
	item->image = this->image->Clone();
	item->item_option = this->item_option;

	return item;
}