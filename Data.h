#pragma once
#include <string>
class Data
{
public:
	virtual ~Data() = default;
	virtual bool	SetData(const std::string& lhs_type_name, const std::string& rhs_type_value) = 0;
	virtual bool	Release() = 0;
	virtual int		GetId() = 0;
};