#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <string>

namespace utility_space
{
	class Encryption
	{
	private:
		std::string passphrase;
	public:
		enum typeCipherAlg {};
		enum typeHashAlg {};
	public:
		explicit Encryption(std::string pass);
		Encryption(Encryption const &other);
		Encryption(Encryption &&other);
		~Encryption();
		Encryption const& operator=(Encryption const &other);
		Encryption const& operator=(Encryption &&other);
		static std::string getHash(std::string msg);
		static std::pair<std::string, std::string> getTwinKey();
		static std::string getSingleKey();
		std::string encryptWithProgramKey(std::string sourceStr);
		std::string decryptWithProgramKey(std::string encryptedStr);
		std::string encryptWithPassphrase(std::string sourceStr);
		std::string decryptWithPassphrase(std::string encryptedStr);
	};
}

#endif // ENCRYPTION_HPP