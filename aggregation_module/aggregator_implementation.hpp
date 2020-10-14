#ifndef AGGREGATOR_IMPLEMENTATION_HPP
#define AGGREGATOR_IMPLEMENTATION_HPP

#include <map>

#include "../utility_module/json.hpp"
#include "../utility_module/encryption.hpp"
#include "../description_module/configuration.hpp"

using namespace utility_space;
using namespace description_space;

namespace aggregation_space
{
    enum grabberCategory { IPTABLES = 0 };

    class SymptomGrabber
    {
    private:
        JsonObject m_parser;
        std::unique_ptr<Encryption> m_encryptor; 
        grabberCategory m_grabType;
        AggregationInfo const &m_info;
    public:
        SymptomGrabber(AggregationInfo const &info, grabberCategory grabType);
        SymptomGrabber(SymptomGrabber const &other);
        SymptomGrabber(SymptomGrabber &&other);
        ~SymptomGrabber();
        bool tryAggregationInfo();
    };

    class GrabberCategoryResolver // STATIC CLASS
    {
    public:
        static grabberCategory stringToGrabberCategory(std::string const &grabName);
        static std::string grabberCategoryToString(grabberCategory grabCatebory);   
    };
}

#endif // AGGREGATOR_IMPLEMENTATION_HPP