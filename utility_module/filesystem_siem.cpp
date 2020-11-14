#include "filesystem_siem.hpp"

using namespace utility_space;
using namespace siem_ex_space;

#define AMOUNT_TO_DELETE 2

static std::map<ino_t, std::shared_ptr<FileObject>> fileObjectsMap;
static std::mutex mapFileObjectsMut;

/*---------------------------------------------------------------*/
/*----------------------FILE OBJECT------------------------------*/
/*---------------------------------------------------------------*/
FileObject::FileObject(std::string const &filePath, std::ios_base::openmode mode)
{
	_fileStream.open(filePath, mode);
}

FileObject::~FileObject()
{
	_fileStream.close();
}

operationState FileObject::write(char const *buf, size_t amountBytes)
{
	std::lock_guard<std::mutex> streamOperationGuard(_fileStreamMut);
	_fileStream.write(buf, amountBytes);
	
	return _fileStream.rdstate();
}

operationState FileObject::write(void const *buf, size_t blockSize, size_t amountBlocks)
{

}

operationState FileObject::writeLine(std::string const &line)
{
	std::lock_guard<std::mutex> streamOperationGuard(_fileStreamMut);
	_fileStream.write(line.c_str(), line.size());

	return _fileStream.rdstate();
}

operationState FileObject::read(char *buf, size_t amountBytes)
{
	std::lock_guard<std::mutex> streamOperationGuard(_fileStreamMut);
	_fileStream.readsome(buf, amountBytes);

	return _fileStream.rdstate();
}

operationState FileObject::read(void *buf, size_t blockSize, size_t amountBlocks)
{

}

operationState FileObject::readLine(std::string &line)
{
	std::lock_guard<std::mutex> streamOperationGuard(_fileStreamMut);
	std::getline(_fileStream, line);

	return _fileStream.rdstate();
}

/*---------------------------------------------------------------*/
/*----------------------FILE MANIPULATOR-------------------------*/
/*---------------------------------------------------------------*/
FileManipulator::FileManipulator() : _isClosed(true)
{

}

FileManipulator::FileManipulator(std::string const &filePath)
{
	this->openManipulator(filePath);
}

FileManipulator::FileManipulator(FileManipulator const &other)
{
	std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);
	_fileSignature = other._fileSignature;
	_filePtr = other._filePtr;
}

FileManipulator::FileManipulator(FileManipulator &&other)
{
	std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);
	_fileSignature = other._fileSignature;
	_filePtr = other._filePtr;
	other._filePtr.reset();
}

FileManipulator const& FileManipulator::operator=(FileManipulator const &other)
{
	std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);
	_fileSignature = other._fileSignature;
	_filePtr = other._filePtr;

	return *this;
}

FileManipulator const& FileManipulator::operator=(FileManipulator &&other)
{
	std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);
	_fileSignature = other._fileSignature;
	_filePtr = other._filePtr;
	other._filePtr.reset();

	return *this;
}

std::shared_ptr<FileObject> FileManipulator::operator->()
{
	return _filePtr;
}

void FileManipulator::openManipulator(std::string const &filePath)
{
	if(!_isClosed)
	{
		return;
	}

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

	std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);

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

	_isClosed = true;
}

void FileManipulator::closeManipulator()
{
	if(_isClosed)
	{
		return;
	}

	std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);

	long amountRefs = _filePtr.use_count();
	_filePtr.reset();

	if(amountRefs == AMOUNT_TO_DELETE)
	{
		fileObjectsMap.erase(_fileSignature);
	}
}

FileManipulator::~FileManipulator()
{
	this->closeManipulator();
}