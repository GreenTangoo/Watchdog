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
#define RESULT_LOG "result-log"
#define AGGR_BEHAVIOUR "aggr-behaviour"
#define CONDITION "condition"
#define INFO_NODE "info-node"
#define ID_NODE "id"
#define AGGR_TYPE "aggr-type"
#define TYPE_NODE "node-type"
#define KEY_NAME "key-name"
#define KEY_GROUP "key-group"
#define VALUE_NAME "value-name"
#define VALUE_GROUP "value-group"
#define PARENT_NODE "parent-node"

using namespace description_space;

typedef std::map<symptomCategory, std::unique_ptr<SearchInfo>>::iterator searchIterator;
typedef std::map<grabberCategory, std::shared_ptr<AggregationInfo>>::iterator grabIterator;

static void putValueNode(std::unique_ptr<SearchInfoNode> &infoStruct, JsonObject const &configObj);

/*-----------------------------------------------------------------*/
/*----------------------IDESCRIPTOR FILLER-------------------------*/
/*-----------------------------------------------------------------*/
IDescriptorFiller::~IDescriptorFiller()
{

}

/*-----------------------------------------------------------------*/
/*----------------------DESCRIPTOR IMPL----------------------------*/
/*-----------------------------------------------------------------*/
DescriptorFillerImpl::DescriptorFillerImpl(std::shared_ptr<AggregationInfo> cfgPtr) :
	_cfgDesc(cfgPtr)
{

}

DescriptorFillerImpl::~DescriptorFillerImpl()
{

}

std::shared_ptr<AggregationInfo> DescriptorFillerImpl::getAggregationCfgInfo(JsonObject const &aggrConfigObj)
{
	putLogInfo(aggrConfigObj);

	std::shared_ptr<JsonContainer> behaivorNodePtr = guaranteeGetPtrByName(aggrConfigObj, AGGR_BEHAVIOUR);
	std::string behaviourStr = behaivorNodePtr->keyValue.second;
	behaviourType aggregationBehaviour = BehaviourTypeResolver::stringToSerializerType(behaviourStr);

	std::vector<std::shared_ptr<JsonContainer>> infoNodes = aggrConfigObj.findElementsByName(INFO_NODE);
	for(size_t i(0); i < infoNodes.size(); i++)
	{
		std::shared_ptr<JsonContainer> nodePtr = infoNodes[i]->childNode;
		JsonObject nodeObj(*(nodePtr.get()));
		std::shared_ptr<AggregationInfoNode> cfgNodeInfoPtr = create_aggr_info_node(aggregationBehaviour);

		putIdNode(cfgNodeInfoPtr, nodeObj);
		putAggrType(cfgNodeInfoPtr, nodeObj);
		putKeyName(cfgNodeInfoPtr, nodeObj);
		putKeyGroup(cfgNodeInfoPtr, nodeObj);
		putValueName(cfgNodeInfoPtr, nodeObj);
		putValueGroup(cfgNodeInfoPtr, nodeObj);
		putAdditionalConditions(cfgNodeInfoPtr, nodeObj);

		putSpecificAggrInfo(cfgNodeInfoPtr, nodeObj);

		_cfgDesc->aggregationsInfoCfg.push_back(cfgNodeInfoPtr);
	}

	return _cfgDesc;
}

void DescriptorFillerImpl::putLogInfo(JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> sourceLogNode = guaranteeGetPtrByName(configObj, SOURCE_LOG);
	_cfgDesc->logFilename = sourceLogNode->keyValue.second;

	std::shared_ptr<JsonContainer> resultLogNode = guaranteeGetPtrByName(configObj, RESULT_LOG);
	_cfgDesc->resultFilename = resultLogNode->keyValue.second;
}

void DescriptorFillerImpl::putIdNode(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, 
	JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> idPtr = guaranteeGetPtrByName(configObj, ID_NODE);
	std::string nodeIdStr = idPtr->keyValue.second;

	aggregationCfgNodeInfoPtr->nodeId = atoi(nodeIdStr.c_str());
}

void DescriptorFillerImpl::putAggrType(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, 
	JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> aggrTypePtr = guaranteeGetPtrByName(configObj, AGGR_TYPE);
	std::string aggrTypeStr = aggrTypePtr->keyValue.second;

	aggrType grabType = description_space::stringToAggregationType(aggrTypeStr);
	aggregationCfgNodeInfoPtr->grabType = grabType;
}

void DescriptorFillerImpl::putKeyName(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, 
	JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> nodePtr = guaranteeGetPtrByName(configObj, KEY_NAME);
	std::string keyNameRegStr = nodePtr->keyValue.second;
	aggregationCfgNodeInfoPtr->regexInfo.keyFindRegex = keyNameRegStr;
}

void DescriptorFillerImpl::putKeyGroup(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr,
	JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> nodePtr = configObj.findNearElementByName(KEY_GROUP);
	if(nodePtr)
	{
		std::string keyRegGroupStr = nodePtr->keyValue.second;
		aggregationCfgNodeInfoPtr->regexInfo.keyRegGroup = std::atoi(keyRegGroupStr.c_str());
	}
}

void DescriptorFillerImpl::putValueName(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr,
	JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> nodePtr = configObj.findNearElementByName(VALUE_NAME);
	if(nodePtr)
	{
		std::string valueNameRegStr = nodePtr->keyValue.second;
		aggregationCfgNodeInfoPtr->regexInfo.valueFindRegex = valueNameRegStr;
	}	
	else
	{
		aggregationCfgNodeInfoPtr->regexInfo.valueFindRegex = EMPTY_PATTERN;
	}
}

void DescriptorFillerImpl::putValueGroup(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr,
	JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> nodePtr = configObj.findNearElementByName(VALUE_GROUP);
	if(nodePtr)
	{
		std::string valueRegGroupStr = nodePtr->keyValue.second;
		aggregationCfgNodeInfoPtr->regexInfo.valueRegGroup = std::atoi(valueRegGroupStr.c_str());
	}
}

void DescriptorFillerImpl::putAdditionalConditions(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, 
	JsonObject const &configObj)
{
	std::vector<std::shared_ptr<JsonContainer>> conditiondsVec = configObj.findElementsByName(CONDITION);
	
	for(std::shared_ptr<JsonContainer> condition : conditiondsVec)
	{
		std::string conditionStr = condition->keyValue.second;
		aggregationCfgNodeInfoPtr->additionalConditions.push_back(this->parseConditionStr(conditionStr));
	}
}

AggregationCondition DescriptorFillerImpl::parseConditionStr(std::string const &conditionStr)
{
	AggregationCondition conditionInfo;
	std::vector<std::string> conditionDetails = StringManager::parseByDelimiter(conditionStr, "=");
	
	std::string nodeDetailsStr = StringManager::getStrBetweenSymbols(conditionDetails[1], '[', ']');
	std::vector<std::string> nodeDetails = StringManager::parseByDelimiter(nodeDetailsStr, ",");

	conditionInfo.aggrConditonType = stringToRelationship(conditionDetails[0]);
	conditionInfo.idAggregationNode = std::atoi(nodeDetails[0].c_str());
	conditionInfo.infoNodeMember = stringToAggrMember(nodeDetails[1]);

	return conditionInfo;
}

/*-----------------------------------------------------------------*/
/*----------------------JSON DESCRIPTOR FILLER---------------------*/
/*-----------------------------------------------------------------*/
JsonDescriptorFiller::JsonDescriptorFiller(behaviourType aggrBehaivour) : 
	DescriptorFillerImpl(std::make_shared<AggregationInfo>())
{
	_cfgDesc->aggregationBehaviour = aggrBehaivour;
}

JsonDescriptorFiller::~JsonDescriptorFiller()
{

}

void JsonDescriptorFiller::putSpecificAggrInfo(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, 
	JsonObject const &aggrConfigObj)
{
	std::shared_ptr<AggregationJsonInfoNode> aggrJsonCfgNodePtr = 
		std::dynamic_pointer_cast<AggregationJsonInfoNode>(aggregationCfgNodeInfoPtr);

	std::shared_ptr<JsonContainer> typeNodePtr = guaranteeGetPtrByName(aggrConfigObj, TYPE_NODE);
	JSONNodeTypeResolver::getInstance().getNodeType(typeNodePtr->keyValue.second);
	aggrJsonCfgNodePtr->typeNode = JSONNodeTypeResolver::getInstance().getNodeType(typeNodePtr->keyValue.second);

	std::shared_ptr<JsonContainer> parentNodePathPtr = guaranteeGetPtrByName(aggrConfigObj, PARENT_NODE);
	aggrJsonCfgNodePtr->parentNodePath = parentNodePathPtr->keyValue.second;
}

void JsonDescriptorFiller::putTypeNode(AggregationJsonInfoNode &jsonAggrNodeInfo, JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> nodePtr = guaranteeGetPtrByName(configObj, TYPE_NODE);
	std::string typeNodeStr = nodePtr->keyValue.second;

	typeNodeJSON nodeType = JSONNodeTypeResolver::getInstance().getNodeType(typeNodeStr);
	jsonAggrNodeInfo.typeNode = nodeType;
}

void JsonDescriptorFiller::putParentPath(AggregationJsonInfoNode &jsonAggrNodeInfo, JsonObject const &configObj)
{
	std::shared_ptr<JsonContainer> parentNodePtr = guaranteeGetPtrByName(configObj, PARENT_NODE);
	std::string parentNodeStr = parentNodePtr->keyValue.second;
	jsonAggrNodeInfo.parentNodePath = parentNodeStr;
}

/*-----------------------------------------------------------------*/
/*----------------------DESCRIPTION TABLE--------------------------*/
/*-----------------------------------------------------------------*/
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

std::shared_ptr<AggregationInfo const> DescriptionTable::getAggrStructure(grabberCategory grabType)
{
	grabIterator structureIt = _aggregationDescriptors.find(grabType);
	if(structureIt == _aggregationDescriptors.end())
	{
		throw ConfigurationException("Cannot find aggregation structure by grabber type: "
			+ GrabberCategoryResolver::grabberCategoryToString(grabType),
			ConfigurationException::BAD_AGGR_STRUCTURE);
	}
	
	return structureIt->second;
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

			std::shared_ptr<JsonContainer> behaivorNodePtr = guaranteeGetPtrByName(configObj, AGGR_BEHAVIOUR);
			std::string behaviourStr = behaivorNodePtr->keyValue.second;
			behaviourType aggregationBehaviour = BehaviourTypeResolver::stringToSerializerType(behaviourStr);

			std::shared_ptr<DescriptorFillerImpl> descriptor = create_descriptor_filler(aggregationBehaviour);
			
			std::shared_ptr<AggregationInfo> grabConfigInfo = descriptor->getAggregationCfgInfo(configObj);
			
			std::shared_ptr<JsonContainer> categoryNodePtr = guaranteeGetPtrByName(configObj, CATEGORY);
			std::string grabberCategoryStr = categoryNodePtr->keyValue.second;
			grabberCategory grabCategory = GrabberCategoryResolver::stringToGrabberCategory(grabberCategoryStr);

			_aggregationDescriptors.insert(std::pair<grabberCategory, std::shared_ptr<AggregationInfo>>(
				grabCategory, std::move(grabConfigInfo)));
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

std::shared_ptr<DescriptorFillerImpl> description_space::create_descriptor_filler(behaviourType behaviourAggr)
{
	switch(behaviourAggr)
	{
	case behaviourType::JSON_BEHAVIOUR:
	{
		return std::make_shared<JsonDescriptorFiller>(behaviourAggr);
	}
	break;
	}
}

std::shared_ptr<AggregationInfoNode> description_space::create_aggr_info_node(behaviourType behaviourAggr)
{
	switch(behaviourAggr)
	{
	case behaviourType::JSON_BEHAVIOUR:
	{
		return std::make_shared<AggregationJsonInfoNode>();
	}
	break;
	}
}

/*--------------------------------------------------------------------------------------*/
/*---------------------------------STATIC FUNCTIONS-------------------------------------*/
/*--------------------------------------------------------------------------------------*/
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