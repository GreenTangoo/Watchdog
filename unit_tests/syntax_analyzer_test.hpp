#ifndef SYNTAX_ANALYZER_TEST_HPP
#define SYNTAX_ANALYZER_TEST_HPP

#include <iostream>
#include <cxxtest/TestSuite.h>
#include "../utility_module/syntax_analyzer.hpp"
#include "../utility_module/syntax_analyzer.cpp"
#include "../utility_module/json.hpp"
#include "../utility_module/json.cpp"
#include "../utility_module/string_manager.hpp"
#include "../utility_module/string_manager.cpp"

using namespace utility_space;

#define FIRST_TESTED_STR "[<=]1000"
#define SECOND_TESTED_STR "[!=][==]2000"


static JsonObject constructJsonContainer();

class SyntaxAnalyzerTest : public CxxTest::TestSuite
{
public:
    void testRelationshipTransforms(void)
    {
        SyntaxAnalyzer analyzer;
        relationshipCondition realtionshipValue = NO_RELATIONSHIP;
        std::string relationshipStr;

        for(std::map<relationshipCondition, std::string>::const_iterator it = 
            relationshipStringMap.begin(); it != relationshipStringMap.end(); it++)
        {
            relationshipStr = analyzer.relationshipToString(it->first);
            TS_ASSERT_EQUALS(relationshipStr, it->second);

            realtionshipValue = analyzer.stringToRelationship(it->second);
            TS_ASSERT_EQUALS(realtionshipValue, it->first);
        }
    }

    void testCompareTransforms(void)
    {
        SyntaxAnalyzer analyzer;
        compareCondition compareValue = NO_CONDITION;
        std::string compareStr;

        for(std::map<compareCondition, std::string>::const_iterator it = 
            compareStringMap.begin(); it != compareStringMap.end(); it++)
        {
            compareStr = analyzer.compareConditionToString(it->first);
            TS_ASSERT_EQUALS(compareStr, it->second);

            compareValue = analyzer.stringToCompareCondition(it->second);
            TS_ASSERT_EQUALS(compareValue, it->first);
        }
    }

    void testCompareFound(void)
    {
        SyntaxAnalyzer analyzer;
        std::string valueWithoutCompare;

        compareCondition compareValue = analyzer.tryFoundCompareCondition(FIRST_TESTED_STR);
        TS_ASSERT_EQUALS(compareValue, LE);

        valueWithoutCompare = StringManager::getAfterSymbol(FIRST_TESTED_STR, R_SQ_BRACKET);
        TS_ASSERT_EQUALS(valueWithoutCompare, "1000");

        compareValue = analyzer.tryFoundCompareCondition(SECOND_TESTED_STR);
        TS_ASSERT_EQUALS(compareValue, NE);

        valueWithoutCompare = StringManager::getAfterSymbol(SECOND_TESTED_STR, R_SQ_BRACKET);
        TS_ASSERT_EQUALS(valueWithoutCompare, "[==]2000");
    }

    void testNextRelationShipFound(void)
    {
        SyntaxAnalyzer analyzer;

        JsonObject obj = constructJsonContainer();

        std::shared_ptr<JsonContainer> relationshipNode = obj.findElementByName("one-config");
        relationshipNode = analyzer.tryFoundNextRelationship(relationshipNode, 3);
        
        if(!relationshipNode)
        {
            TS_FAIL("Nullptr relationship pointer in first relationship");
        }

        std::string relationshipStr = relationshipNode->keyValue.first;
        TS_ASSERT_EQUALS(relationshipStr, INNER_CONDITION);
        
        relationshipNode = analyzer.tryFoundNextRelationship(relationshipNode, 1);
        if(!relationshipNode)
        {
            TS_FAIL("Nullptr relationship pointer in second relationship");
        }

        relationshipStr = relationshipNode->keyValue.first;
        TS_ASSERT_EQUALS(relationshipStr, AND_CONDITION);
    }
};

JsonObject constructJsonContainer()
{
    JsonObject object;
    object.addOrUpdateNode("search-configs", "root/search-configs");
    object.addOrUpdateNode("one-config", "root/search-configs/one-config");
    object.addOrUpdateString(std::pair<std::string, std::string>("key-node", "[ip_addr]"),
        "root/search-configs/one-config/key-node");

    object.addOrUpdateNode(INNER_CONDITION,"root/search-configs/one-config/inner");
    object.addOrUpdateString(std::pair<std::string, std::string>("key-node", "amount_requests"),
        "root/search-configs/one-config/inner/key-node");
        
    object.addOrUpdateNode(AND_CONDITION, "root/search-configs/one-config/inner/and");

    return object;
}

#endif // SYNTAX_ANALYZER_TEST_HPP