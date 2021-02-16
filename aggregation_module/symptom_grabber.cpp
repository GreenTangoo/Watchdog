#include "symptom_grabber.hpp"

using namespace utility_space;
using namespace aggregation_space;
using namespace siem_ex_space;

#define SOURCE_LOG_PATH "source_logs"
#define ID_STR "id"

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
SymptomGrabber::SymptomGrabber(std::shared_ptr<AggregationInfo const> infoPtr, grabberCategory grabType) :
    _grabType(grabType)
{
    create_aggregator(infoPtr);
}

SymptomGrabber::SymptomGrabber(SymptomGrabber const &other) :
    _grabType(other._grabType), _aggregator(other._aggregator)
{

}

SymptomGrabber::SymptomGrabber(SymptomGrabber &&other) :
    _grabType(std::move(other._grabType)), _aggregator(other._aggregator)
{

}

SymptomGrabber::~SymptomGrabber()
{

}

void SymptomGrabber::tryAggregationInfo()
{
    //FileManipulator logFile(_infoPtr->logFilename, FileManipulator::READONLY | FileManipulator::LARGE_FILE);
    
    //logFile->synchronizationStream();
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
/*--------------------------FUNCTIONS------------------------------*/
/*-----------------------------------------------------------------*/
std::shared_ptr<AggregatorImpl> aggregation_space::create_aggregator(std::shared_ptr<AggregationInfo const> infoPtr)
{
    switch(infoPtr->aggregationBehaviour)
    {
    case behaviourType::JSON_BEHAVIOUR:
    {
        return std::make_shared<AggregatorJson>(infoPtr);
    }
        break;
    default:
        break;
    }
}