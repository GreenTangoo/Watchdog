#ifndef SYMPTOM_GRABBER_HPP
#define SYMPTOM_GRABBER_HPP

#include <map>
#include <memory>
#include <algorithm>

#include "aggregator_implementation.hpp"

using namespace utility_space;
using namespace description_space;
using namespace siem_ex_space;

namespace aggregation_space
{
    class SymptomGrabber
    {
    public:
        SymptomGrabber(std::shared_ptr<AggregationInfo const> infoPtr, grabberCategory grabType);
        SymptomGrabber(SymptomGrabber const &other);
        SymptomGrabber(SymptomGrabber &&other);
        ~SymptomGrabber();
        void tryAggregationInfo();
    public:
        class AggregationException : public SIEMExecption
        {
        private:
            int _grabCategory;
        public:
            enum AggregationErrorCode { FAILED_AGGREGATION = 1 };
            AggregationException(std::string const &exMsg, int errCode, int grabType);
            AggregationException(std::string &&exMsg, int errCode, int grabType);
            ~AggregationException();
            int getGrabberType() const noexcept;
        };
    private:
        grabberCategory _grabType;
        std::shared_ptr<IAggregator> _aggregator;
    };

    std::shared_ptr<AggregatorImpl> create_aggregator(std::shared_ptr<AggregationInfo const> infoPtr);
}

#endif // SYMPTOM_GRABBER_HPP