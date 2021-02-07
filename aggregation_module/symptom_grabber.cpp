#include "symptom_grabber.hpp"

using namespace utility_space;
using namespace aggregation_space;
using namespace siem_ex_space;

#define SOURCE_LOG_PATH "source_logs"
#define ID_STR "id"

static std::map<grabberCategory, std::string> categoryStringGrabMap = 
    {
        {IPTABLES, "iptables"}, {APACHE, "apache"}
    };

namespace 
{
    std::vector<int> getNodesIdFromFormatStr(std::string const &formatStr)
    {
        std::vector<std::string> idAttributeVec = 
            StringManager::getVecStrBetweenSymbols(formatStr, symbolType::L_SQ_BRACKET, symbolType::R_SQ_BRACKET);

        std::vector<std::string> idStrVec;
        idStrVec.reserve(idAttributeVec.size());

        for(std::string idAttribute : idAttributeVec)
        {
            if(StringManager::isSubstrIn(idAttribute, "id"))
            {
                idStrVec.push_back(StringManager::getAfterSymbol(idAttribute, symbolType::EQUAL));
            }
        }

        std::vector<int> idNodes;
        idNodes.reserve(idAttributeVec.size());

        std::transform(idStrVec.begin(), idStrVec.end(), std::back_inserter(idNodes),
            [](std::string const &idStr) -> int
        {
            return atoi(idStr.c_str());
        });

        idNodes.shrink_to_fit();
        return idNodes;
    }
}

/*-----------------------------------------------------------------*/
/*----------------------SYMPTOM GRABBER----------------------------*/
/*-----------------------------------------------------------------*/
SymptomGrabber::SymptomGrabber(AggregationInfo const &info, grabberCategory grabType) :
    _grabType(grabType), _info(info)
{
    this->initializeAggregators();
}

SymptomGrabber::SymptomGrabber(SymptomGrabber const &other) :
    _grabType(other._grabType), _info(other._info)
{
    this->initializeAggregators();
}

SymptomGrabber::SymptomGrabber(SymptomGrabber &&other) :
    _grabType(std::move(other._grabType)), _info(std::move(other._info))
{
    this->initializeAggregators();
}

SymptomGrabber::~SymptomGrabber()
{

}

void SymptomGrabber::tryAggregationInfo()
{
    FileManipulator logFile(_info.logFilename, FileManipulator::READONLY | FileManipulator::LARGE_FILE);
    
    this->startAggregate(logFile);
    this->resolveFormatValues();

    logFile->synchronizationStream();

    AggrResultVec aggregationResult;
    std::transform(_aggregators.begin(), _aggregators.end(), std::back_inserter(aggregationResult),
        [](std::shared_ptr<AggregatorImpl> const &aggregator) -> std::shared_ptr<AggregationResult>
    {
        return std::make_shared<AggregationResult>(aggregator->getAggrResult()); 
    });

    for(serializerType saveOption : _info.additionalSerializeFormats)
    {
        std::shared_ptr<AggregatorSerializerImpl> serializer = createSerializer(aggregationResult, saveOption, _info.resultFilename);

        serializer->serialize();
    }
}

std::shared_ptr<AggregatorImpl> SymptomGrabber::createAggregator(AggregationInfoNode const &infoNode)
{
    switch(infoNode.grabType)
    {
    case aggrType::COUNTER:
    {
        return std::make_shared<AggregatorCounter>(infoNode);
    }
        break;
    case aggrType::FINDER:
    {
        return std::make_shared<AggregatorFounder>(infoNode);
    }
        break;
    }
}

void SymptomGrabber::initializeAggregators()
{
    _aggregators.clear();

    for(size_t i(0); i < _info.aggregationsInfoCfg.size(); i++)
    {
        std::unique_ptr<AggregationInfoNode> const &aggrCfg = _info.aggregationsInfoCfg[i];
        _aggregators.push_back(this->createAggregator(*(aggrCfg.get())));
    }
}

void SymptomGrabber::startAggregate(FileManipulator file)
{
    std::string fileStr;
    while(file->readLine(fileStr) == std::ifstream::goodbit)
    {
        for(std::shared_ptr<AggregatorImpl> aggregator : _aggregators)
        {
            aggregator->tryAggregation(fileStr);
        }
    } 
}

void SymptomGrabber::resolveFormatValues()
{
    for(std::shared_ptr<AggregatorImpl> aggregator : _aggregators)
    {
        AggregationResult &aggrRes = aggregator->getAggreResultRef();


        FormatString parentPathFormat(aggrRes.parentPath);
        Attributes filterAttrs = parentPathFormat.getFilterAttributes(FORMAT_ATTR_ID);
        /*TODO: Отрезолвить по отфильтрованным аттрибутам строки std::string parentPath*/
        
    }
}

/*-----------------------------------------------------------------*/
/*-----------------AGREGATION EXCEPTION----------------------------*/
/*-----------------------------------------------------------------*/
SymptomGrabber::AggregationException::AggregationException(std::string const &exMsg, int errCode, int grabType) :
    SIEMExecption(exMsg, errCode), _grabCategory(grabType)
{

}

SymptomGrabber::AggregationException::AggregationException(std::string &&exMsg, int errCode, int grabType) :
    SIEMExecption(std::move(exMsg), errCode), _grabCategory(grabType)
{

}

SymptomGrabber::AggregationException::~AggregationException()
{

}

int SymptomGrabber::AggregationException::getGrabberType() const noexcept
{
    return _grabCategory;
}

/*-----------------------------------------------------------------*/
/*----------------------GRABBER GATEGORY RESOLVER------------------*/
/*-----------------------------------------------------------------*/
grabberCategory GrabberCategoryResolver::stringToGrabberCategory(std::string const &grabName)
{
    for(std::map<grabberCategory, std::string>::const_iterator it = categoryStringGrabMap.begin();
        it != categoryStringGrabMap.end(); it++)
    {
        if(it->second == grabName)
        {
            return it->first;
        }
    }

    throw SymptomGrabber::AggregationException("Invalid aggregation string parameter",
        SymptomGrabber::AggregationException::INCORRECT_AGGR_STRING,
        static_cast<int>(grabberCategory::GRAB_NONE_TYPE));
}

std::string GrabberCategoryResolver::grabberCategoryToString(grabberCategory grabCategory)
{
	auto it = categoryStringGrabMap.find(grabCategory);
    if(it != categoryStringGrabMap.end())
    {
        return it->second;
    }

    throw SymptomGrabber::AggregationException("Invalid aggregation category parameter",
        SymptomGrabber::AggregationException::INCORRECT_AGGR_CATEGORY,
        static_cast<int>(grabberCategory::GRAB_NONE_TYPE));
}