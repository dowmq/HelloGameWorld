#include "CItemDataProvider.h"
#include "TextFileParser.h"
#include "SpriteFactory.h"
#include "StringData.h"
std::map<size_t, CItem*>& CItemDataProvider::GetAllItems() {
	return items;
}

CItemDataProvider::CItemDataProvider()
{
	Initialize(DataProvider__StringData(2));
}


CItemDataProvider::~CItemDataProvider()
{
	for (auto& element : items)
	{
		delete element.second;
	}
	items.clear();
}

bool CItemDataProvider::Initialize(std::string filename)
{
	std::string sp("_sprites");
	std::string count("_item_count");
	std::string info("_info");
	std::vector<std::string> vec;
	vec.push_back("ring" + sp);
	vec.push_back("ring" + count);
	vec.push_back("ring" + info);
	vec.push_back("head" + sp);
	vec.push_back("head" + count);
	vec.push_back("head" + info);
	vec.push_back("weapon" + sp);
	vec.push_back("weapon" + count);
	vec.push_back("weapon" + info);
	vec.push_back("armor" + sp);
	vec.push_back("armor" + count);
	vec.push_back("armor" + info);
	vec.push_back("torch" + sp);
	vec.push_back("torch" + count);
	vec.push_back("torch" + info);
	vec.push_back("feet" + sp);
	vec.push_back("feet" + count);
	vec.push_back("feet" + info);
	try
	{
		TextFileParser parser(filename);
		for (int i = 0; i < vec.size(); ++i)
			parser.Register_expected_string(vec[i]);
		parser.Parse();

		std::vector<std::string> result;
		for (int i = 0; i < vec.size(); ++i)
			result.push_back(parser.GetString(vec[i]));

		for (int i = 0; i < result.size(); i += 3)
		{
			std::vector<Sprite*>* sprites = SpriteFactory::CreateSprites(result[i], std::atoi(result[i + 1].c_str()));
			auto iteminfolines = TextFileParser::GetSerialString(result[i + 2], '/');
			size_t sprite_index = 0;
			for (auto line : *iteminfolines)
			{
				CItem* item = new CItem();
				item->id = std::atoi(line[0].c_str());
				item->name = line[1];
				item->type = (CItem::ItemType)std::atoi(line[2].c_str());
				item->item_option[CItem::ItemOption::lvConstrain] = std::atoi(line[3].c_str());
				item->item_option[CItem::ItemOption::grade] = std::atoi(line[4].c_str());
				item->item_option[CItem::ItemOption::incAtt] = std::atoi(line[5].c_str());
				item->item_option[CItem::ItemOption::incDfn] = std::atoi(line[6].c_str());
				item->item_option[CItem::ItemOption::incSpeed] = std::atoi(line[7].c_str());
				//item->image = (*sprites)[i / 3];
				item->image = *(sprites->begin() + sprite_index);
				item->image->sprite_id = item->id;
				++sprite_index;

				items.emplace(item->id, item);
			}
			delete sprites;
		}
		return true;

	}
	catch (std::exception& e)
	{
		std::string str(e.what());
		std::wstring wstr(str.begin(), str.end());
		MessageBox(CWindow::Instance().m_hWnd, wstr.c_str(), TEXT("Exception"), MB_ICONERROR);
	}
	return false;
}

CItem* CItemDataProvider::GetItemByItemId(size_t itemid)
{
	try
	{
		return items.at(itemid)->Clone(); // new CItem(*items.at(itemid));
	}
	catch (std::exception& e)
	{
		std::string str(e.what());
		std::wstring wstr(str.begin(), str.end());
		MessageBox(CWindow::Instance().m_hWnd, wstr.c_str(), TEXT("Exception"), MB_ICONERROR);
	}
	return nullptr;
}