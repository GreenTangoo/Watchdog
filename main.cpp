#include <iostream>
#include <string>

#include "utility_module/date_time.hpp"
#include "utility_module/json.hpp"
#include "utility_module/syntax_analyzer.hpp"

using namespace utility_space;

JsonObject constructJsonContainer()
{
    JsonObject object;
    object.addOrUpdateNode("search-configs", "root/search-configs");
    object.addOrUpdateNode("one-config", "root/search-configs/one-config");
    object.addOrUpdateString(std::pair<std::string, std::string>("key-node", "[ip_addr]"),
        "root/search-configs/one-config/key-node");

    object.addOrUpdateNode("inner","root/search-configs/one-config/inner");
    object.addOrUpdateString(std::pair<std::string, std::string>("key-node", "amount_requests"),
        "root/search-configs/one-config/inner/key-node");
        
    object.addOrUpdateNode("and", "root/search-configs/one-config/inner/and");

    return object;
}

int main()
{
	JsonObject obj = constructJsonContainer();
	SyntaxAnalyzer analyzer;

    std::shared_ptr<JsonContainer> nodePtr = obj.findElementByName("one-config");
	std::shared_ptr<JsonContainer> ptr = analyzer.tryFoundNextRelationship(nodePtr, 2);
    ptr = analyzer.tryFoundNextRelationship(ptr, 2);
	return 0;
}