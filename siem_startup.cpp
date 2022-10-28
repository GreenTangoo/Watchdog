#include "siem_startup.hpp"

using namespace main_siem_space;

// ////////////////////////////////////////////////////////////////
// SubAggregationBase
// Base class for apply settings for all agregations
// Contains general settings for aggregations.
// ///////////////////////////////////////////////////////////////
static const std::string SourceFilePathKey = "src_file";
static const std::string DestinationFilePathKey = "dst_file";
static const std::string DetailsLevelKey = "details_level";
static const std::string SerializeKindKey = "serialize";

SubAggregationsBaseSettings::SubAggregationsBaseSettings(std::string const &srcFile, std::string const &dstFile) :
    m_SrcFile(srcFile), m_DstFile(dstFile)
{

}

SubAggregationsBaseSettings::SubAggregationsBaseSettings(SubAggregationsBaseSettings const &other) :
    m_SrcFile(other.m_SrcFile), m_DstFile(other.m_DstFile)
{

}

SubAggregationsBaseSettings::SubAggregationsBaseSettings(SubAggregationsBaseSettings &&other) :
    m_SrcFile(std::move(other.m_SrcFile)), m_DstFile(std::move(other.m_DstFile))
{

}

SubAggregationsBaseSettings const& SubAggregationsBaseSettings::operator=(SubAggregationsBaseSettings const &other)
{
    if(this != &other)
    {
        m_SrcFile = other.m_SrcFile;
        m_DstFile = other.m_DstFile;
    }

    return *this;
}

SubAggregationsBaseSettings const& SubAggregationsBaseSettings::operator=(SubAggregationsBaseSettings &&other)
{
    if(this != &other)
    {
        m_SrcFile = std::move(other.m_SrcFile);
        m_DstFile = std::move(other.m_DstFile);
    }

    return *this;
}

void SubAggregationsBaseSettings::ReadSettings(IJsonContainerPtr const &pConfig)
{
    JsonObjectPtr pObjJson = std::dynamic_pointer_cast<JsonObject>(pConfig);
    JsonObject &objJson = *pObjJson;


    if(const auto pSrcFileContainer = std::dynamic_pointer_cast<JsonString>(objJson[SourceFilePathKey]))
    {
        m_SrcFile = static_cast<std::string>(*pSrcFileContainer);
    }

    if(const auto pDstFileContainer = std::dynamic_pointer_cast<JsonString>(objJson[DestinationFilePathKey]))
    {
        m_DstFile = static_cast<std::string>(*pDstFileContainer);
    }

    if(const auto pDetailsLvlContainer = std::dynamic_pointer_cast<JsonString>(objJson[DetailsLevelKey]))
    {
        const std::string detailsLevelStr = static_cast<std::string>(*pDetailsLvlContainer);
        m_DetailsLevel = std::atoi(detailsLevelStr.c_str());
    }

    if(const auto pSerializeKindContainer = std::dynamic_pointer_cast<JsonString>(objJson[SerializeKindKey]))
    {
        const std::string serializeKindStr = static_cast<std::string>(*pSerializeKindContainer);
        m_SerializeKind = static_cast<SerializeType>(std::atoi(serializeKindStr.c_str()));
    }
}

// /////////////////////////////////////////////////////////////
// AggregationIpTablesSettings
// Class for setup IpTables aggregation settings
// ////////////////////////////////////////////////////////////
AggregationIpTablesSettings::AggregationIpTablesSettings(AggregationIpTablesSettings const &other) :
    SubAggregationsBaseSettings(other)
{

}

AggregationIpTablesSettings::AggregationIpTablesSettings(AggregationIpTablesSettings &&other) :
    SubAggregationsBaseSettings(std::move(other))
{

}

AggregationIpTablesSettings const& AggregationIpTablesSettings::operator=(AggregationIpTablesSettings const &other)
{
    if(this != &other)
    {
        SubAggregationsBaseSettings::operator=(other);
    }

    return *this;
}

AggregationIpTablesSettings const AggregationIpTablesSettings::operator=(AggregationIpTablesSettings &&other)
{
    if(this != &other)
    {
        SubAggregationsBaseSettings::operator=(std::move(other));
    }

    return *this;
}

void AggregationIpTablesSettings::ReadSettings(IJsonContainerPtr const &pConfig)
{
    SubAggregationsBaseSettings::ReadSettings(pConfig);
}

AggrGrabber AggregationIpTablesSettings::GetAggrType()
{
    return AggrGrabber::aggrIPtables;
}


// /////////////////////////////////////////////////////////////
// AggregationFtpServerSettings
// Class for setup FTP server aggregation settings
// ////////////////////////////////////////////////////////////
AggregationVsftpdSettings::AggregationVsftpdSettings(AggregationVsftpdSettings const &other) :
    SubAggregationsBaseSettings(other)
{

}

AggregationVsftpdSettings::AggregationVsftpdSettings(AggregationVsftpdSettings &&other) :
    SubAggregationsBaseSettings(std::move(other))
{

}

AggregationVsftpdSettings const& AggregationVsftpdSettings::operator=(AggregationVsftpdSettings const &other)
{
    if(this != &other)
    {
        SubAggregationsBaseSettings::operator=(other);
    }

    return *this;
}

AggregationVsftpdSettings const AggregationVsftpdSettings::operator=(AggregationVsftpdSettings &&other)
{
    if(this != &other)
    {
        SubAggregationsBaseSettings::operator=(std::move(other));
    }

    return *this;
}

void AggregationVsftpdSettings::ReadSettings(IJsonContainerPtr const &pConfig)
{
    SubAggregationsBaseSettings::ReadSettings(pConfig);
}

AggrGrabber AggregationVsftpdSettings::GetAggrType()
{
    return AggrGrabber::aggrVsftpd;
}


// ///////////////////////////////////////////////////////////////
// AggregationSshServerSettings
// Class for setup SSH server aggregation settings
// ///////////////////////////////////////////////////////////////
AggregationSshdSettings::AggregationSshdSettings(AggregationSshdSettings const &other) :
    SubAggregationsBaseSettings(other)
{

}

AggregationSshdSettings::AggregationSshdSettings(AggregationSshdSettings &&other) :
    SubAggregationsBaseSettings(std::move(other))
{

}

AggregationSshdSettings const& AggregationSshdSettings::operator=(AggregationSshdSettings const &other)
{
    if(this != &other)
    {
        SubAggregationsBaseSettings::operator=(other);
    }

    return *this;
}

AggregationSshdSettings const AggregationSshdSettings::operator=(AggregationSshdSettings &&other)
{
    if(this != &other)
    {
        SubAggregationsBaseSettings::operator=(std::move(other));
    }

    return *this;
}

void AggregationSshdSettings::ReadSettings(IJsonContainerPtr const &pConfig)
{
    SubAggregationsBaseSettings::ReadSettings(pConfig);
}

AggrGrabber AggregationSshdSettings::GetAggrType()
{
    return AggrGrabber::aggrSshd;
}


// ///////////////////////////////////////////////////////////////
// SubAggregationSettingsCreator
// Class-factory for creating specific aggregation settings.
// ///////////////////////////////////////////////////////////////
SubAggregationSettingsCreator& SubAggregationSettingsCreator::GetInstance()
{
    static SubAggregationSettingsCreator creator;
    return creator;
}

SubAggregationBaseSettingsPtr SubAggregationSettingsCreator::Create(AggrGrabber grabType)
{
    switch(grabType)
    {
    case AggrGrabber::aggrIPtables:
        return std::make_shared<AggregationIpTablesSettings>();
    case AggrGrabber::aggrVsftpd:
        return std::make_shared<AggregationVsftpdSettings>();
    case AggrGrabber::aggrSshd:
        return std::make_shared<AggregationSshdSettings>();
    default:
        throw AggregationException("Cannot create aggregator settings by aggregation type",
                                   AggregationException::INVALID_GRABBER_SETTINGS_TYPE);
    }
}

AggrGrabber SubAggregationSettingsCreator::StringToAggrType(std::string const &grabberTypeStr)
{
    return m_GrabbersTypeMap[grabberTypeStr];
}

std::unordered_map<std::string, AggrGrabber> const& SubAggregationSettingsCreator::GetTypesMap() const
{
    return m_GrabbersTypeMap;
}


// ///////////////////////////////////////////////////////////////
// AggregationSettings
// Base class implementation for apply aggregation settings.
// ///////////////////////////////////////////////////////////////
AggregationSettings::AggregationSettings(IJsonContainerPtr const &configObj) :
    m_pConfigJson(configObj)
{

}

AggregationSettings::AggregationSettings(AggregationSettings const &other) :
    m_pConfigJson(other.m_pConfigJson), m_SubAggrs(other.m_SubAggrs),
    m_GrabbersTypes(other.m_GrabbersTypes)
{

}

AggregationSettings::AggregationSettings(AggregationSettings &&other) :
    m_pConfigJson(std::move(other.m_pConfigJson)),
    m_SubAggrs(std::move(other.m_SubAggrs)),
    m_GrabbersTypes(std::move(other.m_GrabbersTypes))
{

}

AggregationSettings const& AggregationSettings::operator=(AggregationSettings const &other)
{
    if(this != &other)
    {
        m_pConfigJson = other.m_pConfigJson;
        m_SubAggrs = other.m_SubAggrs;
        m_GrabbersTypes = other.m_GrabbersTypes;
    }

    return *this;
}

AggregationSettings const& AggregationSettings::operator=(AggregationSettings &&other)
{
    if(this != &other)
    {
        m_pConfigJson = std::move(other.m_pConfigJson);
        m_SubAggrs = std::move(other.m_SubAggrs);
        m_GrabbersTypes = std::move(other.m_GrabbersTypes);
    }

    return *this;
}

void AggregationSettings::SetConfig(IJsonContainerPtr const &configObj)
{
    m_pConfigJson = configObj;
}

void AggregationSettings::ReadSettings()
{
    JsonObjectPtr pObjJson = std::dynamic_pointer_cast<JsonObject>(m_pConfigJson);
    JsonObject &objJson = *pObjJson;

    SubAggregationSettingsCreator &creator = SubAggregationSettingsCreator::GetInstance();
    auto grabbersTypesMap = creator.GetTypesMap();

    for(const auto& types : grabbersTypesMap)
    {
        IJsonContainerPtr pConfig = objJson[types.first];

        if(pConfig)
        {
            AggrGrabber kindGrabber = types.second;

            SubAggregationBaseSettingsPtr pSettings = creator.Create(kindGrabber);
            pSettings->ReadSettings(pConfig);

            m_SubAggrs.push_back(pSettings);
            m_GrabbersTypes.push_back(kindGrabber);
        }
    }
}

SubAggregationBaseSettingsPtr AggregationSettings::GetSettingsByType(AggrGrabber kindGrabber) const
{
    // There will not be several thousand aggregations,
    // so let's use a linear search.
    for(SubAggregationBaseSettingsPtr const &pSubAgrSett : m_SubAggrs)
    {
        if(pSubAgrSett->GetAggrType() == kindGrabber)
            return pSubAgrSett;
    }

    return nullptr;
}


// /////////////////////////////////////////////////////////////////////////
// SettingsSIEM
// Main class for apply SIEM settings. Settings get from options.json
// ////////////////////////////////////////////////////////////////////////
static const std::string AmountAggregationThreadsKey = "amount_aggr_threads";
static const std::string AmountCorrelationThreadsKey = "amount_corr_threads";
static const std::string UseCustomAggregationKey = "use_custom_aggregation";
static const std::string UseCustomCorrelationKey = "use_custom_correlation";
static const std::string CustomAggregationConfPathKey = "aggregation_custom_config";
static const std::string CustomCorrelationConfPathKey = "correlation_custom_config";
static const std::string CorrelationTypeKey = "correlation_module";
static const std::string AggregationSettingsKey = "aggregations_settings";

SettingsSIEM::SettingsSIEM(const IJsonContainerPtr &configObj) :
    m_pConfigJson(configObj)
{
    TuneFromConfig();
}

SettingsSIEM::SettingsSIEM(SettingsSIEM const &other) : 
    m_pConfigJson(other.m_pConfigJson), m_AmountAggrThreads(other.m_AmountAggrThreads),
    m_AmountCorrThreads(other.m_AmountCorrThreads), m_AggregationCustomConfigPath(other.m_AggregationCustomConfigPath),
    m_CorrelationCustomConfigPath(other.m_CorrelationCustomConfigPath), m_KindCorrelation(other.m_KindCorrelation),
    m_IsUseCustomAggregation(other.m_IsUseCustomAggregation), m_IsUseCustomCorrelation(other.m_IsUseCustomCorrelation),
    m_AggrSettings(other.m_AggrSettings)

{

}

SettingsSIEM::SettingsSIEM(SettingsSIEM &&other) :
    m_pConfigJson(std::move(other.m_pConfigJson)), m_AmountAggrThreads(other.m_AmountAggrThreads),
    m_AmountCorrThreads(other.m_AmountCorrThreads), m_AggregationCustomConfigPath(std::move(other.m_AggregationCustomConfigPath)),
    m_CorrelationCustomConfigPath(std::move(other.m_CorrelationCustomConfigPath)), m_KindCorrelation(other.m_KindCorrelation),
    m_IsUseCustomAggregation(other.m_IsUseCustomAggregation), m_IsUseCustomCorrelation(other.m_IsUseCustomCorrelation),
    m_AggrSettings(std::move(other.m_AggrSettings))
{
}

SettingsSIEM const& SettingsSIEM::operator=(SettingsSIEM const &other)
{
    if(this != &other)
    {
        m_pConfigJson = other.m_pConfigJson;
        m_AmountAggrThreads = other.m_AmountAggrThreads;
        m_AmountCorrThreads = other.m_AmountCorrThreads;
        m_AggregationCustomConfigPath = other.m_AggregationCustomConfigPath;
        m_CorrelationCustomConfigPath = other.m_CorrelationCustomConfigPath;
        m_KindCorrelation = other.m_KindCorrelation;
        m_IsUseCustomAggregation = other.m_IsUseCustomAggregation;
        m_IsUseCustomCorrelation = other.m_IsUseCustomCorrelation;
        m_AggrSettings = other.m_AggrSettings;
    };

    return *this;
}

SettingsSIEM const& SettingsSIEM::operator=(SettingsSIEM &&other)
{
    if(this != &other)
    {
        m_pConfigJson = std::move(other.m_pConfigJson);
        m_AmountAggrThreads = std::move(other.m_AmountAggrThreads);
        m_AmountCorrThreads = std::move(other.m_AmountCorrThreads);
        m_AggregationCustomConfigPath = std::move(other.m_AggregationCustomConfigPath);
        m_CorrelationCustomConfigPath = std::move(other.m_CorrelationCustomConfigPath);
        m_KindCorrelation = std::move(other.m_KindCorrelation);
        m_IsUseCustomAggregation = std::move(other.m_IsUseCustomAggregation);
        m_IsUseCustomCorrelation = std::move(other.m_IsUseCustomCorrelation);
        m_AggrSettings = std::move(other.m_AggrSettings);
    }

    return *this;
}

void SettingsSIEM::ReadAggregationThreadsCount()
{
    JsonStringPtr amountAggrThreadsContainer =
            std::dynamic_pointer_cast<JsonString>(m_pConfigJson->FindByName(AmountAggregationThreadsKey));

    if(amountAggrThreadsContainer)
    {
        m_AmountAggrThreads = amountAggrThreadsContainer->AsInteger();
    }
    else
    {
        m_AmountAggrThreads = 1;
    }
}

void SettingsSIEM::ReadCorrelationThreadsCount()
{
    JsonStringPtr amountCorrThreadsContainer =
            std::dynamic_pointer_cast<JsonString>(m_pConfigJson->FindByName(AmountCorrelationThreadsKey));

    if(amountCorrThreadsContainer)
    {
        m_AmountCorrThreads = amountCorrThreadsContainer->AsInteger();
    }
    else
    {
        m_AmountCorrThreads = 1;
    }
}

void SettingsSIEM::ReadIsUseCustomAggregation()
{
    JsonStringPtr isUseCustomAggrContainer =
            std::dynamic_pointer_cast<JsonString>(m_pConfigJson->FindByName(UseCustomAggregationKey));

    if(isUseCustomAggrContainer)
    {
        m_IsUseCustomAggregation = isUseCustomAggrContainer->AsBoolean();
    }
    else
    {
        m_IsUseCustomAggregation = false;
    }
}

void SettingsSIEM::ReadIsUseCustomCorrelation()
{
    JsonStringPtr isUseCustomCorrContainer =
            std::dynamic_pointer_cast<JsonString>(m_pConfigJson->FindByName(UseCustomCorrelationKey));

    if(isUseCustomCorrContainer)
    {
        m_IsUseCustomCorrelation = isUseCustomCorrContainer->AsBoolean();
    }
    else
    {
        m_IsUseCustomCorrelation = false;
    }
}

void SettingsSIEM::ReadCustomAggrConfigPath()
{
    JsonStringPtr aggrConfigPathContainer =
            std::dynamic_pointer_cast<JsonString>(m_pConfigJson->FindByName(CustomAggregationConfPathKey));

    if(aggrConfigPathContainer)
    {
        m_AggregationCustomConfigPath = static_cast<std::string>(*aggrConfigPathContainer);
    }
    else
    {
        throw SettingsException("Not set custom aggregation config file path",
                                static_cast<int>(SettingsException::SettingsErrorCode::NOT_SET_CUSTOM_AGGR_CONFIG_PATH));
    }
}

void SettingsSIEM::ReadCustomCorrConfigPath()
{
    JsonStringPtr corrConfigPathContainer =
            std::dynamic_pointer_cast<JsonString>(m_pConfigJson->FindByName(CustomCorrelationConfPathKey));

    if(corrConfigPathContainer)
    {
        m_CorrelationCustomConfigPath = static_cast<std::string>(*corrConfigPathContainer);
    }
    else
    {
        throw SettingsException("Not set custom correlation config file path",
                                static_cast<int>(SettingsException::SettingsErrorCode::NOT_SET_CUSTOM_CORR_CONFIG_PATH));
    }
}

void SettingsSIEM::ReadAggregationsSettings()
{
    JsonObjectPtr pJsonObj = std::dynamic_pointer_cast<JsonObject>(m_pConfigJson);
    JsonObject &objJson = *pJsonObj;

    IJsonContainerPtr pAggrSett = objJson[AggregationSettingsKey];

    m_AggrSettings.SetConfig(pAggrSett);
    m_AggrSettings.ReadSettings();
}

void SettingsSIEM::TuneFromConfig()
{
    ReadAggregationThreadsCount();
    ReadCorrelationThreadsCount();

    ReadIsUseCustomAggregation();
    ReadIsUseCustomCorrelation();

    if(m_IsUseCustomAggregation)
    {
        ReadCustomAggrConfigPath();
    }

    if(m_IsUseCustomCorrelation)
    {
        ReadCustomCorrConfigPath();
    }

    ReadAggregationsSettings();
}

SettingsSIEM::SettingsException::SettingsException(std::string const &exMsg, int errCode) :
    SIEMException(exMsg, errCode)
{

}

SettingsSIEM::SettingsException::SettingsException(std::string &&exMsg, int errCode) :
    SIEMException(std::move(exMsg), errCode)
{

}
