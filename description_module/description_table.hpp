#ifndef DESCRIPTION_TABLE_HPP
#define DESCRIPTION_TABLE_HPP

#include <map>

#include "../correlation_module/symptom_implementation.hpp"
#include "../aggregation_module/symptom_grabber.hpp"
#include "../exception_module/exceptions.hpp"

using namespace correlation_space;
using namespace aggregation_space;
using namespace siem_ex_space;

namespace description_space
{
	class IDescriptorFiller
	{
	public:
		virtual ~IDescriptorFiller();
		virtual std::shared_ptr<AggregationInfo> getAggrInfo(JsonObject const &aggrConfigObj) = 0;
	};

	class DescriptorFillerImpl : public IDescriptorFiller
	{
	public:
		DescriptorFillerImpl();
		virtual ~DescriptorFillerImpl();
		virtual std::shared_ptr<AggregationInfo> getAggrInfo(JsonObject const &aggrConfigObj);
		void setAggrInfoPtr(std::shared_ptr<AggregationInfo> cfgDesc);
	private:
		std::shared_ptr<AggregationInfo> _cfgDesc;
	};

	class JsonDescriptorFiller : public DescriptorFillerImpl
	{
	public:
		JsonDescriptorFiller(behaviourType aggrBehaivour);
		virtual ~JsonDescriptorFiller();
		virtual std::shared_ptr<AggregationInfo> getAggrInfo(JsonObject const &aggrConfigObj);
	private:
		void putIdNode(AggregationJsonInfoNode &jsonAggrNodeInfo, JsonObject const &configObj);
		void putAggrType(AggregationJsonInfoNode &jsonAggrNodeInfo, JsonObject const &configObj);
		void putTypeNode(AggregationJsonInfoNode &jsonAggrNodeInfo, JsonObject const &configObj);
		void putKeyName(AggregationJsonInfoNode &jsonAggrNodeInfo, JsonObject const &configObj);
		void putKeyGroup(AggregationJsonInfoNode &jsonAggrNodeInfo, JsonObject const &configObj);
		void putValueName(AggregationJsonInfoNode &jsonAggrNodeInfo, JsonObject const &configObj);
		void putValueGroup(AggregationJsonInfoNode &jsonAggrNodeInfo, JsonObject const &configObj);
		void putParentPath(AggregationJsonInfoNode &jsonAggrNodeInfo, JsonObject const &configObj);
	private:
		std::shared_ptr<AggregationJsonInfo> _cfgJsonDesc;
	};

	class DescriptionTable
	{
	private:
		std::map<symptomCategory, std::unique_ptr<SearchInfo>> _descriptorSearching;
		std::map<grabberCategory, std::shared_ptr<AggregationInfo>> _aggregationDescriptors;
	private:
		DescriptionTable();	
		DescriptionTable(DescriptionTable const &other) = delete;
		DescriptionTable(DescriptionTable &&other) = delete;
		DescriptionTable const& operator=(DescriptionTable const &other) = delete;
		DescriptionTable const& operator=(DescriptionTable &&other) = delete;
		std::unique_ptr<SearchInfoNode> addSearchingInfo(JsonObject const &searchConfigObj, 
			std::unique_ptr<SearchInfoNode> infoStruct);
		void constructSearchInfoStructures(JsonObject const &searchJsonObj);
		void constructAggregationInfoStructures(JsonObject const &aggregationJsonObj);
	public:
		enum configType { CORRELATION_CONFIG = 0, AGGREGATION_CONFIG }; 
		~DescriptionTable();
		static DescriptionTable& getInstance();
		SearchInfo const& getSearchStructure(symptomCategory sympType);
		std::shared_ptr<AggregationInfo const> getAggrStructure(grabberCategory grabType);
		void tuneFromConfig(Configuration const &config, configType typeConfig);
	};

	std::shared_ptr<DescriptorFillerImpl> createDescriptorFiller(behaviourType behaviourAggr);
}

#endif // DESCRIPTION_TABLE_HPP