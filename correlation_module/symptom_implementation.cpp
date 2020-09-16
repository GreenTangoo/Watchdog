#include "symptom_implementation.hpp"

#define PORT_SCANNING "port_scanning"
#define LFI "lfi"

using namespace correlation_space;

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
    if(symptomStr == PORT_SCANNING)
        return port_scanning;
    if(symptomStr == LFI)
        return lfi;
}

std::string SymptomCategoryResolver::symptomCategoryToString(symptomCategory sympCategory)
{
    switch(sympCategory)
    {
    case port_scanning:
        return std::string("port_scanning");
        break;
    case lfi:
        return std::string("lfi");
        break;
    }
}