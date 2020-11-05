#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>
#include <memory>
#include <mutex>
#include <regex>
#include <cstdlib>

#include "../utility_module/json.hpp"
#include "../exception_module/exceptions.hpp"

using namespace utility_space;
using namespace siem_ex_space;

namespace description_space
{
	enum relationshipCondition { NO_RELATIONSHIP = 0, AND, OR, INNER };
    enum compareCondition { NO_CONDITION = 0, EQ, NE, LT, LE, GT, GE }; 

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
		std::regex keyFindRegex;
		std::regex valueFindRegex;

		AggregationRegexInfo();
	};

	struct AggregationInfoNode
	{
		int nodeId;
		typeNodeJSON typeNode;
		std::string parentNodePath;
		AggregationRegexInfo regexInfo;

		AggregationInfoNode();
	};

	struct AggregationInfo
	{
		std::string logFilename;
		std::string jsonFilename;
		std::vector<std::unique_ptr<AggregationInfoNode>> aggregationsInfo;
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
}

#endif // CONFIGURATION_HPP