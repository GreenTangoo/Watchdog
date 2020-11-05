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
	enum fileOpResultCode {};

	class FileObject
	{
	private:
		std::fstream _fileStream;
		std::mutex _fileStreamMut;
	public:
		explicit FileObject(std::string const &filePath, std::ios_base::open_mode mode = 
			std::ios_base::in | std::ios_base::out);
		FileObject(FileObject const &other) = delete;
		FileObject(FileObject &&other) = delete;
		FileObject const& operator=(FileObject const &other) = delete;
		FileObject const& operator=(FileObject &&other) = delete;
		~FileObject();
		fileOpResultCode write(char const *buf, size_t amountBytes);
		fileOpResultCode write(void const *buf, size_t blockSize, size_t amountBlocks);
		fileOpResultCode writeLine(std::string const &line);
		fileOpResultCode read(char *buf, size_t amountBytes);
		fileOpResultCode read(void *buf, size_t blockSize, size_t amountBlocks);
		fileOpResultCode readLine(std::string &line);
	};

	class FileManipulator
	{
	private:
		ino_t _fileSignature;
		std::shared_ptr<FileObject> _filePtr;
	public:
		explicit FileManipulator(std::string const &filePath);
		FileManipulator(FileManipulator const &other);
		FileManipulator(FileManipulator &&other);
		FileManipulator const& operator=(FileManipulator const &other);
		FileManipulator const& operator=(FileManipulator &&other);
		std::shared_ptr<FileObject> operator->();
		~FileManipulator();
	};
}

#endif // FILESYSTEM_SIEM_HPP