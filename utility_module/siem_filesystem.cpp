#include "siem_filesystem.hpp"

using namespace utility_space;
using namespace siem_ex_space;

SiemFilesystem::SiemFilesystem() : _isClosed(true)
{

}

SiemFilesystem::SiemFilesystem(std::string path, std::ios_base::openmode mode) : 
    _isClosed(true)
{
    try
    {
        std::lock_guard<std::mutex> fileStreamGuard(_fileLockMut);
        _file.open(path, mode);
        _isClosed.store(false);
    }
    catch(std::ios_base::failure const &ex)
    {
        std::string errMsg(ex.what());
        int errCode = ex.code().value();

        throw FilesystemSiemException("Internal error: " + errMsg + 
            " with code: " + std::to_string(errCode),
            FilesystemSiemException::INTERNAL_ERROR);
    }
}

void SiemFilesystem::openFile(std::string path, std::ios_base::openmode mode)
{
    if(!_isClosed.load())
    {
        throw FilesystemSiemException("Cannot open another file, while stream opened.",
            FilesystemSiemException::STREAM_ALREADY_OPEN);
    }
    else
    {
        std::lock_guard<std::mutex> fileStreamGuard(_fileLockMut);
        _file.open(path, mode);
        _isClosed.store(false);
    }
}

void SiemFilesystem::closeFile()
{
    if(_isClosed.load())
    {
        throw FilesystemSiemException("Cannot close filestream, stream already closed.",
            FilesystemSiemException::STREAM_ALREADY_CLOSE);
    }
    else
    {
        std::lock_guard<std::mutex> fileStreamGuard(_fileLockMut);
        _file.close();
        _isClosed.store(true);
    }
}

std::fstream& SiemFilesystem::getStream()
{

}

std::string SiemFilesystem::readLine()
{

}

std::deque<std::string> SiemFilesystem::readLines()
{

}

void SiemFilesystem::read(char *buf, size_t numBytes)
{

}

void SiemFilesystem::writeLine(std::string line)
{

}

void SiemFilesystem::writeLines(std::deque<std::string> lines)
{

}

void SiemFilesystem::write(char *buf, size_t numBytes)
{

}

void SiemFilesystem::createFile(std::string filePath)
{

}

void SiemFilesystem::deleteFile(std::string filePath)
{

}