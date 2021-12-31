#include "correlation_module_initializer.hpp"

using namespace correlation_space;

/*-----------------------------------------------------------------------------*/
/*------------------------SYMPTOM_FOUND_INITIALIZER----------------------------*/
/*-----------------------------------------------------------------------------*/
SymptomFoundInitializer::SymptomFoundInitializer(SettingsSIEM const &settings)
{

}

SymptomFoundInitializer::SymptomFoundInitializer(SymptomFoundInitializer &&other) :
    m_AmountThreads(other.m_AmountThreads), m_Checkers(std::move(other.m_Checkers))
{
    other.m_AmountThreads = 0;
}

SymptomFoundInitializer::~SymptomFoundInitializer()
{

}

SymptomFoundInitializer const& SymptomFoundInitializer::operator=(SymptomFoundInitializer &&other)
{

}

void SymptomFoundInitializer::startCycle()
{

}

void SymptomFoundInitializer::initCustomCheckers(SettingsSIEM const &settings)
{

}

void SymptomFoundInitializer::initDefaultCheckers(SettingsSIEM const &settings)
{

}

/*-----------------------------------------------------------------------------*/
/*----------------------CORRELATION_MODULE_INITIALIZER-------------------------*/
/*-----------------------------------------------------------------------------*/
CorrelationModuleInitializer::CorrelationModuleInitializer(SettingsSIEM const &settings)
{

}

CorrelationModuleInitializer::CorrelationModuleInitializer(CorrelationModuleInitializer &&other)
{

}

CorrelationModuleInitializer::~CorrelationModuleInitializer()
{

}

CorrelationModuleInitializer const& CorrelationModuleInitializer::operator=(CorrelationModuleInitializer &&other)
{

}

void CorrelationModuleInitializer::startCorrelationCycle()
{

}
