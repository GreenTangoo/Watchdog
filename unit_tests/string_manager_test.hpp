#ifndef STRING_MANAGER_TEST_HPP
#define STRING_MANAGER_TEST_HPP

#include <vector>
#include <string>

#include <cxxtest/TestSuite.h>
#include "../utility_module/string_manager.hpp"
#include "../utility_module/string_manager.cpp"

using namespace utility_space;

#define TESTED_STRING "Hello world!\0"

class StringManagerTest : public CxxTest::TestSuite
{
public:
	void testGetSubStr(void)
	{
		std::string resusltSubStr = StringManager::getStrBetweenSymbols(TESTED_STRING, ' ', '!');
		TS_ASSERT_EQUALS(resusltSubStr, "world");

		resusltSubStr = StringManager::getStrBetweenSymbols(TESTED_STRING, 'H', '\0');
		TS_ASSERT_EQUALS(resusltSubStr, "ello world!");

		resusltSubStr = StringManager::getStrBetweenSymbols(TESTED_STRING, '[', ']');
		TS_ASSERT_EQUALS(resusltSubStr, "");
	}

	void testParseByDelimiter(void)
	{
		std::vector<std::string> parsedString 
					= StringManager::parseByDelimiter(TESTED_STRING, " ");
		
		TS_ASSERT_EQUALS(parsedString.size(), 2);
		TS_ASSERT_EQUALS(parsedString[0], "Hello");
		TS_ASSERT_EQUALS(parsedString[1], "world!");

		parsedString = StringManager::parseByDelimiter(TESTED_STRING, "l");
		TS_ASSERT_EQUALS(parsedString.size(), 3);
		TS_ASSERT_EQUALS(parsedString[0], "He");
		TS_ASSERT_EQUALS(parsedString[1], "o wor");
		TS_ASSERT_EQUALS(parsedString[2], "d!");
	}

	void testDeleteSymbols(void)
	{
		std::string resultString;
		resultString = StringManager::deleteSymbols(TESTED_STRING, "o");
		TS_ASSERT_EQUALS(resultString, "Hell wrld!");

		resultString = StringManager::deleteSymbols(TESTED_STRING, "ol");
		TS_ASSERT_EQUALS(resultString, "He wrd!");

		resultString = StringManager::deleteSymbol(TESTED_STRING, symbolType::SPACE);
		TS_ASSERT_EQUALS(resultString, "Helloworld!")

		std::vector<symbolType> symbolsVec = {symbolType::SPACE, symbolType::DQUOTE};
		resultString = StringManager::deleteSymbols(TESTED_STRING, symbolsVec);
		TS_ASSERT_EQUALS(resultString, "Helloworld!");
	}

	void testCropFromEnd(void)
	{
		std::string resultString;
		resultString = StringManager::cropFromEnd(TESTED_STRING, symbolType::SPACE);
		TS_ASSERT_EQUALS(resultString, "Hello");
	}

	void testCropFromBegin(void)
	{
		std::string resultString;
		resultString = StringManager::cropFromBegin(TESTED_STRING, symbolType::SPACE);
		TS_ASSERT_EQUALS(resultString, "world!");
	}

	void testConstructPath(void)
	{
		std::string resultString;
		std::vector<std::string> pathElements = { "", "home", "naruto", "Downloads"};
		resultString = StringManager::constructPath(symbolType::SLASH, pathElements);
		TS_ASSERT_EQUALS(resultString, "/home/naruto/Downloads");
	}
};

#endif // STRING_MANAGER_TEST_HPP