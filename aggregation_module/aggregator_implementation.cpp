#include "aggregator_implementation.hpp"

using namespace utility_space;
using namespace aggregation_space;

static std::map<grabberCategory, std::string> categoryStringGrabMap = 
    {
        {IPTABLES, "iptables"}
    };

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
        AggregationException::INCORRECT_AGGR_STRING);
}

std::string GrabberCategoryResolver::grabberCategoryToString(grabberCategory grabCategory)
{
	auto it = categoryStringGrabMap.find(grabCategory);
    if(it != categoryStringGrabMap.end())
    {
        return it->second;
    }

    throw AggregationException("Invalid aggregation category parameter",
        AggregationException::INCORRECT_AGGR_CATEGORY);
}