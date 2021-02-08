#include "aggr_type_implementaion.hpp"

using namespace aggregation_space;

/*-----------------------------------------------------------------*/
/*----------------------IAGGREGATOR--------------------------------*/
/*-----------------------------------------------------------------*/
IAggregator::~IAggregator()
{

}

/*-----------------------------------------------------------------*/
/*----------------------AGGREGATOR IMPL----------------------------*/
/*-----------------------------------------------------------------*/
AggregatorImpl::AggregatorImpl()
{

}

AggregatorImpl::AggregatorImpl(AggregatorImpl const &other) : _grabResult(other._grabResult)
{
    
}

AggregatorImpl::~AggregatorImpl()
{

}

AggregationResult AggregatorImpl::getAggrResult()
{
    return _grabResult;
}

AggregationResult& AggregatorImpl::getAggreResultRef()
{
    return _grabResult;
}

int AggregatorImpl::getNodeId()
{
   
}

/*-----------------------------------------------------------------*/
/*----------------------AGGREGATOR COUNTER-------------------------*/
/*-----------------------------------------------------------------*/
AggregatorCounter::AggregatorCounter()
{

}

AggregatorCounter::~AggregatorCounter()
{

}

void AggregatorCounter::tryAggregation(std::string const &logStr)
{

}

/*-----------------------------------------------------------------*/
/*----------------------AGGREGATOR FOUNDER-------------------------*/
/*-----------------------------------------------------------------*/
AggregatorFounder::AggregatorFounder() : _isFound(false)
{

}

AggregatorFounder::~AggregatorFounder()
{

}

void AggregatorFounder::tryAggregation(std::string const &logStr)
{
}
