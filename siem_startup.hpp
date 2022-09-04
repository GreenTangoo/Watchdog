#ifndef SIEM_STARTUP_HPP
#define SIEM_STARTUP_HPP

#include <string>
#include <vector>
#include <algorithm>

#include "utility_module/json_proc.hpp"
#include "description_module/configuration.hpp"

using namespace description_space;
using namespace utility_space;

namespace main_siem_space
{
    enum correlationModule { NONE_CORRELATION = 0, STATISTIC, AI, MACHINE};

    class SettingsSIEM
    {
    public:
        explicit SettingsSIEM(IJsonContainerPtr const &configObj);
        explicit SettingsSIEM(Configuration const &config);
        SettingsSIEM(SettingsSIEM const &other);
        SettingsSIEM(SettingsSIEM &&other);
        ~SettingsSIEM() = default;
        SettingsSIEM const& operator=(SettingsSIEM const &other);
        SettingsSIEM const& operator=(SettingsSIEM &&other);
        size_t getAmountAggregationThreads() const { return _amountAggrThreads; }
        size_t getAmountCorrelationThreads() const { return _amountCorrThreads; }
        std::string getAggregationConfigPath() const { return _aggregationCustomConfigPath; }
        std::string getCorrelationConfigPath() const { return _correlationCustomConfigPath; }
        std::vector<correlationModule> getCorrelationModuleType() const { return _kindCorrelation; }
        bool getIsUseCustomAggr() const { return m_isUseCustomAggregation; }
        bool getIsUseCustomCorr() const { return m_isUseCustomCorrelation; }
    private:
        void tuneFromConfig();
    private:
        Configuration _startupSettings;
        size_t _amountAggrThreads;
        size_t _amountCorrThreads;
        std::string _aggregationCustomConfigPath;
        std::string _correlationCustomConfigPath;
        std::vector<correlationModule> _kindCorrelation;
        bool m_isUseCustomAggregation;
        bool m_isUseCustomCorrelation;
    };

    correlationModule stringToCorrelationModule(std::string correlationModuleStr);
    std::string correlationModuleToString(correlationModule correlationKind);
}

#endif // SIEM_STARTUP_HPP
