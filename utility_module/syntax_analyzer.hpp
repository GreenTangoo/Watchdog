#ifndef SYNTAX_ANALYZER_HPP
#define SYNTAX_ANALYZER_HPP

#include <map>

#include "string_manager.hpp"
#include "json.hpp"
#include "../exception_module/exceptions.hpp"

using namespace siem_ex_space;

namespace utility_space
{
    enum relationshipCondition { AND = 0, OR, INNER, NO_RELATIONSHIP };
    enum compareCondition { EQ = 0, NE, LT, LE, GT, GE, NO_CONDITION}; 

    class SyntaxAnalyzer
    {
    public:
        SyntaxAnalyzer();
        SyntaxAnalyzer(SyntaxAnalyzer const &other) = default;
        SyntaxAnalyzer(SyntaxAnalyzer &&other) = default;
        ~SyntaxAnalyzer();
        SyntaxAnalyzer const& operator=(SyntaxAnalyzer const &other) = delete;
        SyntaxAnalyzer const& operator=(SyntaxAnalyzer &&other) = delete;
        compareCondition tryFoundCompareCondition(std::string const &valueStr);
        std::shared_ptr<JsonContainer> tryFoundNextRelationship(std::shared_ptr<JsonContainer> const ptr,
            int subNodeLevel);
        std::shared_ptr<JsonContainer> tryFoundNextRelationship(JsonObject const &obj,
            int subNodeLevel);
        relationshipCondition stringToRelationship(std::string relationshipStr);
        std::string relationshipToString(relationshipCondition relationship);
        compareCondition stringToCompareCondition(std::string conditionStr);
        std::string compareConditionToString(compareCondition condition);
    };
}

#endif // SYNTAX_ANALYZER_HPP