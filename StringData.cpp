#include "StringData.h"



StringData::StringData()
{
}


StringData::~StringData()
{
}

bool StringData::Release()
{
	return true;
}

bool StringData::SetData(const std::string& lhs_type_name, const std::string& rhs_type_value)
{
	if (lhs_type_name == "StringID")
	{
		string_id = std::atoi(rhs_type_value.c_str());
	}
	else if (lhs_type_name == "StringValue")
	{
		str = rhs_type_value;
	}
	return true;
}

int StringData::GetId()
{
	return string_id;
}

const std::string& StringData::GetString()
{
	return str;
}

const std::string StringData::GetInitializeFileName() {
	return std::string("Data\\string_data.txt");
}