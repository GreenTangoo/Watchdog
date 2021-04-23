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
	};

	class DescriptorFillerImpl : public IDescriptorFiller
	{
	public:
		DescriptorFillerImpl(std::shared_ptr<AggregationInfo> cfgPtr);
		virtual ~DescriptorFillerImpl();
		std::shared_ptr<AggregationInfo> getAggregationCfgInfo(JsonObject const &aggrConfigObj);
	private:
		void putLogInfo(JsonObject const &configObj);
		void putIdNode(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, JsonObject const &configObj);
		void putAggrType(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, JsonObject const &configObj);
		void putKeyName(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, JsonObject const &configObj);
		void putKeyGroup(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, JsonObject const &configObj);
		void putValueName(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, JsonObject const &configObj);
		void putValueGroup(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, JsonObject const &configObj);
		void putAdditionalConditions(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, JsonObject const &configObj);
	private:
		AggregationCondition parseConditionStr(std::string const &conditionStr);
		virtual void putSpecificAggrInfo(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, 
			JsonObject const &aggrConfigObj) = 0;
	protected:
		std::shared_ptr<AggregationInfo> _cfgDesc;
	};

	class JsonDescriptorFiller : public DescriptorFillerImpl
	{
	public:
		JsonDescriptorFiller(behaviourType aggrBehaivour);
		virtual ~JsonDescriptorFiller();
		void putTypeNode(AggregationJsonInfoNode &jsonAggrNodeInfo, JsonObject const &configObj);
		void putParentPath(AggregationJsonInfoNode &jsonAggrNodeInfo, JsonObject const &configObj);
	private:
		virtual void putSpecificAggrInfo(std::shared_ptr<AggregationInfoNode> aggregationCfgNodeInfoPtr, 
			JsonObject const &aggrConfigObj) override;
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

	std::shared_ptr<DescriptorFillerImpl> create_descriptor_filler(behaviourType behaviourAggr);
	std::shared_ptr<AggregationInfoNode> create_aggr_info_node(behaviourType behaviourAggr);
}

#endif // DESCRIPTION_TABLE_HPP