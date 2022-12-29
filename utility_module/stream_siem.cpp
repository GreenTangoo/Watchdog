#include "stream_siem.hpp"

using namespace utility_space;
using namespace siem_ex_space;

// SIEMStreamImpl
SIEMStreamImpl::SIEMStreamImpl(size_t bufSize) : m_BufSize(bufSize)
{

}


// SIEMStreamMemory
SIEMStreamMemory::SIEMStreamMemory(size_t bufSize) :
    SIEMStreamImpl(bufSize)
{

}

ISIEMStream::StreamStatus SIEMStreamMemory::Read(char const *buf, size_t bufSize)
{
    const unsigned long long posAfterRead = GetPos() + bufSize;
    if (posAfterRead > GetStreamSize())
    {
        ExtendStream(posAfterRead - GetStreamSize());
    }

    unsigned long long curPos = GetPos();
    const size_t streamBufSize = GetBufSize();
    auto StreamIt = GetStreamIterByPos(curPos);

    // Try to align stream buffer.
    const size_t bytesInBufBlock = curPos % streamBufSize;
    const size_t notAlignedBytesCnt = streamBufSize - bytesInBufBlock;
    if(bufSize < notAlignedBytesCnt)
    {
        char *pBufBlock = *StreamIt;
        std::memcpy(pBufBlock + bytesInBufBlock, buf, bufSize);

        SetPos(curPos + bufSize);
        return StreamStatus::Good;
    }
    else
    {
        char *pBufBlock = *StreamIt;
        std::memcpy(pBufBlock + bytesInBufBlock, buf, notAlignedBytesCnt);

        SetPos(curPos + notAlignedBytesCnt);

        StreamIt++;
        bufSize -= notAlignedBytesCnt;
        buf += notAlignedBytesCnt;
    }

    // Read other.
    const size_t amountBlocks = bufSize / streamBufSize;
    curPos = GetPos();

    for(size_t i{0}; i < amountBlocks; i++)
    {
        char *pBufBlock = *StreamIt;
        std::memcpy(pBufBlock, buf, bufSize);

        StreamIt++;
        buf += bufSize;
        curPos += bufSize;
    }

    // Read remaining bytes.
    const size_t remainingBytes = bufSize % streamBufSize;
    char *pBufBlock = *StreamIt;

    std::memcpy(pBufBlock, buf, remainingBytes);
    curPos += remainingBytes;

    SetPos(curPos);

    return StreamStatus::Good;
}

ISIEMStream::StreamStatus SIEMStreamMemory::Write(char *buf, size_t bufSize)
{

}

ISIEMStream::StreamStatus SIEMStreamMemory::Get(char ch)
{
    const unsigned long long curPos = GetPos();
    if(curPos >= GetStreamSize())
    {
        ExtendStream(1);
    }

    auto StreamIt = GetStreamIterByPos(curPos);
    char *pBlockBuf = *StreamIt;

    const size_t bytesInBufBlock = curPos % GetBufSize();

    std::memcpy(pBlockBuf + bytesInBufBlock, &ch, 1);
    SetPos(curPos + 1);

    return StreamStatus::Good;
}

ISIEMStream::StreamStatus SIEMStreamMemory::Put(char &ch)
{
    const unsigned long long curPos = GetPos();
    if(curPos >= GetStreamSize())
        return StreamStatus::EndStream;

    auto StreamIt = GetStreamIterByPos(curPos);
    char *pBlockBuf = *StreamIt;

    const size_t bytesInBufBlock = curPos % GetBufSize();

    std::memcpy(&ch, pBlockBuf + bytesInBufBlock, 1);
    SetPos(curPos + 1);

    return StreamStatus::Good;
}

void SIEMStreamMemory::ExtendStream(size_t amountBytes)
{
    const size_t amountBlocks = amountBytes / GetBufSize() + 1;

    for (size_t i{0}; i < amountBlocks; i++)
        m_StreamData.push_back(new char[GetBufSize()]);

    SetStreamSize(GetStreamSize() + amountBytes);
}

std::list<char*>::iterator SIEMStreamMemory::GetStreamIterByPos(unsigned long long currPos)
{
    unsigned long long passedBytes = GetBufSize();

    for(std::list<char*>::iterator it = m_StreamData.begin(); it != m_StreamData.end(); it++)
    {
        if(currPos <= passedBytes)
            return it;

        passedBytes += GetBufSize();
    }

    throw StreamSIEMException("Memory stream position higher than stream data.",
                              StreamSIEMException::INVALID_STREAM_POSITION);
}
