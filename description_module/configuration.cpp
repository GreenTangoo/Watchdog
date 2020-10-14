#include "configuration.hpp"

using namespace description_space;



AggregationInfoNode::AggregationInfoNode() :
	keyRegGroup(-1), valueRegGrop(-1), typeNode(NONE)
{}

Configuration::Configuration(JsonObject configJson) : 
	_configurationFileParser(configJson)
{

}

Configuration::Configuration(Configuration const &other) : 
	_configurationFileParser(other._configurationFileParser)
{

}

Configuration::Configuration(Configuration &&other) :
	_configurationFileParser(std::move(other._configurationFileParser))
{

}

Configuration const& Configuration::operator=(Configuration const &other)
{
	if(this != &other)
	{
		_configurationFileParser = other._configurationFileParser;
	}
	return *this;
}

Configuration const& Configuration::operator=(Configuration &&other)
{
	if(this != &other)
	{
		_configurationFileParser = std::move(other._configurationFileParser);
	}
	return *this;
}

JsonObject Configuration::getConfiguration(std::string const &nameNode) const 
{
	JsonObject configNodeObj;
	std::shared_ptr<JsonContainer> foundedContainer = 
		_configurationFileParser.findElementByName(nameNode);
	
	if(foundedContainer == nullptr)
	{
		throw JsonException("Cannot find configuration node by name: " + nameNode, 
			JsonException::BAD_NAME);
	}
	else
	{
		configNodeObj.setContainer(*foundedContainer.get());
	}

	return configNodeObj;
}