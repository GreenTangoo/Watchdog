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
"\"result-json\":\"iptables_log.json\",\"info-node\":{ \"id\":\"1\","\
"\"node-type\":\"object\",\"key-name\":\"regexp\",\"key-group\":\"1\","\
"\"parent-node\":\"root\"},\"info-node\":{ \"id\":\"2\","\
"\"node-type\":\"string\",\"key-name\":\"amount_requests\","\
"\"value-name\":\"regexp\",\"value-group\":\"1\",\"parent-node\":\"[ip_addr]\"},"\
"\"info-node\":{ \"id\":\"3\",\"node-type\":\"string\",\"key-name\":\"protocol\","\
"\"value-name\":\"regexp\",\"value-group\":\"1\",\"parent-node\":\"[ip_addr]\"}}}}"

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

		int standartIdArr[] = { 1, 2, 3 };

        typeNodeJSON standartTypeNodeArr[] = { OBJECT, STRING, STRING };
        std::string standartParentNodeArr[] = { "root", "[ip_addr]", "[ip_addr]" };

        std::string standartKeyNameArr[] = { "regexp", "amount_requests", "protocol" };

        int standartKeyRegGroup[] = { 1, -1, -1 };

        std::string standartValueNameArr[] = { "", "regexp", "regexp" };

        int standartValueRegGroup[] = {-1, 1, 1 };

        for(size_t i(0); i < iptablesInfo.aggregationsInfoCfg.size(); i++)
        {
			int id = iptablesInfo.aggregationsInfoCfg[i].get()->nodeId;
            typeNodeJSON nodeType = iptablesInfo.aggregationsInfoCfg[i].get()->typeNode;
            std::regex keyReg = iptablesInfo.aggregationsInfoCfg[i].get()->regexInfo.keyFindRegex;
            std::regex valueReg = iptablesInfo.aggregationsInfoCfg[i].get()->regexInfo.valueFindRegex;
            std::string parentNodeStr = iptablesInfo.aggregationsInfoCfg[i].get()->parentNodePath;
            int keyRegGroup = iptablesInfo.aggregationsInfoCfg[i].get()->regexInfo.keyRegGroup;
            int valueRegGroup = iptablesInfo.aggregationsInfoCfg[i].get()->regexInfo.valueRegGroup;
            
            bool keyRegMatch = std::regex_match(standartKeyNameArr[i], keyReg);
            bool valueRegMatch = std::regex_match(standartValueNameArr[i], valueReg);

			TS_ASSERT_EQUALS(id, standartIdArr[i]);
            TS_ASSERT_EQUALS(nodeType, standartTypeNodeArr[i]);
            TS_ASSERT_EQUALS(keyRegMatch, true);
            TS_ASSERT_EQUALS(valueRegMatch, true);
            TS_ASSERT_EQUALS(parentNodeStr, standartParentNodeArr[i]);
            TS_ASSERT_EQUALS(keyRegGroup, standartKeyRegGroup[i]);
            TS_ASSERT_EQUALS(valueRegGroup, standartValueRegGroup[i]);
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