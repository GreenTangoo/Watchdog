#ifndef DESCRIPTION_TABLE_HPP
#define DESCRIPTION_TABLE_HPP

#include <map>

#include "search_aggr_info.hpp"
#include "../correlation_module/symptom_implementation.hpp"

using namespace correlation_space;

namespace description_space
{
	class DescriptionTable
	{
	private:
		std::map<symptomCategory, std::unique_ptr<SearchInfo>> _descriptorSearching;
		std::map<symptomCategory, std::unique_ptr<AggregationInfo>> _descriptorAggregation;
	private:
		DescriptionTable();
		~DescriptionTable();
		DescriptionTable(DescriptionTable const &other) = delete;
		DescriptionTable(DescriptionTable &&other) = delete;
		DescriptionTable const& operator=(DescriptionTable const &other) = delete;
		DescriptionTable const& operator=(DescriptionTable &&other) = delete;
		void constructSearchInfoStructures(std::shared_ptr<JsonContainer const> searchConfigNode);
		void constructAggregationInfoStructures(std::shared_ptr<JsonContainer const> aggrConfigNode);
	public:
		static DescriptionTable& getInstance();
		SearchInfo const& getSearchStructure(symptomCategory sympType);
		AggregationInfo const& getAggrStructure(symptomCategory sympType);
		void tuneFromConfig(Configuration const &config);
	};
}

#endif // DESCRIPTION_TABLE_HPP