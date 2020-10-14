#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>
#include <memory>
#include <mutex>
#include <regex>
#include <cstdlib>

#include "../utility_module/json.hpp"
#include "../utility_module/syntax_analyzer.hpp"
#include "../exception_module/exceptions.hpp"

using namespace utility_space;
using namespace siem_ex_space;

namespace description_space
{
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

	struct AggregationInfoNode
	{
		int keyRegGroup;
		int valueRegGrop;
		typeNodeJSON typeNode;
		std::string parentNode;
		std::regex keyFindRegex;
		std::regex valueFindRegex;

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
}

#endif // CONFIGURATION_HPP