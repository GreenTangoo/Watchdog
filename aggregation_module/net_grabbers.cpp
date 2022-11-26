#include "net_grabbers.hpp"

using namespace aggregation_space;
using namespace utility_space;


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
static const std::string IpTablesSrcIpAddrExp = "SRC=(\\S+)";
static const std::string IpTablesDstIpAddrExp = "DST=(\\S+)";
static const std::string IpTablesMacAddrExp = "MAC=(\\S+)";
static const std::string IpTablesInterfaceExp = "IN=(\\S+)";
static const std::string IpTablesProtoExp = "PROTO=(\\S+)";
static const std::string IpTablesSrcPortExp = "SPT=(\\S+)";
static const std::string IpTablesDstPortExp = "DPT=(\\S+)";
static const std::string IpTablesTimeExp = "\\d{2}:\\d{2}:\\d{2}";

AggregationIpTables::AggregationIpTables(const AggregationIpTablesSettingsPtr &settings) :
    GrabberBase(settings)
{

}

AggregationIpTables::AggregationIpTables(AggregationIpTables const &other) :
    GrabberBase(other), m_Records(other.m_Records)
{

}

AggregationIpTables::AggregationIpTables(AggregationIpTables &&other) :
    GrabberBase(std::move(other)), m_Records(std::move(other.m_Records))
{

}

AggregationIpTables const& AggregationIpTables::operator=(AggregationIpTables const &other)
{
    if(this != &other)
    {
        GrabberBase::operator=(other);
        m_Records = other.m_Records;
    }

    return *this;
}

AggregationIpTables const& AggregationIpTables::operator=(AggregationIpTables &&other)
{
    if(this != &other)
    {
        GrabberBase::operator=(std::move(other));
        m_Records = std::move(other.m_Records);
    }

    return *this;
}

bool AggregationIpTables::StartAggregate()
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

        if(ex.getErrorCode() == FileManipulator::FilesystemSiemException::INVALID_PATH)
        {
            logger.WriteLog("AggregationIpTables", "Cannot open log file: " + GetSrcFile(), 1);
        }
        else
        {
            logger.WriteLog("AggregationIpTables", "Unrecognized filesystem error", 1);
        }
    }

    return false;
}

void AggregationIpTables::Accept(IAggrSerializerVisitorPtr const &pVisitor)
{
    pVisitor->Visit(this);
}

std::map<Ip4Addr, std::vector<AggregationIpTables::IpTablesRecordInfo>> const&  AggregationIpTables::GetData() const
{
    return m_Records;
}

void AggregationIpTables::ParseString(std::string const &logLine)
{
    try
    {
        IpTablesRecordInfo& record = CreateRecordInfo(logLine);

        SetDstAddr(record, logLine);
        SetMacAddrs(record, logLine);

        DetailsLevel logLvl = GetAggregationDetailsLevel();

        if((logLvl == DetailsLevel::MEDIUM) || (logLvl == DetailsLevel::HIGH))
        {
            SetInterface(record, logLine);
            SetProtocol(record, logLine);

            if(logLvl == DetailsLevel::HIGH)
            {
                SetPorts(record, logLine);
                SetTime(record, logLine);
            }
        }
    }
    catch(AggregationException const &ex)
    {
        SiemLogger &logger = SiemLogger::GetInstance();
        logger.WriteLog("AggregationIPtables", "Cannot create record info structure", 1);
    }
}

AggregationIpTables::IpTablesRecordInfo& AggregationIpTables::CreateRecordInfo(std::string const &logLine)
{
    const std::string srcIp = FindByRegex(logLine, IpTablesSrcIpAddrExp, 1);

    if(srcIp.length() > 0)
    {
        const Ip4Addr srcAddr = StringIp2Bytes(srcIp);

        return CreateRecordInfoEx(srcAddr, m_Records);
    }
    else
    {
        throw AggregationException("Cannot create iptables record info: source address empty",
                                   AggregationException::CANNOT_CREATE_RECORD);
    }
}

void AggregationIpTables::SetDstAddr(IpTablesRecordInfo &record, std::string const &logLine)
{
    const std::string dstIp = FindByRegex(logLine, IpTablesDstIpAddrExp, 1);

    if(dstIp.length() > 0)
    {
        record.m_DstIpAddr = StringIp2Bytes(dstIp);
    }
}

void AggregationIpTables::SetMacAddrs(IpTablesRecordInfo &record, std::string const &logLine)
{
    const std::string macAddrs = FindByRegex(logLine, IpTablesMacAddrExp, 1);

    const std::string dstMac = macAddrs.substr(0, 17);
    const std::string srcMac = macAddrs.substr(18, 17);

    if(dstMac.length() > 0)
    {
        record.m_DstMacAddr = StringMac2Bytes(dstMac);
    }

    if(srcMac.length() > 0)
    {
        record.m_SrcMacAddr = StringMac2Bytes(srcMac);
    }
}

void AggregationIpTables::SetInterface(IpTablesRecordInfo &record, std::string const &logLine)
{
    const std::string interface = FindByRegex(logLine, IpTablesInterfaceExp, 1);

    record.m_InterfaceName = interface;
}

void AggregationIpTables::SetProtocol(IpTablesRecordInfo &record, std::string const &logLine)
{
    const std::string protocolStr = FindByRegex(logLine, IpTablesProtoExp, 1);

    if(protocolStr.length() > 0)
    {
        size_t protocolNum = static_cast<size_t>(std::atoi(protocolStr.c_str()));
        record.m_Protocol = static_cast<TransportProto>(protocolNum);
    }
}

void AggregationIpTables::SetPorts(IpTablesRecordInfo &record, std::string const &logLine)
{
    const std::string srcPort = FindByRegex(logLine, IpTablesSrcPortExp, 1);
    const std::string dstPort = FindByRegex(logLine, IpTablesDstPortExp, 1);

    if(srcPort.length() > 0)
    {
        record.m_SrcPort = static_cast<size_t>(std::atoi(srcPort.c_str()));
    }

    if(dstPort.length() > 0)
    {
        record.m_DstPort = static_cast<size_t>(std::atoi(dstPort.c_str()));
    }
}

void AggregationIpTables::SetTime(IpTablesRecordInfo &record, std::string const &logLine)
{
    const std::string time = FindByRegex(logLine, IpTablesTimeExp, 0);

    if(time.length() > 0)
    {
        record.m_Time.putFormatTime(time, "%h:%m:%s");
    }
}


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
static const std::string VsftpdSrcAddrExp = "\\d+\\.\\d+\\.\\d+\\.\\d+";
static const std::string VsftpdInteractionTypeExp = "([A-Z]+):";
static const std::string VsftpdInteractionStatusExp = "\\[\\w+\\]\\s(\\w+)";
static const std::string VsftpdFilePathExp = "\\\"\\/*\\w+\\.*\\w*\\\"";
static const std::string VsftpdTimeExp = "\\d{2}:\\d{2}:\\d{2}";

static std::map<std::string, unsigned short> FtpStrToConnMap =
{
    {"CONNECT", 1},
    {"LOGIN", 2},
    {"DOWNLOAD", 3},
    {"UPLOAD", 4}
};


AggregationVsftpd::AggregationVsftpd(const AggregationVsftpdSettingsPtr &settings) :
    GrabberBase(settings)
{

}

AggregationVsftpd::AggregationVsftpd(AggregationVsftpd const &other) :
    GrabberBase(other), m_Records(other.m_Records)
{

}

AggregationVsftpd::AggregationVsftpd(AggregationVsftpd &&other) :
    GrabberBase(std::move(other)), m_Records(std::move(other.m_Records))
{

}

AggregationVsftpd const& AggregationVsftpd::operator=(AggregationVsftpd const &other)
{
    if(this != &other)
    {
        GrabberBase::operator=(other);
        m_Records = other.m_Records;
    }

    return *this;
}

AggregationVsftpd const& AggregationVsftpd::operator=(AggregationVsftpd &&other)
{
    if(this != &other)
    {
        GrabberBase::operator=(std::move(other));
        m_Records = std::move(other.m_Records);
    }

    return *this;
}

bool AggregationVsftpd::StartAggregate()
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
            logger.WriteLog("AggregationVsftpd", "Cannot open log file: " + GetSrcFile(), 1);
        }
        else
        {
            logger.WriteLog("AggregationVsftpd", "Unrecognized filesystem error", 1);
        }
    }

    return false;
}

void AggregationVsftpd::Accept(IAggrSerializerVisitorPtr const &pVisitor)
{
    pVisitor->Visit(this);
}

std::map<Ip4Addr, std::vector<AggregationVsftpd::VsftpdRecordInfo>> const& AggregationVsftpd::GetData() const
{
    return m_Records;
}

void AggregationVsftpd::ParseString(std::string const &logLine)
{
    try
    {
        VsftpdRecordInfo &record = CreateRecordInfo(logLine);

        SetInteractionType(record, logLine);

        DetailsLevel logLvl = GetAggregationDetailsLevel();

        if((logLvl == DetailsLevel::MEDIUM) || (logLvl == DetailsLevel::HIGH))
        {
            SetInteractionStatus(record, logLine);

            if(logLvl == DetailsLevel::HIGH)
            {
                SetFilePath(record, logLine);
                SetTime(record, logLine);
            }
        }
    }
    catch(AggregationException const &ex)
    {
        SiemLogger &logger = SiemLogger::GetInstance();
        logger.WriteLog("AggregationVsftpd", "Cannot create record info structure", 1);
    }

}

AggregationVsftpd::VsftpdRecordInfo& AggregationVsftpd::CreateRecordInfo(std::string const &logLine)
{
    const std::string srcIp = FindByRegex(logLine, VsftpdSrcAddrExp, 0);

    if(srcIp.length() > 0)
    {
        Ip4Addr srcAddr = StringIp2Bytes(srcIp);

        return CreateRecordInfoEx(srcAddr, m_Records);
    }
    else
    {
        throw AggregationException("Cannot create sshd record info: source address empty",
                                   AggregationException::CANNOT_CREATE_RECORD);
    }
}

void AggregationVsftpd::SetInteractionType(VsftpdRecordInfo &record, std::string const &logLine)
{
    const std::string connTypeStr = FindByRegex(logLine, VsftpdInteractionTypeExp, 1);

    if(connTypeStr.length() > 0)
    {
        try
        {
            const unsigned short connType = FtpStrToConnMap[connTypeStr];
            record.m_InteractionType = static_cast<ConnectionType>(connType);
        }
        catch(std::out_of_range const&)
        {
            SiemLogger &logger = SiemLogger::GetInstance();
            logger.WriteLog("AggregationVsftpd", "Unrecgonized interaction type", 1);
            logger.WriteLog("AggregationVsftpd", "In source line: " + logLine, 2);
        }
    }
    else
    {
        record.m_InteractionType = ConnectionType::NONE;
    }
}

void AggregationVsftpd::SetInteractionStatus(VsftpdRecordInfo &record, std::string const &logLine)
{
    const std::string interactionStatusStr = FindByRegex(logLine, VsftpdInteractionStatusExp, 1);

    if(interactionStatusStr == "OK")
    {
        record.m_InteractionStatus = StatusType::OK;
    }
    else if(interactionStatusStr == "FAIL")
    {
        record.m_InteractionStatus = StatusType::FAILED;
    }
    else
    {
        record.m_InteractionStatus = StatusType::NONE;
    }
}

void AggregationVsftpd::SetFilePath(VsftpdRecordInfo &record, std::string const &logLine)
{    
    record.m_filePath = FindByRegex(logLine, VsftpdFilePathExp, 0);
}

void AggregationVsftpd::SetTime(VsftpdRecordInfo &record, std::string const &logLine)
{
    const std::string time = FindByRegex(logLine, VsftpdTimeExp, 0);

    if(time.length() > 0)
    {
        record.m_Time.putFormatTime(time, "%h:%m:%s");
    }
}


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
static const std::string SshdSourceAddrExp = "from\\s([A-Za-z0-9\\.]+)\\s";
static const std::string SshdActionTypeExp = "publickey|password|privatekey";
static const std::string SshdActionStatusExp = "Accepted|Failed";
static const std::string SshdPortNumExp = "port\\s(\\d+)";
static const std::string SshdTimeExp = "\\d{2}:\\d{2}:\\d{2}";

static std::map<std::string, unsigned short> SshdStrToActStatusMap =
{
    {"password", 1},
    {"publickey", 2},
    {"privatekey", 3}
};


AggregationSshd::AggregationSshd(const AggregationSshdSettingsPtr &settings) :
    GrabberBase(settings)
{

}

AggregationSshd::AggregationSshd(AggregationSshd const &other) :
    GrabberBase(other), m_Records(other.m_Records)
{

}

AggregationSshd::AggregationSshd(AggregationSshd &&other) :
    GrabberBase(std::move(other)), m_Records(std::move(other.m_Records))
{

}

AggregationSshd const& AggregationSshd::operator=(AggregationSshd const &other)
{
    if(this != &other)
    {
        GrabberBase::operator=(other);
        m_Records = other.m_Records;
    }

    return *this;
}

AggregationSshd const& AggregationSshd::operator=(AggregationSshd &&other)
{
    if(this != &other)
    {
        GrabberBase::operator=(std::move(other));
        m_Records = std::move(other.m_Records);
    }

    return *this;
}

bool AggregationSshd::StartAggregate()
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
            logger.WriteLog("AggregationSshd", "Cannot open log file: " + GetSrcFile(), 1);
        }
        else
        {
            logger.WriteLog("AggregationSshd", "Unrecognized filesystem error", 1);
        }
    }

    return false;
}

void AggregationSshd::Accept(IAggrSerializerVisitorPtr const &pVisitor)
{
    pVisitor->Visit(this);
}

std::map<std::string, std::vector<AggregationSshd::SshdRecordInfo>> const& AggregationSshd::GetData() const
{
    return m_Records;
}

void AggregationSshd::ParseString(std::string const &logLine)
{
    try
    {
        SshdRecordInfo& record = CreateRecordInfo(logLine);

        SetActionType(record, logLine);

        DetailsLevel logLvl = GetAggregationDetailsLevel();

        if((logLvl == DetailsLevel::MEDIUM) || (logLvl == DetailsLevel::HIGH))
        {
            SetActionStatus(record, logLine);
            SetPortNum(record, logLine);

            if(logLvl == DetailsLevel::HIGH)
            {
                SetTime(record, logLine);
            }
        }
    }
    catch(AggregationException const &ex)
    {
        SiemLogger &logger = SiemLogger::GetInstance();
        logger.WriteLog("AggregationSshd", "Cannot create record info structure", 1);
    }
}

AggregationSshd::SshdRecordInfo& AggregationSshd::CreateRecordInfo(std::string const &logLine)
{
    const std::string sourceAddr = FindByRegex(logLine, SshdSourceAddrExp, 1);

    if(sourceAddr.length() > 0)
    {
        return CreateRecordInfoEx(sourceAddr, m_Records);
    }
    else
    {
        throw AggregationException("Cannot create sshd record info: source address empty",
                                   AggregationException::CANNOT_CREATE_RECORD);
    }
}

void AggregationSshd::SetActionType(SshdRecordInfo &record, std::string const &logLine)
{
    const std::string actionTypeStr = FindByRegex(logLine, SshdActionTypeExp, 0);

    if(actionTypeStr.length() > 0)
    {
        try
        {
            const unsigned int actionType = SshdStrToActStatusMap[actionTypeStr];
            record.m_ActType = static_cast<ActionType>(actionType);
        }
        catch(std::out_of_range const&)
        {
            SiemLogger &logger = SiemLogger::GetInstance();
            logger.WriteLog("AggregationSshd", "Unrecgonized action type", 1);
            logger.WriteLog("AggregationSshd", "In source line: " + logLine, 2);
        }
    }
    else
    {
        record.m_ActType = ActionType::NONE;
    }
}

void AggregationSshd::SetActionStatus(SshdRecordInfo &record, std::string const &logLine)
{
    const std::string actionStatusStr = FindByRegex(logLine, SshdActionStatusExp, 0);

    if(actionStatusStr == "Accepted")
    {
        record.m_Status = ActionStatus::ACCEPTED;
    }
    else if(actionStatusStr == "Failed")
    {
        record.m_Status = ActionStatus::FAILED;
    }
    else
    {
        record.m_Status = ActionStatus::NONE;
    }
}

void AggregationSshd::SetPortNum(SshdRecordInfo &record, std::string const &logLine)
{
    const std::string portNumStr = FindByRegex(logLine, SshdPortNumExp, 1);

    if(portNumStr.length() > 0)
    {
        record.m_Port = static_cast<unsigned short>(std::atoi(portNumStr.c_str()));
    }
}

void AggregationSshd::SetTime(SshdRecordInfo &record, std::string const &logLine)
{
    const std::string timeStr = FindByRegex(logLine, SshdTimeExp, 0);

    if(timeStr.length() > 0)
    {
        record.m_Time.putFormatTime(timeStr, "%h:%m:%s");
    }
}


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
static const std::string ApacheSourceAddrExp = "\\d+\\.\\d+\\.\\d+\\.\\d+";
static const std::string ApacheReqTypeExp = "]\\s\\\"(\\w+)";
static const std::string ApacheReqStatusExp = "\\\"\\s(\\d+)";
static const std::string ApacheServPathExp = "\\\"\\w+\\s(\\S+)";
static const std::string ApacheProtoVerExp = "\\s(\\S+)\\\"";
static const std::string ApacheFileSizeExp = "\\\"\\s\\d+\\s(\\d+)";
static const std::string ApacheTimeExp = "\\d+:\\d+:\\d+:\\d+";

static std::map<std::string, unsigned short> ApacheReqTypeMap =
{
    { "GET", 1 },
    { "POST", 2 }
};

static std::map<std::string, unsigned short> ApacheProtoVerMap =
{
    { "HTTP/1.0", 1 },
    { "HTTP/1.0" , 2 }
};

AggregationApache::AggregationApache(AggregationApacheSettingsPtr const &settings) :
    GrabberBase(settings)
{

}

AggregationApache::AggregationApache(AggregationApache const &other) :
    GrabberBase(other)
{

}

AggregationApache::AggregationApache(AggregationApache &&other) :
    GrabberBase(std::move(other))
{

}

AggregationApache const& AggregationApache::operator=(AggregationApache const &other)
{
    if(this != &other)
    {
        GrabberBase::operator=(other);
    }

    return *this;
}

AggregationApache const& AggregationApache::operator=(AggregationApache &&other)
{
    if(this != &other)
    {
        GrabberBase::operator=(std::move(other));
    }

    return *this;
}

bool AggregationApache::StartAggregate()
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

void AggregationApache::Accept(IAggrSerializerVisitorPtr const &pVisitor)
{
    pVisitor->Visit(this);
}

std::map<Ip4Addr, std::vector<AggregationApache::ApacheRecordInfo>> const& AggregationApache::GetData() const
{
    return m_Records;
}

void AggregationApache::ParseString(std::string const &logLine)
{
    try
    {
        ApacheRecordInfo& record = CreateRecordInfo(logLine);

        SetRequestType(record, logLine);
        SetRequestStatus(record, logLine);

        DetailsLevel logLvl = GetAggregationDetailsLevel();

        if((logLvl == DetailsLevel::MEDIUM) || (logLvl == DetailsLevel::HIGH))
        {
            SetServerPath(record, logLine);
            SetProtocolVersion(record, logLine);

            if(logLvl == DetailsLevel::HIGH)
            {
                SetFileSize(record, logLine);
                SetDateTime(record, logLine);
            }
        }
    }
    catch(AggregationException const &ex)
    {
        SiemLogger &logger = SiemLogger::GetInstance();
        logger.WriteLog("AggregationApache", "Cannot create record info structure", 1);
    }
}

AggregationApache::ApacheRecordInfo& AggregationApache::CreateRecordInfo(std::string const &logLine)
{
    const std::string sourceAddr = FindByRegex(logLine, ApacheSourceAddrExp, 0);

    if(sourceAddr.size() > 0)
    {
        const Ip4Addr srcAddrBytes = StringIp2Bytes(sourceAddr);

        return CreateRecordInfoEx(srcAddrBytes, m_Records);
    }
    else
    {
        throw AggregationException("Cannot create apache record info: source address empty",
                                   AggregationException::CANNOT_CREATE_RECORD);
    }
}

void AggregationApache::SetRequestType(ApacheRecordInfo &record, std::string const &logLine)
{
    const std::string reqTypeStr = FindByRegex(logLine, ApacheReqTypeExp, 1);

    if(reqTypeStr.size() > 0)
    {
        try
        {
            const unsigned short reqType = ApacheReqTypeMap[reqTypeStr];
            record.m_ReqType = static_cast<RequestType>(reqType);
        }
        catch(std::out_of_range const&)
        {
            SiemLogger &logger = SiemLogger::GetInstance();
            logger.WriteLog("AggregationApache", "Unrecgonized request type", 1);
            logger.WriteLog("AggregationApache", "In source line: " + logLine, 2);
        }
    }
    else
    {
        record.m_ReqType = RequestType::NONE;
    }
}

void AggregationApache::SetRequestStatus(ApacheRecordInfo &record, std::string const &logLine)
{
    const std::string reqStatusStr = FindByRegex(logLine, ApacheReqStatusExp, 1);

    if(reqStatusStr.size() > 0)
    {
        record.m_ReqStatus = static_cast<unsigned short>(std::atoi(reqStatusStr.c_str()));
    }
}

void AggregationApache::SetProtocolVersion(ApacheRecordInfo &record, std::string const &logLine)
{
    const std::string protoVersionStr = FindByRegex(logLine, ApacheProtoVerExp, 1);

    if(protoVersionStr.size() > 0)
    {
        try
        {
            const unsigned short protoVer = ApacheProtoVerMap[protoVersionStr];
            record.m_ProtoVer = static_cast<ProtocolVersion>(protoVer);
        }
        catch (std::out_of_range const&)
        {
            SiemLogger &logger = SiemLogger::GetInstance();
            logger.WriteLog("AggregationApache", "Unrecgonized protocol version", 1);
            logger.WriteLog("AggregationApache", "In source line: " + logLine, 2);
        }
    }
    else
    {
        record.m_ProtoVer = ProtocolVersion::NONE;
    }
}

void AggregationApache::SetServerPath(ApacheRecordInfo &record, std::string const &logLine)
{
    const std::string serverPathStr = FindByRegex(logLine, ApacheServPathExp, 1);

    if(serverPathStr.size() > 0)
    {
        record.m_ServerPath = serverPathStr;
    }
}

void AggregationApache::SetFileSize(ApacheRecordInfo &record, std::string const &logLine)
{
    const std::string fileSizeStr = FindByRegex(logLine, ApacheFileSizeExp, 1);

    if(fileSizeStr.size() > 0)
    {
        record.m_FileSize = static_cast<unsigned int>(std::atoi(fileSizeStr.c_str()));
    }
}

void AggregationApache::SetDateTime(ApacheRecordInfo &record, std::string const &logLine)
{
    const std::string dateTimeStr = FindByRegex(logLine, ApacheTimeExp, 0);

    if(dateTimeStr.size() > 0)
    {
        record.m_Time.putFormatTime(dateTimeStr, "%Y:%h:%m:%s");
    }
}


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
static const std::string PamProcessName1Exp = "(\\w+):\\spam_unix";
static const std::string PamProcessName2Exp = "(\\w+)[(\\[]";
static const std::string PamSessionManipulationExp = "session\\s(?:opened|closed)";
static const std::string PamAuthFailruleExp = "authentication\\sfailure";
static const std::string PamInvalidLoginAttemptExp = "check\\spass";
static const std::string PamUsernameExp = "user\\s(\\w+)";
static const std::string PamPidExp = "\\[(\\d+)\\]";
static const std::string PamSessionTypeExp = "session\\s(opened|closed)";
static const std::string PamRemoteHostExp = "rhost=(\\d+\\.\\d+\\.\\d+\\.\\d+)";
static const std::string PamRemoteUserExp = "ruser=(\\w+)";
static const std::string PamTtyExp = "tty=(\\d+)";




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

std::map<std::string, std::vector<AggregationPam::PamRecordInfo>> const& AggregationPam::GetData() const
{
    return m_Records;
}

void AggregationPam::ParseString(std::string const &logLine)
{
    try
    {
        PamRecordInfo& record = CreateRecordInfo(logLine);

        SetRecordType(record, logLine);

        if(std::shared_ptr<PamRecordBase> pRecordDetails =
                CreateDetailsByRecordType(record.m_RecType))
        {
            SetUserName(*pRecordDetails, logLine);
            SetPid(*pRecordDetails, logLine);

            record.m_RecordDetails = pRecordDetails;
        }
        else
        {
            throw AggregationException("Cannot create pam record details by record type", 3);
        }

        DetailsLevel logLvl = GetAggregationDetailsLevel();

        if((logLvl == DetailsLevel::MEDIUM) || (logLvl == DetailsLevel::HIGH))
        {

            if(logLvl == DetailsLevel::HIGH)
            {
            }
        }
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
            record.m_RecType = PamRecordType::SESSION_MANIPULATION;
            break;
        }

        recordTypeStr = FindByRegex(logLine, PamInvalidLoginAttemptExp, 0);
        if(recordTypeStr.size() > 0)
        {
            record.m_RecType = PamRecordType::SESSION_MANIPULATION;
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

}

void AggregationPam::SetRemoteHost(PamRecordBase &failRecord, std::string const &logLine)
{

}

void AggregationPam::SetRemoteUser(PamRecordBase &failRecord, std::string const &logLine)
{

}

void AggregationPam::SetTTY(PamRecordBase &failRecord, std::string const &logLine)
{

}


// /////////////////////////////////////////////////////////////////////////////
// AggrJsonSerializerVisitor
// Class for serialize grabbers to JSON file.
// ////////////////////////////////////////////////////////////////////////////
static const std::string DestIpKey = "destination_ip";
static const std::string SrcMacKey = "source_mac";
static const std::string DestMacKey = "destination_mac";
static const std::string SrcPortKey = "source_port";
static const std::string DestPortKey = "destination_port";
static const std::string InetProtoKey = "internet_proto";
static const std::string InterfaceNameKey = "interface";
static const std::string DateTimeKey = "datetime";

static const std::string ConnectionTypeKey = "connection_type";
static const std::string StatusTypeKey = "status_type";
static const std::string FilePathKey = "filepath";

static const std::string ActionTypeKey = "action_type";
static const std::string ActionStatusKey = "action_status";

static const std::string RequestTypeKey = "request_type";
static const std::string RequestStatusKey = "request_status";
static const std::string ProtocolVersionKey = "protocol_version";
static const std::string FileSizeKey = "filesize";


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

}

IAggrSerializerVisitorPtr aggregation_space::CreateVisitor(SerializeType const serializeKind)
{
    switch(serializeKind)
    {
    case SerializeType::serializeJSON:
        return std::make_shared<AggrJsonSerializerVisitor>();
    }
}
