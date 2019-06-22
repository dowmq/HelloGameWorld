#pragma once
#include "Data.h"
#include <string>
#include "DataProvider.h"
#define DataProvider__StringData(data_id) (*DataProvider<StringData>::Instance().GetData(data_id)).GetString()

class StringData : public Data
{
public:
	StringData();
	~StringData();

	bool		SetData(const std::string& lhs_type_name, const std::string& rhs_type_value) override;
	bool		Release() override;
	int			GetId() override;
	const std::string& GetString();

	static const std::string GetInitializeFileName();
private:
	int				string_id;
	std::string		str;
};