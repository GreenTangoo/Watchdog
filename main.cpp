#include <iostream>
#include <string>

#include "utility_module/date_time.hpp"
#include "utility_module/json.hpp"
#include "description_module/description_table.hpp"
#include "description_module/configuration.hpp"

using namespace utility_space;
using namespace description_space;

#define SEARCH_CONFIG_DATA "{\"search-configs\":{\"one-config\":{"\
"\"json-filename\":\"iptables_log.json\",\"category\":\"port_scanning\","\
"\"key-node\":\"[ip_addr]\",\"inner\":{\"key-node\":\"amount_requests\","\
"\"value-node\":\"[>]1000\",\"and\":{\"key-node\":\"protocol\","\
"\"value-node\":\"[=]tcp\"}}}}"

#define AGGR_CONFIG_DATA "{\"aggregation-configs\":{\"one-config\":{"\
"\"category\":\"iptables\",\"source-log\":\"iptables.log\","\
"\"result-json\":\"iptables_log.json\",\"info-node\":{"\
"\"node-type\":\"object\",\"key-name\":\"[regexp]\","\
"\"parent-node\":\"root\"},\"info-node\":{"\
"\"node-type\":\"string\",\"key-name\":\"amount_requests\","\
"\"value-name\":\"[regexp]\",\"parent-node\":\"[ip_addr]\"},"\
"\"info-node\":{\"node-type\":\"string\",\"key-name\":\"protocol\","\
"\"value-name\":\"[regexp]\",\"parent-node\":\"[ip_addr]\"}}}}"

JsonObject getSearchConfig()
{
   std::stringstream stream(SEARCH_CONFIG_DATA);
   return getJsonData(stream);
}

JsonObject getAggrConfig()
{
    std::stringstream stream(AGGR_CONFIG_DATA);
    return getJsonData(stream);
}

int main()
{
    DescriptionTable &table = DescriptionTable::getInstance();
        JsonObject configJson = getAggrConfig();
        Configuration config(configJson);

        table.tuneFromConfig(config, DescriptionTable::AGGREGATION_CONFIG);
        AggregationInfo const &iptablesInfo = table.getAggrStructure(IPTABLES);
	return 0;
}