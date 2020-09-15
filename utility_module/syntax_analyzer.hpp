#ifndef SYNTAX_ANALYZER_HPP
#define SYNTAX_ANALYZER_HPP

#include "string_manager.hpp"
#include "json.hpp"

namespace utility_space
{
    enum relationshipCondition { AND = 0, OR, NO_RELATIONSHIP };
    enum compareCondition { EQ = 0, NE, LT, LE, GT, GE, NO_CONDITION}; 

    class SyntaxAnalyzer
    {
    private:
        std::string getCompareCondition(std::string const &valueStr);
        std::string getRelationship(std::string const &keyRelationshipStr);
    public:
        SyntaxAnalyzer();
        SyntaxAnalyzer(SyntaxAnalyzer const &other) = default;
        SyntaxAnalyzer(SyntaxAnalyzer &&other) = default;
        ~SyntaxAnalyzer();
        SyntaxAnalyzer const& operator=(SyntaxAnalyzer const &other) = delete;
        SyntaxAnalyzer const& operator=(SyntaxAnalyzer &&other) = delete;
        compareCondition foundCompareCondition(std::string const &valuesStr);
        relationshipCondition foundNextRelationship(std::shared_ptr<JsonContainer> const ptr);
        compareCondition tryFoundCompareCondition(std::string const &valueStr);
        relationshipCondition tryFoundNextRelationship(std::shared_ptr<JsonContainer> const ptr);
    };
}

#endif // SYNTAX_ANALYZER_HPP