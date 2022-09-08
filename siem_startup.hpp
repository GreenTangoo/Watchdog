#ifndef SIEM_STARTUP_HPP
#define SIEM_STARTUP_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

#include "utility_module/json_proc.hpp"
#include "description_module/configuration.hpp"

using namespace description_space;
using namespace utility_space;

namespace main_siem_space
{
    enum class correlationModule { NONE_CORRELATION = 0, STATISTIC, AI, MACHINE};

    class SettingsSIEM
    {
    public:
        explicit SettingsSIEM(IJsonContainerPtr const &configObj);
        SettingsSIEM(SettingsSIEM const &other);
        SettingsSIEM(SettingsSIEM &&other);
        ~SettingsSIEM() = default;
        SettingsSIEM const& operator=(SettingsSIEM const &other);
        SettingsSIEM const& operator=(SettingsSIEM &&other);
        size_t GetAmountAggregationThreads() const { return m_AmountAggrThreads; }
        size_t GetAmountCorrelationThreads() const { return m_AmountCorrThreads; }
        std::string GetAggregationConfigPath() const { return m_AggregationCustomConfigPath; }
        std::string GetCorrelationConfigPath() const { return m_CorrelationCustomConfigPath; }
        std::vector<correlationModule> GetCorrelationModuleType() const { return m_KindCorrelation; }
        bool GetIsUseCustomAggr() const { return m_IsUseCustomAggregation; }
        bool GetIsUseCustomCorr() const { return m_IsUseCustomCorrelation; }
    private:
        void ReadAggregationThreadsCount();
        void ReadCorrelationThreadsCount();
        void ReadIsUseCustomAggregation();
        void ReadIsUseCustomCorrelation();
        void ReadCustomAggrConfigPath();
        void ReadCustomCorrConfigPath();
        void TuneFromConfig();
    private:
        const std::string AmountAggregationThreadsKey = "amount_aggr_threads";
        const std::string AmountCorrelationThreadsKey = "amount_corr_threads";
        const std::string UseCustomAggregationKey = "use_custom_aggregation";
        const std::string UseCustomCorrelationKey = "use_custom_correlation";
        const std::string CustomAggregationConfPathKey = "aggregation_custom_config";
        const std::string CustomCorrelationConfPathKey = "correlation_custom_config";
        const std::string CorrelationTypeKey = "correlation_module";
    private:
        IJsonContainerPtr m_pConfigJson;
        size_t m_AmountAggrThreads;
        size_t m_AmountCorrThreads;
        std::string m_AggregationCustomConfigPath;
        std::string m_CorrelationCustomConfigPath;
        std::vector<correlationModule> m_KindCorrelation;
        bool m_IsUseCustomAggregation;
        bool m_IsUseCustomCorrelation;

    public:
        class SettingsException : public SIEMException
        {
        private:
            int m_Errno;
            char m_ReadedSymbol;
        public:
            enum class SettingsErrorCode : int
            {
                NOT_SET_CUSTOM_AGGR_CONFIG_PATH = 1,
                NOT_SET_CUSTOM_CORR_CONFIG_PATH = 2
            };
        public:
            SettingsException(std::string const &exMsg, int errCode);
            SettingsException(std::string &&exMsg, int errCode);
            ~SettingsException() = default;
        };
    };
}

#endif // SIEM_STARTUP_HPP
