#ifndef AGGREGATOR_IMPLEMENTATION_HPP
#define AGGREGATOR_IMPLEMENTATION_HPP

#include <map>

#include "../utility_module/json.hpp"
#include "../utility_module/encryption.hpp"
#include "../description_module/configuration.hpp"

using namespace utility_space;

namespace aggregation_space
{
    enum grabberCategory { IPTABLES = 0 };

    class SymptomGrabber
    {
    };

    class GrabberCategoryResolver // STATIC CLASS
    {
    public:
        static grabberCategory stringToGrabberCategory(std::string const &grabName);
        static std::string grabberCategoryToString(grabberCategory grabCatebory);   
    };
}

#endif // AGGREGATOR_IMPLEMENTATION_HPP