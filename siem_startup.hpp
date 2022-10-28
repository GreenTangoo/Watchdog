#ifndef SIEM_STARTUP_HPP
#define SIEM_STARTUP_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

#include "siem_defines.hpp"
#include "utility_module/json_proc.hpp"

using namespace utility_space;

namespace main_siem_space
{
    enum class correlationModule { NONE_CORRELATION = 0, STATISTIC, AI, MACHINE};

    enum class AggrGrabber : unsigned short
    {
        aggrIPtables = 0, aggrVsftpd = 1,
        aggrSshd = 2
    };

    enum class SerializeType : unsigned short
    {
        serializeJSON = 0
    };


    class SubAggregationsBaseSettings
    {
    public:
        explicit SubAggregationsBaseSettings() = default;
        explicit SubAggregationsBaseSettings(std::string const &srcFile, std::string const &dstFile);
        SubAggregationsBaseSettings(SubAggregationsBaseSettings const &other);
        SubAggregationsBaseSettings(SubAggregationsBaseSettings &&other);
        ~SubAggregationsBaseSettings() = default;
        SubAggregationsBaseSettings const& operator=(SubAggregationsBaseSettings const &other);
        SubAggregationsBaseSettings const& operator=(SubAggregationsBaseSettings &&other);
        inline std::string GetSrcFile() const { return m_SrcFile; }
        inline std::string GetDstFile() const { return m_DstFile; }
        inline size_t GetDetailsLevel() const { return m_DetailsLevel; }
        inline SerializeType GetSerializeKind() const { return m_SerializeKind; }
        virtual void ReadSettings(IJsonContainerPtr const &pConfig);
        virtual AggrGrabber GetAggrType() = 0;
    private:
        std::string m_SrcFile;
        std::string m_DstFile;
        size_t m_DetailsLevel { 0 };
        SerializeType m_SerializeKind { SerializeType::serializeJSON };
    };
    typedef std::shared_ptr<SubAggregationsBaseSettings> SubAggregationBaseSettingsPtr;


    class AggregationIpTablesSettings : public SubAggregationsBaseSettings
    {
    public:
       explicit AggregationIpTablesSettings() = default;
       AggregationIpTablesSettings(AggregationIpTablesSettings const &other);
       AggregationIpTablesSettings(AggregationIpTablesSettings &&other);
       ~AggregationIpTablesSettings() = default;
       AggregationIpTablesSettings const& operator=(AggregationIpTablesSettings const &other);
       AggregationIpTablesSettings const operator=(AggregationIpTablesSettings &&other);
       void ReadSettings(IJsonContainerPtr const &pConfig) override;
       AggrGrabber GetAggrType() override;
    };
    typedef std::shared_ptr<AggregationIpTablesSettings> AggregationIpTablesSettingsPtr;


    class AggregationVsftpdSettings : public SubAggregationsBaseSettings
    {
    public:
        explicit AggregationVsftpdSettings() = default;
        AggregationVsftpdSettings(AggregationVsftpdSettings const &other);
        AggregationVsftpdSettings(AggregationVsftpdSettings &&other);
        ~AggregationVsftpdSettings() = default;
        AggregationVsftpdSettings const& operator=(AggregationVsftpdSettings const &other);
        AggregationVsftpdSettings const operator=(AggregationVsftpdSettings &&other);
        void ReadSettings(IJsonContainerPtr const &pConfig) override;
        AggrGrabber GetAggrType() override;
    };
    typedef std::shared_ptr<AggregationVsftpdSettings> AggregationVsftpdSettingsPtr;


    class AggregationSshdSettings : public SubAggregationsBaseSettings
    {
    public:
        explicit AggregationSshdSettings() = default;
        AggregationSshdSettings(AggregationSshdSettings const &other);
        AggregationSshdSettings(AggregationSshdSettings &&other);
        ~AggregationSshdSettings() = default;
        AggregationSshdSettings const& operator=(AggregationSshdSettings const &other);
        AggregationSshdSettings const operator=(AggregationSshdSettings &&other);
        void ReadSettings(IJsonContainerPtr const &pConfig) override;
        AggrGrabber GetAggrType() override;
    };
    typedef std::shared_ptr<AggregationSshdSettings> AggregationSshdSettingsPtr;


    class SubAggregationSettingsCreator
    {
    public:
        explicit SubAggregationSettingsCreator(SubAggregationSettingsCreator const &other) = delete;
        explicit SubAggregationSettingsCreator(SubAggregationSettingsCreator &&other) = delete;
        SubAggregationSettingsCreator const& operator=(SubAggregationSettingsCreator const &other) = delete;
        SubAggregationSettingsCreator const& operator=(SubAggregationSettingsCreator &&other) = delete;
        static SubAggregationSettingsCreator& GetInstance();
        SubAggregationBaseSettingsPtr Create(AggrGrabber grabType);
        AggrGrabber StringToAggrType(std::string const &grabberTypeStr);
        std::unordered_map<std::string, AggrGrabber> const& GetTypesMap() const;
    private:
        explicit SubAggregationSettingsCreator() = default;
    private:
        std::unordered_map<std::string, AggrGrabber> m_GrabbersTypeMap =
        {
            { "iptables", AggrGrabber::aggrIPtables },
            { "vsftpd", AggrGrabber::aggrVsftpd },
            { "sshd", AggrGrabber::aggrSshd }
        };
    };

    class AggregationSettings
    {
    public:
        explicit AggregationSettings() = default;
        explicit AggregationSettings(IJsonContainerPtr const &configObj);
        AggregationSettings(AggregationSettings const &other);
        AggregationSettings(AggregationSettings &&other);
        ~AggregationSettings() = default;
        AggregationSettings const& operator=(AggregationSettings const &other);
        AggregationSettings const& operator=(AggregationSettings &&other);
        void SetConfig(IJsonContainerPtr const &configObj);
        void ReadSettings();
        SubAggregationBaseSettingsPtr GetSettingsByType(AggrGrabber kindGrabber) const;
        inline std::vector<AggrGrabber> GetGrabbersTypes() const { return m_GrabbersTypes; }
    private:
        IJsonContainerPtr m_pConfigJson;
        std::vector<SubAggregationBaseSettingsPtr> m_SubAggrs;
        std::vector<AggrGrabber> m_GrabbersTypes;
    };
    typedef std::shared_ptr<AggregationSettings> AggregationSettingsPtr;


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
        AggregationSettings GetAggregationSettings() const { return m_AggrSettings; }
    private:
        void ReadAggregationThreadsCount();
        void ReadCorrelationThreadsCount();
        void ReadIsUseCustomAggregation();
        void ReadIsUseCustomCorrelation();
        void ReadCustomAggrConfigPath();
        void ReadCustomCorrConfigPath();
        void ReadAggregationsSettings();
        void TuneFromConfig();
    private:
        IJsonContainerPtr m_pConfigJson;
        size_t m_AmountAggrThreads;
        size_t m_AmountCorrThreads;
        std::string m_AggregationCustomConfigPath;
        std::string m_CorrelationCustomConfigPath;
        std::vector<correlationModule> m_KindCorrelation;
        bool m_IsUseCustomAggregation;
        bool m_IsUseCustomCorrelation;
        AggregationSettings m_AggrSettings;
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
