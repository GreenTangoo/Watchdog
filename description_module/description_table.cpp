#include "description_table.hpp"

#define SEARCH_CONFIGS "search-configs"
#define AGGR_CONFIGS "aggregation-configs"
#define ONE_CONFIG "one-config"

#define CATEGORY "category"
#define JSON_FILENAME "json_filename"
#define KEY_NODE "key-node"
#define VALUE_NODE "value-node"

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

void DescriptionTable::tuneFromConfig(Configuration const &searchConfig, 
	Configuration const &aggregationConfig)
{
	JsonObject searchingConfig = searchConfig.getConfiguration(SEARCH_CONFIGS);
	constructSearchInfoStructures(searchingConfig);

	JsonObject aggregatingConfig = aggregationConfig.getConfiguration(AGGR_CONFIGS);
	constructSearchInfoStructures(aggregatingConfig);
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

	SearchInfo *searchingConfigStruct = structureIt->second.get();
	return *searchingConfigStruct;
}

AggregationInfo const& DescriptionTable::getAggrStructure(symptomCategory sympType)
{

}

/*-------------------------------------------------------------------*/
/*-----------------------------PRIVATE-------------------------------*/
/*-------------------------------------------------------------------*/
DescriptionTable::DescriptionTable()
{

}

void DescriptionTable::constructSearchInfoStructures(JsonObject const &searchJsonObj)
{
	std::vector<std::shared_ptr<JsonContainer>> searchConfigs = searchJsonObj.findElementsByName(ONE_CONFIG);

	for(size_t i(0); i < searchConfigs.size(); i++)
	{
		try
		{
			JsonObject configObj(*searchConfigs[i].get());
			std::unique_ptr<SearchInfo> oneSearchConfig = std::make_unique<SearchInfo>();

			std::shared_ptr<JsonContainer> jsonLogFilenamePtr = configObj.findElementByName(JSON_FILENAME);
			oneSearchConfig->jsonFilename = jsonLogFilenamePtr->keyValue.second;
			
			std::unique_ptr<SearchInfoNode> rootSearchConfigNode = std::make_unique<SearchInfoNode>();
			rootSearchConfigNode = addSearchingInfo(configObj, std::move(rootSearchConfigNode));
			oneSearchConfig->rootSearchConfigNode = std::move(rootSearchConfigNode);

			std::shared_ptr<JsonContainer> symptomCategoryNodePtr = configObj.findElementByName(CATEGORY);
			if(!symptomCategoryNodePtr)
			{
				throw DescriptionException("Cannot find symptom category in config JSON node",
					DescriptionException::NOT_FOUND_SYMPTOM_CATEGORY);
			}

			std::string symptomCategoryStr = symptomCategoryNodePtr->keyValue.second;
			symptomCategory sympCategory = SymptomCategoryResolver::stringToSymptomCategory(symptomCategoryStr);

			_descriptorSearching.insert(std::pair<symptomCategory, std::unique_ptr<SearchInfo>>(
				sympCategory, std::move(oneSearchConfig)));
		}
		catch(DescriptionException &ex)
		{
			/*PASS TO DEBUG_INFO_OUTPUT*/
		}
	}
}

void DescriptionTable::constructAggregationInfoStructures(JsonObject const &aggregationJsonObj)
{

}

std::unique_ptr<SearchInfoNode> DescriptionTable::addSearchingInfo(JsonObject const &searchConfigObj, 
	std::unique_ptr<SearchInfoNode> infoStruct)
{
	{
		std::shared_ptr<JsonContainer> keyNodePtr = searchConfigObj.findElementByName(KEY_NODE);
		if(!keyNodePtr)
		{
			throw DescriptionException("Cannot find key-node parameter",
				DescriptionException::NOT_FOUND_KEY_NODE);
		}
		infoStruct->keyNode = keyNodePtr->keyValue.second;
	}
	{
		std::shared_ptr<JsonContainer> valueNodePtr = searchConfigObj.findElementByName(VALUE_NODE);
		if(valueNodePtr)
		{
			
		}
	}
}

std::unique_ptr<AggregationInfoNode> DescriptionTable::addAggrInfo(JsonObject const &aggrConfigObj, 
	std::unique_ptr<AggregationInfoNode> aggrStruct)
{

}
