#include "symptom_custom_checking.hpp"

namespace correlation_space
{
    /*-----------------------------------------------------------------------------*/
    /*-----------------------------SYMPTOM_CHECK_CUSTOM----------------------------*/
    /*-----------------------------------------------------------------------------*/
    SymptomCheckCustom::SymptomCheckCustom(symptomCategory sympType, SearchInfo const &infoSearch) :
        SymptomCheckWrapper (sympType), info(infoSearch)
    {

    }

    SymptomCheckCustom::SymptomCheckCustom(SymptomCheckCustom const &other) : info(other.info)
    {

    }

    SymptomCheckCustom::SymptomCheckCustom(SymptomCheckCustom &&other) : info(std::move(other.info))
    {

    }

    SymptomCheckCustom::~SymptomCheckCustom()
    {

    }

    SymptomCheckCustom::symptomFoundResult SymptomCheckCustom::tryFoundSymptom()
    {

    }
}
