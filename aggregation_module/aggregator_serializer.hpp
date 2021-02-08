#ifndef AGGREGATOR_SERIALIZER_HPP
#define AGGREGATOR_SERIALIZER_HPP

#include "aggr_type_implementaion.hpp"

namespace aggregation_space
{
    typedef std::vector<std::shared_ptr<AggregationResult>> AggrResultVec;

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
        private:
            int _serializeType;
        public:
            enum SerializationErrorCode { INCORRECT_BEHAVIOUR_TYPE = 1, INCORRECT_BEHAVIOUR_STRING,
                                        FAILED_SERIALIZATION };
            SerializationException(std::string const &exMsg, int errCode, int saveType);
            SerializationException(std::string &&exMsg, int errCode, int saveType);
            ~SerializationException();
            int getSerializationType() const noexcept;
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

    class BehaviourTypeResolver // STATIC CLASS
    {
    public:
        static behaviourType stringToSerializerType(std::string const &serializerName);
        static std::string serializerTypeToString(behaviourType type);   
    };

}


#endif // AGGREGATOR_SERIALIZER_HPP