#include "aggregator_implementation.hpp"

using namespace utility_space;
using namespace aggregation_space;
using namespace siem_ex_space;

#define SOURCE_LOG_PATH "source_logs"

typedef std::vector<std::unique_ptr<AggregationInfoNode>> AggregationNodes;

static std::map<grabberCategory, std::string> categoryStringGrabMap = 
    {
        {IPTABLES, "iptables"}, {APACHE, "apache"}
    };

/*-----------------------------------------------------------------*/
/*----------------------SYMPTOM GRABBER----------------------------*/
/*-----------------------------------------------------------------*/
SymptomGrabber::SymptomGrabber(AggregationInfo const &info, grabberCategory grabType) :
    _grabType(grabType), _info(info)
{

}

SymptomGrabber::SymptomGrabber(SymptomGrabber const &other) :
    _grabType(other._grabType), _info(other._info)
{

}

SymptomGrabber::SymptomGrabber(SymptomGrabber &&other) :
    _grabType(std::move(other._grabType)), _info(std::move(other._info))
{

}

SymptomGrabber::~SymptomGrabber()
{

}

void SymptomGrabber::tryAggregationInfo()
{
    try
    {

    }
    catch(FilesystemSiemException const &ex)
    {
        std::string errMsg(ex.what());
        int errCode = ex.getErrorCode();

        throw AggregationException("Aggregation ecxeption: " + errMsg + 
            " with code: " + std::to_string(errCode),
             AggregationException::FAILED_AGGREGATION,
             static_cast<int>(_grabType));
    }
    catch(std::regex_error const &ex)
    {
        std::string errMsg(ex.what());
        int errCode = ex.code();

        throw AggregationException("Aggregation exception: " + errMsg + 
            " with code: " + std::to_string(errCode),
            AggregationException::FAILED_AGGREGATION,
            static_cast<int>(_grabType));
    }
}

/*-----------------------------------------------------------------*/
/*-----------------------PRIVATE-----------------------------------*/
/*-----------------------------------------------------------------*/
void SymptomGrabber::aggregateOneInfoNode(std::string const &oneLogStr, 
    AggregationInfoNode const &node)
{

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

    throw AggregationException("Invalid aggregation string parameter",
        AggregationException::INCORRECT_AGGR_STRING,
        static_cast<int>(grabberCategory::GRAB_NONE_TYPE));
}

std::string GrabberCategoryResolver::grabberCategoryToString(grabberCategory grabCategory)
{
	auto it = categoryStringGrabMap.find(grabCategory);
    if(it != categoryStringGrabMap.end())
    {
        return it->second;
    }

    throw AggregationException("Invalid aggregation category parameter",
        AggregationException::INCORRECT_AGGR_CATEGORY,
        static_cast<int>(grabberCategory::GRAB_NONE_TYPE));
}