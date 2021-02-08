#ifndef SYMPTOM_GRABBER_HPP
#define SYMPTOM_GRABBER_HPP

#include <map>
#include <memory>
#include <algorithm>

#include "aggr_type_implementaion.hpp"
#include "aggregator_serializer.hpp"

using namespace utility_space;
using namespace description_space;
using namespace siem_ex_space;

namespace aggregation_space
{
    enum grabberCategory { GRAB_NONE_TYPE = 0, IPTABLES, APACHE };

    class SymptomGrabber
    {
    public:
        SymptomGrabber(AggregationInfo const &info, grabberCategory grabType);
        SymptomGrabber(SymptomGrabber const &other);
        SymptomGrabber(SymptomGrabber &&other);
        ~SymptomGrabber();
        void tryAggregationInfo();
    private:
        void initializeAggregators();
        std::shared_ptr<AggregatorImpl> createAggregator(AggregationInfoNode const &infoNode);
        void startAggregate(FileManipulator file);
        void resolveFormatValues();
    public:
        class AggregationException : public SIEMExecption
        {
        private:
            int _grabCategory;
        public:
            enum AggregationErrorCode { INCORRECT_AGGR_CATEGORY = 1, INCORRECT_AGGR_STRING,
                                        FAILED_AGGREGATION };
            AggregationException(std::string const &exMsg, int errCode, int grabType);
            AggregationException(std::string &&exMsg, int errCode, int grabType);
            ~AggregationException();
            int getGrabberType() const noexcept;
        };
    private:
        grabberCategory _grabType;
        AggregationInfo const &_info;
        std::unique_ptr<Encryption> _encryptor; 
        std::vector<std::shared_ptr<AggregatorImpl>> _aggregators;
    };

    class GrabberCategoryResolver // STATIC CLASS
    {
    public:
        static grabberCategory stringToGrabberCategory(std::string const &grabName);
        static std::string grabberCategoryToString(grabberCategory grabCatebory);   
    };
}

#endif // SYMPTOM_GRABBER_HPP