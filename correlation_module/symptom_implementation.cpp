#include "symptom_implementation.hpp"

using namespace correlation_space;

SymptomChecker::SymptomChecker(SearchInfo const &infoSearch, symptomCategory sympType) : info(infoSearch)
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