#include "siem_startup.hpp"

using namespace main_siem_space;

#define STARTUP "startup"
#define AMOUNT_AGGR_THREADS "amount_aggr_threads"
#define AMOUNT_CORR_THREADS "amount_corr_threads"
#define CORRELATION_MODULE "correlation_module"
#define AGGREGATION_PATH "aggregation_custom_config"
#define CORRELATION_PATH "correlation_custom_config"

#define STATISTIC_STR "statistic"
#define AI_STR "ai"
#define MACHINE_STR "machine"

#define USING_CUSTOM_AGGREGATION "use_custom_aggregation"
#define USING_CUSTOM_CORRELATION "use_custom_correlation"


static std::map<correlationModule, std::string> correlationKindStrMap = 
    {
        {STATISTIC, STATISTIC_STR},
        {AI, AI_STR},
        {MACHINE, MACHINE_STR}
    };


/*---------------------------------------------------------------*/
/*----------------------SETTINGS SIEM----------------------------*/
/*---------------------------------------------------------------*/
SettingsSIEM::SettingsSIEM(JsonObject const &configObj) : 
    _startupSettings(configObj)
{
    tuneFromConfig();
}

SettingsSIEM::SettingsSIEM(Configuration const &config) :
    _startupSettings(config)
{
    tuneFromConfig();
}

SettingsSIEM::SettingsSIEM(SettingsSIEM const &other) : 
    _startupSettings(other._startupSettings), _amountAggrThreads(other._amountAggrThreads), 
    _amountCorrThreads(other._amountCorrThreads), _aggregationCustomConfigPath(other._aggregationCustomConfigPath),
    _correlationCustomConfigPath(other._correlationCustomConfigPath), _kindCorrelation(other._kindCorrelation),
    m_isUseCustomAggregation(other.m_isUseCustomAggregation), m_isUseCustomCorrelation(other.m_isUseCustomCorrelation)

{

}

SettingsSIEM::SettingsSIEM(SettingsSIEM &&other) :
    _startupSettings(std::move(_startupSettings)), _amountAggrThreads(other._amountAggrThreads),
    _amountCorrThreads(other._amountCorrThreads), _aggregationCustomConfigPath(std::move(other._aggregationCustomConfigPath)),
    _correlationCustomConfigPath(std::move(other._correlationCustomConfigPath)), _kindCorrelation(other._kindCorrelation),
    m_isUseCustomAggregation(other.m_isUseCustomAggregation), m_isUseCustomCorrelation(other.m_isUseCustomCorrelation)
{
    other._amountAggrThreads = 0;
    other._amountCorrThreads = 0;
    other._kindCorrelation.clear();
}

SettingsSIEM const& SettingsSIEM::operator=(SettingsSIEM const &other)
{
    if(this != &other)
    {
        _startupSettings = other._startupSettings;
        _amountAggrThreads = other._amountAggrThreads;
        _amountCorrThreads = other._amountCorrThreads;
        _kindCorrelation = other._kindCorrelation;
    }

    return *this;
}

SettingsSIEM const& SettingsSIEM::operator=(SettingsSIEM &&other)
{
    if(this != &other)
        std::swap(*this, other);

    return *this;
}

void SettingsSIEM::tuneFromConfig()
{
    JsonObject configObject = _startupSettings.getConfiguration(STARTUP);

    std::shared_ptr<JsonContainer> amountAggrThreadsPtr = guaranteeGetPtrByName(configObject, AMOUNT_AGGR_THREADS);
    std::string amountAggrThreadsStr = amountAggrThreadsPtr->keyValue.second;
    _amountAggrThreads = static_cast<size_t>(std::atoi(amountAggrThreadsStr.c_str()));

    std::shared_ptr<JsonContainer> amountCorrThreadsPtr = guaranteeGetPtrByName(configObject, AMOUNT_CORR_THREADS);
    std::string amountCorrThreadsStr = amountCorrThreadsPtr->keyValue.second;
    _amountCorrThreads = static_cast<size_t>(std::atoi(amountCorrThreadsStr.c_str()));

    std::shared_ptr<JsonContainer> isUseCustomAggrPtr = guaranteeGetPtrByName(configObject, USING_CUSTOM_AGGREGATION);
    std::string isUseCustomAggrStr = isUseCustomAggrPtr->keyValue.second;

    if(isUseCustomAggrStr != "true")
    {
        m_isUseCustomAggregation = false;
    }
    else
    {
        m_isUseCustomAggregation = true;

        std::shared_ptr<JsonContainer> aggregationConfigPathPtr = guaranteeGetPtrByName(configObject, AGGREGATION_PATH);
        _aggregationCustomConfigPath = aggregationConfigPathPtr->keyValue.second;
    }


    std::shared_ptr<JsonContainer> isUseCustomCorrPtr = guaranteeGetPtrByName(configObject, USING_CUSTOM_CORRELATION);
    std::string isUseCustomCorrStr = isUseCustomCorrPtr->keyValue.second;

    if(isUseCustomCorrStr != "true")
    {
        m_isUseCustomCorrelation = false;
    }
    else
    {
        m_isUseCustomCorrelation = true;

        std::shared_ptr<JsonContainer> correlationConfigPathPtr = guaranteeGetPtrByName(configObject, CORRELATION_PATH);
        _correlationCustomConfigPath = correlationConfigPathPtr->keyValue.second;
    }


    std::shared_ptr<JsonContainer> correlationKindPtr = guaranteeGetPtrByName(configObject, CORRELATION_MODULE);

    std::vector<std::string> correlationModuleStrs = 
        StringManager::parseByDelimiter(correlationKindPtr->keyValue.second, "|");

    std::transform(correlationModuleStrs.begin(), correlationModuleStrs.end(), std::back_inserter(_kindCorrelation),
        [](std::string const &correlationType) -> correlationModule
    {
        return main_siem_space::stringToCorrelationModule(correlationType);
    });
}

/*---------------------------------------------------------------*/
/*---------------------PUBLIC FUNCTIONS--------------------------*/
/*---------------------------------------------------------------*/
correlationModule main_siem_space::stringToCorrelationModule(std::string correlationModuleStr)
{
    for(std::map<correlationModule, std::string>::const_iterator it = correlationKindStrMap.begin();
        it != correlationKindStrMap.end(); it++)
    {
        if(it->second == correlationModuleStr)
            return it->first;
    }

    return correlationModule::NONE_CORRELATION;
}

std::string main_siem_space::correlationModuleToString(correlationModule correlationKind)
{
    auto it = correlationKindStrMap.find(correlationKind);
    if(it != correlationKindStrMap.end())
    {
        return it->second;
    }

    return std::string("");
}
