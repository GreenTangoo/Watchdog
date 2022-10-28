#ifndef SIEM_DEFINES
#define SIEM_DEFINES

#include <cstdlib>
#include <string>
#include <array>
#include <iostream>

#include "utility_module/json_proc.hpp"

namespace main_siem_space
{
    const size_t Ip4AddrLen = 4;
    const size_t MacAddrLen = 6;

    typedef std::array<unsigned char, Ip4AddrLen> Ip4Addr;
    typedef std::array<unsigned char, MacAddrLen> MacAddr;

    enum class TransportProto : size_t
    {
        UDP = 1,
        TCP = 2,
        ICMP = 3
    };

    const std::string g_SIEMConfigPath = "configs/options.json";

    class SiemLogger
    {
    public:
        SiemLogger(SiemLogger const &other) = delete;
        SiemLogger(SiemLogger &&other) = delete;
        SiemLogger const& operator=(SiemLogger const &other) = delete;
        SiemLogger const& operator=(SiemLogger &&other) = delete;
    public:
        static SiemLogger& GetInstance();
        static void WriteLog(std::string const &moduleName, std::string const &msg, const size_t detailsLevel);
    private:
        SiemLogger();
        ~SiemLogger() = default;
    };

    Ip4Addr StringIp2Bytes(std::string const &ipAddr);
    std::string Bytes2StringIp(Ip4Addr const &addr);

    MacAddr StringMac2Bytes(std::string const &macAddr);
    std::string Bytes2StringMac(MacAddr const &addr);

    unsigned char Symbol2Hex(unsigned char const symb);
    unsigned char Hex2Symbol(unsigned char const hexByte);
}

#endif // SIEM_DEFINES
