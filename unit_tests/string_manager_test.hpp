#include <cxxtest/TestSuite.h>
#include "../utility_module/strings/string_manager.hpp"
#include "../utility_module/strings/string_manager.cpp"

using namespace utility_space;

#define TESTEDSTRING "Hello world!\0"

class StringManagerTest : public CxxTest::TestSuite
{
public:
	void testParseByDelimiter(void)
	{
		std::vector<std::string> parsedString 
					= StringManager::parseByDelimiter(TESTEDSTRING, " ");
		
		TS_ASSERT_EQUALS(parsedString.size(), 2);
		TS_ASSERT_EQUALS(parsedString[0], "Hello");
		TS_ASSERT_EQUALS(parsedString[1], "world!");

		parsedString = StringManager::parseByDelimiter(TESTEDSTRING, "l");
		TS_ASSERT_EQUALS(parsedString.size(), 3);
		TS_ASSERT_EQUALS(parsedString[0], "He");
		TS_ASSERT_EQUALS(parsedString[1], "o wor");
		TS_ASSERT_EQUALS(parsedString[2], "d!");
	}

	void testDeleteSymbols(void)
	{
		std::string resultString;
		resultString = StringManager::deleteSymbols(TESTEDSTRING, "o");
		TS_ASSERT_EQUALS(resultString, "Hell wrld!");

		resultString = StringManager::deleteSymbols(TESTEDSTRING, "ol");
		TS_ASSERT_EQUALS(resultString, "He wrd!");

		resultString = StringManager::deleteSymbol(TESTEDSTRING, symbolType::SPACE);
		TS_ASSERT_EQUALS(resultString, "Helloworld!")

		std::vector<symbolType> symbolsVec = {symbolType::SPACE, symbolType::DQUOTE};
		resultString = StringManager::deleteSymbols(TESTEDSTRING, symbolsVec);
		TS_ASSERT_EQUALS(resultString, "Helloworld!");
	}

	void testCropFromEnd(void)
	{
		std::string resultString;
		resultString = StringManager::cropFromEnd(TESTEDSTRING, symbolType::SPACE);
		TS_ASSERT_EQUALS(resultString, "Hello");
	}

	void testCropFromBegin(void)
	{
		std::string resultString;
		resultString = StringManager::cropFromBegin(TESTEDSTRING, symbolType::SPACE);
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