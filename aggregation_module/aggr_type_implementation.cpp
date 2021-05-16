#include "aggr_type_implementation.hpp"

using namespace aggregation_space;

#define EMPTY_REGEX -1

/*-----------------------------------------------------------------*/
/*---------------------AGGR TYPE MANAGER---------------------------*/
/*-----------------------------------------------------------------*/
AggrTypeManager::AggrTypeManager(std::vector<GrabTypeResultPair> &subAggregatorsResultVec) : 
    _subAggregatorsResultVec(subAggregatorsResultVec)
{

}

AggrTypeManager::~AggrTypeManager()
{

}

std::pair<std::string, std::string> AggrTypeManager::getResultById(int idNode)
{
    std::vector<GrabTypeResultPair>::iterator pairIt = this->getResultPairById(idNode);
    if(pairIt != _subAggregatorsResultVec.end())
    {
        return pairIt->first->getResult();
    }
    else
    {
        //THROW
    }
}

std::vector<GrabTypeResultPair>::iterator AggrTypeManager::getResultPairById(int idNode)
{
    auto it = std::find_if(_subAggregatorsResultVec.begin(), _subAggregatorsResultVec.end(), 
        [&idNode](GrabTypeResultPair const &aggregationResultElem) -> bool
    {
        return aggregationResultElem.second->nodeId == idNode;
    });

    return it;
}

/*-----------------------------------------------------------------*/
/*----------------------IAGGREGATOR TYPE---------------------------*/
/*-----------------------------------------------------------------*/
IAggregatorType::~IAggregatorType()
{

}

/*-----------------------------------------------------------------*/
/*----------------------AGGREGATOR TYPE IMPL-----------------------*/
/*-----------------------------------------------------------------*/
AggregatorTypeImpl::AggregatorTypeImpl(std::pair<RegexSiem, int> keyRegex, std::pair<RegexSiem, int> valueRegex,
    AggrTypeManager &manager, std::shared_ptr<AggregationInfoNode> infoNode) : 
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

bool AggregatorTypeImpl::isPassConditions(std::vector<AggregationCondition> const &conditions)
{
    bool isFailed = false;

    for(AggregationCondition const &condition : conditions)
    {
        if(condition.aggrConditonType == relationshipCondition::AND)
        {
            if(!isValidCondition(condition))
            {
                isFailed = true;
                break;
            }
        }
        else if(condition.aggrConditonType == relationshipCondition::OR)
        {
            if(isValidCondition(condition))
            {
                break;
            }
        }
    }

    return !isFailed;
}

bool AggregatorTypeImpl::isValidCondition(AggregationCondition const &condition)
{
    int idNode = condition.idAggregationNode;

    std::pair<std::string, std::string> keyValueMembers = _manager.getResultById(idNode); 

    std::string conditionMember = condition.infoNodeMember == KEY_MEMBER ? keyValueMembers.first : keyValueMembers.second;

    return static_cast<bool>(conditionMember.size());
}

/*-----------------------------------------------------------------*/
/*-------------------AGGREGATOR TYPE COUNTER-----------------------*/
/*-----------------------------------------------------------------*/
AggregatorTypeCounter::AggregatorTypeCounter(std::pair<RegexSiem, int> keyRegex, std::pair<RegexSiem, int> valueRegex,
    AggrTypeManager &manager, std::shared_ptr<AggregationInfoNode> infoNode) : 
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
        std::vector<AggregationCondition> const &conditions = _infoNode->additionalConditions;

        bool passResult = this->isPassConditions(conditions);

        if(passResult)
        {
            _grabResult.second++;
        }
    }
}

std::pair<std::string, std::string> AggregatorTypeCounter::getResult()
{
    return {_grabResult.first, std::to_string(_grabResult.second)};
}

/*-----------------------------------------------------------------*/
/*---------------------AGGREGATOR TYPE FOUNDER---------------------*/
/*-----------------------------------------------------------------*/
AggregatorTypeFounder::AggregatorTypeFounder(std::pair<RegexSiem, int> keyRegex, std::pair<RegexSiem, int> valueRegex,
    AggrTypeManager &manager, std::shared_ptr<AggregationInfoNode> infoNode) : 
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
            std::vector<AggregationCondition> const &conditions = _infoNode->additionalConditions;

            bool passResult = this->isPassConditions(conditions);

            if(passResult)
            {
                _grabResult.second = valueRegResult;
                _isFoundValue = true;
            }
        }
    }
}

std::pair<std::string, std::string> AggregatorTypeFounder::getResult()
{
    return _grabResult;
}