#ifndef AGGREGATOR_IMPLEMENTATION_HPP
#define AGGREGATOR_IMPLEMENTATION_HPP

#include <map>

#include "../utility_module/json.hpp"
#include "../utility_module/encryption.hpp"
#include "../utility_module/siem_filesystem.hpp"
#include "../description_module/configuration.hpp"
#include "../exception_module/exceptions.hpp"

using namespace utility_space;
using namespace description_space;

namespace aggregation_space
{
    enum grabberCategory { GRAB_NONE_TYPE = 0, IPTABLES, APACHE };

    class SymptomGrabber
    {
    private:
        JsonObject _parser;
        std::unique_ptr<Encryption> _encryptor; 
        grabberCategory _grabType;
        AggregationInfo const &_info;
        SiemFilesystem jsonFileStream;
        SiemFilesystem logFileStream;
    private:
        void aggregateOneInfoNode(AggregationInfoNode const *nodePtr);
    public:
        SymptomGrabber(AggregationInfo const &info, grabberCategory grabType);
        SymptomGrabber(SymptomGrabber const &other);
        SymptomGrabber(SymptomGrabber &&other);
        ~SymptomGrabber();
        void tryAggregationInfo();
    };

    class GrabberCategoryResolver // STATIC CLASS
    {
    public:
        static grabberCategory stringToGrabberCategory(std::string const &grabName);
        static std::string grabberCategoryToString(grabberCategory grabCatebory);   
    };
}

#endif // AGGREGATOR_IMPLEMENTATION_HPP