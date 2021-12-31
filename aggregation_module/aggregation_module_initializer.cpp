#include "aggregation_module_initializer.hpp"

using namespace aggregation_space;

AggregationInitializer::AggregationInitializer(SettingsSIEM const &settings) :
    _amountThreads(settings.getAmountAggregationThreads())
{
    initDefaultGrabbers(settings);
}


AggregationInitializer::AggregationInitializer(AggregationInitializer &&other) : 
    _amountThreads(other._amountThreads)//,
    //_grabbers()
{
    other._amountThreads = 0;
    // std::move(other._grabbers.begin(), other._grabbers.end(), std::back_inserter(_grabbers));
}

AggregationInitializer::~AggregationInitializer()
{

}

AggregationInitializer const& AggregationInitializer::operator=(AggregationInitializer &&other)
{
    if(this != &other)
    {
        _amountThreads = other._amountThreads;
        other._amountThreads = 0;

        // _grabbers.clear();
        // std::move(other._grabbers.begin(), other._grabbers.end(), std::back_inserter(_grabbers));
    }

    return *this;
}

void AggregationInitializer::startCycle()
{
    ThreadPool<void> tasks(_amountThreads);

    // for(SymptomGrabber &grabber : _grabbers)
    // {
    //     std::function<void(void)> runAggregationFunc = std::bind(&SymptomGrabber::tryAggregationInfo, &grabber);
    //     tasks.addFunction(runAggregationFunc);
    // }

    tasks.flushAll();
}

void AggregationInitializer::initDefaultGrabbers(SettingsSIEM const &settings)
{
    // TODO: Init default grabbers.

    bool isUseCustomAggregation = settings.getIsUseCustomAggr();

    if(isUseCustomAggregation)
    {
        initCustomGrabbers(settings);
    }
}

void AggregationInitializer::initCustomGrabbers(SettingsSIEM const &settings)
{
    JsonObject aggrConfigJs = getJsonData(settings.getAggregationConfigPath());
	Configuration aggrConfig(aggrConfigJs);
	DescriptionTable &confTable = DescriptionTable::getInstance();

	confTable.tuneFromConfig(aggrConfig, DescriptionTable::AGGREGATION_CONFIG);

    //_grabbers.reserve(confTable.getAmountConfigs(DescriptionTable::AGGREGATION_CONFIG));

    std::map<grabberCategory, std::shared_ptr<AggregationInfo>> const aggregationsMap = 
        confTable.getAllAggrDescriptors();

    for(std::map<grabberCategory, std::shared_ptr<AggregationInfo>>::const_iterator it = aggregationsMap.begin();
        it != aggregationsMap.end(); it++)
    {
        //SymptomGrabber grabber(it->second, it->first);
        //_grabbers.push_back(grabber);
    }
}
