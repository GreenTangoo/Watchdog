#ifndef SYMPTOM_SSH_CHECKING_HPP
#define SYMPTOM_SSH_CHECKING_HPP

#include "symptom_checking.hpp"

namespace correlation_space
{
    class SymptomSSHCheck : public SymptomCheckWrapper
    {
    public:
        SymptomSSHCheck();
        SymptomSSHCheck(symptomCategory sympType);
        SymptomSSHCheck(SymptomSSHCheck const &other);
        SymptomSSHCheck(SymptomSSHCheck &&other);
        SymptomSSHCheck const& operator=(SymptomSSHCheck const &other);
        SymptomSSHCheck const& operator=(SymptomSSHCheck &&other);
        ~SymptomSSHCheck() override;
        virtual symptomFoundResult tryFoundSymptom() = 0;
    protected:
        SymptomInfo m_FoundResult;
    };

    typedef std::shared_ptr<SymptomSSHCheck> SymptomSSHCheckPtr;
}

#endif // SYMPTOM_SSH_CHECKING_HPP
