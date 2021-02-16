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
AggregatorJson::AggregatorJson(std::shared_ptr<AggregationInfo const> infoPtr)
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

}

void AggregatorJson::saveResult()
{

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