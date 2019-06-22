#pragma once
#include <unordered_map>
#include <algorithm>
#include "TSingleton.h"
#include "ScriptParser.h"
#include "CWindow.h"

// Data type�� ���� 1. static const std::string Data::GetInitializeFileName(); �Լ��� �����ؾ� �Ѵ�.
// Data type�� ���� 2. �⺻ �����ڰ� �����ؾ� �Ѵ�.
// Data type�� ���� 3. Init(), Release() ��� �Լ��� �����ؾ���.
// Data type�� ���� 4. SetData(const std::string&, const std::string&) �Լ��� �־����
// Data type�� ���� 5. GetId() �Լ��� �־�� ��.
template<typename Data>
class DataProvider : public TSingleton<DataProvider<Data>>
{
	friend class TSingleton<DataProvider<Data>>;
public:
	DataProvider();
	~DataProvider();
	Data* GetData(int data_id);
	const std::unordered_map<size_t, Data*>& GetAllData() const;
private:
	bool Initialize(std::string filename);
	std::unordered_map<size_t, Data*> stored_data;
};

template<typename Data>
DataProvider<Data>::DataProvider()
{
	Initialize(Data::GetInitializeFileName());
}

template<typename Data>
DataProvider<Data>::~DataProvider()
{
	for (auto& element : stored_data)
	{
		element.second->Release();
		delete element.second;
	}
}

template<typename Data>
bool DataProvider<Data>::Initialize(std::string filename)
{
	ScriptParser parser(filename);
	std::vector< std::map<std::string, std::string>* > result = parser.GetScriptParseResult();

	for (auto& parse_data_map : result) // parse_data_map - std::map<std::string, std::string>*
	{
		Data* data = new Data();
		for (auto pair_string_string : *parse_data_map)
		{
			data->SetData(pair_string_string.first, pair_string_string.second);
		}
		stored_data.emplace(data->GetId(), data);
	}
	for (auto& parse_data_map : result)
	{
		delete parse_data_map;
	}
	return true;
}

template<typename Data>
Data* DataProvider<Data>::GetData(int data_id)
{
	auto iter = stored_data.find(data_id);
	if (iter != stored_data.end())
	{
		return (*iter).second;
	}
	return nullptr;
}

template<typename Data>
const std::unordered_map<size_t, Data*>& DataProvider<Data>::GetAllData() const
{
	return stored_data;
}