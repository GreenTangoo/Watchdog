#include "search_aggr_info.hpp"

using namespace description_space;

Configuration::Configuration(JsonObject obj) : configurationFileParser(obj)
{

}

Configuration::Configuration(Configuration const &other) : 
					configurationFileParser(other.configurationFileParser)
{

}

Configuration::Configuration(Configuration &&other) :
					configurationFileParser(std::move(other.configurationFileParser))
{

}

Configuration const& Configuration::operator=(Configuration const &other)
{
	if(this != &other)
	{
		std::lock(configMutex, other.configMutex);
		std::lock_guard<std::mutex> lhs(configMutex, std::adopt_lock);
		std::lock_guard<std::mutex> rhs(other.configMutex, std::adopt_lock);

		configurationFileParser = other.configurationFileParser;
	}
	return *this;
}

Configuration const& Configuration::operator=(Configuration &&other)
{
	if(this != &other)
	{
		std::lock(configMutex, other.configMutex);
		std::lock_guard<std::mutex> lhs(configMutex, std::adopt_lock);
		std::lock_guard<std::mutex> rhs(other.configMutex, std::adopt_lock);

		configurationFileParser = std::move(other.configurationFileParser);
	}
	return *this;
}

JsonObject Configuration::getConfiguration(std::string const &nameNode)
{
	JsonObject configNodeObj;
	std::shared_ptr<JsonContainer> foundedContainer = configurationFileParser.findElementByName(nameNode);
	
	if(foundedContainer == nullptr)
	{
		throw JsonException("Cannot find configuration node by name: " + nameNode, JsonException::BAD_NAME);
	}
	else
	{
		configNodeObj.setContainer(*foundedContainer.get());
	}

	return configNodeObj;
}