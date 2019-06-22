#include "ScriptParser.h"
#include <Windows.h>
#include "CWindow.h"
#include <algorithm>

ScriptParser::ScriptParser(std::string filename)
{
	target_file.open(filename.c_str());
	if (!target_file.is_open()) {
		throw std::logic_error("File Open Fail");
	}
}


ScriptParser::~ScriptParser()
{
	if (target_file.is_open()) {
		target_file.close();
	}
}

std::vector<std::map<std::string, std::string>*> ScriptParser::GetScriptParseResult()
{
	std::vector<std::map<std::string, std::string>*> ParseResult;
	try {
		std::string start("Start");
		std::string end("End");
		std::map<std::string, std::string>* context = nullptr;
		while (target_file.eof() == false)
		{
			char data[256] = { 0, };
			target_file.getline(data, 256);

			std::string lines(data);
			if (lines == start)
			{
				context = new std::map<std::string, std::string>();
				continue;
			}
			else if (lines == end)
			{
				ParseResult.push_back(context);
				context = nullptr;
				continue;
			}
			int find_result = 0;

			find_result = lines.find_first_of('=');
			if (find_result == lines.npos)
				continue;
			std::string lhs = trim(lines.substr(0, find_result));
			std::string rhs = trim(lines.substr(find_result + 1));
			context->emplace(lhs, rhs);
		}
		return std::move(ParseResult);
	}
	catch (std::exception& e)
	{
		std::string str(e.what());
		std::wstring wstr(str.begin(), str.end());
		MessageBox(CWindow::Instance().m_hWnd, wstr.c_str(), TEXT("Exception"), MB_ICONERROR);
		return std::move(ParseResult);
	}
}

std::string ScriptParser::trim(const std::string& str, const std::string& whitespaces)
{
	std::string result("");

	size_t begin = str.find_first_not_of(whitespaces);
	size_t end = str.find_last_not_of(whitespaces);
	if (begin != std::string::npos && end != std::string::npos)
		result = str.substr(begin, end - begin + 1);

	return result;
}