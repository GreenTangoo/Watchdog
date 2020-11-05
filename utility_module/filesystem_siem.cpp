#include "filesystem_siem.hpp"

using namespace utility_space;
using namespace siem_ex_space;

#define AMOUNT_TO_DELETE 2

static std::map<ino_t, std::shared_ptr<FileObject>> fileObjectsMap;
static std::mutex mapFileObjectsMut;

/*---------------------------------------------------------------*/
/*----------------------FILE OBJECT------------------------------*/
/*---------------------------------------------------------------*/
FileObject::FileObject(std::string const &filePath, std::ios_base::open_mode mode)
{

}

FileObject::~FileObject()
{

}

fileOpResultCode FileObject::write(char const *buf, size_t amountBytes)
{

}

fileOpResultCode FileObject::write(void const *buf, size_t blockSize, size_t amountBlocks)
{

}

fileOpResultCode FileObject::writeLine(std::string const &line)
{

}

fileOpResultCode FileObject::read(char *buf, size_t amountBytes)
{

}

fileOpResultCode FileObject::read(void *buf, size_t blockSize, size_t amountBlocks)
{

}

fileOpResultCode FileObject::readLine(std::string &line)
{

}

/*---------------------------------------------------------------*/
/*----------------------FILE MANIPULATOR-------------------------*/
/*---------------------------------------------------------------*/
FileManipulator::FileManipulator(std::string const &filePath)
{
	int fileDescriptor = open(filePath.c_str(), O_RDONLY);
	if(fileDescriptor == -1)
	{
		throw FilesystemSiemException("Bad descriptor of file.", 
			FilesystemSiemException::INVALID_PATH, errno);
	}

	struct stat fileStat = { 0 };
	if(fstat(fileDescriptor, &fileStat) == -1)
	{
		close(fileDescriptor);
		throw FilesystemSiemException("Cannot get stat information.",
			FilesystemSiemException::BAD_SIGNATURE, errno);
	}

	_fileSignature = fileStat.st_ino;
	close(fileDescriptor);

	mapFileObjectsMut.lock();

	std::map<ino_t, std::shared_ptr<FileObject>>::iterator it = 
		fileObjectsMap.find(_fileSignature);

	if(it != fileObjectsMap.end())
	{
		_filePtr = it->second;
	}
	else
	{
		fileObjectsMap.insert(std::pair<ino_t, std::shared_ptr<FileObject>>(
			_fileSignature, std::make_shared<FileObject>(filePath)));
		
		_filePtr = fileObjectsMap[_fileSignature];
	}

	mapFileObjectsMut.unlock();
}

FileManipulator::FileManipulator(FileManipulator const &other)
{

}

FileManipulator::FileManipulator(FileManipulator &&other)
{

}

FileManipulator const& FileManipulator::operator=(FileManipulator const &other)
{

}

FileManipulator const& FileManipulator::operator=(FileManipulator &&other)
{

}

std::shared_ptr<FileObject> FileManipulator::operator->()
{
	return _filePtr;
}

FileManipulator::~FileManipulator()
{
	mapFileObjectsMut.lock();

	long amountRefs = _filePtr.use_count();
	if(amountRefs == AMOUNT_TO_DELETE)
	{
		fileObjectsMap.erase(_fileSignature);
	}
	else
	{
		_filePtr.reset();
	}

	mapFileObjectsMut.unlock();
}