#ifndef SYMPTOM_IMPLEMENTATION_HPP
#define SYMPTOM_IMPLEMENTATION_HPP

#include "../utility_module/json.hpp"
#include "../utility_module/encryption.hpp"
#include "../description_module/configuration.hpp"

using namespace utility_space;
using namespace description_space;

namespace correlation_space
{
	enum symptomCategory { PORT_SCANNING = 0, LFI };

	class SymptomChecker
	{
	private:
		JsonObject parser;
		std::unique_ptr<Encryption> encryptor;
		symptomCategory sympType;
		SearchInfo const &info;
	public:
		SymptomChecker(SearchInfo const &infoSearch, symptomCategory sympType);
		SymptomChecker(SymptomChecker const &other);
		SymptomChecker(SymptomChecker &&other);
		~SymptomChecker();
		bool tryFoundSymptom();
	};

	class SymptomCategoryResolver // STATIC CLASS
	{
	public:
		static symptomCategory stringToSymptomCategory(std::string const &symptomStr);
		static std::string symptomCategoryToString(symptomCategory sympCategory);
	};

}
#endif // SYMPTOM_IMPLEMENTATION_HPP