#ifndef SEARCH_AGGR_INFO_HPP
#define SEARCH_AGGR_INFO_HPP

#include <string>
#include <memory>
#include <mutex>
#include <regex>

#include "../utility_module/json.hpp"
#include "../exception_module/exceptions.hpp"

using namespace utility_space;
using namespace siem_ex_space;

namespace description_space
{
	enum relationshipCondition { AND = 0, OR };
	enum compareCondition { EQ = '=', LT = '<', GT = '>' };

	struct SearchInfoNode
	{
		relationshipCondition condition;
		std::string keyNode;
		std::pair<std::string, compareCondition> searchDetail;
		std::unique_ptr<SearchInfoNode> additionalSearching;
	};

	struct SearchInfo
	{
		std::mutex fileManipulationMutex;
		std::string jsonFilename;
		std::unique_ptr<SearchInfoNode> rootSearchConfigNode;
	};

	struct AggregationInfoNode
	{
		typeNodeJSON typeNode;
		std::string parentNode;
		std::regex keyFindRegex;
		std::regex valueFindRegex;
		std::unique_ptr<AggregationInfoNode> additionalAggregation;
	};

	struct AggregationInfo
	{
		std::mutex fileManipulationMutex;
		std::string logFilename;
		std::string jsonFilename;
		std::unique_ptr<AggregationInfoNode> rootAggregationInfoNode;
	};

	class Configuration
	{
	private:
		mutable std::mutex configMutex;
		JsonObject configurationFileParser;
	public:
		Configuration(JsonObject obj);
		Configuration(Configuration const &other);
		Configuration(Configuration &&other);
		Configuration const& operator=(Configuration const &other);
		Configuration const& operator=(Configuration &&other);
		JsonObject getConfiguration(std::string const &nameNode);
	};
}

#endif // SEARCH_AGGR_INFO_HPP