#ifndef AGGREGATOR_SERIALIZER_HPP
#define AGGREGATOR_SERIALIZER_HPP

#include <vector>
#include <memory>

#include "../exception_module/exceptions.hpp"
#include "../utility_module/json.hpp"
#include "../utility_module/filesystem_siem.hpp"

using namespace siem_ex_space;
using namespace utility_space;

namespace aggregation_space
{
    struct AggregationJsonResult; 

    typedef std::vector<AggregationJsonResult> AggrJsonResultVec;

    struct AggregationResult
    {
        int nodeId;
        std::string key;
        std::string value;

        AggregationResult();
        AggregationResult(int nodeId);
        AggregationResult(AggregationResult const &other);
        AggregationResult(AggregationResult &&other);
        AggregationResult& operator=(AggregationResult const &other);
        AggregationResult& operator=(AggregationResult &&other);
    };

    struct AggregationJsonResult : public AggregationResult
    {
        typeNodeJSON jsonNode;
        std::string parentPath;

        AggregationJsonResult();
        AggregationJsonResult(int nodeId, typeNodeJSON jsonNode, std::string parentPath);
        AggregationJsonResult(AggregationJsonResult const &other);
        AggregationJsonResult(AggregationJsonResult &&other);
        AggregationJsonResult& operator=(AggregationJsonResult const &other);
        AggregationJsonResult& operator=(AggregationJsonResult &&other);
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
        AggregatorSerializerImpl(std::string const &resultFilename);
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
        std::string _resultFilename;
    };

    class AggregatorJsonSerializer : public AggregatorSerializerImpl
    {
    public:
        AggregatorJsonSerializer(AggrJsonResultVec const &aggrsVec, std::string const &resultFilename);
        virtual ~AggregatorJsonSerializer();
        virtual void serialize() override;
    private:
        AggrJsonResultVec const &_aggrsVec;
        JsonObject _aggrJson;
    };

}

#endif // AGGREGATOR_SERIALIZER_HPP