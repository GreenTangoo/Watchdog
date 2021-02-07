#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>
#include <memory>
#include <mutex>
#include <regex>
#include <cstdlib>

#include "../utility_module/json.hpp"
#include "../utility_module/regex_siem.hpp"
#include "../exception_module/exceptions.hpp"

using namespace utility_space;
using namespace siem_ex_space;

#define EMPTY_REGEX_GROUP -1

namespace description_space
{
	enum relationshipCondition { NO_RELATIONSHIP = 0, AND, OR, INNER };
    enum compareCondition { NO_CONDITION = 0, EQ, NE, LT, LE, GT, GE }; 
	enum aggrType { NO_AGGR_TYPE = 0, FINDER, COUNTER };
	enum serializerType { NONE_SERIALIZER = 0, JSON_SERIALIZER, XML_SERIALIZER };

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

	struct AggregationRegexInfo
	{
		int keyRegGroup;
		int valueRegGroup;
		RegexSiem keyFindRegex;
		RegexSiem valueFindRegex;

		AggregationRegexInfo();
	};

	struct AggregationInfoNode
	{
		int nodeId;
		typeNodeJSON typeNode;
		aggrType grabType;
		std::string parentNodePath;
		AggregationRegexInfo regexInfo;

		AggregationInfoNode();
	};

	struct AggregationInfo
	{
		std::string logFilename;
		std::string resultFilename;
		std::vector<serializerType> additionalSerializeFormats;
		std::vector<std::unique_ptr<AggregationInfoNode>> aggregationsInfoCfg;
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
	std::string aggregationTypeToString(aggrType grabType);
	aggrType stringToAggregationType(std::string aggrTypeStr);
}

#endif // CONFIGURATION_HPP