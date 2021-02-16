#include "types_resolver.hpp"

using namespace description_space;

static std::map<behaviourType, std::string> serializerTypeStringMap = 
    { {JSON_BEHAVIOUR, "json"}, {XML_BEHAVIOUR, "xml"} };

static std::map<grabberCategory, std::string> categoryStringGrabMap = 
    {
        {IPTABLES, "iptables"}, {APACHE, "apache"}
    };

/*-----------------------------------------------------------------*/
/*----------------SERIALIZE TYPE RESOLVER--------------------------*/
/*-----------------------------------------------------------------*/
behaviourType BehaviourTypeResolver::stringToSerializerType(std::string const &serializerName)
{
    for(std::map<behaviourType, std::string>::const_iterator it = serializerTypeStringMap.begin();
        it != serializerTypeStringMap.end(); it++)
    {
        if(it->second == serializerName)
        {
            return it->first;
        }
    }

    throw TypesException("Invalid serializer name",
        TypesException::INVALID_CAST_TYPE);
}

std::string BehaviourTypeResolver::serializerTypeToString(behaviourType type)
{
    auto it = serializerTypeStringMap.find(type);
    if(it != serializerTypeStringMap.end())
    {
        return it->second;
    }

    throw TypesException("Invalid behaviour type",
        TypesException::INVALID_CAST_TYPE);
}

/*-----------------------------------------------------------------*/
/*----------------------GRABBER GATEGORY RESOLVER------------------*/
/*-----------------------------------------------------------------*/
grabberCategory GrabberCategoryResolver::stringToGrabberCategory(std::string const &grabName)
{
    for(std::map<grabberCategory, std::string>::const_iterator it = categoryStringGrabMap.begin();
        it != categoryStringGrabMap.end(); it++)
    {
        if(it->second == grabName)
        {
            return it->first;
        }
    }

    throw TypesException("Invalid aggregation string parameter",
        TypesException::INVALID_CAST_TYPE);
}

std::string GrabberCategoryResolver::grabberCategoryToString(grabberCategory grabCategory)
{
	auto it = categoryStringGrabMap.find(grabCategory);
    if(it != categoryStringGrabMap.end())
    {
        return it->second;
    }

    throw TypesException("Invalid aggregation category parameter",
        TypesException::INVALID_CAST_TYPE);
}

/*-----------------------------------------------------------------*/
/*----------------------TYPES EXCEPTION----------------------------*/
/*-----------------------------------------------------------------*/
TypesException::TypesException(std::string const &exMsg, int errCode) :
    SIEMExecption(exMsg, errCode)
{

}

TypesException::TypesException(std::string &&exMsg, int errCode) : 
    SIEMExecption(std::move(exMsg), errCode)
{

}

TypesException::~TypesException()
{

}