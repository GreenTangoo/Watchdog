#ifndef CORRELATION_HPP
#define CORRELATION_HPP

#include <memory>

namespace correlation_space
{
    class ICorrelation
    {
    public:
        enum CorrelationType { noneCorrelation = 0, linkCorrelation, statisticCorrelation, machineLearningCorrelation };
        ICorrelation() = default;
        ICorrelation(ICorrelation const &other) = default;
        ICorrelation(ICorrelation &&other) = default;
        ICorrelation const& operator=(ICorrelation const &other) = delete;
        ICorrelation const& operator=(ICorrelation &&other) = delete;
        virtual ~ICorrelation() = default;
        virtual void startCorrelation() = 0;
    };

    typedef std::shared_ptr<ICorrelation> ICorrelationPtr;
}

#endif // CORRELATION_HPP
