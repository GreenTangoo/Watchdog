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
#include <shared_mutex>

#include "../exception_module/exceptions.hpp"

namespace utility_space
{
	typedef std::ios_base::iostate operationState;

	class FileObject
	{
	private:
		std::fstream _fileStream;
		std::mutex _fileStreamMut;
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
	};

	class FileManipulator
	{
	private:
		bool _isClosed;
		ino_t _fileSignature;
		std::shared_ptr<FileObject> _filePtr;
	public:
		explicit FileManipulator();
		explicit FileManipulator(std::string const &filePath);
		FileManipulator(FileManipulator const &other);
		FileManipulator(FileManipulator &&other);
		FileManipulator const& operator=(FileManipulator const &other);
		FileManipulator const& operator=(FileManipulator &&other);
		std::shared_ptr<FileObject> operator->();
		void openManipulator(std::string const &filePath);
		void closeManipulator();
		~FileManipulator();
	};
}

#endif // FILESYSTEM_SIEM_HPP