#include "encryption.hpp"

using namespace utility_space;

/*-----------------------------------ENCRYPTION------------------------------------*/
std::pair<std::string, std::string> Encryption::getTwinKey(twinKeyType typeKey)
{
	return std::pair<std::string, std::string>();
}

std::string Encryption::getSingleKey(singleKeyType typeKey)
{
	return "";
}

Encryption::Encryption(std::string pass)
{

}

Encryption::Encryption(Encryption const &other)
{

}

Encryption::~Encryption()
{

}

std::string Encryption::encryptWithProgramKey(std::string sourceStr)
{
	return "";
}

std::string Encryption::decryptWithProgramKey(std::string encryptedStr)
{
	return "";
}

std::string Encryption::encryptWithPassphrase(std::string sourceStr)
{
	return "";
}

std::string Encryption::decryptWithPassphrase(std::string decryptedStr)
{
	return "";
}

std::string Encryption::getDES3Key()
{
	return "";
}

std::string Encryption::getBlowfishKey()
{
	return "";
}

std::string Encryption::getRC5Key()
{
	return "";
}

std::string Encryption::getVMPCKey()
{
	return "";
}

std::pair<std::string, std::string> Encryption::getRSAKey()
{
	return std::pair<std::string, std::string>();
}

std::pair<std::string, std::string> Encryption::getDSAKey()
{
	return std::pair<std::string, std::string>();
}


/*-------------------------ENCRYPTIONDES3--------------------------------*/
EncryptionDES3::EncryptionDES3(std::string pass) : Encryption(pass)
{

}

EncryptionDES3::EncryptionDES3(EncryptionDES3 const &other) : Encryption(other)
{

}

EncryptionDES3::EncryptionDES3(EncryptionDES3 &&other) : Encryption(std::move(other))
{

}

EncryptionDES3::~EncryptionDES3()
{

}

std::string EncryptionDES3::encryptData(std::string srcData, std::string pass)
{
	return "";
}

std::string EncryptionDES3::decryptData(std::string encryptedData, std::string pass)
{
	return "";
}

/*-------------------------ENCRYPTIONBLOWFISH--------------------------------*/
EncryptionBlowfish::EncryptionBlowfish(std::string pass) : Encryption(pass)
{

}

EncryptionBlowfish::EncryptionBlowfish(EncryptionBlowfish const &other) : Encryption(other)
{

}

EncryptionBlowfish::EncryptionBlowfish(EncryptionBlowfish &&other) : Encryption(std::move(other))
{

}

EncryptionBlowfish::~EncryptionBlowfish()
{

}

std::string EncryptionBlowfish::encryptData(std::string srcData, std::string pass)
{
	return "";
}

std::string EncryptionBlowfish::decryptData(std::string encryptedData, std::string pass)
{
	return "";
}

/*-------------------------ENCRYPTIONRC5--------------------------------*/
EncryptionRC5::EncryptionRC5(std::string pass) : Encryption(pass)
{

}

EncryptionRC5::EncryptionRC5(EncryptionRC5 const &other) : Encryption(other)
{

}

EncryptionRC5::EncryptionRC5(EncryptionRC5 &&other) : Encryption(std::move(other))
{

}

EncryptionRC5::~EncryptionRC5()
{

}

std::string EncryptionRC5::encryptData(std::string srcData, std::string pass)
{
	return "";
}

std::string EncryptionRC5::decryptData(std::string encryptedData, std::string pass)
{
	return "";
}

/*-------------------------EncryptionVMPC--------------------------------*/
EncryptionVMPC::EncryptionVMPC(std::string pass) : Encryption(pass)
{

}

EncryptionVMPC::EncryptionVMPC(EncryptionVMPC const &other) : Encryption(other)
{

}

EncryptionVMPC::EncryptionVMPC(EncryptionVMPC &&other) : Encryption(std::move(other))
{

}

EncryptionVMPC::~EncryptionVMPC()
{

}

std::string EncryptionVMPC::encryptData(std::string srcData, std::string pass)
{
	return "";
}

std::string EncryptionVMPC::decryptData(std::string encryptedData, std::string pass)
{
	return "";
}

/*-------------------------EncryptionRSA--------------------------------*/
EncryptionRSA::EncryptionRSA(std::string pass) : Encryption(pass)
{

}

EncryptionRSA::EncryptionRSA(EncryptionRSA const &other) : Encryption(other)
{

}

EncryptionRSA::EncryptionRSA(EncryptionRSA &&other) : Encryption(other)
{

}

EncryptionRSA::~EncryptionRSA()
{

}

std::string EncryptionRSA::encryptData(std::string srcData, std::string pass)
{
	return "";
}

std::string EncryptionRSA::decryptData(std::string encryptedData, std::string pass)
{
	return "";
}

/*-------------------------EncryptionDSA--------------------------------*/
EncryptionDSA::EncryptionDSA(std::string pass) : Encryption(pass)
{

}

EncryptionDSA::EncryptionDSA(EncryptionDSA const &other) : Encryption(other)
{

}

EncryptionDSA::EncryptionDSA(EncryptionDSA &&other) : Encryption(std::move(other))
{

}

EncryptionDSA::~EncryptionDSA()
{

}

std::string EncryptionDSA::encryptData(std::string srcData, std::string pass)
{
	return "";
}

std::string EncryptionDSA::decryptData(std::string encryptedData, std::string pass)
{
	return "";
}