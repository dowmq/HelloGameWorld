#include "TextFileParser.h"
#include <exception>
#include <stdexcept>
#include <algorithm>
TextFileParser::TextFileParser(std::string filename)
{
	target_file.open(filename.c_str());
	if (!target_file.is_open()) {
		throw std::logic_error("File Open Fail");
	}
	equals_sign = '=';
	pass_char.push_back('/');
	is_parsed = false;
	number_of_parsed_line = 0;
	number_of_elements = 0;
}

TextFileParser::~TextFileParser()
{
	if (target_file.is_open()) {
		target_file.close();
	}
}

void TextFileParser::Register_pass_char(char some_char)
{
	pass_char.push_back(some_char);
}

void TextFileParser::Register_expected_string(std::string word)
{
	wishlist.push_back(word);
}

void TextFileParser::Set_equals_sign(char some_char)
{
	equals_sign = some_char;
}

void TextFileParser::Parse()
{
	while (target_file.eof() == false)
	{
		char data[256] = { 0, };
		target_file.getline(data, 256);

		// check comment line;
		if (is_comment_line(data[0])) {
			continue;
		}

		std::string lines(data);
		int find_result = 0;

		find_result = lines.find_first_of(equals_sign);
		if (find_result == lines.npos)
			continue;
		std::string lhs = lines.substr(0, find_result);
		lhs.erase(std::remove_if(lhs.begin(), lhs.end(), ::isspace), lhs.end());
		std::string rhs = lines.substr(find_result + 1);
		rhs.erase(std::remove_if(rhs.begin(), rhs.end(), ::isspace), rhs.end());

		expected_result_list.insert(std::make_pair(lhs, rhs));
	}
}

bool TextFileParser::is_comment_line(char data_0)
{
	if (pass_char.empty() == false)
	{
		bool is_comment = false;
		for (auto char_ : pass_char) {
			if (data_0 == char_) {
				is_comment = true;
			}
		}
		if (is_comment == true)
			return true;
	}
	return false;
}

std::string TextFileParser::GetString(std::string registered_word)
{
	// if registered  word doesn't exist in the map container
	// the operator[] will make pair{ registered_word, std::string("") }
	// then return that gabage result and that maybe be not a reasonable.
	// that situation, the at() will throw out_of_range exception.
	return expected_result_list.at(registered_word);
}

std::unique_ptr<std::vector<std::vector<std::string>>> TextFileParser::GetSerialString
(std::string filename, char comment_word)
{
	std::ifstream inFile(filename.c_str());
	if (!inFile.is_open()) {
		throw OpenFailException(filename.c_str());
	}
	auto result = std::make_unique<std::vector<std::vector<std::string>>>();
	while (!inFile.eof())
	{
		char Data[256] = { 0, };
		memset(Data, 0, 256);

		inFile.getline(Data, 256);

		// if a first word is the comment word, then continue next line;
		if (Data[0] == comment_word) {
			continue;
		}
		std::string lines(Data);
		std::vector<std::string> textArray;
		int cutAt = 0;
		while ((cutAt = lines.find_first_of(' ')) != lines.npos)
		{
			if (cutAt > 0)
			{
				textArray.push_back(lines.substr(0, cutAt));
			}
			lines = lines.substr(cutAt + 1);
		}
		if (lines.empty() == false) {
			textArray.push_back(lines);
		}
		result->push_back(textArray);
	}
	return std::move(result);
}