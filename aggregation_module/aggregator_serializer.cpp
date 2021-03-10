#include "aggregator_serializer.hpp"

using namespace aggregation_space;

/*-----------------------------------------------------------------*/
/*----------------------AGGREGATION JSON RESULT--------------------*/
/*-----------------------------------------------------------------*/
AggregationResult::AggregationResult() : nodeId(-1)
{

}

AggregationResult::AggregationResult(int nodeId) : nodeId(nodeId)
{

}

AggregationResult::AggregationResult(AggregationResult const &other) :
    nodeId(other.nodeId), key(other.key), value(other.value)
{

}

AggregationResult::AggregationResult(AggregationResult &&other) : 
    nodeId(other.nodeId), key(std::move(other.key)), value(std::move(other.value))
{

}

AggregationResult& AggregationResult::operator=(AggregationResult const &other)
{
    if(this != &other)
    {
        this->nodeId = other.nodeId;
        this->key = other.key;
        this->value = other.value;
    }

    return *this;
}

AggregationResult& AggregationResult::operator=(AggregationResult &&other)
{
    if(this != &other)
    {
        this->nodeId = other.nodeId;
        this->key = std::move(other.key);
        this->value = std::move(other.value);
    }

    return *this;
}

/*-----------------------------------------------------------------*/
/*----------------------AGGREGATION JSON RESULT--------------------*/
/*-----------------------------------------------------------------*/
AggregationJsonResult::AggregationJsonResult() : 
    AggregationResult(), jsonNode(typeNodeJSON::NONE) 
{

}

AggregationJsonResult::AggregationJsonResult(int nodeId, typeNodeJSON jsonNode, std::string parentPath) :
    AggregationResult(nodeId), jsonNode(jsonNode), parentPath(parentPath) 
{

}

AggregationJsonResult::AggregationJsonResult(AggregationJsonResult const &other) : 
    AggregationResult(other), jsonNode(other.jsonNode), parentPath(other.parentPath)
{

}

AggregationJsonResult::AggregationJsonResult(AggregationJsonResult &&other) :
    AggregationResult(other), jsonNode(other.jsonNode), parentPath(std::move(other.parentPath))
{

}

AggregationJsonResult& AggregationJsonResult::operator=(AggregationJsonResult const &other)
{
    AggregationResult::operator=(other);
    if(this != &other)
    {
        this->jsonNode = other.jsonNode;
        this->parentPath = other.parentPath;
    }

    return *this;
}

AggregationJsonResult& AggregationJsonResult::operator=(AggregationJsonResult &&other)
{
    AggregationResult::operator=(std::move(other));
    if(this != &other)
    {
        this->jsonNode = other.jsonNode;
        this->parentPath = std::move(other.parentPath);
    }

    return *this;
}

/*-----------------------------------------------------------------*/
/*----------------------IAGGREGATOR SERIALIZER---------------------*/
/*-----------------------------------------------------------------*/
IAggregatorSerializer::~IAggregatorSerializer()
{

}

/*-----------------------------------------------------------------*/
/*-----------------------AGGREGATOR SERIALIZER IMPL----------------*/
/*-----------------------------------------------------------------*/
AggregatorSerializerImpl::AggregatorSerializerImpl(std::string const &resultFilename) : 
    _resultFilename(resultFilename)
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
AggregatorJsonSerializer::AggregatorJsonSerializer(AggrJsonResultVec const &aggrsVec, std::string const &resultFilename) : 
    AggregatorSerializerImpl(resultFilename), _aggrsVec(aggrsVec)
{

}

AggregatorJsonSerializer::~AggregatorJsonSerializer()
{

}

void AggregatorJsonSerializer::serialize()
{
    AggrJsonResultVec copyAggrVec(_aggrsVec.begin(), _aggrsVec.end());

    FileManipulator jsonManip(_resultFilename, FileManipulator::READ_WRITE | FileManipulator::LARGE_FILE);
    _aggrJson = getJsonData(jsonManip->getStream());

    std::sort(copyAggrVec.begin(), copyAggrVec.end(), 
        [](AggregationJsonResult &first, AggregationJsonResult &second) -> bool
    {
        size_t firstPathLen = StringManager::parseByDelimiter(first.parentPath, "/").size();
        size_t secondPathLen = StringManager::parseByDelimiter(second.parentPath, "/").size();

        return secondPathLen > firstPathLen;
    });

    for(AggregationJsonResult const &aggrRes : copyAggrVec)
    {
        try
        {
            switch(aggrRes.jsonNode)
            {
            case typeNodeJSON::OBJECT:
            {
                _aggrJson.addOrUpdateNode(aggrRes.key, aggrRes.parentPath + "/" + aggrRes.key);
            }
                break;
            case typeNodeJSON::STRING:
            {
                std::pair<std::string, std::string> keyValue(aggrRes.key, aggrRes.value);
                _aggrJson.addOrUpdateString(keyValue, aggrRes.parentPath + "/" + aggrRes.key);
            }
                break;
            default:
                // THROW
                break;
            }
        }
        catch(JsonException const &ex)
        {
            //DEBUG
        }
    }

    jsonManip->synchronizationStream();
    _aggrJson.setJson(jsonManip->getStream());
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