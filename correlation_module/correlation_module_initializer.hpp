#ifndef CORRELATION_MODULE_INITIALIZER_HPP
#define CORRELATION_MODULE_INITIALIZER_HPP

#include "symptom_checking.hpp"
#include "correlation/correlation.hpp"

#include "../siem_startup.hpp"
#include "../utility_module/thread_pool.hpp"

using namespace main_siem_space;
using namespace description_space;
using namespace utility_space;

namespace correlation_space
{
    class SymptomFoundInitializer
    {
    public:
        explicit SymptomFoundInitializer(SettingsSIEM const &settings);
        SymptomFoundInitializer(SymptomFoundInitializer const &other) = delete;
        SymptomFoundInitializer(SymptomFoundInitializer &&other);
        ~SymptomFoundInitializer();
        SymptomFoundInitializer const& operator=(SymptomFoundInitializer const &other) = delete;
        SymptomFoundInitializer const& operator=(SymptomFoundInitializer &&other);
        void startCycle();
    private:
        void initCustomCheckers(SettingsSIEM const &settings);
        void initDefaultCheckers(SettingsSIEM const &settings);
    private:
        size_t m_AmountThreads;
        std::vector<ISymptomCheckPtr> m_Checkers;
    };

    class CorrelationModuleInitializer
    {
    public:
        explicit CorrelationModuleInitializer(SettingsSIEM const &settings);
        CorrelationModuleInitializer(CorrelationModuleInitializer const &other) = delete;
        CorrelationModuleInitializer(CorrelationModuleInitializer &&other);
        ~CorrelationModuleInitializer();
        CorrelationModuleInitializer const& operator=(CorrelationModuleInitializer const &other) = delete;
        CorrelationModuleInitializer const& operator=(CorrelationModuleInitializer &&other);
        void startCorrelationCycle();
    private:
        size_t m_AmountThreads;
        std::vector<ICorrelationPtr> m_Correlations;
    };
}

#endif // CORRELATION_MODULE_INITIALIZER_HPP
