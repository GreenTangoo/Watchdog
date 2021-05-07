#ifndef SIEM_STARTUP_HPP
#define SIEM_STARTUP_HPP

#include <string>
#include <vector>
#include <algorithm>

#include "utility_module/json.hpp"
#include "description_module/configuration.hpp"

using namespace description_space;
using namespace utility_space;

namespace main_siem_space
{
    enum correlationModule { NONE_CORRELATION = 0, STATISTIC, AI, MACHINE};

    class SettingsSIEM
    {
    public:
        explicit SettingsSIEM(JsonObject const &configObj);
        explicit SettingsSIEM(Configuration const &config);
        SettingsSIEM(SettingsSIEM const &other);
        SettingsSIEM(SettingsSIEM &&other);
        ~SettingsSIEM() = default;
        SettingsSIEM const& operator=(SettingsSIEM const &other);
        SettingsSIEM const& operator=(SettingsSIEM &&other);
        size_t getAmountAggregationThreads() const { return _amountAggrThreads; };
        size_t getAmountCorrelationThreads() const { return _amountCorrThreads; };
        std::string getAggregationConfigPath() const { return _aggregationConfigPath; };
        std::string getCorrelationConfigPath() const { return _correlationConfigPath; }; 
        std::vector<correlationModule> getCorrelationModuleType() const { return _kindCorrelation; };
    private:
        void tuneFromConfig();
    private:
        Configuration _startupSettings;
        size_t _amountAggrThreads;
        size_t _amountCorrThreads;
        std::string _aggregationConfigPath;
        std::string _correlationConfigPath;
        std::vector<correlationModule> _kindCorrelation;
    };

    correlationModule stringToCorrelationModule(std::string correlationModuleStr);
    std::string correlationModuleToString(correlationModule correlationKind);
}

#endif // SIEM_STARTUP_HPP