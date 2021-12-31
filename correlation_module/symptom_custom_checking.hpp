#ifndef SYMPTOM_CUSTOM_CHECKING_HPP
#define SYMPTOM_CUSTOM_CHECKING_HPP

#include "symptom_checking.hpp"

#include "../description_module/configuration.hpp"

using namespace description_space;

namespace correlation_space
{
    class SymptomCheckCustom : public SymptomCheckWrapper
    {
    private:
        SearchInfo const &info;
    public:
        SymptomCheckCustom(symptomCategory sympType, SearchInfo const &infoSearch);
        SymptomCheckCustom(SymptomCheckCustom const &other);
        SymptomCheckCustom(SymptomCheckCustom &&other);
        ~SymptomCheckCustom() override;
        virtual symptomFoundResult tryFoundSymptom() override;
    };

    typedef std::shared_ptr<SymptomCheckCustom> SymptomCheckCustomPtr;
}

#endif // SYMPTOM_CUSTOM_CHECKING_HPP
