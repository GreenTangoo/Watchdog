#ifndef GRABBERS_HPP
#define GRABBERS_HPP

#include "grabber_base.hpp"
#include "../utility_module/regex_siem.hpp"
#include "../utility_module/date_time.hpp"

namespace aggregation_space
{
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
        inline std::map<Ip4Addr, std::vector<IpTablesRecordInfo>> const&  GetData() const;
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
        inline std::map<Ip4Addr, std::vector<VsftpdRecordInfo>> const& GetData() const;
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
        inline std::map<std::string, std::vector<SshdRecordInfo>> const& GetData() const;
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
        inline std::map<Ip4Addr, std::vector<ApacheRecordInfo>> const& GetData() const;
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


    class AggregationPam : public GrabberBase
    {
    public:
        explicit AggregationPam(const AggregationPamSettingsPtr &settings);
        explicit AggregationPam(AggregationPam const &other);
        explicit AggregationPam(AggregationPam &&other);
        ~AggregationPam() = default;
        AggregationPam const& operator=(AggregationPam const &other);
        AggregationPam const& operator=(AggregationPam &&other);
    public:
        bool StartAggregate() override;
        void Accept(IAggrSerializerVisitorPtr const &pVisitor) override;
    public:
        enum class PamRecordType : unsigned short
        {
            NONE = 0,
            SESSION_MANIPULATION = 1, // Session open/close
            FAILED = 2,               // Authentication failrule
            INVALID_LOGIN_ATTEMPT = 3 // Invalid user login attempt
        };

        struct PamRecordBase
        {
            std::string m_Username;
            unsigned int m_Pid;
        };

        struct SessionRecord : public PamRecordBase
        {
            enum class SessionManipulationType : unsigned short
            {
                NONE = 0,
                SESS_OPENED = 1,
                SESS_CLOSED = 2
            };

            SessionManipulationType m_SessionType;
        };

        struct LoginFailed : public PamRecordBase
        {
            std::string m_RemoteHost;
            std::string m_RemoteUser;
            std::string m_TTY;
        };

        struct PamRecordInfo
        {
            PamRecordType m_RecType;
            std::shared_ptr<PamRecordBase> m_RecordDetails;
            DateTime m_DateTime;
        };
    public:
        inline std::map<std::string, std::vector<PamRecordInfo>> const& GetData() const;
    private:
        void ParseString(std::string const &logLine);
        PamRecordInfo& CreateRecordInfo(std::string const &logLine);
        std::shared_ptr<PamRecordBase> CreateDetailsByRecordType(PamRecordType const detailsType);
        void SetRecordType(PamRecordInfo &record, std::string const &logLine);
        void SetUserName(PamRecordBase &baseRecord, std::string const &logLine);
        void SetPid(PamRecordBase &baseRecord, std::string const &logLine);
        void SetSessionType(PamRecordBase &sessRecord, std::string const &logLine);
        void SetRemoteHost(PamRecordBase &failRecord, std::string const &logLine);
        void SetRemoteUser(PamRecordBase &failRecord, std::string const &logLine);
        void SetTTY(PamRecordBase &failRecord, std::string const &logLine);
    private:
        std::map<std::string, std::vector<PamRecordInfo>> m_Records;
    };
    typedef std::shared_ptr<AggregationPam> AggregationPamPtr;


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

#endif // GRABBERS_HPP
