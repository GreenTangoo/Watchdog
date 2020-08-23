#include "description_table.hpp"

using namespace description_space;

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

void DescriptionTable::tuneFromConfig(Configuration const &config)
{
	
}

SearchInfo const& DescriptionTable::getSearchStructure(symptomCategory sympType)
{

}

AggregationInfo const& DescriptionTable::getAggrStructure(symptomCategory sympType)
{

}

DescriptionTable::DescriptionTable()
{

}

