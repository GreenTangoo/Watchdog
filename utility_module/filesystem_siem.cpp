#include "filesystem_siem.hpp"

using namespace utility_space;
using namespace siem_ex_space;

#define AMOUNT_TO_DELETE 2

static std::map<ino_t, std::shared_ptr<FileManipulator::FileObjectDescriptor>> fileObjectsMap;
static std::mutex mapFileObjectsMut;

/*---------------------------------------------------------------*/
/*----------------------FILE OBJECT------------------------------*/
/*---------------------------------------------------------------*/
FileObject::FileObject(std::string const &filePath, std::ios_base::openmode mode)
{
    m_FileStream.open(filePath, mode);
}

FileObject::~FileObject()
{
    m_FileStream.close();
}

operationState FileObject::Write(char const *buf, size_t amountBytes)
{
    m_FileStream.write(buf, amountBytes);
	
    return m_FileStream.rdstate();
}

operationState FileObject::WriteSymbol(char const ch)
{
    m_FileStream.put(ch);

    return m_FileStream.rdstate();
}

operationState FileObject::WriteLine(std::string const &line)
{
    m_FileStream.write(line.c_str(), line.size());

    return m_FileStream.rdstate();
}

operationState FileObject::PutbackSymbol(char const ch)
{
    m_FileStream.putback(ch);

    return m_FileStream.rdstate();
}

operationState FileObject::Read(char *buf, size_t amountBytes, size_t &readedBytes)
{
    readedBytes = m_FileStream.readsome(buf, amountBytes);

    return m_FileStream.rdstate();
}

operationState FileObject::ReadSymbol(char &ch)
{
    ch = static_cast<int>(m_FileStream.get());

    return m_FileStream.rdstate();
}

operationState FileObject::ReadLine(std::string &line)
{
    std::getline(m_FileStream, line);
    operationState readState = m_FileStream.rdstate();

    return m_FileStream.rdstate();
}

void FileObject::SynchronizationStream()
{
    m_FileStream.sync();
}

std::fstream& FileObject::GetStream()
{
    return m_FileStream;
}

/*---------------------------------------------------------------*/
/*----------------------FILE MANIPULATOR-------------------------*/
/*---------------------------------------------------------------*/
FileManipulator::FileManipulator() : m_IsClosed(true)
{

}

FileManipulator::FileManipulator(std::string const &filePath, openOption flags) :
    m_IsClosed(true)
{
    if(!(IsValidFlags(flags)))
	{
		throw FilesystemSiemException("Incorrect flags.", 
			FilesystemSiemException::BAD_FLAGS,
			flags);
	}

    OpenManipulatorInner(filePath, flags);
}

FileManipulator::FileManipulator(FileManipulator const &other)
{
	std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);
    m_FileSignature = other.m_FileSignature;
    m_pFileDescriptor = other.m_pFileDescriptor;
    m_IsClosed = other.m_IsClosed;
}

FileManipulator::FileManipulator(FileManipulator &&other)
{
	std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);
    m_FileSignature = other.m_FileSignature;
    m_pFileDescriptor = other.m_pFileDescriptor;
    other.m_pFileDescriptor.reset();
    m_IsClosed = other.m_IsClosed;
}

FileManipulator::~FileManipulator()
{
    CloseManipulator();
}

FileManipulator const& FileManipulator::operator=(FileManipulator const &other)
{
	std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);
    m_FileSignature = other.m_FileSignature;
    m_pFileDescriptor = other.m_pFileDescriptor;

	return *this;
}

FileManipulator const& FileManipulator::operator=(FileManipulator &&other)
{
	std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);
    m_FileSignature = other.m_FileSignature;
    m_pFileDescriptor = other.m_pFileDescriptor;
    other.m_pFileDescriptor.reset();

	return *this;
}

std::shared_ptr<FileObject> FileManipulator::operator->()
{
    return m_pFileDescriptor->m_pFileObject;
}

void FileManipulator::OpenManipulator(std::string const &filePath, openOption flags)
{
    if(!m_IsClosed)
	{
		return;
	}

    if(!(IsValidFlags(flags)))
    {
        throw FilesystemSiemException("Incorrect flags.",
            FilesystemSiemException::BAD_FLAGS,
            flags);
    }

    OpenManipulatorInner(filePath, flags);
}

void FileManipulator::CloseManipulator()
{
    if(m_IsClosed)
	{
		return;
	}

	std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);

    if(m_pFileDescriptor->m_IsOpenForRead)
    {
        m_pFileDescriptor->m_DescriptorLock.unlock_shared();
    }
    else
    {
        m_pFileDescriptor->m_DescriptorLock.unlock();
    }

    m_IsClosed = true;
}

bool FileManipulator::IsExistsFile(std::string const &filename) const
{
    return std::filesystem::exists(filename);
}

inline bool FileManipulator::IsOpen() const
{
    return !m_IsClosed;
}

/*----------------------------------------------------------------*/
/*-----------------FILE MANIPULATOR(PRIVATE)----------------------*/
/*----------------------------------------------------------------*/
bool FileManipulator::IsValidFlags(openOption flags)
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

bool FileManipulator::IsOpenForRead(openOption flags)
{
    return flags & READONLY;
}

ino_t FileManipulator::GetFileSignature(std::string const &filePath, openOption flags)
{
    int fileDescriptor = 0;

    fileDescriptor = open(filePath.c_str(), flags, S_IRWXU);

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

    ino_t signature = fileStat.st_ino;
    close(fileDescriptor);

    return signature;
}

void FileManipulator::OpenManipulatorInner(std::string const &filePath, openOption flags)
{
    m_FileSignature = GetFileSignature(filePath, flags);
    bool isOpenForRead = IsOpenForRead(flags);

    std::lock_guard<std::mutex> fileSignaturesMapGuard(mapFileObjectsMut);

    std::map<ino_t, std::shared_ptr<FileObjectDescriptor>>::iterator it =
        fileObjectsMap.find(m_FileSignature);

    if(it != fileObjectsMap.end())
    {
        m_pFileDescriptor = it->second;
    }
    else
    {
        m_pFileDescriptor = std::make_shared<FileObjectDescriptor>();
        fileObjectsMap.insert(std::pair<ino_t, std::shared_ptr<FileObjectDescriptor>>(
            m_FileSignature, m_pFileDescriptor));
    }

    m_pFileDescriptor->m_pFileObject = std::make_shared<FileObject>(filePath);
    if(isOpenForRead)
    {
        m_pFileDescriptor->m_DescriptorLock.lock_shared();
        m_pFileDescriptor->m_IsOpenForRead = true;
    }
    else
    {
        m_pFileDescriptor->m_DescriptorLock.lock();
        m_pFileDescriptor->m_IsOpenForRead = false;
    }

    m_IsClosed = false;
}

/*---------------------------------------------------------------*/
/*------------------FILESYSTEM EXCEPTIONS------------------------*/
/*---------------------------------------------------------------*/
FileManipulator::FilesystemSiemException::FilesystemSiemException(std::string const &exMsg, int errCode, int errnoCode) :
    SIEMException(exMsg, errCode), m_Errno(errnoCode)
{

}

FileManipulator::FilesystemSiemException::FilesystemSiemException(std::string &&exMsg, int errCode, int errnoCode) :
    SIEMException(std::move(exMsg), errCode), m_Errno(errnoCode)
{

}

FileManipulator::FilesystemSiemException::FilesystemSiemException(std::string const &exMsg, int errCode, 
	openOption flags, int errnoCode) : 
    SIEMException(exMsg, errCode), m_Errno(errnoCode), m_Flags(flags)
{

}

FileManipulator::FilesystemSiemException::FilesystemSiemException(std::string &&exMsg, int errCode, 
	openOption flags, int errnoCode) : 
    SIEMException(std::move(exMsg), errCode), m_Errno(errnoCode), m_Flags(flags)
{

}

FileManipulator::FilesystemSiemException::~FilesystemSiemException()
{

}

std::vector<FileManipulator::manipulateOption> FileManipulator::FilesystemSiemException::SeparateFlags()
{
	std::vector<FileManipulator::manipulateOption> flagsVec;

    if(m_Flags & READONLY)
		flagsVec.push_back(READONLY);
    if(m_Flags & WRITEONLY)
		flagsVec.push_back(WRITEONLY);
    if(m_Flags & READ_WRITE)
		flagsVec.push_back(READ_WRITE);
    if(m_Flags & CREATE)
		flagsVec.push_back(CREATE);
    if(m_Flags & APPEND)
		flagsVec.push_back(APPEND);
    if(m_Flags & LARGE_FILE)
		flagsVec.push_back(LARGE_FILE);
    if(m_Flags & TRUNCATE)
		flagsVec.push_back(TRUNCATE);

	return flagsVec;
}
