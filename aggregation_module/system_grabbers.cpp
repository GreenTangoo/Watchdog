#include "system_grabbers.hpp"
#include "aggregation_serializer_visitor.hpp"

using namespace aggregation_space;

namespace
{
    template <class K, class V>
    V& CreateRecordInfoEx(K const &key, std::map<K, std::vector<V>> &container)
    {
        auto it = container.find(key);
        if(it != container.end())
        {
            auto &records = it->second;

            records.push_back(V());
            return records[records.size() - 1];
        }
        else
        {
            auto records = container.emplace(key, std::vector<V>());
            auto &recVec = records.first->second;

            recVec.reserve(10);
            recVec.push_back(V());

            return recVec[recVec.size() -1];
        }
    }
}

// /////////////////////////////////////////////////////////////////////////////
// AggregationPam
// Class for pam symptoms aggregation.
// /////////////////////////////////////////////////////////////////////////////

namespace
{
    const std::string PamProcessName1Exp = "(\\w+):\\spam_unix";
    const std::string PamProcessName2Exp = "(\\w+)[(\\[]";
    const std::string PamSessionManipulationExp = "session\\s(?:opened|closed)";
    const std::string PamAuthFailruleExp = "authentication\\sfailure";
    const std::string PamInvalidLoginAttemptExp = "check\\spass";
    const std::string PamUsernameExp = "user\\s(\\w+)";
    const std::string PamPidExp = "\\[(\\d+)\\]";
    const std::string PamSessionTypeExp = "session\\s(opened|closed)";
    const std::string PamRemoteHostExp = "rhost=(\\d+\\.\\d+\\.\\d+\\.\\d+)";
    const std::string PamRemoteUserExp = "ruser=(\\w+)";
    const std::string PamTtyExp = "tty=(\\d+)";
    const std::string PamTimeExp = "\\d+:\\d+:\\d+";
}


AggregationPam::AggregationPam(AggregationPamSettingsPtr const &settings) :
    GrabberBase(settings)
{

}

AggregationPam::AggregationPam(AggregationPam const &other) :
    GrabberBase(other)
{

}

AggregationPam::AggregationPam(AggregationPam &&other) :
    GrabberBase(std::move(other))
{

}

AggregationPam const& AggregationPam::operator=(AggregationPam const &other)
{
    if(this != &other)
    {
        GrabberBase::operator=(other);
    }

    return *this;
}

AggregationPam const& AggregationPam::operator=(AggregationPam &&other)
{
    if(this != &other)
    {
        GrabberBase::operator=(std::move(other));
    }

    return *this;
}

bool AggregationPam::StartAggregate()
{
    try
    {
        FileManipulator reader(GetSrcFile());

        std::string logLine;
        while(reader->GetStream().peek() != EOF)
        {
            reader->ReadLine(logLine);
            ParseString(logLine);
        }

        return true;
    }
    catch(FileManipulator::FilesystemSiemException const &ex)
    {
        SiemLogger &logger = SiemLogger::GetInstance();

        if(ex.GetErrno() == FileManipulator::FilesystemSiemException::INVALID_PATH)
        {
            logger.WriteLog("AggregationApache", "Cannot open log file: " + GetSrcFile(), 1);
        }
        else
        {
            logger.WriteLog("AggregationApache", "Unrecognized filesystem error", 1);
        }
    }

    return false;
}

void AggregationPam::Accept(IAggrSerializerVisitorPtr const &pVisitor)
{
    pVisitor->Visit(this);
}

void AggregationPam::ParseString(std::string const &logLine)
{
    try
    {
        PamRecordInfo& record = CreateRecordInfo(logLine);

        SetRecordType(record, logLine);

        ParseByRecordType(record, logLine);
    }
    catch(AggregationException const &ex)
    {
        SiemLogger &logger = SiemLogger::GetInstance();
        logger.WriteLog("AggregationPam", ex.what(), 1);
    }
}

AggregationPam::PamRecordInfo& AggregationPam::CreateRecordInfo(std::string const &logLine)
{
    bool isFoundProcName = false;
    std::string procName = FindByRegex(logLine, PamProcessName1Exp, 1);

    if(procName.size() > 0)
    {
        isFoundProcName = true;
    }
    else
    {
        procName = FindByRegex(logLine, PamProcessName2Exp, 1);

        if(procName.size() > 0)
        {
            isFoundProcName = true;
        }
    }

    if(isFoundProcName)
    {
        return CreateRecordInfoEx(procName, m_Records);
    }
    else
    {
        throw AggregationException("Cannot create pam record info: process name empty",
                                    AggregationException::CANNOT_CREATE_RECORD);
    }
}

void AggregationPam::ParseByRecordType(PamRecordInfo &record, std::string const &logLine)
{
    if(std::shared_ptr<PamRecordBase> pRecordDetails =
            CreateDetailsByRecordType(record.m_RecType))
    {
        SetUserName(*pRecordDetails, logLine);
        SetPid(*pRecordDetails, logLine);

        record.m_RecordDetails = pRecordDetails;

        DetailsLevel logLvl = GetAggregationDetailsLevel();

        if((logLvl == DetailsLevel::MEDIUM) || (logLvl == DetailsLevel::HIGH))
        {
            if(record.m_RecType == PamRecordType::SESSION_MANIPULATION)
            {
                SetSessionType(*pRecordDetails, logLine);
            }

            if(record.m_RecType == PamRecordType::FAILED)
            {
                SetRemoteHost(*pRecordDetails, logLine);
                SetRemoteUser(*pRecordDetails, logLine);
                SetTTY(*pRecordDetails, logLine);
            }

            if(logLvl == DetailsLevel::HIGH)
            {
                SetTime(record, logLine);
            }
        }
    }
    else
    {
        throw AggregationException("Cannot create pam record details by record type",
                                   AggregationException::CANNOT_CREATE_RECORD);
    }
}

std::shared_ptr<AggregationPam::PamRecordBase> AggregationPam::CreateDetailsByRecordType(PamRecordType const detailsType)
{
    switch (detailsType)
    {
    case PamRecordType::SESSION_MANIPULATION:
        return std::make_shared<SessionRecord>();
    case PamRecordType::FAILED:
        return std::make_shared<LoginFailed>();
    case PamRecordType::INVALID_LOGIN_ATTEMPT:
        return std::make_shared<PamRecordBase>();
    default:
        return nullptr;
    }
}

void AggregationPam::SetRecordType(PamRecordInfo &record, std::string const &logLine)
{
    for(;;)
    {
        std::string recordTypeStr = FindByRegex(logLine, PamSessionManipulationExp, 0);
        if(recordTypeStr.size() > 0)
        {
            record.m_RecType = PamRecordType::SESSION_MANIPULATION;
            break;
        }

        recordTypeStr = FindByRegex(logLine, PamAuthFailruleExp, 0);
        if(recordTypeStr.size() > 0)
        {
            record.m_RecType = PamRecordType::FAILED;
            break;
        }

        recordTypeStr = FindByRegex(logLine, PamInvalidLoginAttemptExp, 0);
        if(recordTypeStr.size() > 0)
        {
            record.m_RecType = PamRecordType::INVALID_LOGIN_ATTEMPT;
            break;
        }

        record.m_RecType = PamRecordType::NONE;
    }
}

void AggregationPam::SetUserName(PamRecordBase &baseRecord, std::string const &logLine)
{
    const std::string username = FindByRegex(logLine, PamUsernameExp, 1);

    if(username.size() > 0)
    {
        baseRecord.m_Username = username;
    }
}

void AggregationPam::SetPid(PamRecordBase &baseRecord, std::string const &logLine)
{
    const std::string pidStr = FindByRegex(logLine, PamPidExp, 1);

    if(pidStr.size() > 0)
    {
        baseRecord.m_Pid =
                static_cast<unsigned int>(std::atoi(pidStr.c_str()));
    }
}

void AggregationPam::SetSessionType(PamRecordBase &sessRecord, std::string const &logLine)
{
    SessionRecord *pSessRecord = static_cast<SessionRecord*>(&sessRecord);

    const std::string sessionTypeStr = FindByRegex(logLine, PamSessionTypeExp, 1);

    if(sessionTypeStr.size() > 0)
    {
        if(sessionTypeStr == "opened")
        {
            pSessRecord->m_SessionType = SessionRecord::SessionManipulationType::SESS_OPENED;
        }
        else if(sessionTypeStr == "closed")
        {
            pSessRecord->m_SessionType = SessionRecord::SessionManipulationType::SESS_CLOSED;
        }
        else
        {
            pSessRecord->m_SessionType = SessionRecord::SessionManipulationType::NONE;
        }
    }
    else
    {
        pSessRecord->m_SessionType = SessionRecord::SessionManipulationType::NONE;
    }
}

void AggregationPam::SetRemoteHost(PamRecordBase &failRecord, std::string const &logLine)
{
    LoginFailed *pFailRecord = static_cast<LoginFailed*>(&failRecord);
    const std::string remoteHostStr = FindByRegex(logLine, PamRemoteHostExp, 1);

    if(remoteHostStr.size() > 0)
    {
        pFailRecord->m_RemoteHost = remoteHostStr;
    }
}

void AggregationPam::SetRemoteUser(PamRecordBase &failRecord, std::string const &logLine)
{
    LoginFailed *pFailRecord = static_cast<LoginFailed*>(&failRecord);
    const std::string remoteUserStr = FindByRegex(logLine, PamRemoteUserExp, 1);

    if(remoteUserStr.size() > 0)
    {
        pFailRecord->m_RemoteUser = remoteUserStr;
    }
}

void AggregationPam::SetTTY(PamRecordBase &failRecord, std::string const &logLine)
{
    LoginFailed *pFailRecord = static_cast<LoginFailed*>(&failRecord);
    const std::string ttyStr = FindByRegex(logLine, PamTtyExp, 1);

    if(ttyStr.size() > 0)
    {
        pFailRecord->m_TTY = ttyStr;
    }
}

void AggregationPam::SetTime(PamRecordInfo &record, std::string const &logLine)
{
    const std::string timeStr = FindByRegex(logLine, PamTimeExp, 0);

    if(timeStr.size() > 0)
    {
        record.m_DateTime.putFormatTime(timeStr, "%h:%m:%s");
    }
}
