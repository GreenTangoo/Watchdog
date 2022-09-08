#include "siem_startup.hpp"

using namespace main_siem_space;


/*---------------------------------------------------------------*/
/*----------------------SETTINGS SIEM----------------------------*/
/*---------------------------------------------------------------*/
SettingsSIEM::SettingsSIEM(const IJsonContainerPtr &configObj) :
    m_pConfigJson(configObj)
{
    TuneFromConfig();
}

SettingsSIEM::SettingsSIEM(SettingsSIEM const &other) : 
    m_pConfigJson(other.m_pConfigJson), m_AmountAggrThreads(other.m_AmountAggrThreads),
    m_AmountCorrThreads(other.m_AmountCorrThreads), m_AggregationCustomConfigPath(other.m_AggregationCustomConfigPath),
    m_CorrelationCustomConfigPath(other.m_CorrelationCustomConfigPath), m_KindCorrelation(other.m_KindCorrelation),
    m_IsUseCustomAggregation(other.m_IsUseCustomAggregation), m_IsUseCustomCorrelation(other.m_IsUseCustomCorrelation)

{

}

SettingsSIEM::SettingsSIEM(SettingsSIEM &&other) :
    m_pConfigJson(std::move(other.m_pConfigJson)), m_AmountAggrThreads(other.m_AmountAggrThreads),
    m_AmountCorrThreads(other.m_AmountCorrThreads), m_AggregationCustomConfigPath(std::move(other.m_AggregationCustomConfigPath)),
    m_CorrelationCustomConfigPath(std::move(other.m_CorrelationCustomConfigPath)), m_KindCorrelation(other.m_KindCorrelation),
    m_IsUseCustomAggregation(other.m_IsUseCustomAggregation), m_IsUseCustomCorrelation(other.m_IsUseCustomCorrelation)
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
}

SettingsSIEM::SettingsException::SettingsException(std::string const &exMsg, int errCode) :
    SIEMException(exMsg, errCode)
{

}

SettingsSIEM::SettingsException::SettingsException(std::string &&exMsg, int errCode) :
    SIEMException(std::move(exMsg), errCode)
{

}
