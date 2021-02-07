#include "aggregator_implementation.hpp"

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
AggregatorImpl::AggregatorImpl(AggregationInfoNode const &infoNode) : _nodeInfo(infoNode)
{

}

AggregatorImpl::AggregatorImpl(AggregatorImpl const &other) : _nodeInfo(other._nodeInfo), _grabResult(other._grabResult)
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
    return _nodeInfo.nodeId;
}

/*-----------------------------------------------------------------*/
/*----------------------AGGREGATOR COUNTER-------------------------*/
/*-----------------------------------------------------------------*/
AggregatorCounter::AggregatorCounter(AggregationInfoNode const &infoNode) : AggregatorImpl(infoNode)
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
AggregatorFounder::AggregatorFounder(AggregationInfoNode const &infoNode) : AggregatorImpl(infoNode), _isFound(false)
{

}

AggregatorFounder::~AggregatorFounder()
{

}

void AggregatorFounder::tryAggregation(std::string const &logStr)
{
}
