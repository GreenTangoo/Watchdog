#ifndef SYSTEM_GRABBERS_HPP
#define SYSTEM_GRABBERS_HPP

#include "grabber_base.hpp"
#include "../utility_module/regex_siem.hpp"
#include "../utility_module/date_time.hpp"

namespace aggregation_space
{

    // /////////////////////////////////////////////////////////////////////////////
    // AggregationPam
    // Class for pam symptoms aggregation.
    //
    // Depending on the level of detail of the collection of logs,
    // the following information is collected:
    // 1) LOW - source addr, request type, request status.
    // 2) MEDIUM - LOW + server path, protocol version.
    // 3) HIGH - MEDIUM + file size, datetime.
    //
    // Other information collect depends on settings.
    // ////////////////////////////////////////////////////////////////////////////
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
        std::map<std::string, std::vector<PamRecordInfo>> const& GetData() const { return m_Records; }
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
}

#endif // SYSTEM_GRABBERS_HPP
