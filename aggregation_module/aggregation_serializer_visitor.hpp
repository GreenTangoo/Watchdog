#ifndef AGGREGATION_SERIALIZER_VISITOR_HPP
#define AGGREGATION_SERIALIZER_VISITOR_HPP

#include "net_grabbers.hpp"
#include "system_grabbers.hpp"

namespace aggregation_space
{
    class IAggrSerializerVisitor
    {
    public:
        virtual void Visit(AggregationIpTables const *pAggrIpTables) const = 0;
        virtual void Visit(AggregationVsftpd const *pAggrVsftpd) const = 0;
        virtual void Visit(AggregationSshd const *pAggrSshd) const = 0;
        virtual void Visit(AggregationApache const *pAggrApache) const = 0;
        virtual void Visit(AggregationPam const *pAggrPam) const = 0;
    };

    class AggrJsonSerializerVisitor : public IAggrSerializerVisitor
    {
    public:
        void Visit(AggregationIpTables const *pAggrIpTables) const override;
        void Visit(AggregationVsftpd const *pAggrVsftpd) const override;
        void Visit(AggregationSshd const *pAggrSshd) const override;
        void Visit(AggregationApache const *pAggrApache) const override;
        void Visit(AggregationPam const *pAggrPam) const override;
    private:
        template<class RecordStruct>
        void FillSerializeStructure(
                std::string const &key,
                std::vector<RecordStruct> const &values,
                std::map<std::string, IJsonContainerPtr> &resStruct) const;
    private:
        IJsonContainerPtr FromRecordInfo(AggregationIpTables::IpTablesRecordInfo const &record) const;
        IJsonContainerPtr FromRecordInfo(AggregationVsftpd::VsftpdRecordInfo const &record) const;
        IJsonContainerPtr FromRecordInfo(AggregationSshd::SshdRecordInfo const &record) const;
        IJsonContainerPtr FromRecordInfo(AggregationApache::ApacheRecordInfo const &record) const;
        IJsonContainerPtr FromRecordInfo(AggregationPam::PamRecordInfo const &record) const;
    };

    IAggrSerializerVisitorPtr CreateVisitor(SerializeType const serializeKind);
}

#endif // AGGREGATION_SERIALIZER_VISITOR_HPP
