#include "format_string.hpp"

using namespace utility_space;

static std::vector<std::string> s_attrNames = { FORMAT_ATTR_ID }; 

FormatString::FormatString()
{
    this->_isNeedToRefounding = true;
}

FormatString::FormatString(std::string const &str) : _containStr(str), _isNeedToRefounding(true)
{

}

FormatString::FormatString(FormatString const &other) : 
    _containStr(other._containStr), _isNeedToRefounding(other._isNeedToRefounding), _attributesVec(other._attributesVec)
{

}

FormatString::FormatString(FormatString &&other) : 
    _containStr(std::move(other._containStr)), _isNeedToRefounding(other._isNeedToRefounding),
    _attributesVec(std::move(other._attributesVec))
{

}

FormatString const& FormatString::operator=(FormatString const &other)
{
    if(this != &other)
    {
        _containStr = other._containStr;
        _isNeedToRefounding = other._isNeedToRefounding;
        _attributesVec = other._attributesVec;
    }

    return *this;
}

FormatString const& FormatString::operator=(FormatString &&other)
{
    if(this != &other)
    {
        _containStr = std::move(other._containStr);
        _isNeedToRefounding = other._isNeedToRefounding;
        _attributesVec = std::move(other._attributesVec);
    }

    return *this;
}

FormatString const& FormatString::operator=(std::string const &str)
{
    _containStr = str;
    _isNeedToRefounding = true;
}

FormatString const& FormatString::operator+=(FormatString const &other)
{
    _containStr += other._containStr;
    _isNeedToRefounding = true;

    return *this;
}

FormatString::operator std::string()
{
    return _containStr;
}

std::vector<Attributes> FormatString::getAttributes()
{
    if(_isNeedToRefounding)
    {
        this->foundAttributes();
    }

    return _attributesVec;
}

Attributes FormatString::getFilterAttributes(std::string attributeName)
{
    std::vector<Attributes> sourceAttrs = this->getAttributes();

    Attributes filterredAttrs;

    for(Attributes attributesInTag : sourceAttrs)
    {
        std::copy_if(attributesInTag.begin(), attributesInTag.end(), std::back_inserter(filterredAttrs),
            [&attributeName](std::pair<std::string, std::string> const &attributeNameValue) -> bool
        {
            return attributeNameValue.first == attributeName;
        });
    }

    return filterredAttrs;
}

void FormatString::foundAttributes()
{
    std::vector<std::string> attrTagVec = 
        StringManager::getVecStrBetweenSymbols(_containStr, symbolType::L_SQ_BRACKET, symbolType::R_SQ_BRACKET);

    for(std::string formatTag : attrTagVec)
    {
        Attributes foundedAttrs;
        std::vector<std::string> attrVec = StringManager::parseByDelimiter(formatTag, ",");

        for(std::string attrName : s_attrNames)
        {
            std::vector<std::string>::iterator attrNameIt = std::find_if(attrVec.begin(), attrVec.end(), 
                [&attrName](std::string const &oneAttributeStr) -> bool
            {
                return StringManager::isSubstrIn(oneAttributeStr, attrName);
            });

            if(attrNameIt != attrVec.end())
            {
                std::string attrValue = StringManager::getAfterSymbol(*attrNameIt, symbolType::EQUAL);

                foundedAttrs.push_back(std::pair<std::string, std::string>(attrName, attrValue));
            }
        }

        _attributesVec.push_back(foundedAttrs);
    }

    _isNeedToRefounding = false;
}