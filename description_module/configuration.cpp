#include "configuration.hpp"

using namespace description_space;

#define AND_CONDITION "and"
#define OR_CONDITION "or"
#define INNER_CONDITION "inner"

#define EQUAL "="
#define NOT_EQUAL "!="
#define GREATER_THAN ">"
#define GREATER_EQUAL ">="
#define LESS_THAN "<"
#define LESS_EQUAL "<="

#define FIND_TYPE "find"
#define COUNT_TYPE "count"

#define KEY_MEMBER_CONDITION "key"
#define VALUE_MEMBER_CONDITION "value"


static std::map<compareCondition, std::string> const compareStringMap = 
                                                {
                                                    {EQ, EQUAL},
                                                    {NE, NOT_EQUAL},
                                                    {GT, GREATER_THAN},
                                                    {GE, GREATER_EQUAL},
                                                    {LT, LESS_THAN},
                                                    {LE, LESS_EQUAL}
                                                };

static std::map<relationshipCondition, std::string> const relationshipStringMap =
                                                {
                                                    {AND, AND_CONDITION},
                                                    {OR, OR_CONDITION},
                                                    {INNER, INNER_CONDITION}
                                                };

static std::map<aggrType, std::string> const aggregationTypeStringMap = 
                                                {
                                                    {FINDER, FIND_TYPE},
                                                    {COUNTER, COUNT_TYPE}
                                                };

static std::map<aggregationConditionMember, std::string> const aggrCondMemberStringMap = 
                                                {
                                                    {KEY_MEMBER, KEY_MEMBER_CONDITION},
                                                    {VALUE_MEMBER, VALUE_MEMBER_CONDITION}
                                                };

static bool isRelationShipNode(std::string const &keyStr);

AggregationCondition::AggregationCondition() : 
    aggrConditonType(NO_RELATIONSHIP), idAggregationNode(-1)
{

}

AggregationCondition::AggregationCondition(AggregationCondition const &other) : 
    aggrConditonType(other.aggrConditonType), idAggregationNode(other.idAggregationNode)
{

}

AggregationCondition const& AggregationCondition::operator=(AggregationCondition const &other)
{
    if(this != &other)
    {
        this->aggrConditonType = other.aggrConditonType;
        this->idAggregationNode = other.idAggregationNode;
        this->infoNodeMember = other.infoNodeMember;
    }

    return *this;
}

AggregationRegexInfo::AggregationRegexInfo() :
	keyRegGroup(EMPTY_REGEX_GROUP), valueRegGroup(EMPTY_REGEX_GROUP)
{}

AggregationRegexInfo::AggregationRegexInfo(AggregationRegexInfo const &other) :
    keyRegGroup(other.keyRegGroup), valueRegGroup(other.valueRegGroup),
    keyFindRegex(other.keyFindRegex), valueFindRegex(other.valueFindRegex)
{}

AggregationInfo::AggregationInfo() :
    aggregationBehaviour(NONE_BEHAVIOUR)
{}

AggregationInfo::~AggregationInfo()
{}

AggregationJsonInfo::AggregationJsonInfo()
{}

AggregationJsonInfo::~AggregationJsonInfo()
{}

AggregationInfoNode::AggregationInfoNode()
{

}

AggregationInfoNode::~AggregationInfoNode()
{

}

AggregationInfoNode::AggregationInfoNode(AggregationInfoNode const &other) : 
    nodeId(other.nodeId), grabType(other.grabType), regexInfo(other.regexInfo)
{

}

AggregationJsonInfoNode::AggregationJsonInfoNode() :
	AggregationInfoNode(), typeNode(NONE)
{

}

AggregationJsonInfoNode::~AggregationJsonInfoNode()
{

}

AggregationJsonInfoNode::AggregationJsonInfoNode(AggregationJsonInfoNode const &other) : 
    AggregationInfoNode(other), typeNode(other.typeNode), parentNodePath(other.parentNodePath)
{

}

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
	std::shared_ptr<JsonContainer> foundedContainer = _configurationFileParser.findElementByName(nameNode);
	
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

/*--------------------------------------------------------------------------*/
/*-----------------------PUBLIC FUNCTIONS-----------------------------------*/
/*--------------------------------------------------------------------------*/
compareCondition description_space::tryFoundCompareCondition(std::string const &valueStr)
{
	std::string conditionStr = StringManager::getStrBetweenSymbols(valueStr, L_SQ_BRACKET, R_SQ_BRACKET);
    
    return description_space::stringToCompareCondition(conditionStr);
}

std::shared_ptr<JsonContainer> description_space::tryFoundNextRelationship(std::shared_ptr<JsonContainer> const ptr, 
	int subNodeLevel)
{
	JsonObject nodeObj(*(ptr.get()));

    return description_space::tryFoundNextRelationship(nodeObj, subNodeLevel);
}

std::shared_ptr<JsonContainer> description_space::tryFoundNextRelationship(JsonObject const &obj,
	int subNodeLevel)
{
	std::shared_ptr<JsonContainer> relationshipNodePtr = obj.findElementByName("root");

    if(!(relationshipNodePtr->childNode))
    {
        return nullptr;
    }
    
    relationshipNodePtr = relationshipNodePtr->childNode;

    for(int i(0); i < subNodeLevel; i++)
    {
        if(!(relationshipNodePtr->childNode))
            throw ConfigurationException("Incorrect subNodeLevel variable value passed",
                ConfigurationException::BAD_SUBNODE_LEVEL);
                
        relationshipNodePtr = relationshipNodePtr->childNode;
    }

    for(std::shared_ptr<JsonContainer> it = relationshipNodePtr;
        it != nullptr; it = it->nextNode)
    {
        if(isRelationShipNode(it->keyValue.first))
        {
            return it;
        }
    }

    return nullptr;
}

relationshipCondition description_space::stringToRelationship(std::string relationshipStr)
{
	relationshipCondition storeRelationshipVal = NO_RELATIONSHIP;

    for(std::map<relationshipCondition, std::string>::const_iterator it = 
        relationshipStringMap.begin(); it != relationshipStringMap.end(); it++)
    {
        if(it->second == relationshipStr)
        {
            storeRelationshipVal = it->first;
            break;
        }
    }

    return storeRelationshipVal;
}

std::string description_space::relationshipToString(relationshipCondition relationship)
{
	auto it = relationshipStringMap.find(relationship);
    if(it != relationshipStringMap.end())
    {
        return it->second;
    }
	
	return std::string("");
}

compareCondition description_space::stringToCompareCondition(std::string conditionStr)
{
	compareCondition storeCompareVal = NO_CONDITION;
    
    for(std::map<compareCondition, std::string>::const_iterator it = compareStringMap.begin();
        it != compareStringMap.end(); it++)
    {
        if(it->second == conditionStr)
        {
            storeCompareVal = it->first;
            break;
        }
    }

    return storeCompareVal;
}

std::string description_space::compareConditionToString(compareCondition condition)
{
	auto it = compareStringMap.find(condition);
    if(it != compareStringMap.end())
    {
        return it->second;
    }
    
	return std::string("");
}

aggrType description_space::stringToAggregationType(std::string aggrTypeStr)
{
    aggrType storeAggrType = NO_AGGR_TYPE;
    
    for(std::map<aggrType, std::string>::const_iterator it = aggregationTypeStringMap.begin();
        it != aggregationTypeStringMap.end(); it++)
    {
        if(it->second == aggrTypeStr)
        {
            storeAggrType = it->first;
            break;
        }
    }

    return storeAggrType;
}

std::string description_space::aggregationTypeToString(aggrType grabType)
{
    auto it = aggregationTypeStringMap.find(grabType);
    if(it != aggregationTypeStringMap.end())
    {
        return it->second;
    }

    return std::string("");
    
}

aggregationConditionMember description_space::stringToAggrMember(std::string memberStr)
{
    aggregationConditionMember storeAggrMemberCond = NO_MEMBER;
    
    for(std::map<aggregationConditionMember, std::string>::const_iterator it = aggrCondMemberStringMap.begin();
        it != aggrCondMemberStringMap.end(); it++)
    {
        if(it->second == memberStr)
        {
            storeAggrMemberCond = it->first;
            break;
        }
    }

    return storeAggrMemberCond;
}

std::string description_space::aggrMemberToString(aggregationConditionMember memberCondition)
{
    auto it = aggrCondMemberStringMap.find(memberCondition);
    if(it != aggrCondMemberStringMap.end())
    {
        return it->second;
    }

    return std::string("");
}

/*--------------------------------------------------------------------------*/
/*------------------------STATIC FUNCTIONS----------------------------------*/
/*--------------------------------------------------------------------------*/
bool isRelationShipNode(std::string const &keyStr)
{
    if((keyStr == AND_CONDITION) || 
       (keyStr == OR_CONDITION)  || 
       (keyStr == INNER_CONDITION))
        return true;
    return false;
}