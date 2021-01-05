#include "regex_siem.hpp"

using namespace utility_space;

/*---------------------------------------------------------------*/
/*---------------------REGEX SIEM--------------------------------*/
/*---------------------------------------------------------------*/
RegexSiem::RegexSiem()
{

}

RegexSiem::RegexSiem(std::string expr) : std::regex(expr.c_str()), expression(expr)
{

}

RegexSiem::RegexSiem(RegexSiem const &other) : std::regex(other), expression(other.expression)
{

}

RegexSiem::RegexSiem(RegexSiem &&other) : std::regex(std::move(other)), expression(std::move(other.expression))
{

}

RegexSiem::~RegexSiem()
{

}

RegexSiem const& RegexSiem::operator=(std::string const &str)
{
    std::regex::operator=(str.c_str());
    expression = str;
}

RegexSiem const& RegexSiem::operator=(RegexSiem const &other)
{
    std::regex::operator=(other);
    expression = other.expression;
}

RegexSiem const& RegexSiem::operator=(RegexSiem &&other)
{
    std::regex::operator=(std::move(other));
    expression = std::move(other.expression);
}

inline std::string RegexSiem::getExpresison()
{
    return expression;
}

/*---------------------------------------------------------------*/
/*------------------PUBLIC FUNCTIONS-----------------------------*/
/*---------------------------------------------------------------*/
std::string utility_space::findByRegex(std::string const &line, std::regex const reg, int regGroup)
{
    std::smatch matchArr;
    std::string matchStr;

    std::regex_match(line, matchArr, reg);

    if(static_cast<int>(matchArr.size()) > regGroup)
    {
        matchStr = matchArr[regGroup].str();
    }

    return matchStr;
}

std::string utility_space::replaceByRegexVec(std::string const &srcStr, std::vector<std::pair<std::regex, std::string>> regDstPair)
{
    std::string replacedStr = srcStr;

    for(std::pair<std::regex, std::string> regDst : regDstPair)
    {
        replacedStr = std::regex_replace(replacedStr, regDst.first, regDst.second);
    }

    return replacedStr;
}

std::string utility_space::replaceByRegexVec(std::string const &srcStr, std::vector<std::pair<RegexSiem, std::string>> regDstPair)
{
    std::string replacedStr = srcStr;

    for(std::pair<RegexSiem, std::string> regDst : regDstPair)
    {
        replacedStr = std::regex_replace(replacedStr, regDst.first, regDst.second);
    }

    return replacedStr;
}