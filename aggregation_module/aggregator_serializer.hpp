#ifndef AGGREGATOR_SERIALIZER_HPP
#define AGGREGATOR_SERIALIZER_HPP

#include <vector>
#include <memory>

#include "../exception_module/exceptions.hpp"
#include "../utility_module/json.hpp"

using namespace siem_ex_space;
using namespace utility_space;

namespace aggregation_space
{
    struct AggregationJsonResult; 

    typedef std::vector<std::shared_ptr<AggregationJsonResult>> AggrResultVec;

    struct AggregationJsonResult
    {
        int nodeId;
        typeNodeJSON jsonNode;
        std::string key;
        std::string value;
        std::string parentPath;

        AggregationJsonResult() : nodeId(-1), jsonNode(typeNodeJSON::NONE) {}

        AggregationJsonResult(AggregationJsonResult const &other) : 
            nodeId(other.nodeId), jsonNode(other.jsonNode), key(other.key), value(other.value), parentPath(other.parentPath)
        {}
    };

    class IAggregatorSerializer
    {
    public:
        virtual ~IAggregatorSerializer();
        virtual void serialize() = 0;
    };

    class AggregatorSerializerImpl : public IAggregatorSerializer
    {
    public:
        AggregatorSerializerImpl(AggrResultVec const &aggrsVec, std::string const &resultFilename);
        virtual ~AggregatorSerializerImpl();
        virtual void serialize() = 0;
        void setResultFilename(std::string const filename);
    public:
        class SerializationException : public SIEMExecption
        {
        public:
            enum SerializationErrorCode { FAILED_SERIALIZATION = 1 };
            SerializationException(std::string const &exMsg, int errCode);
            SerializationException(std::string &&exMsg, int errCode);
            ~SerializationException();
        };
    protected:
        AggrResultVec const &_aggrsVec;
        std::string _resultFilename;
    };

    class AggregatorJsonSerializer : public AggregatorSerializerImpl
    {
    public:
        AggregatorJsonSerializer(AggrResultVec const &aggrsVec, std::string const &resultFilename);
        virtual ~AggregatorJsonSerializer();
        virtual void serialize();
    };

}


#endif // AGGREGATOR_SERIALIZER_HPP