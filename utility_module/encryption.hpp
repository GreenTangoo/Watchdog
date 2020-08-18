#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <string>

namespace utility_space
{
	class Encryption
	{
	protected:
		std::string passphrase;
	private:
		virtual std::string encryptData(std::string srcData, std::string pass) = 0;
		virtual std::string decryptData(std::string encryptedData, std::string pass) = 0;
		std::string getDES3Key();
		std::string getBlowfishKey();
		std::string getRC5Key();
		std::string getVMPCKey();
		std::pair<std::string, std::string> getRSAKey();
		std::pair<std::string, std::string> getDSAKey();
	public:
		enum singleKeyType { DES3 = 3, BLOWFISH, RC5, VMPC };
		enum twinKeyType { RSA = 0, DSA };
	public:
		explicit Encryption(std::string pass);
		Encryption(Encryption const &other);
		~Encryption();
		static std::pair<std::string, std::string> getTwinKey(twinKeyType typeKey);
		static std::string getSingleKey(singleKeyType typeKey);
		std::string encryptWithProgramKey(std::string sourceStr);
		std::string decryptWithProgramKey(std::string encryptedStr);
		std::string encryptWithPassphrase(std::string sourceStr);
		std::string decryptWithPassphrase(std::string encryptedStr);
	};

	class EncryptionDES3 : public Encryption
	{
	private:
		virtual std::string encryptData(std::string srcData, std::string pass);
		virtual std::string decryptData(std::string ecnryptedData, std::string pass);
	public:
		explicit EncryptionDES3(std::string pass);
		EncryptionDES3(EncryptionDES3 const &other);
		EncryptionDES3(EncryptionDES3 &&other);
		~EncryptionDES3();
	};

	class EncryptionBlowfish : public Encryption
	{
	private:
		virtual std::string encryptData(std::string srcData, std::string pass);
		virtual std::string decryptData(std::string encryptedData, std::string pass);
	public:
		explicit EncryptionBlowfish(std::string pass);
		EncryptionBlowfish(EncryptionBlowfish const &other);
		EncryptionBlowfish(EncryptionBlowfish &&other);
		~EncryptionBlowfish();
	};

	class EncryptionRC5 : public Encryption
	{
	private:
		virtual std::string encryptData(std::string srcData, std::string pass);
		virtual std::string decryptData(std::string encryptedData, std::string pass);
	public:
		explicit EncryptionRC5(std::string pass);
		EncryptionRC5(EncryptionRC5 const &other);
		EncryptionRC5(EncryptionRC5 &&other);
		~EncryptionRC5();
	};

	class EncryptionVMPC : public Encryption
	{
	private:
		virtual std::string encryptData(std::string srcData, std::string pass);
		virtual std::string decryptData(std::string encryptedData, std::string pass);
	public:
		explicit EncryptionVMPC(std::string pass);
		EncryptionVMPC(EncryptionVMPC const &other);
		EncryptionVMPC(EncryptionVMPC &&other);
		~EncryptionVMPC();
	};

	class EncryptionRSA : public Encryption
	{
	private:
		virtual std::string encryptData(std::string srcData, std::string pass);
		virtual std::string decryptData(std::string encryptedData, std::string pass);
	public:
		explicit EncryptionRSA(std::string pass);
		EncryptionRSA(EncryptionRSA const &other);
		EncryptionRSA(EncryptionRSA &&other);
		~EncryptionRSA();
	};

	class EncryptionDSA : public Encryption
	{
	private:
		virtual std::string encryptData(std::string srcData, std::string pass);
		virtual std::string decryptData(std::string encryptedData, std::string pass);
	public:
		explicit EncryptionDSA(std::string pass);
		EncryptionDSA(EncryptionDSA const &other);
		EncryptionDSA(EncryptionDSA &&other);
		~EncryptionDSA();
	};
}

#endif // ENCRYPTION_HPP