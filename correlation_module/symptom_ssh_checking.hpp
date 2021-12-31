#ifndef SYMPTOM_SSH_CHECKING_HPP
#define SYMPTOM_SSH_CHECKING_HPP

#include "symptom_checking.hpp"

namespace correlation_space
{
    class SymptomSshCheck : public SymptomCheckWrapper
    {
    public:
        SymptomSshCheck();
        SymptomSshCheck(symptomCategory sympType);
        SymptomSshCheck(SymptomSshCheck const &other);
        SymptomSshCheck(SymptomSshCheck &&other);
        SymptomSshCheck const& operator=(SymptomSshCheck const &other);
        SymptomSshCheck const& operator=(SymptomSshCheck &&other);
        ~SymptomSshCheck() override;
        symptomFoundResult tryFoundSymptom() override;
    protected:
        SymptomInfo sympFoundResult;
    };

    typedef std::shared_ptr<SymptomSshCheck> SymptomSshCheckPtr;
}

#endif // SYMPTOM_SSH_CHECKING_HPP
