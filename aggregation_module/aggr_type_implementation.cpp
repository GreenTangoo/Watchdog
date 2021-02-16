#include "aggr_type_implementation.hpp"

using namespace aggregation_space;

/*-----------------------------------------------------------------*/
/*----------------------IAGGREGATOR--------------------------------*/
/*-----------------------------------------------------------------*/
IAggregatorType::~IAggregatorType()
{

}

/*-----------------------------------------------------------------*/
/*----------------------AGGREGATOR IMPL----------------------------*/
/*-----------------------------------------------------------------*/
AggregatorTypeImpl::AggregatorTypeImpl()
{

}

AggregatorTypeImpl::~AggregatorTypeImpl()
{

}

/*-----------------------------------------------------------------*/
/*----------------------AGGREGATOR COUNTER-------------------------*/
/*-----------------------------------------------------------------*/
AggregatorTypeCounter::AggregatorTypeCounter()
{

}

AggregatorTypeCounter::~AggregatorTypeCounter()
{

}

void AggregatorTypeCounter::tryAggregation(std::string const &logStr)
{

}

/*-----------------------------------------------------------------*/
/*----------------------AGGREGATOR FOUNDER-------------------------*/
/*-----------------------------------------------------------------*/
AggregatorTypeFounder::AggregatorTypeFounder() : _isFound(false)
{

}

AggregatorTypeFounder::~AggregatorTypeFounder()
{

}

void AggregatorTypeFounder::tryAggregation(std::string const &logStr)
{
}

/*-----------------------------------------------------------------*/
/*--------------------------FREE FUNCTIONS-------------------------*/
/*-----------------------------------------------------------------*/
std::shared_ptr<AggregatorTypeImpl> aggregation_space::create_aggregator_type(AggregationJsonInfoNode const &jsonGrabInfoNode)
{

}