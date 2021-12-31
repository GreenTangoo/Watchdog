#include "symptom_checking.hpp"

using namespace correlation_space;

static std::map<symptomCategory, std::string> categoryStringSearchMap = 
    {
        {PORT_SCANNING, "port_scanning"}, {LFI, "lfi"}
    };

/*-----------------------------------------------------------------------------*/
/*--------------------------SYMPTOM_CHECK_WRAPPER------------------------------*/
/*-----------------------------------------------------------------------------*/
SymptomCheckWrapper::SymptomCheckWrapper() : sympType(SYMPTOM_CATEGORY_NONE)
{

}

SymptomCheckWrapper::SymptomCheckWrapper(symptomCategory sympType) : sympType(sympType)
{

}

SymptomCheckWrapper::SymptomCheckWrapper(SymptomCheckWrapper const &other) :
    sympType(other.sympType), parser(other.parser)
{

}

SymptomCheckWrapper::SymptomCheckWrapper(SymptomCheckWrapper &&other) :
    sympType(std::move(other.sympType)), parser(std::move(other.parser))
{

}

SymptomCheckWrapper const& SymptomCheckWrapper::operator=(SymptomCheckWrapper const &other)
{
    if(this != &other)
    {
        sympType = other.sympType;
        parser = other.parser;
    }

    return *this;
}

SymptomCheckWrapper const& SymptomCheckWrapper::operator=(SymptomCheckWrapper &&other)
{
    if(this != &other)
    {
        sympType = other.sympType;
        other.sympType = SYMPTOM_CATEGORY_NONE;

        parser = std::move(other.parser);
    }

    return *this;
}

/*-----------------------------------------------------------------------------*/
/*----------------------SYMPTOM_CATEGORY_RESOLVER------------------------------*/
/*-----------------------------------------------------------------------------*/
symptomCategory SymptomCategoryResolver::stringToSymptomCategory(std::string const &symptomStr)
{
    for(std::map<symptomCategory, std::string>::const_iterator it = categoryStringSearchMap.begin();
        it != categoryStringSearchMap.end(); it++)
    {
        if(it->second == symptomStr)
        {
            return it->first;
        }
    }

    throw CorrelationException("Incorrect symptom string parameter",
        CorrelationException::INCORRECT_SEARCH_STRING);
}

std::string SymptomCategoryResolver::symptomCategoryToString(symptomCategory sympCategory)
{
    auto it = categoryStringSearchMap.find(sympCategory);
    if(it != categoryStringSearchMap.end())
    {
        return it->second;
    }

    throw CorrelationException("Incorrect symptom category parameter",
        CorrelationException::INCORRECT_SEARCH_CATEGORY);
}
