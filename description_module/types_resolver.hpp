#ifndef TYPES_RESOLVER_HPP
#define TYPES_RESOLVER_HPP

#include <string>
#include <map>

#include "../exception_module/exceptions.hpp"

using namespace siem_ex_space;

namespace description_space
{
    enum behaviourType { NONE_BEHAVIOUR = 0, JSON_BEHAVIOUR, XML_BEHAVIOUR };
    enum aggrType { NO_AGGR_TYPE = 0, FINDER, COUNTER };
    enum grabberCategory { GRAB_NONE_TYPE = 0, IPTABLES, APACHE };

    class BehaviourTypeResolver // STATIC CLASS
    {
    public:
        static behaviourType stringToSerializerType(std::string const &serializerName);
        static std::string serializerTypeToString(behaviourType type);   
    };

    class GrabberCategoryResolver // STATIC CLASS
    {
    public:
        static grabberCategory stringToGrabberCategory(std::string const &grabName);
        static std::string grabberCategoryToString(grabberCategory grabCatebory);   
    };

    class TypesException : public SIEMException
    {
    private:
        int _typeErrCode;
    public:
        enum TypesErrorCode { INVALID_CAST_TYPE = 1 };
        TypesException(std::string const &exMsg, int errCode);
        TypesException(std::string &&exMsg, int errCode);
        ~TypesException();
    };
}

#endif // TYPES_RESOLVER_HPP
