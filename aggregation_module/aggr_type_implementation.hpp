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
    class IAggregatorType
    {
    public:
        virtual ~IAggregatorType();
        virtual void tryAggregation(std::string const &logStr) = 0;
    };

    class AggregatorTypeImpl : public IAggregatorType
    {
    public:
        explicit AggregatorTypeImpl();
        explicit AggregatorTypeImpl(AggregatorTypeImpl const &other) = default;
        AggregatorTypeImpl(AggregatorTypeImpl &&other) = delete;
        virtual ~AggregatorTypeImpl();
        AggregatorTypeImpl const& operator=(AggregatorTypeImpl const &other) = delete;
        AggregatorTypeImpl const& operator=(AggregatorTypeImpl &&other) = delete;
        virtual void tryAggregation(std::string const &logStr) = 0;
    };

    class AggregatorTypeCounter : public AggregatorTypeImpl
    {
    public:
        AggregatorTypeCounter();
        virtual ~AggregatorTypeCounter();
        virtual void tryAggregation(std::string const &logStr) override;
    private:
        std::pair<std::string, int> _grabResult;
    };

    class AggregatorTypeFounder : public AggregatorTypeImpl
    {
    public:
        AggregatorTypeFounder();
        virtual ~AggregatorTypeFounder();
        virtual void tryAggregation(std::string const &logStr) override;
    private:
        bool _isFound;
        std::pair<std::string, std::string> _grabResult;
    };

    std::shared_ptr<AggregatorTypeImpl> create_aggregator_type(AggregationJsonInfoNode const &jsonGrabInfoNode);
}

#endif // AGGREGATOR_IMPLEMENTATION_HPP