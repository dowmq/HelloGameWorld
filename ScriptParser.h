#pragma once
#include <string>
#include <vector>
#include <map>
#include "TextFileParser.h"

class ScriptParser
{
public:
	explicit ScriptParser(std::string filename);
	~ScriptParser();

	std::vector<std::map<std::string, std::string>*> GetScriptParseResult();

private:
	std::string trim(const std::string& str, const std::string& whitespaces = " \t\r\n");
	std::ifstream target_file;
};

