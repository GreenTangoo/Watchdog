#ifndef SYMPTOM_CHECKING_HPP
#define SYMPTOM_CHECKING_HPP

#include <map>

#include "../utility_module/json.hpp"
#include "../utility_module/date_time.hpp"
#include "../exception_module/exceptions.hpp"

using namespace utility_space;
using namespace siem_ex_space;

namespace correlation_space
{
    enum symptomCategory { scNone = 0, scPortScanning, scLFI, scBruteSSH, scAuthSSH };

    enum detailCategory { dtNone = 0, dtAddrIP, dtAddrMAC, dtUsername, dtAmountTryes, dtStatus};

    struct SymptomInfo
    {
        symptomCategory sympType;
        DateTime time;
        std::map<detailCategory, std::string> details;
    };

    class ISymptomCheck
    {
    public:
        enum symptomFoundResult { NOT_FOUND_SYMP = 0, FOUND_SYMP };
    public:
        ISymptomCheck() = default;
        ISymptomCheck(ISymptomCheck const &other) = default;
        ISymptomCheck(ISymptomCheck &&other) = default;
        ISymptomCheck const& operator=(ISymptomCheck const &other) = delete;
        ISymptomCheck const& operator=(ISymptomCheck &&other) = delete;
        virtual ~ISymptomCheck() = default;
        virtual symptomFoundResult tryFoundSymptom() = 0;
    };

    typedef std::shared_ptr<ISymptomCheck> ISymptomCheckPtr;

    class SymptomCheckWrapper : public ISymptomCheck
    {
    public:
        SymptomCheckWrapper();
        SymptomCheckWrapper(symptomCategory sympType);
        SymptomCheckWrapper(SymptomCheckWrapper const &other);
        SymptomCheckWrapper(SymptomCheckWrapper &&other);
        SymptomCheckWrapper const& operator=(SymptomCheckWrapper const &other);
        SymptomCheckWrapper const& operator=(SymptomCheckWrapper &&other);
        virtual ~SymptomCheckWrapper() = default;
        virtual symptomFoundResult tryFoundSymptom() = 0;
    protected:
        symptomCategory sympType;
        JsonObject parser;
    };

    typedef std::shared_ptr<SymptomCheckWrapper> SymptomCheckWrapperPtr;

	class SymptomCategoryResolver // STATIC CLASS
	{
	public:
		static symptomCategory stringToSymptomCategory(std::string const &symptomStr);
		static std::string symptomCategoryToString(symptomCategory sympCategory);
	};

}
#endif // SYMPTOM_IMPLEMENTATION_HPP
