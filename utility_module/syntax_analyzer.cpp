#include "syntax_analyzer.hpp"

using namespace utility_space;

#define AND_CONDITION "and"
#define OR_CONDITION "or"
#define EQUAL "="
#define NOT_EQUAL "!="
#define GREATER_THAN ">"
#define GREATER_EQUAL ">="
#define LESS_THAN "<"
#define LESS_EQUAL "<="

SyntaxAnalyzer::SyntaxAnalyzer()
{

}

SyntaxAnalyzer::~SyntaxAnalyzer()
{

}

compareCondition SyntaxAnalyzer::tryFoundCompareCondition(std::string const &valueStr)
{
    std::string conditionStr = StringManager::getStrBetweenSymbols(valueStr, '[', ']');
    
    if(conditionStr == EQUAL)
        return EQ;
    if(conditionStr == NOT_EQUAL)
        return NE;
    if(conditionStr == GREATER_THAN)
        return GT;
    if(conditionStr == GREATER_EQUAL)
        return GE;
    if(conditionStr == LESS_THAN)
        return LT;
    if(conditionStr == LESS_EQUAL)
        return LE;
    
    return NO_CONDITION;
}

relationshipCondition SyntaxAnalyzer::tryFoundNextRelationship(std::shared_ptr<JsonContainer> const ptr)
{
    JsonObject nodeObj(*(ptr.get()));

    std::shared_ptr<JsonContainer> relationshipNodePtr = nodeObj.findElementByName(AND_CONDITION);
    if(relationshipNodePtr)
    {
        return AND;
    }

    relationshipNodePtr = nodeObj.findElementByName(OR_CONDITION);
    if(relationshipNodePtr)
    {
        return OR;
    }

    return NO_RELATIONSHIP;
}