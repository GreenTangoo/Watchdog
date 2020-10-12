#include "symptom_implementation.hpp"

#define _PORT_SCANNING "port_scanning"
#define _LFI "lfi"

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
    if(symptomStr == _PORT_SCANNING)
        return PORT_SCANNING;
    if(symptomStr == _LFI)
        return LFI;
}

std::string SymptomCategoryResolver::symptomCategoryToString(symptomCategory sympCategory)
{
    switch(sympCategory)
    {
    case PORT_SCANNING:
        return std::string("port_scanning");
        break;
    case LFI:
        return std::string("lfi");
        break;
    }
}