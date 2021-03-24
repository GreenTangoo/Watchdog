#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>
#include <memory>
#include <mutex>
#include <regex>
#include <cstdlib>

#include "types_resolver.hpp"
#include "../utility_module/json.hpp"
#include "../utility_module/regex_siem.hpp"

using namespace utility_space;
using namespace siem_ex_space;

#define EMPTY_REGEX_GROUP -1

namespace description_space
{
	enum relationshipCondition { NO_RELATIONSHIP = 0, AND, OR, INNER };
    enum compareCondition { NO_CONDITION = 0, EQ, NE, LT, LE, GT, GE }; 
	enum aggregationConditionMember {NO_MEMBER = 0, KEY_MEMBER, VALUE_MEMBER };


	struct SearchInfoNode
	{
		relationshipCondition condition;
		std::string keyNode;
		std::pair<std::string, compareCondition> searchDetail;
		std::unique_ptr<SearchInfoNode> additionalSearchNode;
	};

	struct SearchInfo
	{
		std::string jsonFilename;
		std::unique_ptr<SearchInfoNode> rootSearchConfigNode;
	};

	struct AggregationCondition
	{
		relationshipCondition aggrConditonType;
		int idAggregationNode;
		aggregationConditionMember infoNodeMember;

		AggregationCondition();
		AggregationCondition(AggregationCondition const &other);
		AggregationCondition const& operator=(AggregationCondition const &other);
	};

	struct AggregationRegexInfo
	{
		int keyRegGroup;
		int valueRegGroup;
		RegexSiem keyFindRegex;
		RegexSiem valueFindRegex;

		AggregationRegexInfo();
		AggregationRegexInfo(AggregationRegexInfo const &other);
	};

	struct AggregationInfoNode
	{
		int nodeId;
		aggrType grabType;
		AggregationRegexInfo regexInfo;
		std::vector<AggregationCondition> additionalConditions;

		AggregationInfoNode();
		AggregationInfoNode(AggregationInfoNode const &other);
		virtual ~AggregationInfoNode();
	};

	struct AggregationJsonInfoNode : public AggregationInfoNode
	{
		typeNodeJSON typeNode;
		std::string parentNodePath;

		AggregationJsonInfoNode();
		AggregationJsonInfoNode(AggregationJsonInfoNode const &other);
		virtual ~AggregationJsonInfoNode();
	};

	struct AggregationInfo
	{
		std::string logFilename;
		std::string resultFilename;
		behaviourType aggregationBehaviour;
		
		AggregationInfo();
		virtual ~AggregationInfo();
	};

	struct AggregationJsonInfo : public AggregationInfo
	{
		std::vector<AggregationJsonInfoNode> aggregationsInfoCfg;

		AggregationJsonInfo();
		virtual ~AggregationJsonInfo();
	};

	class Configuration
	{
	private:
		JsonObject _configurationFileParser;
	public:
		Configuration(JsonObject configJson);
		Configuration(Configuration const &other);
		Configuration(Configuration &&other);
		Configuration const& operator=(Configuration const &other);
		Configuration const& operator=(Configuration &&other);
		JsonObject getConfiguration(std::string const &nameNode) const;
	};

	compareCondition tryFoundCompareCondition(std::string const &valueStr);

	std::shared_ptr<JsonContainer> tryFoundNextRelationship(std::shared_ptr<JsonContainer> const ptr, 
		int subNodeLevel);

	std::shared_ptr<JsonContainer> tryFoundNextRelationship(JsonObject const &obj,
		int subNodeLevel);

	relationshipCondition stringToRelationship(std::string relationshipStr);
	std::string relationshipToString(relationshipCondition relationship);

	compareCondition stringToCompareCondition(std::string conditionStr);
	std::string compareConditionToString(compareCondition condition);

	aggrType stringToAggregationType(std::string aggrTypeStr);
	std::string aggregationTypeToString(aggrType grabType);

	aggregationConditionMember stringToAggrMember(std::string memberStr);
	std::string aggrMemberToString(aggregationConditionMember memberCondition);
}

#endif // CONFIGURATION_HPP