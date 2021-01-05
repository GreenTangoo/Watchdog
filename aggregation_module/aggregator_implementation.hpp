#ifndef AGGREGATOR_IMPLEMENTATION_HPP
#define AGGREGATOR_IMPLEMENTATION_HPP

#include <map>
#include <memory>

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
    namespace details
    {
        struct AggregationInfoNodeBuffer
        {
            int nodeId;
            aggrType grabType;
            typeNodeJSON typeNode;
            std::string parentPath;
            std::string keyName;
            std::string valueName;
        };
    }

    enum grabberCategory { GRAB_NONE_TYPE = 0, IPTABLES, APACHE };

    class SymptomGrabber
    {
    public:
        SymptomGrabber(AggregationInfo const &info, grabberCategory grabType);
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
            enum AggregationErrorCode { INCORRECT_AGGR_CATEGORY = 1, INCORRECT_AGGR_STRING,
                                        FAILED_AGGREGATION };
            AggregationException(std::string const &exMsg, int errCode, int grabType);
            AggregationException(std::string &&exMsg, int errCode, int grabType);
            ~AggregationException();
            int getGrabberType() const noexcept;
        };
    private:
        std::unique_ptr<std::vector<details::AggregationInfoNodeBuffer>> getFilledCfgBuf(FileManipulator &fileManip);

        details::AggregationInfoNodeBuffer getAggrInfoBuf(std::string const &oneLogStr, 
            std::vector<std::unique_ptr<AggregationInfoNode>> const &infoAggrVec);

        void putAggrInfoToJson(details::AggregationInfoNodeBuffer const &oneAggrInfo);
    private:
        JsonObject _parser;
        std::unique_ptr<Encryption> _encryptor; 
        grabberCategory _grabType;
        AggregationInfo const &_info;
    };

    class GrabberCategoryResolver // STATIC CLASS
    {
    public:
        static grabberCategory stringToGrabberCategory(std::string const &grabName);
        static std::string grabberCategoryToString(grabberCategory grabCatebory);   
    };
}

#endif // AGGREGATOR_IMPLEMENTATION_HPP