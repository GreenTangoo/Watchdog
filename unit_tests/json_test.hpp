#ifndef JSON_TEST_HPP
#define JSON_TEST_HPP

#include <string>
#include <fstream>
#include <filesystem>

#include <cxxtest/TestSuite.h>

#include "../utility_module/json.hpp"
#include "../utility_module/json.cpp"
#include "../utility_module/string_manager.hpp"
#include "../utility_module/string_manager.cpp"
#include "../exception_module/exceptions.hpp"

using namespace utility_space;
using namespace siem_ex_space;

#define JSON_FILENAME "json_result.json"
#define JSON_DATA "{\"firstRecord\":{\"name\":\"Alex\",\"id\":\"1337\"},\"secondRecord\":{\"name\":\"Kevin\",\"id\":\"1225\"},\
\"array\":[\"first\",\"second\",\"third\"]}"

class JsonTest : public CxxTest::TestSuite
{
public:
	void testReadData(void)
	{
		std::string writeStr(JSON_DATA);
		std::ofstream fout(JSON_FILENAME, std::ios_base::out);
		fout.write(writeStr.c_str(), writeStr.size());
		fout.close();

		JsonObject parser = getJsonData(JSON_FILENAME);
		std::shared_ptr<JsonContainer> firstElement = parser.findElementByName("firstRecord");
		std::shared_ptr<JsonContainer> secondElement = parser.findElementByName("secondRecord");

		std::string firstName = firstElement->childNode->keyValue.second;
		std::string secondName = secondElement->childNode->keyValue.second;
		TS_ASSERT_EQUALS(firstName, "Alex");
		TS_ASSERT_EQUALS(secondName, "Kevin");

		std::vector<std::shared_ptr<JsonContainer>> elements = parser.findElementsByName("name");
		TS_ASSERT_EQUALS(elements.size(), 2);

		std::filesystem::remove(JSON_FILENAME);
	}

	void testAddAndWriteData(void)
	{
		JsonObject parser;
		parser.addOrUpdateNode("firstRecord", "root/firstRecord");
		parser.addOrUpdateString(std::pair<std::string, std::string>("name", "Alex"), "root/firstRecord/name");
		parser.addOrUpdateString(std::pair<std::string, std::string>("id", "1337"), "root/firstRecord/id");

		parser.addOrUpdateNode("secondRecord", "root/secondRecord");
		parser.addOrUpdateString(std::pair<std::string, std::string>("name", "Kevin"), "root/secondRecord/name");
		parser.addOrUpdateString(std::pair<std::string, std::string>("id", "1225"), "root/secondRecord/id");

		parser.addArray("array", std::vector<std::string>(), "root");
		parser.addArrayElement("first", "root/array");
		parser.addArrayElement("second", "root/array");
		parser.addArrayElement("third", "root/array");

		std::ofstream fout(JSON_FILENAME, std::ios_base::out);
		parser.setJson(fout);
		fout.close();

		std::ifstream fin(JSON_FILENAME, std::ios_base::in);
		std::string allData;
		std::getline(fin, allData);
		fin.close();

		TS_ASSERT_EQUALS(allData, std::string(JSON_DATA));

		std::filesystem::remove(JSON_FILENAME);
	}

	void testFindElements(void)
	{
		std::string writeStr(JSON_DATA);
		std::ofstream fout(JSON_FILENAME, std::ios_base::out);
		fout.write(writeStr.c_str(), writeStr.size());
		fout.close();

		JsonObject parser = getJsonData(JSON_FILENAME);

		std::string elementValue;
		std::shared_ptr<JsonContainer> foundedElement = parser.findElementByName("name");
		
		elementValue = foundedElement->keyValue.second;
		TS_ASSERT_EQUALS(elementValue, "Alex");

		foundedElement = parser.findElementByPath("root/secondRecord/name");
		elementValue = foundedElement->keyValue.second;
		TS_ASSERT_EQUALS(elementValue, "Kevin");

		foundedElement = parser.findElementByName("third");
		elementValue = foundedElement->keyValue.first;
		TS_ASSERT_EQUALS(elementValue, "third");

		foundedElement = parser.findElementByPath("root/array/first");
		elementValue = foundedElement->keyValue.first;
		TS_ASSERT_EQUALS(elementValue, "first");

		std::filesystem::remove(JSON_FILENAME);
	}
};

#endif // JSON_TEST_HPP