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
		std::map<symptomCategory, std::unique_ptr<SearchInfo>> descriptorSearching;
		std::map<symptomCategory, std::unique_ptr<AggregationInfo>> descriptorAggregation;
	private:
		DescriptionTable();
		DescriptionTable(DescriptionTable const &other) = delete;
		DescriptionTable(DescriptionTable &&other) = delete;
		DescriptionTable const& operator=(DescriptionTable const &other) = delete;
		DescriptionTable const& operator=(DescriptionTable &&other) = delete;
		void constructSearchInfoStructures();
		void constructAggregationInfoStructures();
	public:
		~DescriptionTable();
		static DescriptionTable& getInstance();
		std::unique_ptr<SearchInfo> const getSearchStructure(symptomCategory sympType);
		std::unique_ptr<AggregationInfo> const getAggrStructure(symptomCategory sympType);
		void tuneFromConfig(Configuration const &config);
	};
}

#endif // DESCRIPTION_TABLE_HPP