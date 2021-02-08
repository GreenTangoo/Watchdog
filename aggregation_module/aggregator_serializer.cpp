#include "aggregator_serializer.hpp"

using namespace aggregation_space;

static std::map<behaviourType, std::string> serializerTypeStringMap = 
    { {JSON_BEHAVIOUR, "json"}, {XML_BEHAVIOUR, "xml"} };

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
AggregatorSerializerImpl::SerializationException::SerializationException(std::string const &exMsg, int errCode, int saveType) :
    SIEMExecption(exMsg, errCode), _serializeType(saveType)
{

}

AggregatorSerializerImpl::SerializationException::SerializationException(std::string &&exMsg, int errCode, int saveType) :
    SIEMExecption(std::move(exMsg), errCode), _serializeType(saveType)
{

}

AggregatorSerializerImpl::SerializationException::~SerializationException()
{

}

int AggregatorSerializerImpl::SerializationException::getSerializationType() const noexcept
{
    return _serializeType;
}

/*-----------------------------------------------------------------*/
/*----------------SERIALIZE TYPE RESOLVER--------------------------*/
/*-----------------------------------------------------------------*/
behaviourType BehaviourTypeResolver::stringToSerializerType(std::string const &serializerName)
{
    for(std::map<behaviourType, std::string>::const_iterator it = serializerTypeStringMap.begin();
        it != serializerTypeStringMap.end(); it++)
    {
        if(it->second == serializerName)
        {
            return it->first;
        }
    }

    throw AggregatorSerializerImpl::SerializationException("Invalid serializer name",
        AggregatorSerializerImpl::SerializationException::INCORRECT_BEHAVIOUR_STRING,
        static_cast<int>(behaviourType::NONE_BEHAVIOUR));
}

std::string BehaviourTypeResolver::serializerTypeToString(behaviourType type)
{
    auto it = serializerTypeStringMap.find(type);
    if(it != serializerTypeStringMap.end())
    {
        return it->second;
    }

    throw AggregatorSerializerImpl::SerializationException("Invalid behaviour type",
        AggregatorSerializerImpl::SerializationException::INCORRECT_BEHAVIOUR_TYPE,
        static_cast<int>(type));
}
