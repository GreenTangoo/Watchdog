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
    const unsigned long long posAfterRead = GetPosR() + bufSize;
    if (posAfterRead > GetStreamSize())
    {
        ExtendStream(posAfterRead - GetStreamSize());
    }

    // Try to align stream buffer.
    buf = AlignBuffer(const_cast<char*>(buf), bufSize, BufInteractType::ReadFromBuffer);
    if(!buf)
    {
        return StreamStatus::Good;
    }

    // Read other.
    const size_t streamBufSize = GetBufSize();
    const size_t amountBlocks = bufSize / streamBufSize;

    buf = CopyBufferToRead(buf, amountBlocks);

    // Read remaining bytes.
    const size_t remainingBytes = bufSize % streamBufSize;
    auto streamIt = GetStreamIterByPos(GetPosR());
    char *pBufBlock = *streamIt;

    std::memcpy(pBufBlock, buf, remainingBytes);
    SetPosR(GetPosR() + remainingBytes);

    return StreamStatus::Good;
}

ISIEMStream::StreamStatus SIEMStreamMemory::Write(char *buf, size_t bufSize)
{
    unsigned long long curPos = GetPosW();
    const unsigned long long streamSize = GetStreamSize();
    if(curPos + bufSize > streamSize)
    {
        bufSize = streamSize - curPos;
    }

    // Try align stream to write(changed bufSize).
    buf = AlignBuffer(buf, bufSize, BufInteractType::WriteToBuffer);
    if(!buf)
    {
        return StreamStatus::Good;
    }

    // Write other.
    const size_t streamBufSize = GetBufSize();
    const size_t amountBlocks = bufSize / streamBufSize;

    buf = CopyBufferToWrite(buf, amountBlocks);

    // Write remaining bytes.
    const size_t remainingBytes = bufSize % streamBufSize;
    auto streamIt = GetStreamIterByPos(GetPosW());
    char *pBufBlock = *streamIt;

    std::memcpy(buf, pBufBlock, remainingBytes);
    SetPosW(GetPosW() + remainingBytes);

    return StreamStatus::Good;
}

ISIEMStream::StreamStatus SIEMStreamMemory::Get(char ch)
{
    const unsigned long long curPos = GetPosR();
    if(curPos >= GetStreamSize())
    {
        ExtendStream(1);
    }

    auto StreamIt = GetStreamIterByPos(curPos);
    char *pBlockBuf = *StreamIt;

    const size_t bytesInBufBlock = curPos % GetBufSize();

    std::memcpy(pBlockBuf + bytesInBufBlock, &ch, 1);
    SetPosR(curPos + 1);

    return StreamStatus::Good;
}

ISIEMStream::StreamStatus SIEMStreamMemory::Put(char &ch)
{
    const unsigned long long curPos = GetPosW();
    if(curPos >= GetStreamSize())
        return StreamStatus::EndStream;

    auto StreamIt = GetStreamIterByPos(curPos);
    char *pBlockBuf = *StreamIt;

    const size_t bytesInBufBlock = curPos % GetBufSize();

    std::memcpy(&ch, pBlockBuf + bytesInBufBlock, 1);
    SetPosW(curPos + 1);

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
    unsigned long long filledBytes = GetBufSize();

    for(std::list<char*>::iterator it = m_StreamData.begin(); it != m_StreamData.end(); it++)
    {
        if(currPos < filledBytes)
            return it;

        filledBytes += GetBufSize();
    }

    throw StreamSIEMException("Memory stream position higher than stream data.",
                              StreamSIEMException::INVALID_STREAM_POSITION);
}

char* SIEMStreamMemory::CopyBufferToWrite(char *pDest, size_t amountBlocks)
{
    return CopyBuffer(pDest, amountBlocks, BufInteractType::WriteToBuffer);
}

char const* SIEMStreamMemory::CopyBufferToRead(char const *pSrc, size_t amountBlocks)
{
    return CopyBuffer(const_cast<char*>(pSrc), amountBlocks, BufInteractType::ReadFromBuffer);
}

char* SIEMStreamMemory::AlignBuffer(char *buf, size_t &bufSize, BufInteractType interaction)
{
    const auto CopyBufferLocal = [this](char *buf, size_t offset, size_t amountBytes, BufInteractType interaction)
    {
        const unsigned long long curPos = GetPosW();
        auto streamIt = GetStreamIterByPos(curPos);
        char *pBufBlock = *streamIt;

        if(interaction == BufInteractType::ReadFromBuffer)
        {
            std::memcpy(pBufBlock + offset, buf, amountBytes);
        }
        else
        {
            std::memcpy(buf, pBufBlock + offset, amountBytes);
        }

        SetPosW(curPos + amountBytes);
    };

    const unsigned long long curPos = GetPosW();
    const size_t streamBufSize = GetBufSize();
    const size_t bytesInBufBlock = curPos % streamBufSize;
    const size_t notAlignedBytesCnt = streamBufSize - bytesInBufBlock;

    if(bufSize < notAlignedBytesCnt)
    {
        CopyBufferLocal(buf, bytesInBufBlock, bufSize, interaction);
        return nullptr;
    }
    else
    {
        CopyBufferLocal(buf, bytesInBufBlock, notAlignedBytesCnt, interaction);

        bufSize -= notAlignedBytesCnt;
        buf += notAlignedBytesCnt;
    }

    return buf;
}

char* SIEMStreamMemory::CopyBuffer(char *buf, size_t amountBlocks, BufInteractType interaction)
{
    const size_t streamBufSize = GetBufSize();
    unsigned long long curPos = GetPosW();
    auto streamIt = GetStreamIterByPos(curPos);

    for(size_t i{0}; i < amountBlocks; i++)
    {
        char *pBufBlock = *streamIt;

        if(interaction == BufInteractType::ReadFromBuffer)
        {
            std::memcpy(pBufBlock, buf, streamBufSize);
        }
        else
        {
            std::memcpy(buf, pBufBlock, streamBufSize);
        }

        streamIt++;
        buf += streamBufSize;
        curPos += streamBufSize;
    }

    SetPosW(curPos);

    return buf;
}



// SIEMStreamFilesystem
SIEMStreamFilesystem::SIEMStreamFilesystem(size_t bufSize) :
    SIEMStreamImpl(bufSize)
{

}


ISIEMStream::StreamStatus SIEMStreamFilesystem::Read(char const *buf, size_t bufSize)
{
    while(true)
    {
        m_FileStream.write(buf, bufSize);
        unsigned long long writedBytes = m_FileStream.gcount();

        if(writedBytes == 0)
            break;

        buf += writedBytes;
        bufSize -= writedBytes;
    }

    if(bufSize != 0)
        return StreamStatus::EndStream;
    else
        return StreamStatus::Good;
}

ISIEMStream::StreamStatus SIEMStreamFilesystem::Write(char *buf, size_t bufSize)
{
    while(true)
    {
        m_FileStream.read(buf, bufSize);
        unsigned long long readedBytes = m_FileStream.gcount();

        if(readedBytes == 0)
            break;

        buf += readedBytes;
        bufSize -= readedBytes;
    }

    if(bufSize != 0)
        return StreamStatus::EndStream;
    else
        return StreamStatus::Good;
}

ISIEMStream::StreamStatus SIEMStreamFilesystem::Get(char ch)
{
    m_FileStream.put(ch);
    return StreamStatus::Good;
}

ISIEMStream::StreamStatus SIEMStreamFilesystem::Put(char &ch)
{
    m_FileStream.get(ch);
    return StreamStatus::Good;
}

unsigned long long SIEMStreamFilesystem::GetStreamSize() const
{
    unsigned long long curPos = m_FileStream.tellg();

    m_FileStream.seekg(0, std::ios::beg);
    unsigned long long startPos = m_FileStream.tellg();

    m_FileStream.seekg(0, std::ios::end);
    unsigned long long endPos = m_FileStream.tellg();

    m_FileStream.seekg(curPos, std::ios::beg);
    return endPos - startPos;
}

void SIEMStreamFilesystem::GenerateFilename()
{
    m_Filename = "stream_file_" + std::to_string(random());
}
