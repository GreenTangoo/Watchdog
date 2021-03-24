#include "aggregator_implementation.hpp"

using namespace aggregation_space;


/*-----------------------------------------------------------------*/
/*-------------------------IAGGREGATOR-----------------------------*/
/*-----------------------------------------------------------------*/
IAggregator::IAggregator()
{

}

IAggregator::~IAggregator()
{

}

/*-----------------------------------------------------------------*/
/*----------------------IAGGREGATOR IMPL---------------------------*/
/*-----------------------------------------------------------------*/
AggregatorImpl::AggregatorImpl()
{

}

AggregatorImpl::~AggregatorImpl()
{

}

/*-----------------------------------------------------------------*/
/*-----------------------AGGREGATOR JSON---------------------------*/
/*-----------------------------------------------------------------*/
AggregatorJson::AggregatorJson(std::shared_ptr<AggregationInfo const> infoPtr) :
    _jsonInfoPtr(std::dynamic_pointer_cast<AggregationJsonInfo const>(infoPtr))
{
    
}

AggregatorJson::AggregatorJson(AggregatorJson const &other)
{

}

AggregatorJson::AggregatorJson(AggregatorJson &&other)
{

}

AggregatorJson::~AggregatorJson()
{

}

void AggregatorJson::runAggregation()
{
    this->initializeAggrTypeVec();

    FileManipulator fileDesc(_jsonInfoPtr->logFilename, FileManipulator::READONLY | FileManipulator::LARGE_FILE);

    std::string logStr;
    while(fileDesc->readLine(logStr) == std::ios_base::goodbit)
    {
        for(GrabTypeResultPair &subAggregatorResultPair: _subAggregatorsResultVec)
        {
            subAggregatorResultPair.first->tryAggregation(logStr);
        }
    }

    this->fillAggrResultStructs();
}

void AggregatorJson::saveResult()
{
    AggrJsonResultVec aggregationData;
    std::transform(_subAggregatorsResultVec.begin(), _subAggregatorsResultVec.end(), 
        std::back_inserter(aggregationData), [](GrabTypeResultPair const &grabResultPair) -> AggregationJsonResult
    {
        return *(grabResultPair.second);
    });

    AggregatorJsonSerializer serializer(aggregationData, _jsonInfoPtr->resultFilename);
    serializer.serialize();
}

void AggregatorJson::initializeAggrTypeVec()
{
    _subAggregatorsResultVec.clear();
    _manager.reset();
    _manager = std::make_shared<AggrTypeManager>(_subAggregatorsResultVec);

    std::vector<AggregationJsonInfoNode> const &aggrJsonNodesCfgs = _jsonInfoPtr->aggregationsInfoCfg;

    for(AggregationJsonInfoNode const &oneCfg : aggrJsonNodesCfgs)
    {
        std::shared_ptr<AggregationJsonResult> aggrResPtr = 
            std::make_shared<AggregationJsonResult>(oneCfg.nodeId, oneCfg.typeNode, oneCfg.parentNodePath);

        std::shared_ptr<AggregatorTypeImpl> aggregatorTypeObj = create_aggregator_type(*_manager, oneCfg);

        _subAggregatorsResultVec.push_back({aggregatorTypeObj, aggrResPtr});
    }
}

void AggregatorJson::fillAggrResultStructs()
{
    for(GrabTypeResultPair &subAggregatorResultPair : _subAggregatorsResultVec)
    {
        std::pair<std::string, std::string> grabResult = subAggregatorResultPair.first->getResult();
        subAggregatorResultPair.second->key = grabResult.first;
        subAggregatorResultPair.second->value = grabResult.second;
    }

    this->resolveFormatParams();
}

void AggregatorJson::resolveFormatParams()
{
    for(GrabTypeResultPair &subAggregatorResultPair : _subAggregatorsResultVec)
    {
        std::string &parentPath = subAggregatorResultPair.second->parentPath;
        std::vector<std::string> idAttributes = StringManager::getVecStrBetweenSymbols(parentPath, '[', ']');

        std::vector<int> idValues;
        std::transform(idAttributes.begin(), idAttributes.end(), std::back_inserter(idValues),
            [](std::string const &idStr) -> int
        {
            return std::atoi(idStr.c_str());
        });

        std::vector<std::pair<RegexSiem, std::string>> replaceRegVec = this->generateRegexVec(idValues);

        parentPath = replaceByRegexVec(parentPath, replaceRegVec);
    }   
}

std::vector<std::pair<RegexSiem, std::string>> AggregatorJson::generateRegexVec(std::vector<int> const &idValues)
{
    std::vector<std::pair<RegexSiem, std::string>> idRegexKeyVec;

    for(int i(0); i < idValues.size(); i++)
    {
        int id = idValues[i];

        auto it = std::find_if(_subAggregatorsResultVec.begin(), _subAggregatorsResultVec.end(),
            [id](GrabTypeResultPair const &AggrResultPair) -> bool
        {
            return id == AggrResultPair.second->nodeId;
        });

        if(it == _subAggregatorsResultVec.end())
        {
            //THROW EXCEPTION
        }

        RegexSiem regex("\\[" + std::to_string(id) + "\\]");
        std::string replaceResult = it->second->key;

        idRegexKeyVec.push_back({regex, replaceResult});
    }

    return idRegexKeyVec;
}

/*-----------------------------------------------------------------*/
/*-----------------AGGREGATOR JSON EXCEPTION-----------------------*/
/*-----------------------------------------------------------------*/
AggregatorJson::AggregatorJsonException::AggregatorJsonException(std::string const &exMsg, int errCode) :
    SIEMExecption(exMsg, errCode)
{

}

AggregatorJson::AggregatorJsonException::AggregatorJsonException(std::string &&exMsg, int errCode) :
    SIEMExecption(std::move(exMsg), errCode)
{

}
AggregatorJson::AggregatorJsonException::~AggregatorJsonException()
{

}

/*-----------------------------------------------------------------*/
/*--------------------------FREE FUNCTIONS-------------------------*/
/*-----------------------------------------------------------------*/
std::shared_ptr<AggregatorTypeImpl> aggregation_space::create_aggregator_type( 
    AggrTypeManager &manager, AggregationInfoNode const &infoNode)
{
    AggregationRegexInfo const &regexInfo = infoNode.regexInfo;

    std::pair<RegexSiem, int> keyRegex({regexInfo.keyFindRegex, regexInfo.keyRegGroup});
    std::pair<RegexSiem, int> valueRegex({regexInfo.valueFindRegex, regexInfo.valueRegGroup});
    
    switch(infoNode.grabType)
    {
    case aggrType::FINDER:
    {
        return std::make_shared<AggregatorTypeFounder>(keyRegex, valueRegex, manager, infoNode);
    }
        break;
    case aggrType::COUNTER:
    {
        return std::make_shared<AggregatorTypeCounter>(keyRegex, valueRegex, manager, infoNode);
    }
        break;
    }
}