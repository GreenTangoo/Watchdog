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

std::string SyntaxAnalyzer::getCompareCondition(std::string const &valueStr)
{

}

std::string SyntaxAnalyzer::getRelationship(std::string const &keyRelationshipStr)
{

}

compareCondition SyntaxAnalyzer::foundCompareCondition(std::string const &valueStr)
{

}

relationshipCondition SyntaxAnalyzer::foundNextRelationship(std::shared_ptr<JsonContainer> const ptr)
{

}

compareCondition SyntaxAnalyzer::tryFoundCompareCondition(std::string const &valueStr)
{

}

relationshipCondition SyntaxAnalyzer::tryFoundNextRelationship(std::shared_ptr<JsonContainer> const ptr)
{
    
}