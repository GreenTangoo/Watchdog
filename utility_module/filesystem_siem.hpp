#ifndef FILESYSTEM_SIEM_HPP
#define FILESYSTEM_SIEM_HPP

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <map>
#include <fstream>
#include <memory>
#include <mutex>
#include <vector>
#include <shared_mutex>
#include <filesystem>

#include "../exception_module/exceptions.hpp"

using namespace siem_ex_space;

namespace utility_space
{
	typedef std::ios_base::iostate operationState;
	typedef unsigned int openOption;

    class FileObject
    {
    public:
        explicit FileObject(std::string const &filePath, std::ios_base::openmode mode =
                std::ios_base::in | std::ios_base::out);
        FileObject(FileObject const &other) = delete;
        FileObject(FileObject &&other) = delete;
        FileObject const& operator=(FileObject const &other) = delete;
        FileObject const& operator=(FileObject &&other) = delete;
        ~FileObject();
        operationState Write(char const *buf, size_t amountBytes);
        operationState WriteSymbol(char const ch);
        operationState WriteLine(std::string const &line);
        operationState PutbackSymbol(char const ch);
        operationState Read(char *buf, size_t amountBytes, size_t &readedBytes);
        operationState ReadSymbol(char &ch);
        operationState ReadLine(std::string &line);
        void SynchronizationStream();
        std::fstream& GetStream();
    private:
        std::fstream m_FileStream;
    };

    class FileManipulator
    {
    public:
        enum manipulateOption { READONLY = O_RDONLY, WRITEONLY = O_WRONLY,
                READ_WRITE = O_RDWR, CREATE = O_CREAT, APPEND = O_APPEND,
                LARGE_FILE = O_LARGEFILE, TRUNCATE = O_TRUNC };

        explicit FileManipulator();
        explicit FileManipulator(std::string const &filePath, openOption flags = READONLY);
        FileManipulator(FileManipulator const &other);
        FileManipulator(FileManipulator &&other);
        ~FileManipulator();
        FileManipulator const& operator=(FileManipulator const &other);
        FileManipulator const& operator=(FileManipulator &&other);
        std::shared_ptr<FileObject> operator->();
        void OpenManipulator(std::string const &filePath, openOption flags);
        void CloseManipulator();
        bool IsExistsFile(std::string const &filename) const;
        inline bool IsOpen() const;
    public:
        struct FileObjectDescriptor
        {
            bool m_IsOpenForRead{ false };
            std::shared_mutex m_DescriptorLock;
            std::shared_ptr<FileObject> m_pFileObject;
        };

        class FilesystemSiemException : public SIEMException
        {
        private:
            int m_Errno;
            openOption m_Flags;
        public:
            enum FilesystemErrorCode { INVALID_PATH = 1, PERMISSION_DENIED,
                                                                    INTERNAL_ERROR, CANNOT_OPEN_FILE, BAD_SIGNATURE,
                                                                    BAD_FLAGS };
            FilesystemSiemException(std::string const &exMsg, int errCode, int errnoCode = 0);
            FilesystemSiemException(std::string &&exMsg, int errCode, int errnoCode = 0);
            FilesystemSiemException(std::string const &exMsg, int errCode, openOption flags, int errnoCode = 0);
            FilesystemSiemException(std::string &&exMsg, int errCode, openOption flags, int errnoCode = 0);
            ~FilesystemSiemException();
            inline int GetErrno() const { return m_Errno; }
            std::vector<manipulateOption> SeparateFlags();
        };
    private:
        bool IsValidFlags(openOption flags);
        bool IsOpenForRead(openOption flags);
        ino_t GetFileSignature(std::string const &filePath, openOption flags);
        void OpenManipulatorInner(std::string const &filePath, openOption flags);
    private:
        bool m_IsClosed;
        ino_t m_FileSignature;
        std::shared_ptr<FileObjectDescriptor> m_pFileDescriptor;
    };
}

#endif // FILESYSTEM_SIEM_HPP
