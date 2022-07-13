#ifndef GRABBER_BASE_HPP
#define GRABBER_BASE_HPP

#include <string>

#include "../utility_module/filesystem_siem.hpp"

namespace aggregation_space
{
    enum AggrGrabber {aggrIPtables = 0, aggrFTPserver, aggrSSHserver, aggrEND_ENUM_THRESHOLD};

    class IGrabber
    {
    public:
        IGrabber() = default;
        ~IGrabber() = default;
        virtual void StartAggregate() = 0;
        void OpenSourceFile(std::string const &filepath);
    protected:
        std::shared_ptr<utility_space::FileManipulator> m_LogFile;
    };

    typedef std::shared_ptr<IGrabber> IGrabberPtr;
}

#endif // GRABBER_BASE_HPP
