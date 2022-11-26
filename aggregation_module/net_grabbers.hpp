#ifndef NET_GRABBERS_HPP
#define NET_GRABBERS_HPP

#include "grabber_base.hpp"
#include "../utility_module/regex_siem.hpp"
#include "../utility_module/date_time.hpp"

namespace aggregation_space
{
    // /////////////////////////////////////////////////////////////////////////////
    // AggregationIPtables
    // Class for IpTables symptoms aggregation.
    //
    // Depending on the level of detail of the collection of logs,
    // the following information is collected:
    // 1) LOW - source and destination ip address, src and dest mac address, datetime.
    // 2) MEDIUM - LOW + interface name, protocol type.
    // 3) HIGH - MEDIUM + source and destination ports.
    //
    // Other information collect depends on settings.
    // ////////////////////////////////////////////////////////////////////////////
    class AggregationIpTables : public GrabberBase
    {
    public:
        explicit AggregationIpTables(AggregationIpTablesSettingsPtr const &settings);
        explicit AggregationIpTables(AggregationIpTables const &other);
        explicit AggregationIpTables(AggregationIpTables &&other);
        ~AggregationIpTables() = default;
        AggregationIpTables const& operator=(AggregationIpTables const &other);
        AggregationIpTables const& operator=(AggregationIpTables &&other);
    public:
        bool StartAggregate() override;
        void Accept(IAggrSerializerVisitorPtr const &pVisitor) override;
    public:
        struct IpTablesRecordInfo
        {
            Ip4Addr m_DstIpAddr;
            MacAddr m_SrcMacAddr;
            MacAddr m_DstMacAddr;
            unsigned short m_SrcPort;
            unsigned short m_DstPort;
            TransportProto m_Protocol;
            std::string m_InterfaceName;
            DateTime m_Time;
        };
    public:
        std::map<Ip4Addr, std::vector<IpTablesRecordInfo>> const&  GetData() const { return m_Records; }
    private:
        void ParseString(std::string const &logLine);
        IpTablesRecordInfo& CreateRecordInfo(std::string const &logLine);
        void SetDstAddr(IpTablesRecordInfo &record, std::string const &logLine);
        void SetMacAddrs(IpTablesRecordInfo &record, std::string const &logLine);
        void SetInterface(IpTablesRecordInfo &record, std::string const &logLine);
        void SetProtocol(IpTablesRecordInfo &record, std::string const &logLine);
        void SetPorts(IpTablesRecordInfo &record, std::string const &logLine);
        void SetTime(IpTablesRecordInfo &record, std::string const &logLine);
    private:
        std::map<Ip4Addr, std::vector<IpTablesRecordInfo>> m_Records;
    };
    typedef std::shared_ptr<AggregationIpTables> AggregationIPtablesPtr;



    // /////////////////////////////////////////////////////////////////////////////
    // AggregationVsftpd
    // Class for vsftpd symptoms aggregation.
    //
    // Depending on the level of detail of the collection of logs,
    // the following information is collected:
    // 1) LOW - source addr, connection type.
    // 2) MEDIUM - LOW + interaction status
    // 3) HIGH - MEDIUM + file path, datetime.
    //
    // Other information collect depends on settings.
    // ////////////////////////////////////////////////////////////////////////////
    class AggregationVsftpd : public GrabberBase
    {
    public:
        explicit AggregationVsftpd(const AggregationVsftpdSettingsPtr &settings);
        explicit AggregationVsftpd(AggregationVsftpd const &other);
        explicit AggregationVsftpd(AggregationVsftpd &&other);
        ~AggregationVsftpd() = default;
        AggregationVsftpd const& operator=(AggregationVsftpd const &other);
        AggregationVsftpd const& operator=(AggregationVsftpd &&other);
    public:
        bool StartAggregate() override;
        void Accept(IAggrSerializerVisitorPtr const &pVisitor) override;
    public:
        enum class ConnectionType : unsigned short
        {
            NONE = 0,
            CONNECTION = 1,
            LOGIN = 2,
            FILE_DOWNLOAD = 3,
            FILE_UPLOAD = 4
        };

        enum class StatusType : unsigned short
        {
            NONE = 0,
            OK = 1,
            FAILED = 2
        };

        struct VsftpdRecordInfo
        {
            ConnectionType m_InteractionType;
            StatusType m_InteractionStatus;
            std::string m_filePath;
            DateTime m_Time;
        };
    public:
        std::map<Ip4Addr, std::vector<VsftpdRecordInfo>> const& GetData() const { return m_Records; }
    private:
        void ParseString(std::string const &logLine);
        VsftpdRecordInfo& CreateRecordInfo(std::string const &logLine);
        void SetInteractionType(VsftpdRecordInfo &record, std::string const &logLine);
        void SetInteractionStatus(VsftpdRecordInfo &record, std::string const &logLine);
        void SetFilePath(VsftpdRecordInfo &record, std::string const &logLine);
        void SetTime(VsftpdRecordInfo &record, std::string const &logLine);
    private:
        std::map<Ip4Addr, std::vector<VsftpdRecordInfo>> m_Records;
    };
    typedef std::shared_ptr<AggregationVsftpd> AggregationVsftpdPtr;



    // /////////////////////////////////////////////////////////////////////////////
    // AggregationSshd
    // Class for sshd symptoms aggregation.
    //
    // Depending on the level of detail of the collection of logs,
    // the following information is collected:
    // 1) LOW - source addr, action type.
    // 2) MEDIUM - LOW + action status, port.
    // 3) HIGH - MEDIUM + datetime.
    //
    // Other information collect depends on settings.
    // ////////////////////////////////////////////////////////////////////////////
    class AggregationSshd : public GrabberBase
    {
    public:
        explicit AggregationSshd(AggregationSshdSettingsPtr const &settings);
        explicit AggregationSshd(AggregationSshd const &other);
        explicit AggregationSshd(AggregationSshd &&other);
        ~AggregationSshd() = default;
        AggregationSshd const& operator=(AggregationSshd const &other);
        AggregationSshd const& operator=(AggregationSshd &&other);
    public:
        bool StartAggregate() override;
        void Accept(IAggrSerializerVisitorPtr const &pVisitor) override;
    public:
        enum class ActionType : unsigned short
        {
            NONE = 0,
            PASSWORD = 1,
            PUBLIC_KEY = 2,
            PRIVATE_KEY = 3
        };

        enum class ActionStatus : unsigned short
        {
            NONE = 0,
            ACCEPTED = 1,
            FAILED = 2
        };

        struct SshdRecordInfo
        {
            ActionType m_ActType;
            ActionStatus m_Status;
            unsigned short m_Port;
            DateTime m_Time;
        };
    public:
        std::map<std::string, std::vector<SshdRecordInfo>> const& GetData() const { return m_Records; }
    private:
        void ParseString(std::string const &logLine);
        SshdRecordInfo& CreateRecordInfo(std::string const &logLine);
        void SetActionType(SshdRecordInfo &record, std::string const &logLine);
        void SetActionStatus(SshdRecordInfo &record, std::string const &logLine);
        void SetPortNum(SshdRecordInfo &record, std::string const &logLine);
        void SetTime(SshdRecordInfo &record, std::string const &logLine);
    private:
        std::map<std::string, std::vector<SshdRecordInfo>> m_Records;
    };
    typedef std::shared_ptr<AggregationSshd> AggregationSshdPtr;



    // /////////////////////////////////////////////////////////////////////////////
    // AggregationApache
    // Class for apache symptoms aggregation.
    //
    // Depending on the level of detail of the collection of logs,
    // the following information is collected:
    // 1) LOW - source addr, request type, request status.
    // 2) MEDIUM - LOW + server path, protocol version.
    // 3) HIGH - MEDIUM + file size, datetime.
    //
    // Other information collect depends on settings.
    // ////////////////////////////////////////////////////////////////////////////
    class AggregationApache : public GrabberBase
    {
    public:
        explicit AggregationApache(AggregationApacheSettingsPtr const &settings);
        explicit AggregationApache(AggregationApache const &other);
        explicit AggregationApache(AggregationApache &&other);
        ~AggregationApache() = default;
        AggregationApache const& operator=(AggregationApache const &other);
        AggregationApache const& operator=(AggregationApache &&other);
    public:
        bool StartAggregate() override;
        void Accept(IAggrSerializerVisitorPtr const &pVisitor) override;
    public:
        enum class RequestType : unsigned short
        {
            NONE = 0,
            GET = 1,
            POST = 2
        };

        enum class ProtocolVersion : unsigned short
        {
            NONE = 0,
            HTTP_1_0 = 1,
            HTTP_1_1 = 2
        };

        struct ApacheRecordInfo
        {
            RequestType m_ReqType;
            ProtocolVersion m_ProtoVer;
            unsigned short m_ReqStatus;
            std::string m_ServerPath;
            unsigned int m_FileSize;
            DateTime m_Time;
        };
    public:
        std::map<Ip4Addr, std::vector<ApacheRecordInfo>> const& GetData() const { return m_Records; }
    private:
        void ParseString(std::string const &logLine);
        ApacheRecordInfo& CreateRecordInfo(std::string const &logLine);
        void SetRequestType(ApacheRecordInfo &record, std::string const &logLine);
        void SetRequestStatus(ApacheRecordInfo &record, std::string const &logLine);
        void SetProtocolVersion(ApacheRecordInfo &record, std::string const &logLine);
        void SetServerPath(ApacheRecordInfo &record, std::string const &logLine);
        void SetFileSize(ApacheRecordInfo &record, std::string const &logLine);
        void SetDateTime(ApacheRecordInfo &record, std::string const &logLine);
    private:
        std::map<Ip4Addr, std::vector<ApacheRecordInfo>> m_Records;
    };
    typedef std::shared_ptr<AggregationApache> AggregationApachePtr;
}

#endif // GRABBERS_HPP
