#include "aggr_type_implementation.hpp"

using namespace aggregation_space;

#define EMPTY_REGEX -1

/*-----------------------------------------------------------------*/
/*----------------------IAGGREGATOR--------------------------------*/
/*-----------------------------------------------------------------*/
IAggregatorType::~IAggregatorType()
{

}

/*-----------------------------------------------------------------*/
/*----------------------AGGREGATOR IMPL----------------------------*/
/*-----------------------------------------------------------------*/
AggregatorTypeImpl::AggregatorTypeImpl(std::pair<RegexSiem, int> keyRegex, std::pair<RegexSiem, int> valueRegex,
    AggrTypeManager &manager, AggregationInfoNode const &infoNode) : 
    _keyRegexInfo(keyRegex), _valueRegexInfo(valueRegex), _isFoundKey(false), _manager(manager), _infoNode(infoNode)
{
    if(_keyRegexInfo.second == EMPTY_REGEX)
        _isFoundKey = true;
}

AggregatorTypeImpl::~AggregatorTypeImpl()
{

}

AggregatorTypeImpl::AggregatorTypeImpl(AggregatorTypeImpl const &other) :
    _keyRegexInfo(other._keyRegexInfo), _valueRegexInfo(other._valueRegexInfo), _isFoundKey(other._isFoundKey),
    _manager(other._manager), _infoNode(other._infoNode)
{

}

/*-----------------------------------------------------------------*/
/*----------------------AGGREGATOR COUNTER-------------------------*/
/*-----------------------------------------------------------------*/
AggregatorTypeCounter::AggregatorTypeCounter(std::pair<RegexSiem, int> keyRegex, std::pair<RegexSiem, int> valueRegex,
    AggrTypeManager &manager, AggregationInfoNode const &infoNode) : 
    AggregatorTypeImpl(keyRegex, valueRegex, manager, infoNode)
{
    if(_isFoundKey)
        _grabResult.first = _keyRegexInfo.first.getExpressison();

    _grabResult.second = 0;
}

AggregatorTypeCounter::~AggregatorTypeCounter()
{

}

void AggregatorTypeCounter::tryAggregation(std::string const &logStr)
{
    std::string valueRegResult = findByRegex(logStr, _valueRegexInfo.first, _valueRegexInfo.second);
    if(valueRegResult.length())
    {
        _grabResult.second++;
    }
}

std::pair<std::string, std::string> AggregatorTypeCounter::getResult()
{
    return {_grabResult.first, std::to_string(_grabResult.second)};
}

/*-----------------------------------------------------------------*/
/*----------------------AGGREGATOR FOUNDER-------------------------*/
/*-----------------------------------------------------------------*/
AggregatorTypeFounder::AggregatorTypeFounder(std::pair<RegexSiem, int> keyRegex, std::pair<RegexSiem, int> valueRegex,
    AggrTypeManager &manager, AggregationInfoNode const &infoNode) : 
    AggregatorTypeImpl(keyRegex, valueRegex, manager, infoNode), _isFoundValue(false)
{
    if(_isFoundKey)
        _grabResult.first = _keyRegexInfo.first.getExpressison();
}

AggregatorTypeFounder::~AggregatorTypeFounder()
{

}

void AggregatorTypeFounder::tryAggregation(std::string const &logStr)
{
    if(_isFoundValue && _isFoundKey)
        return;

    if(!_isFoundKey)
    {
        std::string keyRegResult = findByRegex(logStr, _keyRegexInfo.first, _keyRegexInfo.second);
        if(keyRegResult.length())
        {
            _grabResult.first = keyRegResult;
            _isFoundKey = true;
        }
    }

    if(!_isFoundValue)
    {
        std::string valueRegResult = findByRegex(logStr, _valueRegexInfo.first, _valueRegexInfo.second);
        if(valueRegResult.length())
        {
            _grabResult.second = valueRegResult;
            _isFoundValue;
        }
    }
}

std::pair<std::string, std::string> AggregatorTypeFounder::getResult()
{
    return _grabResult;
}