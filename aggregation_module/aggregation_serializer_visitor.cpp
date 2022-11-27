#include "aggregation_serializer_visitor.hpp"

using namespace aggregation_space;

namespace
{
    const std::string DestIpKey = "destination_ip";
    const std::string SrcMacKey = "source_mac";
    const std::string DestMacKey = "destination_mac";
    const std::string SrcPortKey = "source_port";
    const std::string DestPortKey = "destination_port";
    const std::string InetProtoKey = "internet_proto";
    const std::string InterfaceNameKey = "interface";
    const std::string DateTimeKey = "datetime";

    const std::string ConnectionTypeKey = "connection_type";
    const std::string StatusTypeKey = "status_type";
    const std::string FilePathKey = "filepath";

    const std::string ActionTypeKey = "action_type";
    const std::string ActionStatusKey = "action_status";

    const std::string RequestTypeKey = "request_type";
    const std::string RequestStatusKey = "request_status";
    const std::string ProtocolVersionKey = "protocol_version";
    const std::string FileSizeKey = "filesize";

    const std::string ProcessIdKey = "process_id";
    const std::string UsernameKey = "username";
    const std::string SessionTypeKey = "session_type";
    const std::string RemoteHostKey = "remote_host";
    const std::string RemoteUserKey = "remote_user";
    const std::string TtyKey = "tty";
}


// /////////////////////////////////////////////////////////////////////////////
// AggrJsonSerializerVisitor
// Class for serialize grabbers to JSON file.
// ////////////////////////////////////////////////////////////////////////////
void AggrJsonSerializerVisitor::Visit(AggregationIpTables const *pAggrIpTables) const
{
    std::map<std::string, IJsonContainerPtr> serializeMap;

    for(const auto& pair : pAggrIpTables->GetData())
    {
        const std::string key = Bytes2StringIp(pair.first);
        FillSerializeStructure(key, pair.second, serializeMap);
    }

    IJsonContainerPtr pRecordsContainer = CreateContainer(serializeMap);
    std::map<std::string, IJsonContainerPtr> finalContainer =
    {
        { "iptables", pRecordsContainer }
    };

    JsonFileSerializer serializer(CreateContainer(finalContainer), pAggrIpTables->GetDstFile());
    serializer.Write();
}

void AggrJsonSerializerVisitor::Visit(AggregationVsftpd const *pAggrVsftpd) const
{
    std::map<std::string, IJsonContainerPtr> serializeMap;

    for(const auto& pair : pAggrVsftpd->GetData())
    {
        const std::string key = Bytes2StringIp(pair.first);
        FillSerializeStructure(key, pair.second, serializeMap);
    }

    IJsonContainerPtr pRecordsContainer = CreateContainer(serializeMap);
    std::map<std::string, IJsonContainerPtr> finalContainer =
    {
        { "vsftpd", pRecordsContainer }
    };

    JsonFileSerializer serializer(CreateContainer(finalContainer), pAggrVsftpd->GetDstFile());
    serializer.Write();
}

void AggrJsonSerializerVisitor::Visit(AggregationSshd const *pAggrSshd) const
{
    std::map<std::string, IJsonContainerPtr> serializeMap;

    for(const auto& pair : pAggrSshd->GetData())
    {
        const std::string key = pair.first;
        FillSerializeStructure(key, pair.second, serializeMap);
    }

    IJsonContainerPtr pRecordsContainer = CreateContainer(serializeMap);
    std::map<std::string, IJsonContainerPtr> finalContainer =
    {
        { "sshd", pRecordsContainer }
    };

    JsonFileSerializer serializer(CreateContainer(finalContainer), pAggrSshd->GetDstFile());
    serializer.Write();
}

void AggrJsonSerializerVisitor::Visit(AggregationApache const *pAggrApache) const
{
    std::map<std::string, IJsonContainerPtr> serializeMap;

    for(const auto& pair : pAggrApache->GetData())
    {
        const std::string key = Bytes2StringIp(pair.first);
        FillSerializeStructure(key, pair.second, serializeMap);
    }

    IJsonContainerPtr pRecordsContainer = CreateContainer(serializeMap);
    std::map<std::string, IJsonContainerPtr> finalContainer =
    {
        { "apache", pRecordsContainer }
    };

    JsonFileSerializer serializer(CreateContainer(finalContainer), pAggrApache->GetDstFile());
    serializer.Write();
}

void AggrJsonSerializerVisitor::Visit(AggregationPam const *pAggrPam) const
{
    std::map<std::string, IJsonContainerPtr> serializeMap;

    for(const auto& pair : pAggrPam->GetData())
    {
        const std::string key = pair.first;
        FillSerializeStructure(key, pair.second, serializeMap);
    }

    IJsonContainerPtr pRecordsContainer = CreateContainer(serializeMap);
    std::map<std::string, IJsonContainerPtr> finalContainer =
    {
        { "pam", pRecordsContainer }
    };

    JsonFileSerializer serializer(CreateContainer(finalContainer), pAggrPam->GetDstFile());
    serializer.Write();
}

template<class RecordStruct>
void AggrJsonSerializerVisitor::FillSerializeStructure(std::string const &key,
        std::vector<RecordStruct> const &values,
        std::map<std::string, IJsonContainerPtr> &resStruct) const
{
    std::vector<IJsonContainerPtr> containersVec;
    containersVec.reserve(values.size());

    for(const auto& record : values)
    {
        containersVec.emplace_back(FromRecordInfo(record));
    }

    resStruct.emplace(key, CreateContainer(containersVec));
}

IJsonContainerPtr AggrJsonSerializerVisitor::FromRecordInfo(AggregationIpTables::IpTablesRecordInfo const &record) const
{
    std::map<std::string, IJsonContainerPtr> const recordContainer =
    {
        {DestIpKey, CreateContainer(Bytes2StringIp(record.m_DstIpAddr))},
        {SrcMacKey, CreateContainer(Bytes2StringMac(record.m_SrcMacAddr))},
        {DestMacKey, CreateContainer(Bytes2StringMac(record.m_DstMacAddr))},
        {SrcPortKey, CreateContainer(std::to_string(record.m_SrcPort))},
        {DestPortKey, CreateContainer(std::to_string(record.m_DstPort))},
        {InetProtoKey, CreateContainer(std::to_string(static_cast<int>(record.m_Protocol)))},
        {InterfaceNameKey, CreateContainer(record.m_InterfaceName)},
        {DateTimeKey, CreateContainer(record.m_Time.getFormatTime("%h:%m:%s"))}
    };

    return CreateContainer(recordContainer);
}

IJsonContainerPtr AggrJsonSerializerVisitor::FromRecordInfo(AggregationVsftpd::VsftpdRecordInfo const &record) const
{
    std::map<std::string, IJsonContainerPtr> const recordContainer =
    {
        {ConnectionTypeKey, CreateContainer(std::to_string(static_cast<int>(record.m_InteractionType)))},
        {StatusTypeKey, CreateContainer(std::to_string(static_cast<int>(record.m_InteractionStatus)))},
        {FilePathKey, CreateContainer(record.m_filePath)},
        {DateTimeKey, CreateContainer(record.m_Time.getFormatTime("%h:%m:%s"))}
    };

    return CreateContainer(recordContainer);
}

IJsonContainerPtr AggrJsonSerializerVisitor::FromRecordInfo(AggregationSshd::SshdRecordInfo const &record) const
{
    std::map<std::string, IJsonContainerPtr> const recordContainer =
    {
        {ActionTypeKey, CreateContainer(std::to_string(static_cast<int>(record.m_ActType)))},
        {ActionStatusKey, CreateContainer(std::to_string(static_cast<int>(record.m_Status)))},
        {DestPortKey, CreateContainer(std::to_string(record.m_Port))},
        {DateTimeKey, CreateContainer(record.m_Time.getFormatTime("%h:%m:%s"))}
    };

    return CreateContainer(recordContainer);
}

IJsonContainerPtr AggrJsonSerializerVisitor::FromRecordInfo(AggregationApache::ApacheRecordInfo const &record) const
{
    std::map<std::string, IJsonContainerPtr> const recordContainer =
    {
        {RequestTypeKey, CreateContainer(std::to_string(static_cast<int>(record.m_ReqType)))},
        {RequestStatusKey, CreateContainer(std::to_string(static_cast<int>(record.m_ReqStatus)))},
        {ProtocolVersionKey, CreateContainer(std::to_string(static_cast<int>(record.m_ProtoVer)))},
        {FilePathKey, CreateContainer(record.m_ServerPath)},
        {FileSizeKey, CreateContainer(std::to_string(record.m_FileSize))},
        {DateTimeKey, CreateContainer(record.m_Time.getFormatTime("%Y:%h:%m:%s"))}
    };

    return CreateContainer(recordContainer);
}

IJsonContainerPtr AggrJsonSerializerVisitor::FromRecordInfo(AggregationPam::PamRecordInfo const &record) const
{
    std::map<std::string, IJsonContainerPtr> recordContainer =
    {
        {ProcessIdKey, CreateContainer(std::to_string(static_cast<int>(record.m_RecordDetails->m_Pid)))},
        {SessionTypeKey, CreateContainer(std::to_string(static_cast<int>(record.m_RecType)))}
    };

    if(record.m_RecType == AggregationPam::PamRecordType::SESSION_MANIPULATION)
    {
        AggregationPam::SessionRecord *pSession =
                static_cast<AggregationPam::SessionRecord*>(record.m_RecordDetails.get());

        const std::string sessionTypeStr = std::to_string(
                    static_cast<int>(pSession->m_SessionType));

        recordContainer.insert({SessionTypeKey, CreateContainer(sessionTypeStr)});
    }
    else if(record.m_RecType == AggregationPam::PamRecordType::FAILED)
    {
        AggregationPam::LoginFailed *pFailed =
                static_cast<AggregationPam::LoginFailed*>(record.m_RecordDetails.get());

        recordContainer.insert({RemoteHostKey, CreateContainer(pFailed->m_RemoteHost)});
        recordContainer.insert({RemoteUserKey, CreateContainer(pFailed->m_RemoteUser)});
        recordContainer.insert({TtyKey, CreateContainer(pFailed->m_TTY)});
    }


    return CreateContainer(recordContainer);
}

IAggrSerializerVisitorPtr aggregation_space::CreateVisitor(SerializeType const serializeKind)
{
    switch(serializeKind)
    {
    case SerializeType::serializeJSON:
        return std::make_shared<AggrJsonSerializerVisitor>();
    default:
        throw AggregationException("Cannot create serializer by serializer type",
                                   AggregationException::INVALID_GRABBER_SERIALIZER_TYPE);
    }
}
