#include <iostream>
#include <string>

#include "utility_module/date_time.hpp"
#include "utility_module/json.hpp"
#include "utility_module/syntax_analyzer.hpp"
#include "description_module/description_table.hpp"
#include "description_module/configuration.hpp"

using namespace utility_space;
using namespace description_space;

#define SEARCH_CONFIG_DATA "{\"search-configs\":{\"one-config\":{"\
"\"json-filename\":\"iptables_log.json\",\"category\":\"port_scanning\","\
"\"key-node\":\"[ip_addr]\",\"inner\":{\"key-node\":\"amount_requests\","\
"\"value-node\":\"[>]1000\",\"and\":{\"key-node\":\"protocol\","\
"\"value-node\":\"[=]tcp\"}}}}"

JsonObject getSearchConfig()
{
   std::stringstream stream(SEARCH_CONFIG_DATA);
   return getJsonData(stream);
}

int main()
{
    DescriptionTable &table = DescriptionTable::getInstance();
    JsonObject configJson = getSearchConfig();
    Configuration config(configJson);

    table.tuneFromConfig(config, DescriptionTable::CORRELATION_CONFIG);
	return 0;
}