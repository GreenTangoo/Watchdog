#ifndef AGGREGATOR_IMPLEMENTATION_HPP
#define AGGREGATOR_IMPLEMENTATION_HPP

#include <string>

#include "../utility_module/json.hpp"
#include "../utility_module/encryption.hpp"
#include "../utility_module/filesystem_siem.hpp"
#include "../utility_module/regex_siem.hpp"
#include "../utility_module/format_string.hpp"
#include "../description_module/configuration.hpp"
#include "../exception_module/exceptions.hpp"

using namespace utility_space;
using namespace description_space;
using namespace siem_ex_space;

namespace aggregation_space
{
    struct AggregationResult
    {
        typeNodeJSON jsonNode;
        std::string key;
        std::string value;
        std::string parentPath;

        AggregationResult() : jsonNode(typeNodeJSON::NONE) {}

        AggregationResult(AggregationResult const &other) : 
            jsonNode(other.jsonNode), key(other.key), value(other.value), parentPath(other.parentPath)
        {}
    };

    class IAggregator
    {
    public:
        virtual ~IAggregator();
        virtual void tryAggregation(std::string const &logStr) = 0;
    };

    class AggregatorImpl : public IAggregator
    {
    public:
        explicit AggregatorImpl(AggregationInfoNode const &nodeInfo);
        explicit AggregatorImpl(AggregatorImpl const &other);
        AggregatorImpl(AggregatorImpl &&other) = delete;
        virtual ~AggregatorImpl();
        AggregatorImpl const& operator=(AggregatorImpl const &other) = delete;
        AggregatorImpl const& operator=(AggregatorImpl &&other) = delete;
        virtual void tryAggregation(std::string const &logStr) = 0;
        AggregationResult getAggrResult();
        AggregationResult& getAggreResultRef();
        int getNodeId();
    protected:
        AggregationInfoNode const &_nodeInfo;
        AggregationResult _grabResult;
    };

    class AggregatorCounter : public AggregatorImpl
    {
    public:
        AggregatorCounter(AggregationInfoNode const &infoNode);
        virtual ~AggregatorCounter();
        virtual void tryAggregation(std::string const &logStr) override;
    };

    class AggregatorFounder : public AggregatorImpl
    {
    public:
        AggregatorFounder(AggregationInfoNode const &infoNode);
        virtual ~AggregatorFounder();
        virtual void tryAggregation(std::string const &logStr) override;
    private:
        bool _isFound;
    };
}

#endif // AGGREGATOR_IMPLEMENTATION_HPP