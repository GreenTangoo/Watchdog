#ifndef DATE_TIME_TEST_HPP
#define DATE_TIME_TEST_HPP

#include <string>

#include <cxxtest/TestSuite.h>

#include "../exception_module/exceptions.hpp"
#include "../correlation_module/symptom_implementation.hpp"
#include "../correlation_module/symptom_implementation.cpp"
#include "../aggregation_module/aggregator_implementation.hpp"
#include "../aggregation_module/aggregator_implementation.cpp"
#include "../description_module/description_table.hpp"
#include "../description_module/description_table.cpp"
#include "../description_module/configuration.hpp"
#include "../description_module/configuration.cpp"
#include "../utility_module/string_manager.hpp"
#include "../utility_module/string_manager.cpp"
#include "../utility_module/syntax_analyzer.hpp"
#include "../utility_module/syntax_analyzer.cpp"
#include "../utility_module/encryption.hpp"
#include "../utility_module/encryption.cpp"
#include "../utility_module/json.hpp"
#include "../utility_module/json.cpp"

using namespace siem_ex_space;
using namespace utility_space;
using namespace correlation_space;

#define SEARCH_CONFIG_DATA "{\"search-configs\":{\"one-config\":{"\
"\"json-filename\":\"iptables_log.json\",\"category\":\"port_scanning\","\
"\"key-node\":\"[ip_addr]\",\"inner\":{\"key-node\":\"amount_requests\","\
"\"value-node\":\"[>]1000\",\"and\":{\"key-node\":\"protocol\","\
"\"value-node\":\"[=]tcp\"}}}}"

#define AGGR_CONFIG_DATA "{\"aggregation-configs\":{\"one-config\":{"\
"\"category\":\"iptables\",\"source-log\":\"iptables.log\","\
"\"result-json\":\"iptables_log.json\",\"info-node\":{"\
"\"node-type\":\"object\",\"key-name\":\"regexp\","\
"\"parent-node\":\"root\"},\"info-node\":{"\
"\"node-type\":\"string\",\"key-name\":\"amount_requests\","\
"\"value-name\":\"regexp\",\"parent-node\":\"[ip_addr]\"},"\
"\"info-node\":{\"node-type\":\"string\",\"key-name\":\"protocol\","\
"\"value-name\":\"regexp\",\"parent-node\":\"[ip_addr]\"}}}}"

static JsonObject getSearchConfig();
static JsonObject getAggrConfig();

class DescriptionTableTest : public CxxTest::TestSuite
{
public:
	void testGetSearchConfig(void)
    {
        DescriptionTable &table = DescriptionTable::getInstance();
        JsonObject configJson = getSearchConfig();
        Configuration config(configJson);

        table.tuneFromConfig(config, DescriptionTable::CORRELATION_CONFIG);
        SearchInfo const &portScanningInfo = table.getSearchStructure(PORT_SCANNING);

        TS_ASSERT_EQUALS(portScanningInfo.jsonFilename,"iptables_log.json");

        relationshipCondition standartConditionsArr[] = {
            AND, INNER, AND };

        std::string standartKeyNodeArr[] = {
            "[ip_addr]", "amount_requests", "protocol" };

        std::pair<std::string, compareCondition> standartDetailArr[] = {
            {"", NO_CONDITION}, {"1000", GT}, {"tcp", EQ} };

        SearchInfoNode *nodePtr = portScanningInfo.rootSearchConfigNode.get();
        for(int i(0); i < 3; i++)
        {
            TS_ASSERT_EQUALS(nodePtr->condition, standartConditionsArr[i]);
            TS_ASSERT_EQUALS(nodePtr->keyNode, standartKeyNodeArr[i]);
            TS_ASSERT_EQUALS(nodePtr->searchDetail.first, standartDetailArr[i].first);
            TS_ASSERT_EQUALS(nodePtr->searchDetail.second, standartDetailArr[i].second);

            nodePtr = nodePtr->additionalSearchNode.get();
        }
    }

    void testGetAggrConfig(void)
    {
        DescriptionTable &table = DescriptionTable::getInstance();
        JsonObject configJson = getAggrConfig();
        Configuration config(configJson);

        table.tuneFromConfig(config, DescriptionTable::AGGREGATION_CONFIG);
        AggregationInfo const &iptablesInfo = table.getAggrStructure(IPTABLES);

        TS_ASSERT_EQUALS(iptablesInfo.logFilename, "iptables.log");
        TS_ASSERT_EQUALS(iptablesInfo.jsonFilename, "iptables_log.json")

        typeNodeJSON standartTypeNodeArr[] = { OBJECT, STRING, STRING };
        std::string standartParentNodeArr[] = { "root", "[ip_addr]", "[ip_addr]" };

        std::string standartKeyNameArr[] = { "regexp", "amount_requests", "protocol" };

        std::string standartValueNameArr[] = { "", "regexp", "regexp" };

        for(size_t i(0); i < iptablesInfo.aggregationsInfo.size(); i++)
        {
            typeNodeJSON nodeType = iptablesInfo.aggregationsInfo[i].get()->typeNode;
            std::regex keyReg = iptablesInfo.aggregationsInfo[i].get()->keyFindRegex;
            std::regex valueReg = iptablesInfo.aggregationsInfo[i].get()->valueFindRegex;
            std::string parentNodeStr = iptablesInfo.aggregationsInfo[i].get()->parentNode;
            
            bool keyRegMatch = std::regex_match(standartKeyNameArr[i], keyReg);
            bool valueRegMatch = std::regex_match(standartValueNameArr[i], valueReg);

            TS_ASSERT_EQUALS(nodeType, standartTypeNodeArr[i]);
            TS_ASSERT_EQUALS(keyRegMatch, true);
            TS_ASSERT_EQUALS(valueRegMatch, true);
            TS_ASSERT_EQUALS(parentNodeStr, standartParentNodeArr[i]);
        }

    }
};

JsonObject getSearchConfig()
{
   std::stringstream stream(SEARCH_CONFIG_DATA);
   return getJsonData(stream);
}

JsonObject getAggrConfig()
{
    std::stringstream stream(AGGR_CONFIG_DATA);
    return getJsonData(stream);
}

#endif // DATE_TIME_TEST_HPP