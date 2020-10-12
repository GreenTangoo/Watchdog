#include "syntax_analyzer.hpp"

using namespace utility_space;

#define AND_CONDITION "and"
#define OR_CONDITION "or"
#define INNER_CONDITION "inner"
#define EQUAL "="
#define NOT_EQUAL "!="
#define GREATER_THAN ">"
#define GREATER_EQUAL ">="
#define LESS_THAN "<"
#define LESS_EQUAL "<="


static std::map<compareCondition, std::string> const compareStringMap = 
                                                {
                                                    {EQ, EQUAL},
                                                    {NE, NOT_EQUAL},
                                                    {GT, GREATER_THAN},
                                                    {GE, GREATER_EQUAL},
                                                    {LT, LESS_THAN},
                                                    {LE, LESS_EQUAL}
                                                };

static std::map<relationshipCondition, std::string> const relationshipStringMap =
                                                {
                                                    {AND, AND_CONDITION},
                                                    {OR, OR_CONDITION},
                                                    {INNER, INNER_CONDITION}
                                                };

static bool isRelationShipNode(std::string const &keyStr);

SyntaxAnalyzer::SyntaxAnalyzer()
{

}

SyntaxAnalyzer::~SyntaxAnalyzer()
{

}

compareCondition SyntaxAnalyzer::tryFoundCompareCondition(std::string const &valueStr)
{
    std::string conditionStr = StringManager::getStrBetweenSymbols(valueStr, 
        L_SQ_BRACKET, R_SQ_BRACKET);
    
    return stringToCompareCondition(conditionStr);
}

std::shared_ptr<JsonContainer> SyntaxAnalyzer::tryFoundNextRelationship(
        std::shared_ptr<JsonContainer> const ptr, int subNodeLevel)
{
    JsonObject nodeObj(*(ptr.get()));

    return tryFoundNextRelationship(nodeObj, subNodeLevel);
}

std::shared_ptr<JsonContainer> SyntaxAnalyzer::tryFoundNextRelationship(JsonObject const &obj,
    int subNodeLevel)
{
    std::shared_ptr<JsonContainer> relationshipNodePtr = obj.findElementByName("root");

    if(!(relationshipNodePtr->childNode))
    {
        throw JsonException("Empty container passed", 
            JsonException::EMPTY_CONTAINER);
    }
    
    relationshipNodePtr = relationshipNodePtr->childNode;

    for(int i(0); i < subNodeLevel; i++)
    {
        if(!(relationshipNodePtr->childNode))
            throw SyntaxAnalyzeException("Incorrect subNodeLevel variable value passed",
                SyntaxAnalyzeException::BAD_SUBNODE_LEVEL);
                
        relationshipNodePtr = relationshipNodePtr->childNode;
    }

    for(std::shared_ptr<JsonContainer> it = relationshipNodePtr;
        it != nullptr; it = it->nextNode)
    {
        if(isRelationShipNode(it->keyValue.first))
        {
            return it;
        }
    }

    return nullptr;
}

relationshipCondition SyntaxAnalyzer::stringToRelationship(std::string relationshipStr)
{
    relationshipCondition storeRelationshipVal = NO_RELATIONSHIP;

    for(std::map<relationshipCondition, std::string>::const_iterator it = 
        relationshipStringMap.begin(); it != relationshipStringMap.end(); it++)
    {
        if(it->second == relationshipStr)
        {
            storeRelationshipVal = it->first;
            break;
        }
    }

    return storeRelationshipVal;
}

std::string SyntaxAnalyzer::relationshipToString(relationshipCondition relationship)
{
    auto it = relationshipStringMap.find(relationship);
    if(it != relationshipStringMap.end())
    {
        return it->second;
    }
    else
    {
        throw SyntaxAnalyzeException("Incorrect relationship parameter passed",
            SyntaxAnalyzeException::BAD_RELATIONSHIP_PARAMETER);
    }
}

compareCondition SyntaxAnalyzer::stringToCompareCondition(std::string conditionStr)
{
    compareCondition storeCompareVal = NO_CONDITION;
    
    for(std::map<compareCondition, std::string>::const_iterator it = compareStringMap.begin();
        it != compareStringMap.end(); it++)
    {
        if(it->second == conditionStr)
        {
            storeCompareVal = it->first;
            break;
        }
    }

    return storeCompareVal;
}

std::string SyntaxAnalyzer::compareConditionToString(compareCondition condition)
{
    auto it = compareStringMap.find(condition);
    if(it != compareStringMap.end())
    {
        return it->second;
    }
    else
    {
        throw SyntaxAnalyzeException("Incorrect compare parameter passed", 
            SyntaxAnalyzeException::BAD_COMPARE_PARAMETER);
    }
}

static bool isRelationShipNode(std::string const &keyStr)
{
    if((keyStr == AND_CONDITION) || 
       (keyStr == OR_CONDITION)  || 
       (keyStr == INNER_CONDITION))
        return true;
    return false;
}