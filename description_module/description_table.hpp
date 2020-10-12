#ifndef DESCRIPTION_TABLE_HPP
#define DESCRIPTION_TABLE_HPP

#include <map>

#include "../correlation_module/symptom_implementation.hpp"
#include "../aggregation_module/aggregator_implementation.hpp"
#include "../exception_module/exceptions.hpp"

using namespace correlation_space;
using namespace aggregation_space;
using namespace siem_ex_space;

namespace description_space
{
	class DescriptionTable
	{
	private:
		std::map<symptomCategory, std::unique_ptr<SearchInfo>> _descriptorSearching;
		std::map<grabberCategory, std::unique_ptr<AggregationInfo>> _descriptorAggregation;
	private:
		DescriptionTable();	
		DescriptionTable(DescriptionTable const &other) = delete;
		DescriptionTable(DescriptionTable &&other) = delete;
		DescriptionTable const& operator=(DescriptionTable const &other) = delete;
		DescriptionTable const& operator=(DescriptionTable &&other) = delete;
		std::unique_ptr<SearchInfoNode> addSearchingInfo(JsonObject const &searchConfigObj, 
			std::unique_ptr<SearchInfoNode> infoStruct);
		std::unique_ptr<AggregationInfoNode> addAggrInfo(JsonObject const &aggrConfigObj, 
			std::unique_ptr<AggregationInfoNode> aggrStruct);
		void constructSearchInfoStructures(JsonObject const &searchJsonObj);
		void constructAggregationInfoStructures(JsonObject const &aggregationJsonObj);
	public:
		enum configType { CORRELATION_CONFIG = 0, AGGREGATION_CONFIG }; 
		~DescriptionTable();
		static DescriptionTable& getInstance();
		SearchInfo const& getSearchStructure(symptomCategory sympType);
		AggregationInfo const& getAggrStructure(grabberCategory grabType);
		void tuneFromConfig(Configuration const &config, configType typeConfig);
	};
}

#endif // DESCRIPTION_TABLE_HPP