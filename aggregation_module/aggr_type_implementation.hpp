#ifndef AGGREGATOR_IMPLEMENTATION_HPP
#define AGGREGATOR_IMPLEMENTATION_HPP

#include <string>

#include "../utility_module/json.hpp"
#include "../utility_module/encryption.hpp"
#include "../utility_module/filesystem_siem.hpp"
#include "../utility_module/regex_siem.hpp"
#include "../description_module/configuration.hpp"
#include "../exception_module/exceptions.hpp"

using namespace utility_space;
using namespace description_space;
using namespace siem_ex_space;

namespace aggregation_space
{
    class AggrTypeManager;

    class IAggregatorType
    {
    public:
        virtual ~IAggregatorType();
        virtual void tryAggregation(std::string const &logStr) = 0;
    };

    class AggregatorTypeImpl : public IAggregatorType
    {
    public:
        explicit AggregatorTypeImpl(std::pair<RegexSiem, int> keyRegex, std::pair<RegexSiem, int> valueRegex,
            AggrTypeManager &manager, AggregationInfoNode const &infoNode);
        explicit AggregatorTypeImpl(AggregatorTypeImpl const &other);
        AggregatorTypeImpl(AggregatorTypeImpl &&other) = delete;
        virtual ~AggregatorTypeImpl();
        AggregatorTypeImpl const& operator=(AggregatorTypeImpl const &other) = delete;
        AggregatorTypeImpl const& operator=(AggregatorTypeImpl &&other) = delete;
        virtual void tryAggregation(std::string const &logStr) = 0;
        virtual std::pair<std::string, std::string> getResult() = 0;
    protected:
        std::pair<RegexSiem, int> _keyRegexInfo;
        std::pair<RegexSiem, int> _valueRegexInfo;
        bool _isFoundKey;
        AggrTypeManager &_manager;
        AggregationInfoNode const &_infoNode;
    };

    class AggregatorTypeCounter : public AggregatorTypeImpl
    {
    public:
        AggregatorTypeCounter(std::pair<RegexSiem, int> keyRegex, std::pair<RegexSiem, int> valueRegex,
            AggrTypeManager &manager, AggregationInfoNode const &infoNode);
        virtual ~AggregatorTypeCounter();
        virtual void tryAggregation(std::string const &logStr) override;
        virtual std::pair<std::string, std::string> getResult() override;
    private:
        std::pair<std::string, int> _grabResult;
    };

    class AggregatorTypeFounder : public AggregatorTypeImpl
    {
    public:
        AggregatorTypeFounder(std::pair<RegexSiem, int> keyRegex, std::pair<RegexSiem, int> valueRegex,
            AggrTypeManager &manager, AggregationInfoNode const &infoNode);
        virtual ~AggregatorTypeFounder();
        virtual void tryAggregation(std::string const &logStr) override;
        virtual std::pair<std::string, std::string> getResult() override;
        inline bool isFound();
    private:
        bool _isFoundValue;
        std::pair<std::string, std::string> _grabResult;
    };

    
}

#endif // AGGREGATOR_IMPLEMENTATION_HPP