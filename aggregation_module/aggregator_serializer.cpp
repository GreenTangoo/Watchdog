#include "aggregator_serializer.hpp"

using namespace aggregation_space;


/*-----------------------------------------------------------------*/
/*----------------------IAGGREGATOR SERIALIZER---------------------*/
/*-----------------------------------------------------------------*/
IAggregatorSerializer::~IAggregatorSerializer()
{

}

/*-----------------------------------------------------------------*/
/*-----------------------AGGREGATOR SERIALIZER IMPL----------------*/
/*-----------------------------------------------------------------*/
AggregatorSerializerImpl::AggregatorSerializerImpl(AggrResultVec const &aggrsVec, std::string const &resultFilename) : 
    _aggrsVec(aggrsVec)
{

}

AggregatorSerializerImpl::~AggregatorSerializerImpl()
{

}

void AggregatorSerializerImpl::setResultFilename(std::string const filename)
{
    _resultFilename = filename;
}

/*-----------------------------------------------------------------*/
/*-----------------------AGGREGATOR JSON SERIALIZER----------------*/
/*-----------------------------------------------------------------*/
AggregatorJsonSerializer::AggregatorJsonSerializer(AggrResultVec const &aggrsVec, std::string const &resultFilename) : 
    AggregatorSerializerImpl(aggrsVec, resultFilename)
{

}

AggregatorJsonSerializer::~AggregatorJsonSerializer()
{

}

void AggregatorJsonSerializer::serialize()
{

}

/*-----------------------------------------------------------------*/
/*----------------SERIALIZATION EXCEPTION--------------------------*/
/*-----------------------------------------------------------------*/
AggregatorSerializerImpl::SerializationException::SerializationException(std::string const &exMsg, int errCode) :
    SIEMExecption(exMsg, errCode)
{

}

AggregatorSerializerImpl::SerializationException::SerializationException(std::string &&exMsg, int errCode) :
    SIEMExecption(std::move(exMsg), errCode)
{

}

AggregatorSerializerImpl::SerializationException::~SerializationException()
{

}