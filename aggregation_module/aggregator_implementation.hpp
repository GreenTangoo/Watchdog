#ifndef AGGREGATOR_IMPLEMENTATION
#define AGGREGATOR_IMPLEMENTATION

#include <memory>

#include "aggr_type_implementation.hpp"
#include "aggregator_serializer.hpp"

using namespace description_space;

namespace aggregation_space
{
    typedef std::pair<std::shared_ptr<AggregatorTypeImpl>, AggregationJsonResult> GrabTypeResultPair;

    class IAggregator
    {
    public:
        IAggregator();
        virtual ~IAggregator();
        virtual void runAggregation() = 0;
    };

    class AggregatorImpl : public IAggregator
    {
    public:
        AggregatorImpl();
        virtual ~AggregatorImpl();
        virtual void runAggregation() = 0;
        virtual void saveResult() = 0;
    };

    class AggregatorJson : public AggregatorImpl
    {
    public:
        AggregatorJson(std::shared_ptr<AggregationInfo const> infoPtr);
        AggregatorJson(AggregatorJson const &other);
        AggregatorJson(AggregatorJson &&other);
        virtual ~AggregatorJson();
        virtual void runAggregation() override;
        virtual void saveResult() override;
    public:
        class AggregatorJsonException : public SIEMExecption
        {
        public:
            enum AggregatorJsonErrorCode { FAILED_JSON_AGGREGATION = 1 };
            AggregatorJsonException(std::string const &exMsg, int errCode);
            AggregatorJsonException(std::string &&exMsg, int errCode);
            ~AggregatorJsonException();
        };
    private:
        std::shared_ptr<AggregationJsonInfo const> _jsonInfoPtr;
        std::vector<std::shared_ptr<AggregatorTypeImpl>> _subAggregators;
        std::vector<GrabTypeResultPair> _subAggregatorsResultVec;
    };
}

#endif // AGGREGATOR_IMPLEMENTATION