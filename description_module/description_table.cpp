#include "description_table.hpp"

using namespace description_space;

typedef std::map<symptomCategory, std::unique_ptr<SearchInfo>>::iterator searchIterator;

DescriptionTable::~DescriptionTable()
{

}

DescriptionTable& DescriptionTable::getInstance()
{
	static DescriptionTable descriptionObject;
	return descriptionObject;
}

void DescriptionTable::constructSearchInfoStructures(std::shared_ptr<JsonContainer const> searchConfigNode)
{

}

void DescriptionTable::constructAggregationInfoStructures(std::shared_ptr<JsonContainer const> aggrConfigNode)
{

}

void tuneFromConfig(Configuration const &searchconfig, Configuration const &aggregationConfig)
{
	
}

SearchInfo const& DescriptionTable::getSearchStructure(symptomCategory sympType)
{
	searchIterator structureIt = _descriptorSearching.find(sympType);
	if(structureIt == _descriptorSearching.end())
	{
		throw ConfigurationException("Cannot find search structure by symptom type: " 
			+ std::to_string(static_cast<int>(sympType)),
			ConfigurationException::BAD_SEARCH_STRUCTURE);
	}
	return *(structureIt->second.get());
}

AggregationInfo const& DescriptionTable::getAggrStructure(symptomCategory sympType)
{

}

DescriptionTable::DescriptionTable()
{

}

