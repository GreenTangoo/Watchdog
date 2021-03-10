#include "symptom_grabber.hpp"

using namespace utility_space;
using namespace aggregation_space;
using namespace siem_ex_space;


/*-----------------------------------------------------------------*/
/*----------------------SYMPTOM GRABBER----------------------------*/
/*-----------------------------------------------------------------*/
SymptomGrabber::SymptomGrabber(std::shared_ptr<AggregationInfo const> infoPtr, grabberCategory grabType) :
    _grabType(grabType)
{
    _aggregator = create_aggregator(infoPtr);
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
    _aggregator->runAggregation();
    _aggregator->saveResult();
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