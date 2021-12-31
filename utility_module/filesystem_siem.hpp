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
		operationState write(char const *buf, size_t amountBytes);
		operationState write(void const *buf, size_t blockSize, size_t amountBlocks);
		operationState writeLine(std::string const &line);
		operationState read(char *buf, size_t amountBytes);
		operationState read(void *buf, size_t blockSize, size_t amountBlocks);
		operationState readLine(std::string &line);
		void synchronizationStream();
		std::fstream& getStream();
	private:
		std::fstream _fileStream;
		std::mutex _fileStreamMut;
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
		FileManipulator const& operator=(FileManipulator const &other);
		FileManipulator const& operator=(FileManipulator &&other);
		std::shared_ptr<FileObject> operator->();
		void openManipulator(std::string const &filePath, openOption flags);
		void closeManipulator();
		~FileManipulator();
	public:
        class FilesystemSiemException : public SIEMException
		{
		private:
			int _errno;
			openOption _flags;
		public:
			enum FilesystemErrorCode { INVALID_PATH = 1, PERMISSION_DENIED,
										INTERNAL_ERROR, CANNOT_OPEN_FILE, BAD_SIGNATURE,
										BAD_FLAGS };
			FilesystemSiemException(std::string const &exMsg, int errCode, int errnoCode = 0);
			FilesystemSiemException(std::string &&exMsg, int errCode, int errnoCode = 0);
			FilesystemSiemException(std::string const &exMsg, int errCode, openOption flags, int errnoCode = 0);
			FilesystemSiemException(std::string &&exMsg, int errCode, openOption flags, int errnoCode = 0);
			~FilesystemSiemException();
			int getErrno();
			std::vector<manipulateOption> separateFlags();
		};
	private:
		bool isValidFlags(openOption flags);
	private:
		bool _isClosed;
		ino_t _fileSignature;
		std::shared_ptr<FileObject> _filePtr;
	};
}

#endif // FILESYSTEM_SIEM_HPP
