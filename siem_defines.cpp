#include "siem_defines.hpp"
#include "../utility_module/string_manager.hpp"

using namespace main_siem_space;
using namespace utility_space;

// //////////////////////////////////////////////////////////////////
// SiemLogger
// Class for write log according to details level
// //////////////////////////////////////////////////////////////////
static const std::string MainLogDetailsLvlKey = "log_details";

namespace
{
    size_t LogDetailsLevel;
    std::mutex WriteMut;
}

SiemLogger& SiemLogger::GetInstance()
{
    static SiemLogger object;
    return object;
}

void SiemLogger::WriteLog(std::string const &moduleName, std::string const &msg, size_t const detailsLevel)
{
    if(LogDetailsLevel >= detailsLevel)
    {
        std::lock_guard<std::mutex> logGuard(WriteMut);
        std::cout << "[SIEM LOG] " + moduleName + ": " + msg << std::endl;
    }
}

SiemLogger::SiemLogger()
{
    LogDetailsLevel = 0;

    JsonFileDeserializer reader(g_SIEMConfigPath);
    const IJsonContainerPtr pConfig = std::dynamic_pointer_cast<IJsonContainer>(reader.Read());

    const JsonObjectPtr pObjJson = std::dynamic_pointer_cast<JsonObject>(pConfig);
    JsonObject &objJson = *pObjJson;

    const JsonStringPtr pLogDetailsContainer =
            std::dynamic_pointer_cast<JsonString>(objJson.FindByName(MainLogDetailsLvlKey));

    if(pLogDetailsContainer)
    {
        const std::string logDetailsStr = static_cast<std::string>(*pLogDetailsContainer);

        LogDetailsLevel = static_cast<size_t>(std::atoi(logDetailsStr.c_str()));
    }
}

// ////////////////////////////////////////////////////////////////
// Public utility functions
// ////////////////////////////////////////////////////////////////
Ip4Addr main_siem_space::StringIp2Bytes(std::string const &ipAddr)
{
    Ip4Addr addrBytes;
    std::vector<std::string> ipNumbers = StringManager::parseByDelimiter(ipAddr, ".");

    for(size_t i(0); i < ipNumbers.size(); i++)
    {
        addrBytes[i] = std::atoi(ipNumbers[i].c_str());
    }

    return addrBytes;
}

std::string main_siem_space::Bytes2StringIp(Ip4Addr const &addr)
{
    std::string ipAddrStr;
    for(size_t i(0); i < addr.size(); i++)
    {
        int addrByte = static_cast<int>(addr[i]);
        ipAddrStr += std::to_string(addrByte);
        ipAddrStr.push_back('.');
    }

    ipAddrStr.pop_back();

    return ipAddrStr;
}

MacAddr main_siem_space::StringMac2Bytes(std::string const &macAddr)
{
    MacAddr addrBytes = {0};
    std::vector<std::string> macNumbers = StringManager::parseByDelimiter(macAddr, ":");

    size_t byteIndex = 0;
    for(size_t i(0); i < macNumbers.size(); i++)
    {
        if(byteIndex > MacAddrLen)
            break;

        char highByte = Symbol2Hex(macNumbers[i][0]);
        char lowByte = Symbol2Hex(macNumbers[i][1]);

        addrBytes[byteIndex] |= lowByte;
        addrBytes[byteIndex] |= (highByte << 4);

        byteIndex++;
    }

    return addrBytes;
}

std::string main_siem_space::Bytes2StringMac(MacAddr const &addr)
{
    std::string macAddrStr;

    for(char byte : addr)
    {
        char highByte = (byte & 0xF0) >> 4; // Get high part of byte and then shift
        char lowByte = byte & 0x0F;

        macAddrStr += Hex2Symbol(highByte);
        macAddrStr += Hex2Symbol(lowByte);
        macAddrStr += ':';
    }

    macAddrStr.pop_back(); // Delete last ':' symbol

    return macAddrStr;
}

unsigned char main_siem_space::Symbol2Hex(unsigned char const symb)
{
    char res = 0;

    // Translate symbol to hex
    // representation with ASCII table

    if((symb >= '0') && (symb <= '9'))
    {
        res = symb - 48;
    }
    else if((symb >= 'A') && (symb <= 'F'))
    {
        res = symb - 55;
    }
    else if((symb >= 'a') && (symb <= 'f'))
    {
        res = symb - 87;
    }

    return res;
}

unsigned char main_siem_space::Hex2Symbol(unsigned char const hexByte)
{
    char res = 0;

    if((hexByte >= 0) && (hexByte <= 9))
    {
        res = hexByte + 48;
    }
    else if((hexByte >= 10) && (hexByte <= 15))
    {
        res = hexByte + 55;
    }

    return res;
}
