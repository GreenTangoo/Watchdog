#include "description_table.hpp"

#define EMPTY_PATTERN ""
#define EMPTY_GROUP -1

#define SEARCH_CONFIGS "search-configs"
#define AGGR_CONFIGS "aggregation-configs"
#define ONE_CONFIG "one-config"

#define CATEGORY "category"
#define JSON_FILENAME "json-filename"
#define KEY_NODE "key-node"
#define VALUE_NODE "value-node"

#define SOURCE_LOG "source-log"
#define RESULT_JSON "result-json"
#define INFO_NODE "info-node"
#define ID_NODE "id"
#define TYPE_NODE "node-type"
#define KEY_NAME "key-name"
#define KEY_GROUP "key-group"
#define VALUE_NAME "value-name"
#define VALUE_GROUP "value-group"
#define PARENT_NODE "parent-node"

using namespace description_space;

typedef std::map<symptomCategory, std::unique_ptr<SearchInfo>>::iterator searchIterator;
typedef std::map<grabberCategory, std::unique_ptr<AggregationInfo>>::iterator grabIterator;

static std::shared_ptr<JsonContainer> guaranteeGetPtrByName(JsonObject const &obj, std::string nameStr);

static void putIdNode(std::unique_ptr<AggregationInfoNode> &aggrStruct, JsonObject const &configObj);
static void putTypeNode(std::unique_ptr<AggregationInfoNode> &aggrStruct, JsonObject const &configObj);
static void putKeyName(std::unique_ptr<AggregationInfoNode> &aggrStruct, JsonObject const &configObj);
static void putKeyGroup(std::unique_ptr<AggregationInfoNode> &aggrStruct, JsonObject const &configObj);
static void putValueName(std::unique_ptr<AggregationInfoNode> &aggrStruct, JsonObject const &configObj);
static void putValueGroup(std::unique_ptr<AggregationInfoNode> &aggrStruct, JsonObject const &configObj);
static void putParentPath(std::unique_ptr<AggregationInfoNode> &aggrStruct, JsonObject const &configObj);

static void putValueNode(std::unique_ptr<SearchInfoNode> &infoStruct, JsonObject const &configObj);


DescriptionTable::~DescriptionTable()
{

}

DescriptionTable& DescriptionTable::getInstance()
{
	static DescriptionTable descriptionObject;
	return descriptionObject;
}

void DescriptionTable::tuneFromConfig(Configuration const &config, configType typeConfig)
{
	JsonObject configObj;

	if(typeConfig == DescriptionTable::CORRELATION_CONFIG)
	{
		configObj = config.getConfiguration(SEARCH_CONFIGS);
		constructSearchInfoStructures(configObj);
	}
	else if(typeConfig == DescriptionTable::AGGREGATION_CONFIG)
	{
		configObj = config.getConfiguration(AGGR_CONFIGS);
		constructAggregationInfoStructures(configObj);
	}
	else
	{
		throw DescriptionException("Invalid config type recieved in tuneFromConfig method",
			DescriptionException::INVALID_CONFIG_TYPE);
	}
}

SearchInfo const& DescriptionTable::getSearchStructure(symptomCategory sympType)
{
	searchIterator structureIt = _descriptorSearching.find(sympType);
	if(structureIt == _descriptorSearching.end())
	{
		throw ConfigurationException("Cannot find search structure by symptom type: " 
			+ SymptomCategoryResolver::symptomCategoryToString(sympType),
			ConfigurationException::BAD_SEARCH_STRUCTURE);
	}

	SearchInfo *searchingConfigStruct = structureIt->second.get();
	return *searchingConfigStruct;
}

AggregationInfo const& DescriptionTable::getAggrStructure(grabberCategory grabType)
{
	grabIterator structureIt = _descriptorAggregation.find(grabType);
	if(structureIt == _descriptorAggregation.end())
	{
		throw ConfigurationException("Cannot find aggregation structure by grabber type: "
			+ GrabberCategoryResolver::grabberCategoryToString(grabType),
			ConfigurationException::BAD_AGGR_STRUCTURE);
	}

	AggregationInfo *aggregatingConfigStruct = structureIt->second.get();
	return *aggregatingConfigStruct;
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

			std::shared_ptr<JsonContainer> jsonLogFilenamePtr = guaranteeGetPtrByName(configObj, JSON_FILENAME);
			oneSearchConfig->jsonFilename = jsonLogFilenamePtr->keyValue.second;
			
			std::unique_ptr<SearchInfoNode> rootSearchConfigNode = std::make_unique<SearchInfoNode>();
			rootSearchConfigNode->condition = AND;

			oneSearchConfig->rootSearchConfigNode = 
				addSearchingInfo(configObj, std::move(rootSearchConfigNode));

			std::shared_ptr<JsonContainer> symptomCategoryNodePtr = guaranteeGetPtrByName(configObj, CATEGORY);

			std::string symptomCategoryStr = symptomCategoryNodePtr->keyValue.second;
			symptomCategory sympCategory = SymptomCategoryResolver::stringToSymptomCategory(symptomCategoryStr);

			_descriptorSearching.insert(std::pair<symptomCategory, std::unique_ptr<SearchInfo>>(
				sympCategory, std::move(oneSearchConfig)));
		}
		catch(DescriptionException const &ex)
		{
			/*PASS TO DEBUG_INFO_OUTPUT*/
		}
		catch(JsonException const &ex)
		{
			/*PASS TO DEBUG_INFO_OUTPUT*/
		}
	}
}

void DescriptionTable::constructAggregationInfoStructures(JsonObject const &aggregationJsonObj)
{
	std::vector<std::shared_ptr<JsonContainer>> aggrConfigs = aggregationJsonObj.findElementsByName(ONE_CONFIG);

	for(size_t i(0); i < aggrConfigs.size(); i++)
	{
		try
		{
			JsonObject configObj(*(aggrConfigs[i].get()));
			std::unique_ptr<AggregationInfo> oneGrabConfig = std::make_unique<AggregationInfo>();

			std::shared_ptr<JsonContainer> sourceLogNode = guaranteeGetPtrByName(configObj, SOURCE_LOG);
			oneGrabConfig->logFilename = sourceLogNode->keyValue.second;

			std::shared_ptr<JsonContainer> resultJsonNode = guaranteeGetPtrByName(configObj, RESULT_JSON);
			oneGrabConfig->jsonFilename = resultJsonNode->keyValue.second;

			std::vector<std::shared_ptr<JsonContainer>> infoNodes = configObj.findElementsByName(INFO_NODE);
			for(size_t j(0); j < infoNodes.size(); j++)
			{
				std::unique_ptr<AggregationInfoNode> oneInfoNode = std::make_unique<AggregationInfoNode>();
				oneInfoNode = addAggrInfo(*(infoNodes[j].get()), std::move(oneInfoNode));

				oneGrabConfig->aggregationsInfo.push_back(std::move(oneInfoNode));
			}

			std::shared_ptr<JsonContainer> categoryNodePtr = guaranteeGetPtrByName(configObj, CATEGORY);
			std::string grabberCategoryStr = categoryNodePtr->keyValue.second;
			
			grabberCategory grabCategory = GrabberCategoryResolver::stringToGrabberCategory(grabberCategoryStr);

			_descriptorAggregation.insert(std::pair<grabberCategory, std::unique_ptr<AggregationInfo>>(
				grabCategory, std::move(oneGrabConfig)));
		}
		catch(DescriptionException const &ex)
		{
			/*DEBUG INFO*/
		}
		catch(JsonException const &ex)
		{
			/*DEBUG INFO*/
		}
	}
}

std::unique_ptr<SearchInfoNode> DescriptionTable::addSearchingInfo(JsonObject const &searchConfigObj, 
	std::unique_ptr<SearchInfoNode> infoStruct)
{
	try
	{
		std::shared_ptr<JsonContainer> nodePtr = 
			guaranteeGetPtrByName(searchConfigObj, KEY_NODE);

		infoStruct->keyNode = nodePtr->keyValue.second;
		JsonObject nodeObj(*(nodePtr.get()));

		putValueNode(infoStruct, nodeObj);

		std::shared_ptr<JsonContainer> relationshipNode = tryFoundNextRelationship(searchConfigObj, 1);

		if(relationshipNode)
		{
			std::unique_ptr<SearchInfoNode> additionalSearchNode = 
				std::make_unique<SearchInfoNode>();

			std::string relationshipStr = relationshipNode->keyValue.first;
			additionalSearchNode->condition = stringToRelationship(relationshipStr);

			infoStruct->additionalSearchNode = 
				addSearchingInfo(JsonObject(*relationshipNode.get()), 
				std::move(additionalSearchNode));
		}
	}
	catch(JsonException const &ex)
	{
		std::string errMsg(ex.what());
		int errCode = ex.getErrorCode();
		throw DescriptionException("Adding search info failed: " + errMsg + 
			", code: " + std::to_string(errCode),
			DescriptionException::INVALID_SEARCH_CONFIG);
	}
	catch(ConfigurationException const &ex)
	{
		std::string errMsg(ex.what());
		int errCode = ex.getErrorCode();
		throw DescriptionException("Falied analyzing process: " + errMsg +
			", code: " + std::to_string(errCode), 
			DescriptionException::INVALID_PARAMETER);
	}

	return infoStruct;
}

std::unique_ptr<AggregationInfoNode> DescriptionTable::addAggrInfo(JsonObject const &aggrConfigObj, 
	std::unique_ptr<AggregationInfoNode> aggrStruct)
{
	try
	{
		std::shared_ptr<JsonContainer> nodePtr = guaranteeGetPtrByName(aggrConfigObj, INFO_NODE);
		nodePtr = nodePtr->childNode;

		JsonObject nodeObj(*(nodePtr.get()));

		putIdNode(aggrStruct, nodeObj);
		putTypeNode(aggrStruct, nodeObj);
		putKeyName(aggrStruct, nodeObj);
		putKeyGroup(aggrStruct, nodeObj);
		putValueName(aggrStruct, nodeObj);
		putValueGroup(aggrStruct, nodeObj);
		putParentPath(aggrStruct, nodeObj);
	}
	catch(JsonException const &ex)
	{
		throw DescriptionException(ex.what(), DescriptionException::NOT_FOUND_NODE);
	}

	return aggrStruct;
}

/*--------------------------------------------------------------------------------------*/
/*---------------------------------STATIC FUNCTIONS-------------------------------------*/
/*--------------------------------------------------------------------------------------*/
std::shared_ptr<JsonContainer> guaranteeGetPtrByName(JsonObject const &obj, std::string nameStr)
{
	std::shared_ptr<JsonContainer> foundedPtr = obj.findElementByName(nameStr);

	if(!foundedPtr)
	{
		throw JsonException("Node name: " + nameStr + " doesn't exists", 
			JsonException::BAD_NODE);
	}

	return foundedPtr;
}

void putIdNode(std::unique_ptr<AggregationInfoNode> &aggrStruct,
	JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> idPtr = guaranteeGetPtrByName(configObj, ID_NODE);
	std::string nodeIdStr = idPtr->keyValue.second;

	aggrStruct->nodeId = atoi(nodeIdStr.c_str());
}

void putTypeNode(std::unique_ptr<AggregationInfoNode> &aggrStruct, 
	JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> nodePtr = guaranteeGetPtrByName(configObj, TYPE_NODE);
	std::string typeNodeStr = nodePtr->keyValue.second;

	typeNodeJSON nodeType = JSONNodeTypeResolver::getInstance().getNodeType(typeNodeStr);
	aggrStruct->typeNode = nodeType;
}

void putKeyName(std::unique_ptr<AggregationInfoNode> &aggrStruct,
	JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> nodePtr = guaranteeGetPtrByName(configObj, KEY_NAME);
	std::string keyNameRegStr = nodePtr->keyValue.second;
	aggrStruct->regexInfo.keyFindRegex = std::regex(keyNameRegStr);
}

void putKeyGroup(std::unique_ptr<AggregationInfoNode> &aggrStruct,
	JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> nodePtr = configObj.findNearElementByName(KEY_GROUP);
	if(nodePtr)
	{
		std::string keyRegGroupStr = nodePtr->keyValue.second;
		aggrStruct->regexInfo.keyRegGroup = std::atoi(keyRegGroupStr.c_str());
	}
}

void putValueName(std::unique_ptr<AggregationInfoNode> &aggrStruct,
	JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> nodePtr = configObj.findNearElementByName(VALUE_NAME);
	if(nodePtr)
	{
		std::string valueNameRegStr = nodePtr->keyValue.second;
		aggrStruct->regexInfo.valueFindRegex = std::regex(valueNameRegStr);
	}	
	else
	{
		aggrStruct->regexInfo.valueFindRegex = std::regex(EMPTY_PATTERN);
	}
}

void putValueGroup(std::unique_ptr<AggregationInfoNode> &aggrStruct,
	JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> nodePtr = configObj.findNearElementByName(VALUE_GROUP);
	if(nodePtr)
	{
		std::string valueRegGroupStr = nodePtr->keyValue.second;
		aggrStruct->regexInfo.valueRegGroup = std::atoi(valueRegGroupStr.c_str());
	}
}

void putParentPath(std::unique_ptr<AggregationInfoNode> &aggrStruct,
	JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> parentNodePtr = guaranteeGetPtrByName(configObj, PARENT_NODE);
	std::string parentNodeStr = parentNodePtr->keyValue.second;
	aggrStruct->parentNodePath = parentNodeStr;
}

void putValueNode(std::unique_ptr<SearchInfoNode> &infoStruct, JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> nodePtr = configObj.findNearElementByName(VALUE_NODE);

	if(nodePtr)
	{
		std::string valueStr = nodePtr->keyValue.second;
		compareCondition valueCondition = tryFoundCompareCondition(valueStr);

		if(valueCondition == NO_CONDITION)
		{
			throw DescriptionException("Cannot get compare condition",
				DescriptionException::INVALID_VALUE_STRING);
		}

		std::string valueWithoutCondition = StringManager::getAfterSymbol(valueStr,
			R_SQ_BRACKET);

		infoStruct->searchDetail = 
			std::pair<std::string, compareCondition>(valueWithoutCondition, valueCondition);
	}
}