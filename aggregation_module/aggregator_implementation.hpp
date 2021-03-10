#ifndef AGGREGATOR_IMPLEMENTATION
#define AGGREGATOR_IMPLEMENTATION

#include <memory>

#include "aggr_type_implementation.hpp"
#include "aggregator_serializer.hpp"

using namespace description_space;

namespace aggregation_space
{
    typedef std::pair<std::shared_ptr<AggregatorTypeImpl>, std::shared_ptr<AggregationJsonResult>> GrabTypeResultPair;

    class AggrTypeManager
    {
    public:
        AggrTypeManager(std::vector<GrabTypeResultPair> &subAggregatorsResultVec);
        AggrTypeManager(AggrTypeManager const &other) = delete;
        AggrTypeManager(AggrTypeManager &&other) = delete;
        AggrTypeManager const& operator=(AggrTypeManager const &other) = delete;
        AggrTypeManager const& operator=(AggrTypeManager &&other) = delete;
        ~AggrTypeManager();
        std::string getKey(int idNode);
        std::string getValue(int idNode);
        void setKey(int idNode, std::string newKey);
        void setValue(int idNode, std::string newValue);
    private:
        std::vector<GrabTypeResultPair> &_subAggregatorsResultVec;
    };

    class IAggregator
    {
    public:
        IAggregator();
        virtual ~IAggregator();
        virtual void runAggregation() = 0;
        virtual void saveResult() = 0;
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
        void initializeAggrTypeVec();
        void fillAggrResultStructs();
        void resolveFormatParams();
        std::vector<std::pair<RegexSiem, std::string>> generateRegexVec(std::vector<int> const &idValues);
    private:
        std::shared_ptr<AggregationJsonInfo const> _jsonInfoPtr;
        std::vector<GrabTypeResultPair> _subAggregatorsResultVec;
        std::shared_ptr<AggrTypeManager> _manager;
    };

    std::shared_ptr<AggregatorTypeImpl> create_aggregator_type(AggregationInfoNode const &jsonGrabInfoNode,
        AggrTypeManager &manager, AggregationInfoNode const &infoNode);
}

#endif // AGGREGATOR_IMPLEMENTATION