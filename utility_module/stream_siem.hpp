#ifndef STREAM_SIEM_HPP
#define STREAM_SIEM_HPP

#include <memory>
#include <cstring>
#include <list>

#include "../exception_module/exceptions.hpp"

namespace utility_space
{
    // ISIEMStream
    // Base class for SIEM streams
    class ISIEMStream
    {
    public:
        enum class StreamStatus : int
        {
            None = 0,
            Good = 1,
            Bad = 2,
            EndStream = 3
        };
    public:
        ISIEMStream() = default;
        virtual ~ISIEMStream() = default;
        ISIEMStream(ISIEMStream const &other) = delete;
        ISIEMStream(ISIEMStream && other) = delete;
        ISIEMStream& operator=(ISIEMStream const &other) = delete;
        ISIEMStream& operator=(ISIEMStream &&other) = delete;
    public:
        virtual StreamStatus Read(char const *buf, size_t bufSize) = 0;
        virtual StreamStatus Write(char *buf, size_t bufSize) = 0;
        virtual StreamStatus Get(char ch) = 0;
        virtual StreamStatus Put(char &ch) = 0;
    };
    typedef std::shared_ptr<ISIEMStream> ISIEMStreamPtr;


    // SIEMStreamImpl
    // Base implementation of ISIEMStream. Should not to use.
    class SIEMStreamImpl : public ISIEMStream
    {
    public:
        SIEMStreamImpl() = default;
        SIEMStreamImpl(size_t bufSize);
        virtual ~SIEMStreamImpl() = default;
        SIEMStreamImpl(SIEMStreamImpl const &other) = delete;
        SIEMStreamImpl(SIEMStreamImpl && other) = delete;
        SIEMStreamImpl& operator=(SIEMStreamImpl const &other) = delete;
        SIEMStreamImpl& operator=(SIEMStreamImpl &&other) = delete;
    public:
        size_t GetBufSize() const { return m_BufSize; }
        unsigned long long GetStreamSize() const { return m_StreamSize; }
        unsigned long long GetPos() const { return m_StreamPos; }
        void SetPos(unsigned long long newPos) { m_StreamPos = newPos; }
    protected:
        void SetStreamSize(unsigned long long newSize) { m_StreamSize = newSize; }
    private:
        const size_t m_BufSize = 1024;
        unsigned long long m_StreamSize = 0;
        unsigned long long m_StreamPos = 0;
        StreamStatus m_LastStatus = StreamStatus::None;
    };
    typedef std::shared_ptr<SIEMStreamImpl> SIEMStreamImplPtr;


    // SIEMStreamMemory
    // Class implementation stream in heap.
    // Stream consists of list of buffers with m_BufSize size each one.
    // For optimization purposes should to read/write a large data
    //  with minimum calls. Multiple read/write will call
    //  ExtendStream method many times.
    class SIEMStreamMemory : public SIEMStreamImpl
    {
    public:
        SIEMStreamMemory() = default;
        SIEMStreamMemory(size_t bufSize);
        virtual ~SIEMStreamMemory() = default;
        SIEMStreamMemory(SIEMStreamMemory const &other) = delete;
        SIEMStreamMemory(SIEMStreamMemory && other) = delete;
        SIEMStreamMemory& operator=(SIEMStreamMemory const &other) = delete;
        SIEMStreamMemory& operator=(SIEMStreamMemory &&other) = delete;
    public:
        virtual StreamStatus Read(char const *buf, size_t bufSize) override;
        virtual StreamStatus Write(char *buf, size_t bufSize) override;
        virtual StreamStatus Get(char ch) override;
        virtual StreamStatus Put(char &ch) override;
    private:
        void ExtendStream(size_t amountBytes);
        std::list<char*>::iterator GetStreamIterByPos(unsigned long long  currPos);
    private:
        std::list<char*> m_StreamData;
    };

}

#endif // STREAM_SIEM_HPP
