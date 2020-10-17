#ifndef SIEM_FILESYSTEM_HPP
#define SIEM_FILESYSTEM_HPP

#include <fstream>
#include <mutex>
#include <atomic>
#include <deque>
#include <filesystem>
#include <iostream>

#include "../exception_module/exceptions.hpp"

namespace utility_space
{
    class SiemFilesystem
    {
    private:
        std::fstream _file;
        std::mutex _fileLockMut;
        std::atomic<bool> _isClosed;
    public:
        SiemFilesystem();
        SiemFilesystem(std::string path, std::ios_base::openmode mode);
        SiemFilesystem(SiemFilesystem const &other) = delete;
        SiemFilesystem(SiemFilesystem &&other) = delete;
        SiemFilesystem const& operator=(SiemFilesystem const &other) = delete;
        SiemFilesystem const& operator=(SiemFilesystem &&other) = delete;
        void openFile(std::string path, std::ios_base::openmode mode);
        void closeFile();
        std::fstream& getStream();
        std::string readLine();
        std::deque<std::string> readLines();
        void read(char *buf, size_t numBytes);
        void writeLine(std::string line);
        void writeLines(std::deque<std::string> lines);
        void write(char *buf, size_t numBytes);
        static void createFile(std::string filePath);
        static void deleteFile(std::string filePath);
    };
}

#endif // SIEM_FILESYSTEM_HPP