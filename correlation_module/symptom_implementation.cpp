#include "symptom_implementation.hpp"

using namespace correlation_space;

static std::map<symptomCategory, std::string> categoryStringSearchMap = 
    {
        {PORT_SCANNING, "port_scanning"}, {LFI, "lfi"}
    };

/*-----------------------------------------------------------------------------*/
/*-----------------------------SYMPTOM_CHECKER---------------------------------*/
/*-----------------------------------------------------------------------------*/
SymptomChecker::SymptomChecker(SearchInfo const &infoSearch, symptomCategory sympType) : 
    info(infoSearch)
{

}

SymptomChecker::SymptomChecker(SymptomChecker const &other) : info(other.info)
{

}

SymptomChecker::SymptomChecker(SymptomChecker &&other) : info(std::move(other.info))
{

}

SymptomChecker::~SymptomChecker()
{

}

bool SymptomChecker::tryFoundSymptom()
{

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