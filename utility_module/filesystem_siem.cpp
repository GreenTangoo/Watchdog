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

operationState FileObject::readLine(std::string &line)
{
	std::lock_guard<std::mutex> streamOperationGuard(_fileStreamMut);
	std::getline(_fileStream, line);

	std::ios_base::iostate state = _fileStream.rdstate();
	return state;
}

void FileObject::synchronizationStream()
{
	std::lock_guard<std::mutex> streamOperationGuard(_fileStreamMut);
	_fileStream.sync();
}

std::fstream& FileObject::getStream()
{
	return _fileStream;
}

/*---------------------------------------------------------------*/
/*----------------------FILE MANIPULATOR-------------------------*/
/*---------------------------------------------------------------*/
FileManipulator::FileManipulator() : _isClosed(true)
{

}

FileManipulator::FileManipulator(std::string const &filePath, openOption flags) :
	_isClosed(true)
{
	if(!(this->isValidFlags(flags)))
	{
		throw FilesystemSiemException("Incorrect flags.", 
			FilesystemSiemException::BAD_FLAGS,
			flags);
	}

	this->openManipulator(filePath, flags);
}

FileManipulator::FileManipulator(FileManipulator const &other)
{
	std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);
	_fileSignature = other._fileSignature;
	_filePtr = other._filePtr;
	_isClosed = other._isClosed;
}

FileManipulator::FileManipulator(FileManipulator &&other)
{
	std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);
	_fileSignature = other._fileSignature;
	_filePtr = other._filePtr;
	other._filePtr.reset();
	_isClosed = other._isClosed;
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

void FileManipulator::openManipulator(std::string const &filePath, openOption flags)
{
	if(!_isClosed)
	{
		return;
	}

	int fileDescriptor = 0;

	if(flags & CREATE)
		fileDescriptor = open(filePath.c_str(), flags, S_IWUSR | S_IRUSR);
	else
		fileDescriptor = open(filePath.c_str(), flags);

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

	_isClosed = false;
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

	_isClosed = true;
}

FileManipulator::~FileManipulator()
{
	this->closeManipulator();
}

/*----------------------------------------------------------------*/
/*-----------------FILE MANIPULATOR(PRIVATE)----------------------*/
/*----------------------------------------------------------------*/
bool FileManipulator::isValidFlags(openOption flags)
{
	bool validFlags = false;

	do
	{
		if((flags & TRUNCATE) && (flags & READONLY))
			break;
		if((flags & APPEND) && (flags & READONLY))
			break;
		if((flags & READ_WRITE) && ((flags & READONLY) || (flags & WRITEONLY)))
			break;

		validFlags = true;
	} while (false);
	
	return validFlags;
}

/*---------------------------------------------------------------*/
/*------------------FILESYSTEM EXCEPTIONS------------------------*/
/*---------------------------------------------------------------*/
FileManipulator::FilesystemSiemException::FilesystemSiemException(std::string const &exMsg, int errCode, int errnoCode) :
	SIEMExecption(exMsg, errCode), _errno(errnoCode)
{

}

FileManipulator::FilesystemSiemException::FilesystemSiemException(std::string &&exMsg, int errCode, int errnoCode) :
	SIEMExecption(std::move(exMsg), errCode), _errno(errnoCode)
{

}

FileManipulator::FilesystemSiemException::FilesystemSiemException(std::string const &exMsg, int errCode, 
	openOption flags, int errnoCode) : 
	SIEMExecption(exMsg, errCode), _errno(errnoCode), _flags(flags)
{

}

FileManipulator::FilesystemSiemException::FilesystemSiemException(std::string &&exMsg, int errCode, 
	openOption flags, int errnoCode) : 
	SIEMExecption(std::move(exMsg), errCode), _errno(errnoCode), _flags(flags)
{

}

FileManipulator::FilesystemSiemException::~FilesystemSiemException()
{

}

int FileManipulator::FilesystemSiemException::getErrno()
{
	return _errno;
}

std::vector<FileManipulator::manipulateOption> FileManipulator::FilesystemSiemException::separateFlags()
{
	std::vector<FileManipulator::manipulateOption> flagsVec;

	if(_flags & READONLY)
		flagsVec.push_back(READONLY);
	if(_flags & WRITEONLY)
		flagsVec.push_back(WRITEONLY);
	if(_flags & READ_WRITE)
		flagsVec.push_back(READ_WRITE);
	if(_flags & CREATE)
		flagsVec.push_back(CREATE);
	if(_flags & APPEND)
		flagsVec.push_back(APPEND);
	if(_flags & LARGE_FILE)
		flagsVec.push_back(LARGE_FILE);
	if(_flags & TRUNCATE)
		flagsVec.push_back(TRUNCATE);

	return flagsVec;
}