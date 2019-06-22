#pragma once
#include <vector>
#include <string>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <memory>
#include <map>

class StringParseException : public std::exception
{
public:
	StringParseException(const char* msg_) : std::exception{ msg_ } {}
};

class OpenFailException : public StringParseException
{
public:
	OpenFailException(const char* msg) : StringParseException(msg) {}
};

class TextFileParser
{
public:
public:
	// The TextFileParser will parse line at once.
	// two expected value in one line will result in undefined behavior.
	explicit TextFileParser(std::string filename);
	~TextFileParser();

	// If the Parser encounter the pass_char in the frist of the line, 
	// then Parser will not parse and go to the next line.
	// the basic pass_char is '/'
	void Register_pass_char(char some_char);

	// You can set equals sign char list. basic equals sign = '='
	void Set_equals_sign(char sign);

	// You can register the "expected word"
	// if the parse completed, you can get the result
	// example) Register_word_to_be_expected("ip") and 
	// the file has "ip = 127.0.0.1", you will get "127.0.0.1"
	// basically '=' is the equals sign, and you don't wish, call Set_equals_sign()
	void Register_expected_string(std::string word);

	void Parse();

	// the string you are expecting isn't in the container, the method will throw std::out_of_range exception.
	std::string GetString(std::string registered_word);
private:
	bool									is_comment_line(char data_0);

	std::ifstream							target_file;
	bool									is_parsed;
	size_t									number_of_parsed_line;
	size_t									number_of_elements;
	std::vector<char>						pass_char;
	char									equals_sign;
	std::vector<std::string>				wishlist;
	std::map<std::string, std::string>		expected_result_list;

public:
	// Use when you need Serial String in Text File
	// if comment word is 0, the result is undefined.
	// if file open is failed, then throws OpenFailException
	static std::unique_ptr<std::vector<std::vector<std::string>>> GetSerialString
	(std::string filename, char comment_word);
};


