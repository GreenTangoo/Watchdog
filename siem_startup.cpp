#include "siem_startup.hpp"

using namespace main_siem_space;

#define STARTUP "startup"
#define AMOUNT_AGGR_THREADS "amount_aggr_threads"
#define AMOUNT_CORR_THREADS "amount_corr_threads"
#define CORRELATION_MODULE "correlation_module"

#define STATISTIC_STR "statistic"
#define AI_STR "ai"
#define MACHINE_STR "machine"


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

}

SettingsSIEM::SettingsSIEM(Configuration const &config) :
    _startupSettings(config)
{

}

SettingsSIEM::SettingsSIEM(SettingsSIEM const &other) : 
    _startupSettings(other._startupSettings), _amountAggrThreads(other._amountAggrThreads), 
    _amountCorrThreads(other._amountCorrThreads), _kindCorrelation(other._kindCorrelation)
{

}

SettingsSIEM::SettingsSIEM(SettingsSIEM &&other) :
    _startupSettings(std::move(_startupSettings)), _amountAggrThreads(other._amountAggrThreads),
    _amountCorrThreads(other._amountCorrThreads), _kindCorrelation(other._kindCorrelation)
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