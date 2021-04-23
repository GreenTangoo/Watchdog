#include <iostream>
#include <thread>
#include <string>
#include <cstdlib>
#include <cstring>

#include "utility_module/json.hpp"
#include "description_module/configuration.hpp"
#include "description_module/description_table.hpp"
#include "aggregation_module/symptom_grabber.hpp"

using namespace utility_space;
using namespace description_space;
using namespace aggregation_space;

std::vector<std::string> vec;
std::vector<int> intVec;

int main()
{
	std::string filepath = "aggregation_config.json";
	JsonObject aggrConfigJs = getJsonData(filepath);
	Configuration aggrConfig(aggrConfigJs);
	DescriptionTable &confTable = DescriptionTable::getInstance();

	confTable.tuneFromConfig(aggrConfig, DescriptionTable::AGGREGATION_CONFIG);

	std::shared_ptr<AggregationInfo const> grabInfo = confTable.getAggrStructure(grabberCategory::IPTABLES);
	SymptomGrabber grabber(grabInfo, grabberCategory::IPTABLES);
	grabber.tryAggregationInfo();

	return 0;
}