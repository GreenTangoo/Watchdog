#ifndef FORMAT_STRING_HPP
#define FORMAT_STRING_HPP

#include <string>
#include <vector>

#include "string_manager.hpp"

#define FORMAT_ATTR_ID "id"

namespace utility_space
{
    typedef std::vector<std::pair<std::string, std::string>> Attributes;

    class FormatString
    {
    public:
        FormatString();
        FormatString(std::string const &str);
        FormatString(FormatString const &other);
        FormatString(FormatString &&other);
        FormatString const& operator=(FormatString const &other);
        FormatString const& operator=(FormatString &&other);
        FormatString const& operator=(std::string const &str);
        FormatString const& operator+=(FormatString const &other);
        operator std::string();
        std::vector<Attributes> getAttributes();
        Attributes getFilterAttributes(std::string attributeName);
    private:
        void foundAttributes();
    private:
        std::string _containStr;
        bool _isNeedToRefounding;
        std::vector<Attributes> _attributesVec; 
    };

}

#endif // FORMAT_STRING_HPP